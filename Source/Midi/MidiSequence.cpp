/*>>MS

This file is part of the Midi Humanizer application.
Copyright 2018 by Alatar79

The Midi Humanizer  is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Midi Humanizer  is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the Midi Humanizer.  If not, see <http://www.gnu.org/licenses/>.

MS<<*/
/*
  ==============================================================================

    MidiSequence.cpp
    Created: 21 Jun 2015 12:54:57pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#include "MidiSequence.h"
#include "../AppData.h"
#include "MidiHelpers.h"


TriggerNote::TriggerNote()
{
    this->noteNumber = -1;
    this->midiOutName = "";
}

TriggerNote::TriggerNote(const int note, const String& midiName)
{
    this->noteNumber = note;
    this->midiOutName = midiName;

}



//--------------------------------------------------------------------

const String MidiSequence::MidiStreamLoader::loadingFinishedSuccessfully = "MidiSequence::MidiStreamLoader: Loading of Midi stream finished.";
const String MidiSequence::MidiStreamLoader::loadingFinishedWithError = "MidiSequence::MidiStreamLoader: Loading of Midi stream finished with error.";

MidiSequence::MidiStreamLoader::MidiStreamLoader(InputStream* input, MidiSequence& dest)
    : Thread("LoaderThread"),
    destSequence(dest)

{
    if (input != nullptr)
    {
        //make a copy of the input stream. That way we do not care, what happens to it, as it comes from another thread. 
        MemoryBlock memBlock;
        input->readIntoMemoryBlock(memBlock);
        midiInputStream = std::unique_ptr<InputStream>(new MemoryInputStream(memBlock, true));
    }
}

void MidiSequence::MidiStreamLoader::run()
{
    if (loadMidiStream())
        sendActionMessage(loadingFinishedSuccessfully);
    else
        sendActionMessage(loadingFinishedWithError);
}

bool MidiSequence::MidiStreamLoader::loadMidiStream()
{
    destSequence.loadMidiProgress = 0;

    destSequence.reset();
    if (!destSequence.midiFile.readFrom(*midiInputStream))
        return false;
    
    destSequence.loadMidiProgress = 0.1;

    destSequence.midiFile.convertTimestampTicksToSeconds();
    destSequence.loadMidiProgress = 0.2;

    double totalEvents = 0;
    double summedWeight = 0;
    Array<double> weight;
    Array<double> weightUpTillNow;
    for (int i = 0; i < destSequence.midiFile.getNumTracks(); i++)
        totalEvents += destSequence.midiFile.getTrack(i)->getNumEvents(); //destSequence.tracks[i]->getNumEvents();
    for (int i = 0; i < destSequence.midiFile.getNumTracks(); i++)
    {
        double currentWeight = (double)destSequence.midiFile.getTrack(i)->getNumEvents() / totalEvents;
        weight.add(currentWeight);
        weightUpTillNow.add(summedWeight);
        summedWeight += currentWeight;
    }

    for (int i = 0; i < destSequence.midiFile.getNumTracks(); i++)
    {
        destSequence.tracks.add(new ExtendedMidiMessageSequence(*destSequence.midiFile.getTrack(i)));

        double dbg_startTime = Time::getMillisecondCounterHiRes();

        //for each note-on event, find out all midi CC events needed, to start playing at this note
        //add the midi CC events to the MidiMessageSequence.
        for (int j = 0; j < destSequence.tracks[i]->getNumEvents(); j++)
        {
            MidiMessageSequence::MidiEventHolder* evt = destSequence.tracks[i]->getEventPointer(j);
            if (evt->message.isNoteOn())
            {
                double evtTime = evt->message.getTimeStamp();
                for (int channel = 1; channel <= 16; channel++)
                {
                    Array<MidiMessage> messages;
                    destSequence.tracks[i]->createControllerUpdatesForTime(channel, evtTime, messages);
                    for (int k = 0; k < messages.size(); k++)
                    {
                        /*
                        DBG("Message Time: " + String(messages[k].getTimeStamp()));
                        DBG("Message Channel: " + String(messages[k].getChannel()));
                        DBG("Message CC: " + String(messages[k].getControllerNumber()));
                        DBG("Message Value: " + String(messages[k].getControllerValue()));
                        */
                        messages.getReference(k).setTimeStamp(evtTime);
                        destSequence.tracks[i]->addEvent(messages[k]);
                    }
                }
            }

            destSequence.loadMidiProgress = 0.2 
                                          + (weightUpTillNow[i]) * 0.8 
                                          + ((double)j / (double)destSequence.tracks[i]->getNumEvents()) * weight[i] * 0.8;
        }

        double dbg_endTime = Time::getMillisecondCounterHiRes();
        DBG("Track CC time: " + String(dbg_endTime - dbg_startTime));
    }

    midiInputStream->setPosition(0);
    destSequence.midiFileBackup.readFrom(*midiInputStream);

    destSequence.loadMidiProgress = 1.0;
    midiInputStream = nullptr;
    return true;

}

