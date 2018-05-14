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

    TrackPanel.cpp
    Created: 12 Jul 2015 6:42:23pm
    Author:  MartinG

  ==============================================================================
*/

#include "TrackPanel.h"
#include "../../Midi/MidiPlayer.h"
#include "PianoRoll.h"
#include "../../AppData.h"
#include "../GuiHelpers.h"


MidiDeviceComboBox::MidiDeviceComboBox()
    :ExtendedComboBox()
{
}

void MidiDeviceComboBox::setSelectedId(const String& idText, NotificationType notification)
{
    StringArray textEntries;
    Array<int> comboBoxIDs;
    for (int i = 0; i < getNumItems(); i++)
    {
        textEntries.add(getItemText(i));
        comboBoxIDs.add(this->getItemId(i));
    }


    int id = textEntries.indexOf(idText);
    if (id != -1)
        ComboBox::setSelectedId(comboBoxIDs[id], notification);
    else if (idText == AppData::noMidiSelected)
        setText(AppData::noMidiSelected, notification);
    else
        setText(idText, notification);

}


TrackPanel::TrackPanel(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll, const String& trackName, const String& midiName, const int midiChannel, const bool isEnabled, const int trackNo)
    : midiHelper(midiHelperIn),
    pianoRollRef(pianoRoll),
    appWindowRef(appWindow)

{

    if (trackNo >= 0)
        this->trackNumber = trackNo;
    else
        this->trackNumber = 0;


    setWantsKeyboardFocus(true);

    String trackTitle;
    if (trackNo != -1)
    {
        trackTitle = String(trackNo + 1);
        if (!trackName.isEmpty())
            trackTitle += " - " + trackName;
    }
    else
        trackTitle = trackName;


    trackNameLabel.setText(trackTitle, juce::NotificationType::dontSendNotification);
    trackNameLabel.setMinimumHorizontalScale(0.8f);
    trackNameLabel.setTooltip(trackName);
    addAndMakeVisible(&trackNameLabel);

    midiOutLabel.setText("Midi Out:", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&midiOutLabel);

    midiChannelLabel.setText("Midi Channel:", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&midiChannelLabel);
    
    enableLabel.setText("Enabled", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&enableLabel);
    enableLabel.setTooltip("Enable/Disable the track.");

    enabledButton.setButtonText("");
    enabledButton.setToggleState(isEnabled, juce::NotificationType::dontSendNotification);
    enabledButton.setWantsKeyboardFocus(true);
    enabledButton.addListener(this);
    enabledButton.setTooltip("Enable/Disable the track.");
    addAndMakeVisible(&enabledButton);

    for (int i = 1; i <= 16; i++)
        midiChannelComboBox.addItem(String(i),i);
    midiChannelComboBox.setSelectedId(midiChannel, juce::NotificationType::dontSendNotification);
    midiChannelComboBox.addListener(this);
    //midiChannelComboBox.setColour(juce::ComboBox::buttonColourId, Colours::grey);
    midiChannelComboBox.setTooltip("Select the midi channel for the track.");
    midiChannelComboBox.setWantsKeyboardFocus(true);
    addAndMakeVisible(&midiChannelComboBox);

    midiOutComboBox.addItem(AppData::noMidiSelected, 1);
    StringArray& midiDevices = midiHelper.getMidiInputs();
    
    for (int i = 0; i < midiDevices.size();i++)
    {
        midiOutComboBox.addItem(midiDevices[i], i+2);
    }

    midiOutComboBox.setSelectedId(midiName, juce::NotificationType::dontSendNotification);
    midiOutComboBox.addListener(this);
    //midiOutComboBox.setColour(juce::ComboBox::buttonColourId, Colours::grey);
    midiOutComboBox.setTooltip("Select the midi output for the track.");
    midiOutComboBox.setWantsKeyboardFocus(true);
    addAndMakeVisible(&midiOutComboBox);

    backgroundColour = Colour(AppData::componentBackgroundColour);
    higlightColour = Colour(AppData::componentHighlightColour);
    //higlightColour = GuiHelpers::makeBrighter(Colour(AppData::componentBackgroundColour));

    setOpaque(false);
}

