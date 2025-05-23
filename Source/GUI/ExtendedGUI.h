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

    ExtendedTabbedComponent.h
    Created: 29 Jun 2015 11:10:34am
    Author:  MartinG

  ==============================================================================
*/

#ifndef EXTENDEGUICOMPONENTS_H_INCLUDED
#define EXTENDEGUICOMPONENTS_H_INCLUDED

#include "JuceHeader.h"
#include "../MainWindow.h"

class ProgressBarComp : public ProgressBar
{
    
public:

    ProgressBarComp (double& progress_, int width_, int height_);
    
    void resized() override;

private:
    int width = 100;
    int height = 10;
    
};

/** This class creates a progress bar, which dims the main window, when active. 
*/
class ExtendedProgressBar
{
public:
    ExtendedProgressBar(String text, double& progress, MainAppWindow& newMainAppWindowRef, int width, int height);
    ~ExtendedProgressBar();

private:
    MainAppWindow& mainAppWindowRef;
    juce::DialogWindow* dialogWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedProgressBar);

};

class ExtendedComboBox : public ComboBox
{
public:
    ExtendedComboBox();

    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedComboBox);
};


class ExtendedTabbedComponent : public TabbedComponent
{

public:
    ExtendedTabbedComponent(TabbedButtonBar::Orientation orientation);

    void currentTabChanged(int newCurrentTabIndex, const String &newCurrentTabName);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedTabbedComponent);
};


class ExtendedTextEditor : public TextEditor
{
public:

    ExtendedTextEditor();

    /** Re-center the TextEditors text vertically.
        Need to do this manually. Not done automatically, when font or size is changed. 
    */
    void recenter();

    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedTextEditor);
};


class ExtendedLabel : public juce::Label
{
public:
    ExtendedLabel();

    /** Get the width of the labe, by "multiplying" font size and text length
    */
    int getWidthFromFont();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedLabel);

};


//=========================================================================


class ExtendedMidiKeyboardComponent : public MidiKeyboardComponent
{
public:

    ExtendedMidiKeyboardComponent(MidiKeyboardState& state, Orientation orientation);

    void setPlayingRange(int lowestNote, int highestNote);

protected:

    void drawWhiteNote(int midiNoteNumber,
        Graphics& g, Rectangle<float> area,
        bool isDown, bool isOver,
        Colour lineColour,
        Colour textColour) override;

    String getWhiteNoteText(int midiNoteNumber) override;

    void drawBlackNote(int /*midiNoteNumber*/,
        Graphics& g, Rectangle<float> area,
        bool isDown, bool isOver,
        Colour noteFillColour) override;

    void drawUpDownButton(Graphics& g, int w, int h,
        bool mouseOver,
        bool buttonDown,
        bool movesOctavesUp) override;


    bool mouseDownOnKey(int, const MouseEvent &) override;

    void mouseUpOnKey(int, const MouseEvent &) override;


private:

    int minAvaiableNote;
    int maxAvaiableNote;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtendedMidiKeyboardComponent);
};

#endif  // EXTENDEGUICOMPONENTS_H_INCLUDED 