//--------------------------------------------------------------------

const String MidiSequence::ProjectFileLoader::loadingFinishedSuccessfully = "MidiSequence::ProjectFileLoader: Loading of Midi stream finished.";
const String MidiSequence::ProjectFileLoader::loadingFinishedWithError = "MidiSequence::ProjectFileLoader: Loading of Midi stream finished with error.";

MidiSequence::ProjectFileLoader::ProjectFileLoader(File& file, MidiSequence& dest)
    :MidiStreamLoader(nullptr, dest)
{
    zipFile = std::unique_ptr<ZipFile>(new ZipFile(file));
}


void MidiSequence::ProjectFileLoader::run()
{
    if (loadProjectFile())
        sendActionMessage(loadingFinishedSuccessfully);
    else
        sendActionMessage(loadingFinishedWithError);
}

bool MidiSequence::ProjectFileLoader::loadProjectFile()
{
    const ZipFile::ZipEntry* zipMidi = zipFile->getEntry("MidiData.mid");
    const ZipFile::ZipEntry* zipXml = zipFile->getEntry("TrackData.xml");

    midiInputStream = std::unique_ptr<InputStream>(zipFile->createStreamForEntry(*zipMidi));
    std::unique_ptr<InputStream> xmlStream;
    xmlStream = std::unique_ptr<InputStream>(zipFile->createStreamForEntry(*zipXml));

    if (midiInputStream == nullptr || xmlStream == nullptr)
        return false;

    loadMidiStream();

    XmlDocument xmlDoc(xmlStream->readEntireStreamAsString());
    String dbg_err = xmlDoc.getLastParseError();
    std::unique_ptr<XmlElement> xml = xmlDoc.getDocumentElement();
    dbg_err = xmlDoc.getLastParseError();

    //ScopedPointer<XmlElement> xml = XmlDocument::parse(xmlStream->readEntireStreamAsString());

    if (xml == nullptr ||
        !xml->hasTagName("HUMANIZER_PROJECT"))
    {
        return false;
    }

    int noOfTracks = xml->getIntAttribute("Tracks", 0);

    if (noOfTracks != destSequence.tracks.size())
        return false;

    for (int i = 0; i < noOfTracks; i++)
    {
        String attributeString = "Track_" + String(i);
        XmlElement *child = xml->getChildByName(attributeString);
        if (child == nullptr)
            return false;

        attributeString = "Track_" + String(i) + "_MidiDevice";
        destSequence.tracks[i]->setMidiOut(child->getStringAttribute(attributeString, AppData::noMidiSelected));

        attributeString = "Track_" + String(i) + "_MidiChannel";
        destSequence.tracks[i]->setMidiChannel(child->getIntAttribute(attributeString, 1));

        attributeString = "Track_" + String(i) + "_Enabled";
        if (child->getBoolAttribute(attributeString, false))
            destSequence.tracks[i]->enable();
        else
            destSequence.tracks[i]->disable();
    }

    zipFile = nullptr;
    return true;


}



