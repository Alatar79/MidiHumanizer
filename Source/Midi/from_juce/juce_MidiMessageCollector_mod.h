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

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_MIDIMESSAGECOLLECTOR_MOD_H_INCLUDED
#define JUCE_MIDIMESSAGECOLLECTOR_MOD_H_INCLUDED

#include "../../AppData.h"
#include "JuceHeader.h"

//==============================================================================
/**
    Collects incoming realtime MIDI messages and turns them into blocks suitable for
    processing by a block-based audio callback.

    The class can also be used as either a MidiKeyboardStateListener or a MidiInputCallback
    so it can easily use a midi input or keyboard component as its source.

    @see MidiMessage, MidiInput
*/
class JUCE_API  MidiMessageCollectorMod    : public MidiKeyboardStateListener,
                                          public MidiInputCallback
{
public:
    //==============================================================================
    /** Creates a MidiMessageCollectorMod. */
    MidiMessageCollectorMod();

    /** Destructor. */
    ~MidiMessageCollectorMod();

    //==============================================================================
    /** Clears any messages from the queue.

        You need to call this method before starting to use the collector, so that
        it knows the correct sample rate to use.
    */
    void reset (double sampleRate);

    /** Takes an incoming real-time message and adds it to the queue.

        The message's timestamp is taken, and it will be ready for retrieval as part
        of the block returned by the next call to removeNextBlockOfMessages().

        This method is fully thread-safe when overlapping calls are made with
        removeNextBlockOfMessages().
    */
    void addMessageToQueue (const MidiMessage& message);

    /** Removes all the pending messages from the queue as a buffer.

        This will also correct the messages' timestamps to make sure they're in
        the range 0 to numSamples - 1.

        This call should be made regularly by something like an audio processing
        callback, because the time that it happens is used in calculating the
        midi event positions.

        This method is fully thread-safe when overlapping calls are made with
        addMessageToQueue().

        Precondition: numSamples must be greater than 0.
    */
    void removeNextBlockOfMessages (MidiBuffer& destBuffer, int numSamples);


    //==============================================================================
    /** @internal */
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    /** @internal */
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    /** @internal */
    void handleIncomingMidiMessage (MidiInput*, const MidiMessage&) override;

private:
    //==============================================================================
    double lastCallbackTime;
    CriticalSection midiCallbackLock;
    MidiBuffer incomingMessages;
    double sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessageCollectorMod)
};


#endif   // JUCE_MidiMessageCollectorMod_H_INCLUDED
