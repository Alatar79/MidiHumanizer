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

    ExtendedMidiMessageSequence.cpp
    Created: 13 Jul 2015 7:00:39pm
    Author:  MartinG

  ==============================================================================
*/

#include "ExtendedMidiMessageSequence.h"
#include "../AppData.h"
#include "MidiHelpers.h"






/** Creates an empty midi sequence object. */
ExtendedMidiMessageSequence::ExtendedMidiMessageSequence()
    :MidiMessageSequence()
{
    enabled = true;
    resetMidi();
}

ExtendedMidiMessageSequence::~ExtendedMidiMessageSequence()
{
    resetMidi();
}

/** Creates a copy from another sequence. */
ExtendedMidiMessageSequence::ExtendedMidiMessageSequence(const ExtendedMidiMessageSequence& other)
    : MidiMessageSequence(other)
{
    this->enabled = other.enabled;
    //cannot copy midiOut, because it is a unique pointer!
    this->resetMidi();
}

ExtendedMidiMessageSequence::ExtendedMidiMessageSequence(const MidiMessageSequence& mmSequence)
    : MidiMessageSequence(mmSequence)
{
    this->enabled = true;
    this->resetMidi();
}


/** Replaces this sequence with another one. */
ExtendedMidiMessageSequence& ExtendedMidiMessageSequence::operator= (const ExtendedMidiMessageSequence& other)
{
    MidiMessageSequence::operator=(other);
    this->enabled = other.enabled;
    //cannot copy midiOut, because it is a unique pointer!
    this->resetMidi();
    return *this;
}

int ExtendedMidiMessageSequence::getClosestIndexAtTimeBinarySearch(const double timeStamp) const
{
    if (getNumEvents() == 0)
        return getNumEvents();

    int foundMatch = -1;
    int start = 0;
    int end = getNumEvents() - 1;
    int midpoint = -1;
    // All events on the track should be sorted in ascending time order.
    // Perform a binary search, to find the closest match for timeStamp. 
    do
    {
        midpoint = (start + end) / 2;

        double currTime = getEventPointer(midpoint)->message.getTimeStamp();
        if (fabs(currTime - timeStamp) < MidiHelpers::doubleComparisonThreshold)
        {
            foundMatch = midpoint;
            return foundMatch;
        }
        else if (currTime < timeStamp)
        {
            start = midpoint + 1;
        }
        else  /* (currTime > timeStamp) */
        {
            end = midpoint - 1;
        }

    } while (end > start);

    return midpoint;

}

/*
int ExtendedMidiMessageSequence::getPreviousIndexAtTime(const double timeStamp) const
{
    const int numEvents = getNumEvents();

    int i;
    for (i = numEvents - 1; i > 0; --i)
        if (getEventPointer(i)->message.getTimeStamp() <= timeStamp)
            break;

    return i;
}
*/


int ExtendedMidiMessageSequence::getNextIndexAtTimeBinarySearch(const double timeStamp) const
{
    int foundMatch = getClosestIndexAtTimeBinarySearch(timeStamp);

    int newStart = foundMatch - 2;
    if (newStart < 0) 
        newStart = 0;

    double newStartTime = getEventPointer(newStart)->message.getTimeStamp();
    int newNewStart = newStart;
    //we need to find the first index, where the time stamp changes. 
    for (newNewStart = newStart; newNewStart > 0; newNewStart--)
    {
        double newNewStartTime = getEventPointer(newNewStart)->message.getTimeStamp();
        if (newNewStartTime < newStartTime)
            break;
    }

    //start from the closest point to the timeStamp, which getClosestIndexAtTimeBinarySearch gave us. 
    //There should be a match nearby (if it exists at all). But it could also be farther away. 
    for (int i = newNewStart; i < getNumEvents(); i++)
    {
        if (getEventPointer(i)->message.getTimeStamp() >= timeStamp)
        {
            foundMatch = i;
            break;
        }
    }

    if (foundMatch < getNumEvents())
        return foundMatch;
    else 
        return getNumEvents();
}


int ExtendedMidiMessageSequence::getPreviousIndexAtTimeBinarySearch(const double timeStamp) const
{
    int foundMatch = getClosestIndexAtTimeBinarySearch(timeStamp);

    int newEnd = foundMatch + 2;
    if (newEnd >(getNumEvents() - 1))
        newEnd = getNumEvents() - 1;

    double newEndTime = getEventPointer(newEnd)->message.getTimeStamp();
    int newNewEnd = newEnd;
    //we need to find the first index, where the time stamp changes. 
    for (;newNewEnd < getNumEvents()-1; newNewEnd++)
    {
        double newNewEndTime = getEventPointer(newNewEnd)->message.getTimeStamp();
        if (newNewEndTime > newEndTime)
            break;
    }

    //start from the closest point to the timeStamp, which getClosestIndexAtTimeBinarySearch gave us. 
    //There should be a match nearby (if it exists at all). But it could also be farther away. 
    for (int i = newNewEnd; i >= 0; i--)
    {
        if (getEventPointer(i)->message.getTimeStamp() <= timeStamp)
        {
            foundMatch = i;
            break;
        }
    }

    if (foundMatch < getNumEvents())
        return foundMatch;
    else
        return 0;
}



bool ExtendedMidiMessageSequence::isEnabled() const 
{
    return enabled;
}

void ExtendedMidiMessageSequence::disable()
{
    enabled = false;
}

void ExtendedMidiMessageSequence::enable()
{
    enabled = true;
}

MidiOutput* ExtendedMidiMessageSequence::getMidiOut()
{
    //TODO: this is a bit dangerous, returning a raw ptr from a unique_ptr.
    //It works, because the life time of this sequence should be longer than the callers lifetime. But not ideal. 
    return midiOut.get();
}

void ExtendedMidiMessageSequence::setMidiOut(const String& name)
{
    setMidiOut(name, juce::MidiOutput::getAvailableDevices());
}

void ExtendedMidiMessageSequence::setMidiOut(const String& name, const Array<MidiDeviceInfo>& availableMidiDevices)
{
    //Yes, always set the name, even if we cannot find it on the list of MidiOuts.
    //Reason: user might just have forgotten to turn on his midi device.
    //and we do not want to delete the information, in case user re-saves.
    midiOutName = name;
    int index = -1;
    for (int i = 0; i < availableMidiDevices.size(); ++i)
    {
        if (availableMidiDevices[i].name == name)
        {
            index = i;
            break;
        }
            
    }
    if (index != -1)
        midiOut = juce::MidiOutput::openDevice(availableMidiDevices[index].identifier);
    else
        midiOut = nullptr;
}

void ExtendedMidiMessageSequence::setMidiChannel(const int channel)
{
    jassert(channel > 0 && channel <= 16);
    midiChannel = channel; 
}


String ExtendedMidiMessageSequence::getMidiOutName() const
{
    return midiOutName;
}

String* ExtendedMidiMessageSequence::getMidiOutNamePtr()
{
    return &midiOutName;
}

int ExtendedMidiMessageSequence::getMidiChannel() const
{
    return midiChannel;
}

void ExtendedMidiMessageSequence::resetMidi()
{
    midiOut = nullptr;
    midiOutName = AppData::noMidiSelected;
    midiChannel = 1;
}

String ExtendedMidiMessageSequence::generateName()
{
    String s; 
    Random rnd;

    for (int i = 0; i < 10; i++)
    {
        char c = (rnd.nextInt() % 25) + 65; 
        s = s + c;
    }

    return s;

}
