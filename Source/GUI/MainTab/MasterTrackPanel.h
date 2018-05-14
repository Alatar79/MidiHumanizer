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

    MasterTrackPanel.h
    Created: 26 Jul 2015 6:20:07pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef MASTERTRACKPANEL_H_INCLUDED
#define MASTERTRACKPANEL_H_INCLUDED

#include "JuceHeader.h"
#include "../../AppData.h"
#include "../../GUI/MainTab/TrackPanel.h"
#include "../../Midi/MidiHelpers.h"

// Forward declaration in order to avaoid errors. Can't include the .h file. Circular inclusion??
class MainTab;

class MasterTrackPanel : public TrackPanelOblong
{
public:
    /** Constructor
    */
    MasterTrackPanel(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, MainTab& mainTab);
    void paint(Graphics& g) override; 

    /** Get a pointer to the main tab, which this object is part of.
    */
    void setAllTracksToCurrentMidiout();


private:

    /** Need access to the parent mainTab, in order to operate on all track panels. 
    */
    MainTab& mainTabRef;

    void comboBoxChanged(ComboBox* comboBox) override;
    void buttonClicked(Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterTrackPanel)
};




#endif  // MASTERTRACKPANEL_H_INCLUDED
