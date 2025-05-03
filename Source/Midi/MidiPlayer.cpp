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

    MidiPlayer.cpp
    Created: 28 Jun 2015 5:57:25pm
    Author:  MartinG

  ==============================================================================
*/
//Git Test
//GitTest2
#include "MidiPlayer.h"

const String MidiPlayer::midiLoadingFinishedSuccessfully = "MidiPlayer: midiLoadingFinishedSuccessfully";
const String MidiPlayer::midiLoadingFinishedWithErrors = "MidiPlayer: midiLoadingFinishedWithErrors";
const String MidiPlayer::projectLoadingFinishedSuccessfully = "MidiPlayer: projectLoadingFinishedSuccessfully";
const String MidiPlayer::projectLoadingFinishedWithErrors = "MidiPlayer: projectLoadingFinishedWithErrors";

MidiPlayer::MidiPlayer()
{
    for (int i = 0; i <= 127; i++)
    {
        velocityMap[i] = ((float)i) / (float) 127.0;
    }

    chordTime = 40;
    resetVars();
}

MidiPlayer::~MidiPlayer()
{
    resetVars();
}

void MidiPlayer::loadMidiFile(const String& fileString, ActionListener& listener)
{
    addActionListener(&listener);
    midiData.loadMidiFile(fileString, *this);
}

double& MidiPlayer::getRefToLoadMidiProgressIndicator()
{
    return midiData.getRefToLoadMidiProgressIndicator();
}

void MidiPlayer::loadMidiStream(InputStream* inputStream, ActionListener& listener)
{
    addActionListener(&listener);
    midiData.loadMidiStream(inputStream, *this);
}

void MidiPlayer::loadProjectFile(File& file, ActionListener& listener)
{
    addActionListener(&listener);
    midiData.loadProjectFile(file, *this);
}


bool MidiPlayer::saveProjectFile(ZipFile::Builder& zip)
{
    return midiData.saveProjectFile(zip);
}