// ------------------------------------------------------------------------
MidiSequence::MidiSequence()
{
    reset();
}

MidiSequence::~MidiSequence()
{
    reset();
}

/*
MidiSequence::MidiSequence(String fileString)
{
    loadMidiFile(fileString);
}
*/

void MidiSequence::loadMidiFile(const String& fileString, ActionListener& listener)
{
  
    File file(fileString);
    if (!file.exists())
        return;
    FileInputStream inputStream(file);

    loadMidiStream(&inputStream, listener);

}

void MidiSequence::loadMidiStream(InputStream* inputStream, ActionListener& listener)
{
    streamLoader = std::unique_ptr<MidiStreamLoader> (new MidiStreamLoader(inputStream, *this));
    //add listener, which gets notified, once loading has finished.
    streamLoader->addActionListener(&listener);
    streamLoader->addActionListener(this);
    streamLoader->startThread();

}

double& MidiSequence::getRefToLoadMidiProgressIndicator()
{
    return loadMidiProgress;
}

void MidiSequence::loadProjectFile(File& file, ActionListener& listener)
{

    projectLoader = std::unique_ptr<ProjectFileLoader>(new ProjectFileLoader(file, *this));
    //add listener, which gets notified, once loading has finished.
    projectLoader->addActionListener(&listener);
    projectLoader->addActionListener(this);
    projectLoader->startThread();

}

bool MidiSequence::saveProjectFile(ZipFile::Builder& zip)
{
    
    MemoryOutputStream midiOutStream;
    if (!midiFileBackup.writeTo(midiOutStream))
        return false;

    juce::XmlElement xml("HUMANIZER_PROJECT");
    xml.setAttribute("Tracks", tracks.size());
    for (int i = 0; i < tracks.size(); i++)
    {
        String attributeString = "Track_" + String(i);
        XmlElement *child = xml.createNewChildElement(attributeString);

        attributeString = "Track_" +  String(i) + "_MidiDevice";
        child->setAttribute(attributeString, tracks[i]->getMidiOutName());

        attributeString = "Track_" + String(i) + "_MidiChannel";
        child->setAttribute(attributeString, tracks[i]->getMidiChannel());

        attributeString = "Track_" + String(i) + "_Enabled";
        child->setAttribute(attributeString, tracks[i]->isEnabled());
    }

    MemoryOutputStream xmlOutStream;
    //xml.writeToStream(xmlOutStream, "<no DTD=\"0\">");
    xml.writeToStream(xmlOutStream, "");

    MemoryBlock midiMemoryBlock = midiOutStream.getMemoryBlock();
    MemoryBlock xmlMemoryBlock = xmlOutStream.getMemoryBlock();
    
    zip.addEntry(new juce::MemoryInputStream(midiMemoryBlock, true), 9, "MidiData.mid", Time::getCurrentTime());
    zip.addEntry(new juce::MemoryInputStream(xmlMemoryBlock, true), 9, "TrackData.xml", Time::getCurrentTime());
   
    return true;

}

double MidiSequence::getEndTime()
{
    double endTime = -1;
    for (int i = 0; i < tracks.size(); i++)
    {
        if (tracks[i]->getEndTime() > endTime)
            endTime = tracks[i]->getEndTime();
    }

    return endTime;
}

double MidiSequence::getStartTime()
{
    double startTime = getEndTime() + 1;
    for (int i = 0; i < tracks.size(); i++)
    {
        if (tracks[i]->getStartTime() < startTime)
            startTime = tracks[i]->getStartTime();
    }

    return startTime;

}

int MidiSequence::getNextNoteOnIndexAtTime(double time, const ExtendedMidiMessageSequence* track)
{

    int evtIdx = track->getNextIndexAtTimeBinarySearch(time);
    for (;evtIdx < track->getNumEvents(); evtIdx++)
    {
        if (track->getEventPointer(evtIdx)->message.isNoteOn())
            break;
    }

    return evtIdx;

}

