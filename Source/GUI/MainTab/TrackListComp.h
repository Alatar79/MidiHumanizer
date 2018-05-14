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

    TrackListComp.h
    Created: 13 Jul 2015 11:01:40am
    Author:  MartinG

  ==============================================================================
*/

#ifndef TRACKLISTCOMP_H_INCLUDED
#define TRACKLISTCOMP_H_INCLUDED

#include "JuceHeader.h"
#include "../../AppData.h"
#include "TrackPanel.h"
#include "../../Midi/MidiHelpers.h"

//==============================================================================
/*
*/
class TrackListComp    : public Component
{
public:
    TrackListComp(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll);
    ~TrackListComp();

    void paint (Graphics& g);
    void resized();

    /** Indicates that a new file is laoded and we have to change the trac list
    */
    void newFileLoaded();

    /** Get a pointer to the trackPanels. 
    */
    OwnedArray<TrackPanel>& getTrackPanelList();


private:

    /** Access to common midi functions
    */
    MidiHelpers& midiHelper;
    /** Reference to the application's main window. 
    */
    MainAppWindow& appWindowRef;

    /** The track panels can modify the piano roll. 
    */
    PianoRoll& pianoRollRef;

    OwnedArray<TrackPanel> trackPanels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackListComp)
};


#endif  // TRACKLISTCOMP_H_INCLUDED
