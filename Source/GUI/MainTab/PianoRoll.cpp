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

    PianoRoll.cpp
    Created: 22 Jun 2015 5:10:57pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#include "PianoRoll.h"
#include "../../Midi/MidiPlayer.h"
#include "../GuiHelpers.h"


PianoRollViewport::PianoRollViewport(const String& name)
    :Viewport(name)
{
    setOpaque(false);
    pianoRollRef = nullptr;
}

void PianoRollViewport::setViewedComponent(PianoRoll *newPianoRoll, bool deleteComponentWhenNoLongerNeeded)
{
    Viewport::setViewedComponent(newPianoRoll, deleteComponentWhenNoLongerNeeded);
    pianoRollRef = newPianoRoll;

}

void PianoRollViewport::paint(Graphics& g)
{
   
    g.setColour(Colour(AppData::componentHighlightColour));
    g.fillRoundedRectangle(0.0f, 0.0f, 
        (float) getWidth(),
        (float) pianoRollHeight(),
        AppData::scalef(AppData::cornerSize));
}

void PianoRollViewport::setBounds(int x, int y, int width, int height)
{
    Viewport::setBounds(x,y,width,height);
    setBoundsForPianoRoll();
}

void PianoRollViewport::setBoundsForPianoRoll()
{
    jassert(pianoRollRef != nullptr);
    pianoRollRef->setBounds(0, 0, pianoRollRef->getWidth(), pianoRollHeight());
}

void PianoRollViewport::newFileLoaded()
{
    jassert(pianoRollRef != nullptr);
    pianoRollRef->newFileLoaded();
    setBoundsForPianoRoll();
}


int PianoRollViewport::pianoRollHeight()
{
    ScrollBar& horScrollBar = getHorizontalScrollBar();
    return horScrollBar.getY() - AppData::scalei(AppData::scrollBarBorder) -2;
}

//--------------------------------------------------------------------------------------------
PianoRoll::PianoRoll(MidiHelpers& midiHelperIn) :
    midiHelper(midiHelperIn)
{
    setOpaque(false);
    reset();
}


PianoRoll::~PianoRoll()
{
    reset();
}

void PianoRoll::paint(Graphics& g)
{

/*
#ifdef DEBUG    
    double dbg_paintStart = Time::getMillisecondCounterHiRes();
#endif
*/
    //g.setGradientFill(ColourGradient(Colours::white, 0, 0, Colours::grey, 0, (float) getParentComponent()->getHeight(), false));
    //g.fillRect(0, 0, getParentComponent()->getWidth(), getParentComponent()->getHeight());


    if (markerLineX != -1)
    {

        g.setColour(Colour(AppData::defaultTextColour));
        g.drawLine((float)markerLineX + 1.0f, 0,
            (float)markerLineX + 1.0f, (float)getHeight());
        g.drawLine((float)markerLineX + 2.0f, 0,
            (float)markerLineX + 2.0f, (float)getHeight());

    }

}

void PianoRoll::resized()
{
    xOffset =  getParentComponent()->getWidth() / 3;

    if (noteButtons.size() > 0)
    {
        MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());

        int topBorder = 5;
        int bottomBorder = 5;
        int heightWithoutBorder = getHeight() - topBorder - bottomBorder;
        int lowestNote = midiData.getLowestNote();
        int highestNote = midiData.getHighestNote();
        double rectHeight = (double)heightWithoutBorder / ((double)(highestNote - lowestNote + 1));

        double sequenceStartTime = midiData.getStartTime();
        double sequenceEndTime = midiData.getEndTime();
        //double timePerPixel = (sequenceEndTime - sequenceStartTime) / (double)getWidth();
        double pixelPerTime = (double)(getWidth() - xOffset) / (sequenceEndTime - sequenceStartTime);

        NoteButton **noteButtonsRaw = noteButtons.getRawDataPointer();
        for (int i = 0; i < noteButtons.size(); i++)
        {

            int x = (int)(noteButtonsRaw[i]->getMidiStartTime() * pixelPerTime) + xOffset;
            int y = topBorder + (int)((double)heightWithoutBorder - (double)(noteButtonsRaw[i]->getNoteNumber() - lowestNote + 1) * rectHeight);
            int width = (int)(noteButtonsRaw[i]->getNoteDuration() * pixelPerTime);
            int height = (int)rectHeight;
            //don't paint slivers
            if (width < 5) width = 5;
            if (height < 3) height = 3;

            noteButtonsRaw[i]->setBounds(x, y, width-1, height-1);
        }

    }

}