int MidiSequence::getPreviousNoteOnIndexAtTime(double time, const ExtendedMidiMessageSequence* track)
{

    int evtIdx = track->getPreviousIndexAtTimeBinarySearch(time);
    for (;evtIdx > 0; evtIdx--)
    {
        if (track->getEventPointer(evtIdx)->message.isNoteOn())
            break;
    }

    return evtIdx;

}


Array<double> MidiSequence::getNextNoteOnTimes(const double startTime, const bool skipDisabledTracks /*= true*/)
{
    Array<double> nextNoteOnTimes;

    //for al tracks find the time of the next note(s) and create a table from that data
    for (int i = 0; i < tracks.size(); i++)
    {
        //set next note time for track i to maxTime+1 (= not found)
        nextNoteOnTimes.add(getEndTime() + 1);

        //if the track is not enabled, skip it
        //NOTE: yes, we do this after adding the "not found" time above.
        if (skipDisabledTracks && !(tracks[i]->isEnabled()))
            continue;

        int nextNoteOnIndex = getNextNoteOnIndexAtTime(startTime, tracks[i]);
        if ( nextNoteOnIndex < tracks[i]->getNumEvents())
        {
            int evtIdx = nextNoteOnIndex;
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(evtIdx);
            if (evt->message.isNoteOn())
                nextNoteOnTimes.set(i, tracks[i]->getEventTime(evtIdx));
        }

    }

    return nextNoteOnTimes;
}


Array<double> MidiSequence::getPreviousNoteOnTimes(double startTime)
{
    Array<double> previousNoteOnTimes;

    //for al tracks find the time of the next note(s) and create a table from that data
    for (int i = 0; i < tracks.size(); i++)
    {
        //set next note time for track i to maxTime+1 (= not found)
        previousNoteOnTimes.add(getStartTime() - 1);

        //if the track is not enabled, skip it
        //NOTE: yes, we do this after adding the "not found" time above.
        if (!(tracks[i]->isEnabled()))
            continue;

        int previousNoteOnIndex = getPreviousNoteOnIndexAtTime(startTime, tracks[i]);
        if ( previousNoteOnIndex > 0)
        {
            int evtIdx = previousNoteOnIndex;
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(evtIdx);
            if (evt->message.isNoteOn())
                previousNoteOnTimes.set(i, tracks[i]->getEventTime(evtIdx));
        }

    }

    return previousNoteOnTimes;
}

double MidiSequence::getNextChordTime(const double startTime, const bool skipDisabledTracks /*= true*/)
{

    Array<juce::MidiMessageSequence::MidiEventHolder*> notes;
    Array<double> minChordTimes = getNextNoteOnTimes(startTime, skipDisabledTracks);

    double nextChordTime = getEndTime() + 1;
    //now find the minimum/closest time for the next chord
    for (int i = 0; i < tracks.size(); i++)
    {
        if (minChordTimes[i] < nextChordTime)
        {
            nextChordTime = minChordTimes[i];
            //if the closest chord time matches the startTime, we cannot find a better time => break;
            if (fabs(nextChordTime - startTime) < MidiHelpers::doubleComparisonThreshold)
                break;
        }
    }
    return nextChordTime;
}

