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

#include "juce_MidiMessageCollector_mod.h"
#include "../MidiHelpers.h"

MidiMessageCollectorMod::MidiMessageCollectorMod()
    : lastCallbackTime (0),
      sampleRate (44100.0001)
{
}

MidiMessageCollectorMod::~MidiMessageCollectorMod()
{
}

//==============================================================================
void MidiMessageCollectorMod::reset (const double sampleRate_)
{
    jassert (sampleRate_ > 0);

    const ScopedLock sl (midiCallbackLock);
    sampleRate = sampleRate_;
    incomingMessages.clear();
    lastCallbackTime = Time::getMillisecondCounterHiRes();
}

void MidiMessageCollectorMod::addMessageToQueue (const MidiMessage& message)
{
    // you need to call reset() to set the correct sample rate before using this object
    jassert (sampleRate != 44100.0001);

    // the messages that come in here need to be time-stamped correctly - see MidiInput
    // for details of what the number should be.
    jassert (message.getTimeStamp() != 0);

    const ScopedLock sl (midiCallbackLock);

    const int sampleNumber
        = (int) ((message.getTimeStamp() - 0.001 * lastCallbackTime) * sampleRate);

    incomingMessages.addEvent (message, sampleNumber);

    // if the messages don't get used for over a second, we'd better
    // get rid of any old ones to avoid the queue getting too big
    if (sampleNumber > sampleRate)
        incomingMessages.clear (0, sampleNumber - (int) sampleRate);
}

void MidiMessageCollectorMod::removeNextBlockOfMessages (MidiBuffer& destBuffer,
                                                      const int numSamples)
{
    // you need to call reset() to set the correct sample rate before using this object
    jassert (sampleRate != 44100.0001);
    jassert (numSamples > 0);

    const double timeNow = Time::getMillisecondCounterHiRes();
    const double msElapsed = timeNow - lastCallbackTime;

    const ScopedLock sl (midiCallbackLock);
    lastCallbackTime = timeNow;

    if (! incomingMessages.isEmpty())
    {
        int numSourceSamples = jmax (1, roundToInt (msElapsed * 0.001 * sampleRate));

        int startSample = 0;
        int scale = 1 << 16;

        const uint8* midiData;
        int numBytes, samplePosition;

        MidiBuffer::Iterator iter (incomingMessages);

        if (numSourceSamples > numSamples)
        {
            // if our list of events is longer than the buffer we're being
            // asked for, scale them down to squeeze them all in..
            const int maxBlockLengthToUse = numSamples << 5;

            if (numSourceSamples > maxBlockLengthToUse)
            {
                startSample = numSourceSamples - maxBlockLengthToUse;
                numSourceSamples = maxBlockLengthToUse;
                iter.setNextSamplePosition (startSample);
            }

            scale = (numSamples << 10) / numSourceSamples;

            while (iter.getNextEvent (midiData, numBytes, samplePosition))
            {
                samplePosition = ((samplePosition - startSample) * scale) >> 10;

                destBuffer.addEvent (midiData, numBytes,
                                     jlimit (0, numSamples - 1, samplePosition));
            }
        }
        else
        {
            // if our event list is shorter than the number we need, put them
            // towards the end of the buffer
            startSample = numSamples - numSourceSamples;

            while (iter.getNextEvent (midiData, numBytes, samplePosition))
            {
                destBuffer.addEvent (midiData, numBytes,
                                     jlimit (0, numSamples - 1, samplePosition + startSample));
            }
        }

        incomingMessages.clear();
    }
}

//==============================================================================
void MidiMessageCollectorMod::handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);

    addMessageToQueue (m);
}

void MidiMessageCollectorMod::handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);

    addMessageToQueue (m);
}

void MidiMessageCollectorMod::handleIncomingMidiMessage (MidiInput*, const MidiMessage& message)
{
#ifdef DEBUG    
    if (message.isNoteOn())
    {
        dbg_midiInTime = Time::getMillisecondCounterHiRes();
        //DBG("Real Start Time: " + String(dbg_midiInTime));
    }
#endif

    addMessageToQueue (message);
}
