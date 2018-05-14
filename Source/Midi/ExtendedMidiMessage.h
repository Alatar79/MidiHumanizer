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

    ExtendedMidiMessage.h
    Created: 25 Oct 2015 12:29:00pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef EXTENDEDMIDIMESSAGE_H_INCLUDED
#define EXTENDEDMIDIMESSAGE_H_INCLUDED

#include "JuceHeader.h"

/** This class adds functionality and info variables to the juce MidiMessage
*/
class ExtendedMidiMessage : public MidiMessage
{
public:
    /** Create an ExtendedMidiMessage object from an MidiEventHolder object + the track to which it belongs.
    */
    ExtendedMidiMessage(juce::MidiMessageSequence::MidiEventHolder& ev, MidiOutput* midiOut_, String* midiOutName_, int midiChannel_, int trackNo, int evtIdx);

    /** Default Constructor.
    */
    
    ExtendedMidiMessage();

    /** Set the MidiMessage (i.e. the base class)
    */
    ExtendedMidiMessage& setMidiMessage(MidiMessage& message);

    /** Destructor
    */
    ~ExtendedMidiMessage();

    //Copy constructor should not be needed. Standard copy constructor should suffice.
    //ExtendedMidiMessage(const ExtendedMidiMessage& other);

    //Should not need assignment operator. Default assignment should suffice. 
    //ExtendedMidiMessage& ExtendedMidiMessage::operator= (const ExtendedMidiMessage&);

    // No destructor needed. Nothing to destruct. 
    // ~ExtendedMidiMessage();

    /** Get the midi out device
    */
    MidiOutput* getMidiOut();
    /** Get the name of the midi out device.
    */
    const String* getMidiOutName() const;
    /** Get the track number, which this MidiMessage belongs to 
    */
    int getTrackIndex() const;
    /** Get the index inside the track, which this MidiMessage belongs to
    */
    int getEventIndex() const;

private:
    /** track, which this ExtendedMidiMesssage is belonging to.
    */
    int trackRef;
    /** Index in the MidiMessageSequence, where this MidiMessage comes from
    */
    int eventIndex; 
    /** The midi out device assigned to the track.
    */
    MidiOutput* midiOut;
    /** Name of the midi out device assigned to this track
    */
    String* midiOutName;

    JUCE_LEAK_DETECTOR(ExtendedMidiMessage);
};



#endif  // EXTENDEDMIDIMESSAGE_H_INCLUDED
