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

    MidiHelpers.cpp
    Created: 17 Dec 2017 7:48:07pm
    Author:  MartinG

  ==============================================================================
*/

#include "MidiHelpers.h"

double MidiHelpers::midiDoubleThreshold = 1e-4;
float MidiHelpers::midiFloatThreshold = (float)MidiHelpers::midiFloatThreshold;
const double MidiHelpers::doubleComparisonThreshold = 1e-9;


MidiHelpers::MidiHelpers()
{
    midiPlayer = nullptr;
    midiMessageCollector = nullptr;
    midiOut = nullptr;
   
    //workaround, because sometimes MidiOutput::getDevices() can fail, if you call it a second time.
    //therefore store the names here, after the first call.
    Array<MidiDeviceInfo> devices = juce::MidiOutput::getAvailableDevices();
    for (const MidiDeviceInfo& device : devices)
        midiInputs.add(device.name);    
}

StringArray& MidiHelpers::getMidiInputs()
{
    return midiInputs;
}

void MidiHelpers::setMidiChordThreshold(const double threshold)
{
    midiDoubleThreshold = threshold;
    midiFloatThreshold = (float)threshold;
}

double MidiHelpers::getMidiChordThreshold() const
{
    return midiDoubleThreshold;
}

MidiPlayer* MidiHelpers::getMidiPlayer()
{
    //Should never be null, if we did everything correctly. 
    jassert(midiPlayer != nullptr);
    return midiPlayer;
}

void MidiHelpers::setMidiPlayer(MidiPlayer* midiPlayer_)
{
    this->midiPlayer = midiPlayer_;
}

MidiMessageCollectorMod* MidiHelpers::getMidiMessageCollector()
{
    return midiMessageCollector;
}

void MidiHelpers::setMidiMessageCollector(MidiMessageCollectorMod* midiMessageCollector_)
{
    this->midiMessageCollector = midiMessageCollector_;
}

