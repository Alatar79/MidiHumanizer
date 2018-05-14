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

    MainController.h
    Created: 14 Jan 2018 2:50:36pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef MAINCONTROLLER_H_INCLUDED
#define MAINCONTROLLER_H_INCLUDED

#include "JuceHeader.h"
#include "Midi/MidiSequence.h"
#include "Midi/MidiThread.h"
#include <math.h>

/** Forward declration, because of circular references */
class MainTab; 

/** This class controls the GUI and the Midi thread. 
*/
class MainController : public Timer
{
public:
    MainController(MainTab& mainTabIn, MidiHelpers& midiHelperIn );
    ~MainController();

    /** Callback for processing midi */
    void timerCallback() override;

private:

    /** This is used to make sure the timer does not run into itself. */
    CriticalSection lock;

    /** Reference to the tab for the main application screen */
    MainTab& mainTab;

    /** Collection of common midi functions. */
    MidiHelpers& midiHelper;

    /** Midi processor. */
    MidiThread midiThread;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainController);
};




#endif  // MAINCONTROLLER_H_INCLUDED
