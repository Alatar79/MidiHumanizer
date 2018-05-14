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

    ExtendedMidiMessage.cpp
    Created: 25 Oct 2015 12:29:00pm
    Author:  MartinG

  ==============================================================================
*/

#include "ExtendedMidiMessage.h"
#include "../AppData.h"

//==============================================================================



ExtendedMidiMessage::ExtendedMidiMessage()
{
    trackRef = -1;
    eventIndex = -1;
    midiOut = nullptr;
    midiOutName = nullptr;
}

ExtendedMidiMessage::ExtendedMidiMessage(juce::MidiMessageSequence::MidiEventHolder& ev, MidiOutput* midiOut_, String* midiOutName_, int midiChannel_, int trackNo, int evtIdx)
    :MidiMessage(ev.message)
{
    this->trackRef = trackNo;
    this->eventIndex = evtIdx;
    this->midiOut = midiOut_;
    this->midiOutName = midiOutName_;
    setChannel(midiChannel_);

}

ExtendedMidiMessage::~ExtendedMidiMessage()
{
    trackRef = -1;
    eventIndex = -1;
    midiOut = nullptr;
    midiOutName = nullptr;
}


ExtendedMidiMessage& ExtendedMidiMessage::setMidiMessage(MidiMessage& message)
{
    MidiMessage::operator=(message);
    return *this;
}

MidiOutput* ExtendedMidiMessage::getMidiOut()
{
    return midiOut;
}

const String* ExtendedMidiMessage::getMidiOutName() const
{
    if (midiOutName != nullptr)
        return midiOutName;
    else return
        &AppData::noMidiSelected;

}

int ExtendedMidiMessage::getTrackIndex() const
{
    return trackRef;
}

int ExtendedMidiMessage::getEventIndex() const
{
    return eventIndex;
}
