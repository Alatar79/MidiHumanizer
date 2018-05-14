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

    NoteCounter.cpp
    Created: 27 Jul 2015 10:37:54am
    Author:  MartinG

  ==============================================================================
*/

#include "NoteCounter.h"

NoteCounter::NoteCounter()
{
    reset();
}

NoteCounter::~NoteCounter()
{
    reset();
}

void NoteCounter::add(const ExtendedMidiMessage& note, const MidiMessage& trigger)
{
    int idx = -1;
    //do we already have a midi lane for the midi out name?
    for (int i = 0; i < midiLane.size(); i++)
    {
        if (noteIsPartOfMidiLane(midiLane[i], note))
        {
            idx = i;
            break;
        }
    }

    //if not, create one
    if (idx == -1)
    {
        MidiLaneEntry mlEntry;
        mlEntry.midiName = *(note.getMidiOutName());
        mlEntry.midiChannel = note.getChannel();
        midiLane.add(mlEntry);
        idx = midiLane.size() - 1;
    }

    //add trigger to the collection
    MidiLaneEntry* rawPtr = midiLane.getRawDataPointer();
    rawPtr[idx].triggerColletion[note.getNoteNumber()].add(trigger.getNoteNumber());
}

void NoteCounter::remove(const ExtendedMidiMessage& note, const MidiMessage& trigger)
{

    for (int i = 0; i < midiLane.size(); i++)
    {
        if (noteIsPartOfMidiLane(midiLane[i], note))
        {
            int noteNumber = note.getNoteNumber();
            MidiLaneEntry* rawPtr = midiLane.getRawDataPointer();
            rawPtr[i].triggerColletion[noteNumber].removeFirstMatchingValue(trigger.getNoteNumber());
            break;
        }
    }


}

bool NoteCounter::isClear(const ExtendedMidiMessage& note)
{

    for (int i = 0; i < midiLane.size(); i++)
    {
        if (noteIsPartOfMidiLane(midiLane[i], note))
        {
            int noteNumber = note.getNoteNumber();
            MidiLaneEntry* rawPtr = midiLane.getRawDataPointer();
            if (rawPtr[i].triggerColletion[noteNumber].size() != 0)
                    return false;
        }
    }

    return true;
}

NoteCounter::MidiLaneEntry* NoteCounter::getMidiLaneFromNote(const ExtendedMidiMessage& note)
{
    for (int i = 0; i < midiLane.size(); i++)
    {
        if (noteIsPartOfMidiLane(midiLane[i], note))
        {
            return &(midiLane.getReference(i));
        }
    }

    return nullptr;
}


void NoteCounter::reset()
{
    for (int i = 0; i < midiLane.size(); i++)
    {
        for (int j = 0; j < maxMidiNotes; j++)
        {
            midiLane[i].triggerColletion[j].clear();
        }
    }

    midiLane.clear();
}


bool NoteCounter::noteIsPartOfMidiLane(const MidiLaneEntry& midiLaneEntry, const ExtendedMidiMessage& note) const
{
    if (midiLaneEntry.midiChannel == note.getChannel() &&
        midiLaneEntry.midiName == *(note.getMidiOutName()))
    {
        return true;
    }
    else
    {
        return false;
    }
        

}