void MidiPlayer::processNextMidiBlock(AudioSampleBuffer& /*outputBuffer*/,
                                      const MidiBuffer& data,
                                      int startSample,
                                      int numSamples)
{
    const ScopedLock sl(lock);

    MidiBuffer::Iterator midiIterator(data);
    midiIterator.setNextSamplePosition(startSample);
    MidiMessage m(0xf4, 0.0);

    while (numSamples > 0)
    {
        int midiEventPos;
        const bool useEvent = midiIterator.getNextEvent(m, midiEventPos)
            && midiEventPos < startSample + numSamples;

        const int numThisTime = useEvent ? midiEventPos - startSample
            : numSamples;

        if (useEvent)
        {
            handleMidiEvent(m);

        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}


void MidiPlayer::processNextMidiBlock(const MidiBuffer& data,
                                      int startSample,
                                      int numSamples)
{
    const ScopedLock sl(lock);

    MidiBuffer::Iterator midiIterator(data);
    midiIterator.setNextSamplePosition(startSample);
    MidiMessage m(0xf4, 0.0);

    while (numSamples > 0)
    {
        int midiEventPos;
        const bool useEvent = midiIterator.getNextEvent(m, midiEventPos)
            && midiEventPos < startSample + numSamples;

        const int numThisTime = useEvent ? midiEventPos - startSample
            : numSamples;

        if (useEvent)
        {
            handleMidiEvent(m);

        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}




//Most of this code is a direct copy from the JUCE Synthesiser.
//But I have added a line for mapping MIDI events
void MidiPlayer::handleMidiEvent(const MidiMessage& m)
{
    //no scoped lock here, because this function is called from processNextMidiBlock, which already locks
    if (m.isNoteOn())
    {

		//Check for errors in the Midi stream.
		//A key should not send a noteOn event, if it is already down.
		//If this error happens, stop the key/note first. 
		if (keyIsDown[m.getNoteNumber()])
		{
			player.stop(m);
			DBG("*** Midi Stream Error Detected ***");
		}

		keyIsDown[m.getNoteNumber()] = true;

        if (fabs(lastNoteOnTime - juce::Time::getMillisecondCounterHiRes()) < chordTime)
        {
            //if we are here, we detcted that the player plays a chord on the keyboard.
            player.addWithoutPlaying(chordNotes, m);
        }
        else
        {
            double dbg_innerPlayStartTime = Time::getMillisecondCounterHiRes();

            currentMidiTime = midiData.getNextChordTime(currentMidiTime);
            chordNotes = midiData.getNextChord(currentMidiTime);

            chordNotes = removeDuplicates(chordNotes);

            Array<ExtendedMidiMessage> midiCCs;
            midiData.getMidiCCsFromChord(chordNotes, midiCCs);
            //player.sendMidiCCs(midiCCs);
            player.play(chordNotes, m);
            //advance the midi time counter
            currentMidiTime = midiData.getNextChordTime(currentMidiTime + MidiHelpers::midiDoubleThreshold + 1e-6);
            midiTimeHasChangedFlag = true;
            //in order to detect chords, remember the time we hit this note
            lastNoteOnTime = juce::Time::getMillisecondCounterHiRes();
            double dbg_innerPlayEndTime = Time::getMillisecondCounterHiRes();
            DBG("Inner Play Time: " + String(dbg_innerPlayEndTime - dbg_innerPlayStartTime));
        }
    }
    else if (m.isNoteOff() && keyIsDown[m.getNoteNumber()])
    {
        player.stop(m);
		keyIsDown[m.getNoteNumber()] = false;
    }
    else if (m.isAllNotesOff() || m.isAllSoundOff())
    {
    }
    else if (m.isPitchWheel())
    {
    }
    else if (m.isController())
    {
        /*FIXME: temporarily deactivated. Needs menu choice.*/
        //send the controller to all tracks
        
        Array<MidiOutput*> midiOutList = midiData.getMidiOutputList();
        for (int i = 0; i < midiOutList.size(); i++)
        {
            if (midiOutList[i]!=nullptr)
                midiOutList[i]->sendMessageNow(m);
        }
        
        
    }
}


void MidiPlayer::resetMidiCounter()
{
    const ScopedLock sl(lock);
    currentMidiTime = 0;
    midiTimeHasChangedFlag = true;
}

double MidiPlayer::getMidiCounter()
{
    const ScopedLock sl(lock);
    return currentMidiTime;
}

double MidiPlayer::getMidiCounterInPercent()
{
    const ScopedLock sl(lock);
    return currentMidiTime/midiData.getEndTime();
}

void MidiPlayer::setMidiCounter(double counter)
{
    const ScopedLock sl(lock);
    currentMidiTime = counter;
    midiTimeHasChangedFlag = true;
}

bool MidiPlayer::midiTimeHasChanged()
{
    const ScopedLock sl(lock);
    return midiTimeHasChangedFlag;
}

void MidiPlayer::clearMidiTimeHasChangedFlag()
{
    const ScopedLock sl(lock);
    midiTimeHasChangedFlag = false;
}

MidiSequence& MidiPlayer::getMidiSequence()
{
    return midiData;
}

Array<double> MidiPlayer::getCurrentlyActiveChordTimes() const
{
    return player.getCurrentlyActiveChordTimes();
}

//=======================================================
void MidiPlayer::resetVars()
{
    const ScopedLock sl(lock);
	for (int i = 0; i < maxMidiNotes; i++)
		keyIsDown[i] = false;

    currentMidiTime = midiData.getStartTime();
    midiTimeHasChangedFlag = true;
    chordNotes.clear();
    lastNoteOnTime = -1;
}

void MidiPlayer::actionListenerCallback(const String &message)
{
    if (message == MidiSequence::MidiStreamLoader::loadingFinishedSuccessfully)
    {
        resetVars();
        sendActionMessage(midiLoadingFinishedSuccessfully);
    }
    else if (message == MidiSequence::MidiStreamLoader::loadingFinishedWithError)
    {
        sendActionMessage(midiLoadingFinishedWithErrors);
    }
    else if (message == MidiSequence::ProjectFileLoader::loadingFinishedSuccessfully)
    {
        resetVars();
        sendActionMessage(projectLoadingFinishedSuccessfully);
    }
    else if (message == MidiSequence::ProjectFileLoader::loadingFinishedWithError)
    {
        sendActionMessage(projectLoadingFinishedWithErrors);
    }
    
}

Array<ExtendedMidiMessage>& MidiPlayer::removeDuplicates(Array<ExtendedMidiMessage>& notes)
{
   //No scoped lock here, because it is called from a function, which already locks. 
    bool marked[128];
    Array<MidiCompareObject> midiDeviceNames[128];

    for (int i = 0; i < 128; i++)
    {
        marked[i] = false;
    }

    for (int i = 0; i < notes.size(); i++)
    {
        int playingNote = notes[i].getNoteNumber();
        
        MidiCompareObject midiObject(notes[i].getMidiOutName(), notes[i].getChannel());
        if (marked[playingNote] &&
            midiDeviceNames[playingNote].contains(midiObject))
        {
            notes.remove(i);
            i--;
        }
        else
        {
            marked[playingNote] = true;
            midiDeviceNames[playingNote].add(midiObject);
        }
    }
    /*
#ifdef DEBUG
    int dbg_after = notes.size();
    DBG("Removed "+String(dbg_before-dbg_after)+" notes.");
    DBG("Left: " + String(dbg_after) + "notes");
#endif
    */
    return notes;
}