void PianoRoll::mouseDown(const MouseEvent &event)
{

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());

    double sequenceStartTime = midiData.getStartTime();
    double sequenceEndTime = midiData.getEndTime();
    double timePerPixel = (sequenceEndTime - sequenceStartTime) / (double)(getWidth() - xOffset);
    //double pixelPerTime = (double)(getWidth() - xOffset) / (sequenceEndTime - sequenceStartTime);

    markerLineX = event.getMouseDownX();
    midiHelper.getMidiPlayer()->setMidiCounter((markerLineX - xOffset) * timePerPixel);
    //setMidiTime((markerLineX - xOffset) * timePerPixel);
    repaint();
}

void PianoRoll::scale(double scaleFactor)
{

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
    int chordEvents = midiData.noOfChordEvents();
    int oldWidth = getWidth();
    if (chordEvents > 0)
    {
        setBounds(0, 0, (int)((chordEvents * 25) * scaleFactor) + xOffset, getHeight());
    }

    //Need to resize manually, in case the new bounds are exactly as the old ones.
    if (oldWidth == getWidth())
        resized();
}

void PianoRoll::newFileLoaded()
{
    reset();
    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());

    int noteButtonCounter = 0;
    for (int i = 0; i < midiData.getNoOfTracks(); i++)
    {
        const ExtendedMidiMessageSequence* track = midiData.getTrack(i);
        if (track)
        {
            int trackButtonCounter = 0;
            for (int j = 0; j < track->getNumEvents(); j++)
            {
                juce::MidiMessageSequence::MidiEventHolder* evt = track->getEventPointer(j);
                if (evt->message.isNoteOn())
                {
                    double startTime = evt->message.getTimeStamp();
                    double endTime;
                    if (evt->noteOffObject)
                        endTime = evt->noteOffObject->message.getTimeStamp();
                    else endTime = startTime + 0.1;

                    double duration = endTime - startTime;

                    NoteButton* noteButton = new NoteButton(startTime, duration, i, evt->message.getNoteNumber());
                    noteButton->setTooltip(MidiMessage::getMidiNoteName(evt->message.getNoteNumber(), true, true, 4));
                    addAndMakeVisible(noteButton);
                    noteButtons.add(noteButton);
                    if (trackButtonCounter == 0)
                    {
                        noteButtonStart.add(noteButtonCounter);
                    }
                    noteButtonCounter++;
                    trackButtonCounter++;
                }
            }
            if (trackButtonCounter != 0)
            {
                noteButtonEnd.add(noteButtonCounter -1);
            }
            else
            {
                //these inverted extreme values mark the track as "no note events"
                noteButtonStart.add(INT_MAX);
                noteButtonEnd.add(0);
            }
        }
    }

    scale(1.0);
}

int PianoRoll::getXOffsetFromMidiTime(double time)
{
    int offset = 0;

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
    double sequenceStartTime = midiData.getStartTime();
    double sequenceEndTime = midiData.getEndTime();
    double pixelPerTime = (double)(getWidth() - xOffset) / (sequenceEndTime - sequenceStartTime);

    offset = (int) (time * pixelPerTime) + xOffset;

    return offset;

}

bool PianoRoll::isEnabled(const int track) const
{
    if (track >= 0 && track < noteButtonStart.size())
    {
        if (noteButtonStart[track] <= noteButtonEnd[track])
            if (noteButtons[noteButtonStart[track]]->getNoteButtonState() != NoteButton::NoteButtonState::disabled)
                return true;
    }

    return false;
}

void PianoRoll::setFocusTrack(const int track)
{

    //defocus old track
    if (isEnabled(focusTrack))
        setTrackState(focusTrack, NoteButton::NoteButtonState::normal);

    //set new track in focus
    if (isEnabled(track))
    {
        bool bringToFront = true;
        setTrackState(track, NoteButton::NoteButtonState::higlighted, bringToFront);
        focusTrack = track;
    }
}

void PianoRoll::enableTrack(const int track)
{
    setTrackState(track, NoteButton::NoteButtonState::normal);
}

void PianoRoll::disableTrack(const int track)
{
    setTrackState(track, NoteButton::NoteButtonState::disabled);
}

