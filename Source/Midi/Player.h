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

    Player.h
    Created: 21 Jun 2015 12:54:38pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "JuceHeader.h"
#include "MidiSequence.h"
#include "ExtendedMidiMessage.h"
#include "NoteCounter.h"

/** This class manages a whole midi file, i.e. several midi sequences. 
*/
class Player
{
public:
    Player();
    ~Player();
    /** Play notes, i.e. send them to the midi device. Keep track of trigger and notes for later use (note-off).
    */
    void play(const Array<ExtendedMidiMessage>& notes, const MidiMessage& triggerEvent);
    /** Send/play the Midi CCs
    */
    void sendMidiCCs(Array<ExtendedMidiMessage>& midiCCs);
    /** Only add notes and trigger to internal database, but do not play them. 
    */
    bool addWithoutPlaying(const Array<ExtendedMidiMessage>& notes, const MidiMessage& triggerEvent);
    /** Stop key associated with triggerEvent.
    */
    void stop(const MidiMessage& triggerEvent);
    /** Get the chord times from all currently active triggers. 
     */
    Array<double> getCurrentlyActiveChordTimes() const;
    /** Reset all data, e.g. when loading a file.
    */
    void reset();


private:
    /** Check if a given midiOut String is part of the array notes. 
    */
    bool existsMidiOutInNotes(const String& midiOut, Array<TriggerNote>& notes) const;

    /** This is used to make sure only one thread at a time can access the player object.
    */
    CriticalSection lock;

    static const int maxMidiNotes = 128;
    Array<ExtendedMidiMessage> notesByTrigger[maxMidiNotes];
    /**list of all notes currently playing, i.e. which are noteOn. The value is the note, which triggered them. 
    */
    //Array<TriggerNote> triggerByNotes[maxMidiNotes];
    /**list of all notes and their associated triggers
    */
    NoteCounter noteCounter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Player);

};





#endif  // PLAYER_H_INCLUDED
