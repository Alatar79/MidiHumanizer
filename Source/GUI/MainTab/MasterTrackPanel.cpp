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

    MasterTrackPanel.cpp
    Created: 26 Jul 2015 6:20:07pm
    Author:  MartinG

  ==============================================================================
*/

#include "MasterTrackPanel.h"
#include "../../GUI/MainTab/MainTab.h"


MasterTrackPanel::MasterTrackPanel(MidiHelpers& midiHelper, MainAppWindow& appWindow, MainTab& mainTab)
    :TrackPanelOblong(midiHelper, appWindow, mainTab.getPianoRoll(), "Change ALL tracks:", AppData::noMidiSelected, 1, true, -1),
    mainTabRef(mainTab)
{
    midiOutComboBox.setTooltip("This application produces no sound.\n It only produces midi out. Select a midi output for all tracks.");
    midiChannelComboBox.setTooltip("Select Midi channel for all tracks.");
    enabledButton.setTooltip("Enable/disable all tracks.");
    enableLabel.setTooltip("Enable/disable all tracks.");

    setFramesPerSecond(64);
}

void MasterTrackPanel::paint(Graphics & g)
{

    TrackPanelOblong::paint(g);
    
    //draw higlight, if no midi has been selected. 
    if (midiOutComboBox.getText() == AppData::noMidiSelected)
    {
        uint8 alphaValue = (getFrameCounter() % 192);
        if (alphaValue > 128)
            alphaValue = 128;
        if (alphaValue > 64)
            alphaValue = 128 - alphaValue;
        alphaValue = (64 - alphaValue) * 2;

        g.setColour(Colour(AppData::defaultTextColour).withAlpha(alphaValue));
        Rectangle<float> midInHighlightBounds = midiOutComboBox.getBounds().toFloat().expanded(AppData::scalef(1.0));
        g.drawRoundedRectangle(midInHighlightBounds, AppData::scalef(midInHighlightBounds.getHeight() / 12.5), AppData::scalef(3.5));
    }
}



void MasterTrackPanel::setAllTracksToCurrentMidiout()
{
    OwnedArray<TrackPanel>& panels = mainTabRef.getTrackPanelList();
    for (int i = 0; i < panels.size(); i++)
    {
        panels[i]->setMidiOut(midiOutComboBox.getText());
    }
}


//------------------------- Private ------------------------

void MasterTrackPanel::comboBoxChanged(ComboBox* comboBox)
{

    if (comboBox == &midiOutComboBox)
    {
        setAllTracksToCurrentMidiout();
    }
    else if (comboBox == &midiChannelComboBox)
    {
        OwnedArray<TrackPanel>& panels = mainTabRef.getTrackPanelList();
        for (int i = 0; i < panels.size(); i++)
        {
            panels[i]->setMidiChannel(comboBox->getSelectedId());
        }
    }
}


void MasterTrackPanel::buttonClicked(juce::Button* button)
{
    if (button == &enabledButton)
    {
        OwnedArray<TrackPanel>& panels = mainTabRef.getTrackPanelList();
        for (int i = 0; i < panels.size(); i++)
        {
            panels[i]->setEnableState(button->getToggleState());

        }

    }
}
