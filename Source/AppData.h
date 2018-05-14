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

    AppData.h
    Created: 1 May 2012 11:59:59am
    Author:  Martin

    Class for all data definitions, that the instrument has.
    We collect them here, so they are avaiable easily both for the app 
    version and the stand-alone version

  ==============================================================================
*/

#ifndef __DATA_H_E0C949F5__
#define __DATA_H_E0C949F5__

#include "JuceHeader.h"
#include "GUI/PluginLookAndFeel.h"
#include <math.h>


class AppData
{
public:

    AppData();

    /** Function, which scales up/down all GUI components. I.e. all GUI components are using this function. */
    static int scalei(int value);
    /** Function, which scales up/down all GUI components. I.e. all GUI components are using this function. */
    static float scalef(double value);

    /** Get the application icon. */
    static Image getAppIcon();

    /** Height for all one-line text fields in this application */
    static int textFieldHeight;

    /** Fallback name for uninitialized midi Output Name */
    static const String noMidiSelected;

    /** Indices of the main tab menu */
    enum TabIndices
    {
        Home = 0,
        About = 1,
        Help = 2
    };

    /** Names of the tabs in the main tab menu. */
    static const char* tabNames[];

    /** Set my own global scale factor */
    static double appGlobalScaleFactor;

    static const juce::uint32 appToolTipBlueColour = 0xffb4e2ff;
    static const juce::uint32 mainBackgroundColour = 0xFF292C33;
    static const juce::uint32 componentBackgroundColour = 0xFF32363E;
    static const juce::uint32 componentHighlightColourLighter = 0xff535b6a; //0xff525866; 
    static const juce::uint32 componentHighlightColour = 0xff4a505c;//0xFF4e5563;
    static const juce::uint32 scrollBarThumbColour = 0xFF4e5563;
    static const juce::uint32 optionBackgroundColour = 0xFF3b404a;
    static const juce::uint32 defaultTextColour = 0xFFA9BFCF;
    static const juce::uint32 comboBoxTextColour = 0xff767d8c;//0xFF606876;//0xFF4E5563;
    static const juce::uint32 iconColour = 0xFFa2b7c7;
    static const juce::uint32 noteNormalColour = 0xffa2c4de; //0xffb37676; // 0xff8b6752;//0xFFEC8282;
    static const juce::uint32 noteHighlightColour = 0xff9dd683;//0xff5298a0; //orig: 0xFF81BAE5;
    static const juce::uint32 notePlayingColour = 0xfff0f682;//0xFFCED78D;
    static const juce::uint32 noteDisabledColour = 0xff373e4e;//0xFF4E5563;
    static const juce::uint32 tabBackgroundColour = 0xFF343842;

    static const juce::uint32 pianoScrollButtonColour = 0xFF4E5563;
    static const juce::uint32 pianoScrollButtonTriangleColour = 0xff81bae5;
    static const juce::uint32 pianoWhiteKeyColour = 0xffffffff;
    static const juce::uint32 pianoBlackKeyColour = 0xff474c56;
    static const juce::uint32 pianoLineColour = 0xff2a2e37;
    static const juce::uint32 pianoFrontShadowColour = 0x19000000;
    static const juce::uint32 pianobackShadowColour = 0x26000000;

    static const juce::uint32 pianoKeyRedColour = 0xFFFC9292;//0xFFEC8282;
    static const juce::uint32 pianoKeyGreenColour = 0xFFC4F7AD;//0xFFA4D78D;
    static const juce::uint32 pianoKeyBlueColour = 0xFF91CAF5; //0xFF81BAE5;

    static const juce::uint32  toggleButtonTickColour = 0xff6dd596;

    static const String fontName;
    static const int mainFontSize = 21;
    static const int tabMenuFontSize = 19;
    static const int tooltipFontSize = 19;
    static const int midiKeyboardFontSize = 13;
    static const int headingFontSize = 35;
    static const int subHeadingFontSize = 26;
    static const int abouTextFontSize = 24;

    static const int mainTabIconSize = 60;
    static const int mainTabIconDistance = 10;
    static const int mainTabMenuStartY = 30;
    static const int mainTabMenuStartX = 20;
    static const int mainTabMenuHeight = 40;
    static const int mainTabMenuDistanceIconsBoxes = 170;
    static const int mainTabMenuDistanceLabelsComboboxes = 15;
    static const int mainTabMenuDistanceComboboxesLabels = 60;
    static const int mainTabMenuMidiInComboBoxSize = 225;
    static const int mainTabMenuMidiPositionTextFieldSize = 122;

    static const int mainTabKeyboardHeight = 140;
    static const int mainTabKeyboardKeyWidth = 27;
    static const int mainTabKeyboarOuterMargin = 11;

    static const int iconMenuHeight = 100;

    static const int trackPanelStartX = 16;
    static const int trackPanelWidth = 410;
    static const int trackPanelHeight = 182;
    static const int trackPanelBorder = 20;
    static const int trackPanelOuterHeight = trackPanelHeight + trackPanelBorder;
    static const int trackPanelCCOptsWidth = 109;

    static const int distanceTrackPanelKeyboard = 26;
    static const int trackPanelRightBorder = 10;

    static const int scrollBarBorder = 10;
    static const int scrollBarThickness = 11;

    static const int masterTrackPanelStartX = trackPanelStartX;
    static const int masterTrackPanelHeight = 70;
    static const int masterTrackPanelInnerCompStartY = (masterTrackPanelHeight - mainTabMenuHeight) / 2;
    static const int masterTrackPanelWidth = 1145;
    static const int masterTrackPanelBottomBorder = 16;
    static const int enableAllTracksButtonStartX = 20;
    static const int enableAllTracksLabelStartX = 15;
    static const int masterTrackInnerMiniGap = 20;
    static const int toggleButtonGap = 10;

    static const int trackPanelBorderGapLeft = 18;
    static const int trackPanelBorderGapTopBottom = 10;
    static const int trackPanelTrackNameWidth = mainTabMenuMidiInComboBoxSize;
    static const int TrackPanelEnabledGap = 45;
    static const int TrackPanelInnerGapX = 70;
    static const int TrackPanelInnerGapY = 15;

    static const int midiOutputLabelAllStartX = 66;
    static const int midiChannelLabelAllStartX = 66;

    static const int midiChannelComboWidth = 96;
    static const int pianoRollRightBorder = 20;

    static const int zoomComponentWidth = 170;
    static const int zoomTextFieldWidth = 78;
    static const int zoomComponentMiniGap = 10;

    static const int cornerSize = 3;

    static const int DialogWindowWidth = 966;
    static const int DialogWindowHeight = 727;

    static const int DialogButtonHeight = 43;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppData);
};


#endif  // __DATA_H_E0C949F5__
