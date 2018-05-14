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

    TrackPanel.h
    Created: 12 Jul 2015 6:42:29pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#ifndef TRACKPANEL_H_INCLUDED
#define TRACKPANEL_H_INCLUDED

#include "JuceHeader.h"
#include "../../AppData.h"
#include "../../GUI/ExtendedGUI.h"
#include "../../Midi/MidiHelpers.h"

//Forward declaration of PianoRoll in order to prevent error with header files. (circular inclusion?). 
class PianoRoll;

/** Convenient way of setting String ID. 
*/
class MidiDeviceComboBox : public ExtendedComboBox
{
public:
    /** Default Constructor
    */
    MidiDeviceComboBox();

    /**
    Searches the ComboBox for the string idText. If found, it sets the ComboBox to that selection.
    */
    void setSelectedId(const String& idText, NotificationType notification = sendNotificationAsync);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiDeviceComboBox);
};

class TrackPanel : public AnimatedAppComponent,
                   public ComboBox::Listener,
                   public Button::Listener
{
public:
    /** Create a TrackPanel with access to the AppData, with the specified name to be displayed and 
        associated with the track number trackNo.
    */
    TrackPanel(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll, const String& trackName, const String& midiName, const int midiChannel, const bool isEnabled, const int trackNo);

    virtual ~TrackPanel();

    void paint(Graphics& g) override;
    void resized() override;
    void update() override {};

    /** Set the midi output of this track. 
    */
    void setMidiOut(const String& midiOutString);
    /**
    */
    String getMidiOut();
    /** Set the midi channel of this track.
    */
    void setMidiChannel(const int channel);
    /** Enables/Disabels this this track.
    */
    void setEnableState(bool enableState);
    /* Height in pixels of a track panel
    */
    static const int trackPanelHeight;
    /* Width in pixels of a track panel
    */
    static const int trackPanelWidth;


protected:

    //TrackPanel() : appData(*(new AppData())) {}

    void comboBoxChanged(ComboBox* comboBox) override;
    void focusLost(FocusChangeType cause) override;
    void focusGained(FocusChangeType cause) override;
    void focusOfChildComponentChanged(FocusChangeType cause) override;

    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;

    void repaintPianoRoll();
    void pianoRollSetFocusOnTrack(int trackNo);

    /** Enable the track in the piano roll. 
    */
    void pianoRollEanbleTrack(const int trackNo) const;

    /** Disable the track in the piano roll.
    */
    void pianoRollDisableTrack(const int trackNo) const;

    /* Collection of common Midi functions*/
    MidiHelpers& midiHelper; 
    /** A reference to the piano roll, which paints our notes. 
    */
    PianoRoll& pianoRollRef;
    /** Midi Out Label
    */
    ExtendedLabel midiOutLabel;
    /** ComboBox for choosing the midi output.
    */
    MidiDeviceComboBox midiOutComboBox;
    /** Enable Label
    */
    ExtendedLabel enableLabel;
    /** Button for enable/disable playback of this midi track. 
    */
    ToggleButton enabledButton;
    /** Name of track to be displayed
    */
    ExtendedLabel trackNameLabel;
    /** Midi Channel Label
    */
    ExtendedLabel midiChannelLabel;
    /** ComboBox for choosing the midi channel.
        NOT to confuse with the midi output!
    */
    ExtendedComboBox midiChannelComboBox;
    /** Colour to use for the background of the component
    */
    Colour backgroundColour;
    /** Colour to use, if the component is highlighted (e.g. if it has keyboard focus)
    */
    Colour higlightColour;

private:

    /**  Reference to the applications main window. 
    */
    MainAppWindow& appWindowRef;

    /** Midi Track number associated witht this button.
    */
    int trackNumber;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPanel);
};


class TrackPanelOblong : public TrackPanel
{
public:
    /** Create a TrackPanel with access to the AppData, with the specified name to be displayed and
    associated with the track number trackNo.
    */
    TrackPanelOblong(MidiHelpers& midiHelperIn, MainAppWindow& appWindow, PianoRoll& pianoRoll, const String& trackName, const String& midiName, const int midiChannel, const bool isEnabled, const int trackNo);

    void paint(Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPanelOblong);
};



#endif  // TRACKPANEL_H_INCLUDED
