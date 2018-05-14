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

    NoteCounter.h
    Created: 27 Jul 2015 10:37:54am
    Author:  MartinG

  ==============================================================================
*/

#ifndef NOTECOUNTER_H_INCLUDED
#define NOTECOUNTER_H_INCLUDED

#include "JuceHeader.h"
#include "ExtendedMidiMessageSequence.h"
#include "MidiSequence.h"

/* This class takes care of counting which notes have been associated with which trigger(s).
*/
class NoteCounter
{
public:

    static const int maxMidiNotes = 128;

    /** Each Midi lane is a structure, which is unique in respect to Midi (Output)Name and Midi Channel.
        It keeps a record of all its notes and triggers. 
    */
    struct MidiLaneEntry
    {
        String midiName;
        int midiChannel;
        /** Two dimensional array. A note can have several triggers.
            Account for those triggers. 
        */
        Array<int> triggerColletion[maxMidiNotes];

    };
    /** Constructor
    */
    NoteCounter();

    /** Destructor
    */
    ~NoteCounter();

    /* Add a note and its associated trigger
    */
    void add(const ExtendedMidiMessage& note, const MidiMessage& trigger);

    /* Remove a trigger, which is associated with a note.
    */
    void remove(const ExtendedMidiMessage& note, const MidiMessage& trigger);

    /** Checks, whether a certain note is no longer associated with the given trigger.
    */
    bool isClear(const ExtendedMidiMessage& note);

    /** Find the MidiLaneEntry, which has the same channel number and Midi (ouput) name as the note.
        If none can be found, a nullptr is returned. 
    */
    MidiLaneEntry* getMidiLaneFromNote(const ExtendedMidiMessage& note);

    /** Reset all midi lanes. E.g. when loading a new file. 
    */
    void reset();

private:

    /** Check if an MidiLaneEntry is "equal" to an ExtendedMidiMessage.
        Two things are compared:
        - Midi Device Name
        - Midi Channel
    */
    bool noteIsPartOfMidiLane(const MidiLaneEntry& midiLaneEntry, const ExtendedMidiMessage& note) const;

    Array<MidiLaneEntry> midiLane;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteCounter);
};


#endif  // NOTECOUNTER_H_INCLUDED