Array<ExtendedMidiMessage> MidiSequence::getNextChord(double startTime)
{
    Array<ExtendedMidiMessage> notes;

    //Array<double> minChordTimes = getNextNoteOnTimes(startTime);
    double nextChordTime = getNextChordTime(startTime);

    //only proceed, if we really found some events and thus got a nextChordTime
    if (nextChordTime <= getEndTime())
    {
        //now find all notes
        for (int i = 0; i < tracks.size(); i++)
        {

            //if the track is not enabled, skip it
            if (!(tracks[i]->isEnabled()))
                continue;

            int evtIdx = tracks[i]->getNextIndexAtTimeBinarySearch(nextChordTime);
            while ( (evtIdx < tracks[i]->getNumEvents()) &&
                    (fabs(tracks[i]->getEventTime(evtIdx) - nextChordTime) < MidiHelpers::midiDoubleThreshold))
            {
                juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(evtIdx);
                if (evt->message.isNoteOn())
                {
                    ExtendedMidiMessage extEvt(*evt, tracks[i]->getMidiOut(), tracks[i]->getMidiOutNamePtr(), tracks[i]->getMidiChannel(), i, evtIdx);
                    notes.add(extEvt);
                }
                evtIdx++;
            }

        }
    }

    return notes;
}

void MidiSequence::getMidiCCsFromChord(const Array<ExtendedMidiMessage>& chordNotes, Array<ExtendedMidiMessage>& resultMidiCCs)
{
    if (chordNotes.size() <= 0)
        return; 
    
    Array<bool> trackHasBeenHandled;
    for (int i = 0; i < tracks.size(); i++)
        trackHasBeenHandled.add(false);

    double chordTime = chordNotes[0].getTimeStamp();

    for (int i = 0; i < chordNotes.size(); i++)
    {
        int trackIdx = chordNotes[i].getTrackIndex();
        if (!trackHasBeenHandled[trackIdx])
        {
            trackHasBeenHandled.getReference(trackIdx) = true;
            MidiMessage message;

            int eventIdx = chordNotes[i].getEventIndex() - 1;
            do
            {
                if (eventIdx >= 0)
                {
                    message = tracks[trackIdx]->getEventPointer(eventIdx)->message;
                    if (message.isController() || message.isPitchWheel())
                        resultMidiCCs.add(chordNotes[i].setMidiMessage(message));
                }
                eventIdx--;
            } while (eventIdx >= 0 && message.getTimeStamp() == chordTime);

            eventIdx = chordNotes[i].getEventIndex() + 1;
            do
            {
                if (eventIdx <= tracks[trackIdx]->getNumEvents())
                {
                    message = tracks[trackIdx]->getEventPointer(eventIdx)->message;
                    if (message.isController() || message.isPitchWheel())
                        resultMidiCCs.add(chordNotes[i].setMidiMessage(message));
                }
                eventIdx++;
            } while (eventIdx <= tracks[trackIdx]->getNumEvents() &&
                message.getTimeStamp() == chordTime);

        }
    }

}


int MidiSequence::getLowestNote()
{
    int lowestNote = 0xFFFF;

    for (int i = 0; i < tracks.size(); i++)
    {
        for (int j = 0; j < tracks[i]->getNumEvents(); j++)
        {
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(j);
            if (evt->message.isNoteOn() && 
                evt->message.getNoteNumber() < lowestNote)
                lowestNote = evt->message.getNoteNumber();
        }
    }

    return lowestNote;
}

int MidiSequence::getHighestNote()
{
    int highestNote = -1;

    for (int i = 0; i < tracks.size(); i++)
    {
        for (int j = 0; j < tracks[i]->getNumEvents(); j++)
        {
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(j);
            if (evt->message.isNoteOn() &&
                evt->message.getNoteNumber() > highestNote)
                highestNote = evt->message.getNoteNumber();
        }
    }

    return highestNote;
}

int MidiSequence::getNoOfTracks()
{
    return tracks.size();
}

const ExtendedMidiMessageSequence* MidiSequence::getTrack(int idx)
{
    if (idx > -1 && idx < tracks.size())
        return tracks[idx];
    else 
        return nullptr;
}

