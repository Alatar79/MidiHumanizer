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

    MidiThread.h
    Created: 10 Aug 2015 1:21:34pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef MIDITHREAD_H_INCLUDED
#define MIDITHREAD_H_INCLUDED

#include "JuceHeader.h"
#include "MidiPlayer.h"
#include "../AppData.h"
#include "from_juce/juce_MidiMessageCollector_mod.h"
#include "MidiHelpers.h"



class MidiThread : public juce::Thread
{
public:
    MidiThread(MidiHelpers& midiHelperIn);
    
    /** Here all the Midi logic is implemented. */
    void run() override;

    /** Return a handle to the midi player */
    MidiPlayer& getMidiPlayer();
private:

    /** The callback/collector for all midi events. */
    MidiMessageCollectorMod midiCollector;
    /** Collection of midi helper functions */
    MidiHelpers& midiHelper;
    /** the midi player */
    MidiPlayer midiPlayer;
    /** "Sample rate" to use */
    static const double sampleRate;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiThread);
};




#endif  // MIDITHREAD_H_INCLUDED
