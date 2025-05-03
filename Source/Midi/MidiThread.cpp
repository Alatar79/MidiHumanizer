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

    MidiThread.cpp
    Created: 10 Aug 2015 1:21:34pm
    Author:  MartinG

  ==============================================================================
*/

#include "MidiThread.h"

const double MidiThread::sampleRate = 1000;

MidiThread::MidiThread(MidiHelpers& midiHelperIn)
    : Thread("MidiThread"),
      midiHelper(midiHelperIn)

{
    //highest possible priority
    //TODO: Fix set priority, which no longer exists in latest JUCE
    //setPriority(10);
    midiCollector.reset(sampleRate);
    //appData.keyboardState.addListener(&midiCollector);
    midiHelper.setMidiPlayer(&midiPlayer);
    midiHelper.setMidiMessageCollector(&midiCollector);
}

void MidiThread::run()
{
    double lastCallbackTime = Time::getMillisecondCounterHiRes();

    bool continueRunning = true;

    do
    {
        double timeNow = Time::getMillisecondCounterHiRes();
        double deltaTime = (timeNow - lastCallbackTime) * 0.001;
        //make sure, we dont run this faster than once every millisecond. 
        while (deltaTime < 0.001)
        {
            wait(1);
            timeNow = Time::getMillisecondCounterHiRes();
            deltaTime = (timeNow - lastCallbackTime) * 0.001;
        }
        lastCallbackTime = timeNow;

        int numSamples = (int)(deltaTime * sampleRate);
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, numSamples);
        //input the current midi buffer to the virtual keyboard and add events from it. 
        midiHelper.virtualKeyboardState.processNextMidiBuffer(incomingMidi, 0, numSamples, true);
        midiPlayer.processNextMidiBlock(incomingMidi, 0, numSamples);

        if (threadShouldExit())
            continueRunning = false;
    } while (continueRunning);

    return;

}

MidiPlayer& MidiThread::getMidiPlayer()
{
    return midiPlayer;
}