void PianoRoll::setTrackState(const int track, const NoteButton::NoteButtonState state, const bool bringToFront)
{
    if (track >= 0 && track < noteButtonStart.size())
    {
        for (int i = noteButtonStart[track]; i <= noteButtonEnd[track]; i++)
        {
            jassert(i < noteButtons.size());
            NoteButton** rawPtr = noteButtons.getRawDataPointer();
            rawPtr[i]->setNoteButtonState(state);
            if (bringToFront)
                rawPtr[i]->toFront(false);
        }
    }
}

void PianoRoll::highlightNotes()
{
    double time;
    NoteButton** noteButtonsRawPtr = noteButtons.getRawDataPointer();

    //unhighlight all currently highlighted notes
    for (int i = 0; i < higlightedNotes.size(); i++)
    {
        noteButtonsRawPtr[higlightedNotes[i]]->setHighlightState(false);
        noteButtonsRawPtr[higlightedNotes[i]]->repaint();
    }
    if (higlightedNotes.size() > 0)
        higlightedNotes.clear();
    
    jassert(midiHelper.getMidiPlayer() != nullptr);
    Array<double> currentChordTimes = midiHelper.getMidiPlayer()->getCurrentlyActiveChordTimes();


    if (currentChordTimes.size() == 0)
        return;
    
    //FIXME : indentation
    for (int i=0; i < currentChordTimes.size(); i++)
    {
        time = currentChordTimes[i];
    

    //Find potential notes to higlight
    for (int j = 0; j < noteButtonStart.size(); j++)
    {
        int foundMatch = -1;
        int start = noteButtonStart[j];
        int end = noteButtonEnd[j];
        if (end < start)
            continue;
        int midpoint = -1;
        // All notes on a track a sorted in ascending time order.
        // Perform a binary search, to find a match. 
        do
        {
            midpoint = (start + end) / 2;
            
            double currTime = noteButtons[midpoint]->getMidiStartTime();
            if (fabs(currTime - time) < MidiHelpers::midiDoubleThreshold)
            {
                foundMatch = midpoint;
                break;
            }

            if (currTime < time)
            {
                start = midpoint+1;
            }
            else
            {
                end = midpoint-1;
            }

        } while (end > start);

        if (start >= noteButtonStart[j] &&
            start <= noteButtonEnd[j] &&
            fabs(noteButtons[start]->getMidiStartTime() - time) < MidiHelpers::midiDoubleThreshold)
        {
            foundMatch = start;
        }
        else if (end >= noteButtonStart[j] &&
            end <= noteButtonEnd[j] &&
            fabs(noteButtons[end]->getMidiStartTime() - time) < MidiHelpers::midiDoubleThreshold)
        {
            foundMatch = end;
        }


        //Got a match? Higlight all notes in the vinicinty.
        if (foundMatch != -1)
        {
            int k = foundMatch;
            //could be notes down
            do
            {
                noteButtonsRawPtr[k]->setHighlightState(true);
                noteButtonsRawPtr[k]->toFront(false);
                noteButtonsRawPtr[k]->repaint();
                higlightedNotes.add(k);
                k--;
            } while (k >= 0 && fabs(noteButtons[k]->getMidiStartTime() - time) < MidiHelpers::midiDoubleThreshold);
            k = foundMatch + 1;
            //or could be notes up
            while (k < noteButtons.size() && fabs(noteButtons[k]->getMidiStartTime() - time) < MidiHelpers::midiDoubleThreshold)
            {
                noteButtonsRawPtr[k]->setHighlightState(true);
                noteButtonsRawPtr[k]->toFront(false);
                noteButtonsRawPtr[k]->repaint();
                higlightedNotes.add(k);
                k++;
            }

        }
    }
        
    }
        
}

void PianoRoll::setMidiTime(const double time)
{

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());

    double sequenceStartTime = midiData.getStartTime();
    double sequenceEndTime = midiData.getEndTime();
    //double timePerPixel = (sequenceEndTime - sequenceStartTime) / (double)(getWidth() - xOffset);
    double pixelPerTime = (double)(getWidth() - xOffset) / (sequenceEndTime - sequenceStartTime);

    markerLineX = (int) (time * pixelPerTime) + xOffset;
    midiHelper.getMidiPlayer()->setMidiCounter(time);
    repaint();

}

void PianoRoll::reset()
{
    setFocusTrack(-1);

    noteButtons.clear();
    noteButtonStart.clear();
    noteButtonEnd.clear();
    higlightedNotes.clear();
    markerLineX = -1;
}