TrackPanel::~TrackPanel()
{
    //FIXME: Should not be necessary addording to the doc. The ComboBox should take care of this by itself
    //midiOutComboBox.removeListener(this);

    //FIXME What about removing the button listener to enableButton? In the API it says nothing, if this is 
    //done automagically.
    enabledButton.removeListener(this);

}

//==============================================================================
void TrackPanel::paint(Graphics& g)
{


    juce::Rectangle<float> rect(1, 1, (float)getWidth() - 2, (float)getHeight() - 2);

    if (hasKeyboardFocus(true))
        g.setColour(higlightColour);
    else
        g.setColour(backgroundColour);
    g.fillRoundedRectangle(rect, AppData::scalef(AppData::cornerSize));

}

void TrackPanel::resized()
{

    Font plainFont(AppData::scalef(AppData::mainFontSize), Font::plain);
    Font boldFont(AppData::scalef(AppData::mainFontSize), Font::bold);

    juce::BorderSize<int> zeroBorder(0, 0, 0, 0);

    int height = AppData::scalei(AppData::mainTabMenuHeight);
    trackNameLabel.setFont(boldFont);
    trackNameLabel.setMinimumHorizontalScale(1.0f);
    trackNameLabel.setBounds(AppData::scalei(AppData::trackPanelBorderGapLeft), 
        AppData::scalei(AppData::trackPanelBorderGapTopBottom) + AppData::scalei(2),
        AppData::scalei(AppData::trackPanelTrackNameWidth), height-AppData::scalei(4));
    trackNameLabel.setBorderSize(zeroBorder);

    int width = (int)plainFont.getStringWidthFloat("Eai");
    enabledButton.setBounds(trackNameLabel.getRight() + AppData::scalei(AppData::TrackPanelEnabledGap),
        trackNameLabel.getY(),
        width, height);


    enableLabel.setFont(plainFont);
    enableLabel.setMinimumHorizontalScale(1.0f);
    enableLabel.setBounds(enabledButton.getRight() + AppData::scalei(AppData::toggleButtonGap), 
        trackNameLabel.getY(),
        enableLabel.getWidthFromFont(), height);
    enableLabel.setBorderSize(zeroBorder);

    midiOutLabel.setFont(plainFont);
    midiOutLabel.setMinimumHorizontalScale(1.0f);
    midiOutLabel.setBounds(AppData::scalei(AppData::trackPanelBorderGapLeft),
        trackNameLabel.getBottom() + AppData::scalei(AppData::TrackPanelInnerGapY), 
        midiOutLabel.getWidthFromFont(), height);
    midiOutLabel.setBorderSize(zeroBorder);

    midiOutComboBox.setBounds(midiOutLabel.getRight() + +AppData::scalei(AppData::TrackPanelInnerGapX), 
        midiOutLabel.getY(),
        AppData::scalei(AppData::mainTabMenuMidiInComboBoxSize), height);

    midiChannelLabel.setFont(plainFont);
    midiChannelLabel.setMinimumHorizontalScale(1.0f);
    midiChannelLabel.setBounds(AppData::scalei(AppData::trackPanelBorderGapLeft), 
        midiOutLabel.getBottom() + AppData::scalei(AppData::TrackPanelInnerGapY),
        midiChannelLabel.getWidthFromFont(), height);
    midiChannelLabel.setBorderSize(zeroBorder);

    midiChannelComboBox.setBounds(midiOutComboBox.getX(),
        midiChannelLabel.getY(),
        AppData::scalei(AppData::mainTabMenuMidiInComboBoxSize), height);

}

void TrackPanel::setMidiOut(const String& midiOutString)
{
    midiOutComboBox.setSelectedId(midiOutString, juce::NotificationType::sendNotification);
}

String TrackPanel::getMidiOut()
{
    return(midiOutComboBox.getText());
}

void TrackPanel::setMidiChannel(const int channel)
{
    midiChannelComboBox.setSelectedId(channel, juce::NotificationType::sendNotification);
}


void TrackPanel::setEnableState(bool enableState)
{
    enabledButton.setToggleState(enableState, juce::NotificationType::sendNotification);
}

void TrackPanel::focusGained(FocusChangeType )
{

    repaint();
    pianoRollSetFocusOnTrack(trackNumber);
    repaintPianoRoll();

}

void TrackPanel::focusLost(FocusChangeType )
{
    repaint();
    /*pianoRollSetFocusOnTrack(-1);
    repaintPianoRoll();*/
}