int MidiSequence::maxNoOfNoteOnEvents()
{
    int maxNoteOnEvents = -1;
    for (int i = 0; i < tracks.size(); i++)
    {
        int tempNoteOnEvents = -1;
        for (int j = 0; j < tracks[i]->getNumEvents(); j++)
        {
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[i]->getEventPointer(j);
            if (evt->message.isNoteOn())
                tempNoteOnEvents++;
        }
        if (tempNoteOnEvents > maxNoteOnEvents)
            maxNoteOnEvents = tempNoteOnEvents;
    }
    return maxNoteOnEvents;
}


int MidiSequence::noOfChordEvents()
{
    if (noOfChordEventsCached > -1)
        return noOfChordEventsCached;

    double dbg_startTime = Time::getMillisecondCounterHiRes();
    int maxChordEvents = 0;

    double time = getNextChordTime(getStartTime(), false);
    double endTime = getEndTime();
    while (time < endTime)
    {
        ++maxChordEvents;
        time = getNextChordTime(time + 1e-4, false);
    }

    noOfChordEventsCached = maxChordEvents;

    double dbg_endTime = Time::getMillisecondCounterHiRes();
    DBG("Calc Time: " + String(dbg_endTime - dbg_startTime));
    return maxChordEvents;
}


String MidiSequence::getTrackName(int trackNo)
{
    String name = "";
    if (trackNo >= 0 && trackNo < tracks.size())
    {
        for (int i = 0; i < tracks[trackNo]->getNumEvents(); i++)
        {
            juce::MidiMessageSequence::MidiEventHolder* evt = tracks[trackNo]->getEventPointer(i);
            if (evt->message.isTrackNameEvent())
            {
                name = evt->message.getTextFromTextMetaEvent();
                break;
            }
        }
    }
    return name;
}

void MidiSequence::enableTrack(int trackNo)
{
    if (trackNo >= 0 && trackNo < tracks.size())
        tracks[trackNo]->enable();
}

void MidiSequence::disableTrack(int trackNo)
{
    if (trackNo >= 0 && trackNo < tracks.size())
        tracks[trackNo]->disable();
}

bool MidiSequence::isEnabled(int trackNo)
{
    if (trackNo >= 0 && trackNo < tracks.size())
        return tracks[trackNo]->isEnabled();
    else
        return false;

}

void MidiSequence::setMidiOut(const String& midiOutName, const int trackNo)
{
    if (trackNo >= 0 && trackNo < tracks.size())
    {

        tracks[trackNo]->setMidiOut(midiOutName);
    }
}


void MidiSequence::setMidiChannel(const int midiChannel, const int trackNo)
{
    if (trackNo >= 0 && trackNo < tracks.size())
    {

        tracks[trackNo]->setMidiChannel(midiChannel);
    }
}

int MidiSequence::getMidiChannel(const int trackNo) const
{
    if (trackNo >= 0 && trackNo < tracks.size())
    {
        return tracks[trackNo]->getMidiChannel();
    }
    else
    {
        return 1;
    }
}

String MidiSequence::getMidiOutName(const int trackNo) const
{
    if (trackNo >= 0 && trackNo < tracks.size())
    {
        return tracks[trackNo]->getMidiOutName();
    }

    return "";
}

Array<MidiOutput*> MidiSequence::getMidiOutputList()
{
    Array<MidiOutput*> midiOutList;
    for (int i = 0; i < tracks.size(); i++)
    {
        midiOutList.add(tracks[i]->getMidiOut());
    }

    return midiOutList;
}


//----------------------------------------------------------

void MidiSequence::actionListenerCallback(const String &message)
{
    if (message == MidiSequence::MidiStreamLoader::loadingFinishedSuccessfully || 
        message == MidiSequence::MidiStreamLoader::loadingFinishedWithError)
    {
        //streamLoader = nullptr;

    }
    else if (message == MidiSequence::ProjectFileLoader::loadingFinishedSuccessfully ||
             message == MidiSequence::ProjectFileLoader::loadingFinishedWithError)
    {
        //projectLoader = nullptr;
    }
}


void MidiSequence::reset()
{
    tracks.clear();
    noOfChordEventsCached = -1;
}
