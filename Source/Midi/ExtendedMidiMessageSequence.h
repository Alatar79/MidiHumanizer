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

    ExtendedMidiMessageSequence.h
    Created: 13 Jul 2015 7:00:39pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef EXTENDEDMIDIMESSAGESEQUENCE_H_INCLUDED
#define EXTENDEDMIDIMESSAGESEQUENCE_H_INCLUDED

#include "JuceHeader.h"

/**

Extends the MidiMessageSequence class with additional functionality, such as "is enabled for playback" and
midi out. 

*/
class ExtendedMidiMessageSequence : public MidiMessageSequence
{
public:

    //ExtendedMidiMessageSequence(const MidiMessageSequence* msg);

    /** Creates an empty midi sequence object. */
    ExtendedMidiMessageSequence();

    /** Creates a copy of another sequence. */
    ExtendedMidiMessageSequence(const ExtendedMidiMessageSequence& other);

    /** Replaces this sequence with another one. */
    ExtendedMidiMessageSequence& operator= (const ExtendedMidiMessageSequence& other);

    /** Creates an ExtendedMidiMessageSequence from a MidiMessageSequence. */
    ExtendedMidiMessageSequence(const MidiMessageSequence& mmSequence);

    /** Destructor
    */
    ~ExtendedMidiMessageSequence();

    /** Returns the index of the event, which is closest to "timestamp".
        If the time is beyond the end of the sequence, this will return the number of events.
        Binary search is used as the search algorithm.
    */
    int getClosestIndexAtTimeBinarySearch(const double timeStamp) const;

    /** Returns the index of the first event on or after the given timestamp.
        If the time is beyond the end of the sequence, this will return the number of events.
        Binary serach is used as the algorithm. 
    */
    int getNextIndexAtTimeBinarySearch(const double timeStamp) const;

    /** Returns the index of the first event on or before the given timestamp.
    If the time is beyond the end of the sequence, this will return zero.
    */
    //int getPreviousIndexAtTime(const double timeStamp) const;

    /** Returns the index of the first event on or before the given timestamp.
        If the time is beyond the end of the sequence, this will return zero.
        Binary serach is used as the algorithm.
    */
    int getPreviousIndexAtTimeBinarySearch(const double timeStamp) const;

    /** Check if we are enabled for playback.
    */
    bool isEnabled() const;
    /** Disable this sequence for playback. 
    */
    void disable();
    /** Enable this sequence for playback.
    */
    void enable();
    /** Set the Midi Output, which we send the midi data to.
        Only the String-name is needed. The corresponding midi device pointer
        is figured out by the function.
    */
    void setMidiOut(const String& name);
    /* Set the Midi channel.
    */
    void setMidiChannel(const int channel);

    /** Get the Midi Output, which we send the midi data to
    */
    MidiOutput* getMidiOut();

    /** Set the Name of the MidiOut device
    */
    //void setMidiOutName(const String& name);

    /** Get the Name of the MidiOut device
    */
    String getMidiOutName() const;

    /** Get the Name of the MidiOut device. But return only a pointer to the name.
    */
    String* getMidiOutNamePtr();
    /** Get the midi channel this track is using. 
    */
    int getMidiChannel() const;

private:
    /** Reset all variabels associated with the Midi output 
    */
    void resetMidi();
    /** Generate a random name for debugging 
    */
    String generateName();
    /** Is this sequence currently enabled for playback?
    */
    bool enabled;
    /** the Midi Output
    */
    ScopedPointer<MidiOutput> midiOut;
    /** The name of the midi output.
    */
    String midiOutName;
    /** The midi channel, which this track should use. 
    */
    int midiChannel;


    JUCE_LEAK_DETECTOR(ExtendedMidiMessageSequence);

};

#endif  // EXTENDEDMIDIMESSAGESEQUENCE_H_INCLUDED