void TrackPanel::focusOfChildComponentChanged(FocusChangeType )
{
    repaint();
    if (hasKeyboardFocus(true))
        pianoRollSetFocusOnTrack(trackNumber);
    /*else
        pianoRollSetFocusOnTrack(-1);*/
    repaintPianoRoll();
}

void TrackPanel::comboBoxChanged(ComboBox* comboBox)
{
    if (comboBox == &midiOutComboBox)
    {
        MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
        midiData.setMidiOut(midiOutComboBox.getText(), trackNumber);
    }
    else if (comboBox == &midiChannelComboBox)
    {
        MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
        midiData.setMidiChannel(comboBox->getSelectedId(), trackNumber);
    }
}


void TrackPanel::buttonClicked(juce::Button* button)
{
    if (button == &enabledButton)
    {
        MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());

        if (button->getToggleState())
        {
            midiData.enableTrack(trackNumber);
            pianoRollEanbleTrack(trackNumber);
            pianoRollSetFocusOnTrack(trackNumber);
        }
        else
        {
            midiData.disableTrack(trackNumber);
            pianoRollDisableTrack(trackNumber);
        }

        repaintPianoRoll();
    }
}


void TrackPanel::buttonStateChanged(juce::Button* )
{
    
}

void TrackPanel::repaintPianoRoll()
{

    pianoRollRef.repaint();

}

void TrackPanel::pianoRollSetFocusOnTrack(int trackNo)
{

    pianoRollRef.setFocusTrack(trackNo);
}

void TrackPanel::pianoRollEanbleTrack(const int trackNo) const
{
    pianoRollRef.enableTrack(trackNo);
}

void TrackPanel::pianoRollDisableTrack(const int trackNo) const
{

    pianoRollRef.disableTrack(trackNo);
}

const int TrackPanel::trackPanelHeight = 125;
const int TrackPanel::trackPanelWidth = 200;

TrackPanelOblong::TrackPanelOblong(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll,const String& trackName, const String& midiName, const int midiChannel, const bool isEnabled, const int trackNo)
    : TrackPanel(midiHelperIn, appWindow, pianoRoll, trackName, midiName, midiChannel, isEnabled, trackNo)
{

    backgroundColour = Colour(AppData::componentBackgroundColour);
    higlightColour = backgroundColour;
    
}

void TrackPanelOblong::paint(Graphics& g)
{
    TrackPanel::paint(g);
    
}

void TrackPanelOblong::resized()
{
    TrackPanel::resized();

    int y = AppData::scalei(AppData::masterTrackPanelInnerCompStartY);
    int height = AppData::scalei(AppData::mainTabMenuHeight);
    trackNameLabel.setBounds(AppData::scalei(AppData::trackPanelBorderGapLeft), y + AppData::scalei(2),
        trackNameLabel.getWidthFromFont(), height - AppData::scalei(4));

    Font font = enableLabel.getFont();
    int width = (int)font.getStringWidthFloat("Eai");
    enabledButton.setBounds(trackNameLabel.getRight() + AppData::scalei(AppData::masterTrackInnerMiniGap * 2), y,
        width, height);

    enableLabel.setBounds(enabledButton.getRight() + AppData::scalei(AppData::toggleButtonGap) , y,
        enableLabel.getWidthFromFont(), height);

    midiOutLabel.setBounds(enableLabel.getRight() + AppData::scalei(AppData::masterTrackInnerMiniGap*2 + 59), y,
        midiOutLabel.getWidthFromFont(), height);

    midiOutComboBox.setBounds(midiOutLabel.getRight() + AppData::scalei(AppData::masterTrackInnerMiniGap + 8), y, 
        AppData::scalei(AppData::mainTabMenuMidiInComboBoxSize), height);

    midiChannelLabel.setBounds(midiOutComboBox.getRight() + AppData::scalei(AppData::masterTrackInnerMiniGap * 2 + 20), y,
        midiChannelLabel.getWidthFromFont(), height);

    midiChannelComboBox.setBounds(midiChannelLabel.getRight() + AppData::scalei(AppData::masterTrackInnerMiniGap - 7), y, 
        AppData::scalei(AppData::midiChannelComboWidth -7), height);


}
