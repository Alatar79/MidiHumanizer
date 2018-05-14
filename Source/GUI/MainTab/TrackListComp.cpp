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

    TrackListComp.cpp
    Created: 13 Jul 2015 11:01:40am
    Author:  MartinG

  ==============================================================================
*/

#include "JuceHeader.h"
#include "../../Midi/MidiPlayer.h"
#include "TrackListComp.h"


//==============================================================================
TrackListComp::TrackListComp(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll)
    : midiHelper(midiHelperIn),
      appWindowRef(appWindow),
      pianoRollRef(pianoRoll)
{

}

TrackListComp::~TrackListComp()
{
}

void TrackListComp::newFileLoaded()
{
    //Deletes all panels.
    //According to the JUCE API, this should automatically remove the 
    //component from the TrackListComp. 
    trackPanels.clear();

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
    for (int i = 0; i < midiData.getNoOfTracks(); i++)
    {
        String trackName = midiData.getTrackName(i);
        String midiName = midiData.getMidiOutName(i);
        int midiChannel = midiData.getMidiChannel(i);
        bool isEnabled = midiData.isEnabled(i);
        TrackPanel* trackPanel = new TrackPanel(midiHelper, appWindowRef, pianoRollRef, trackName, midiName, midiChannel, isEnabled, i);
        //Add track panel. It is now owned by the owned array trackPanels. 
        trackPanels.add(trackPanel);
        addAndMakeVisible(trackPanel);

    }

    resized();
    repaint();
}

void TrackListComp::paint (Graphics& /*g*/)
{

    //g.fillAll (Colour(AppData::mainBackgroundColour));   // clear the background

}

void TrackListComp::resized()
{

    setBounds(0, 0,
        AppData::scalei(AppData::trackPanelWidth),
        AppData::scalei(AppData::trackPanelOuterHeight) * trackPanels.size() - AppData::scalei(AppData::trackPanelBorder));

    for (int i = 0; i < trackPanels.size(); i++)
    {
        trackPanels[i]->setBounds(1, i * AppData::scalei(AppData::trackPanelOuterHeight), AppData::scalei(AppData::trackPanelWidth), AppData::scalei(AppData::trackPanelHeight));
    }

}

OwnedArray<TrackPanel>& TrackListComp::getTrackPanelList()
{
    return trackPanels;
}
