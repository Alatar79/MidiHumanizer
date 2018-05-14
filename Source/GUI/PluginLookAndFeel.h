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

    AppLookAndFeel.h
    Created: 5 Jun 2012 9:58:19pm
    Author:  Martin

  ==============================================================================
*/

#ifndef __PLUGINLOOKANDFEEL_H_C4BCEF54__
#define __PLUGINLOOKANDFEEL_H_C4BCEF54__

#include "JuceHeader.h"
#include "CIELCHColour.h"

/** This class defines the look and feel, i.e. the "skinning" of the application.
*/
class AppLookAndFeel : public LookAndFeel_V3
{

public:

    AppLookAndFeel();

    /** Draw the triangle, which is used as icon by the AlertWindow
    */
    void drawAlertSign(Graphics& g, const float x, const float y, const float width, const float height,
                       const CIELCHColour& c1);

    void drawAlertIcon(Graphics& g, const float x, const float y, const float width, const float height,
        AlertWindow::AlertIconType iconType);

    void drawAlertBox(Graphics& g, AlertWindow& alert,
        const Rectangle<int>& textArea, TextLayout& textLayout) override;


    TextLayout layoutTooltipText (const String& text, const Colour& colour) noexcept;
    void drawTooltip (Graphics& g, const String& text, int width, int height) override;
    juce::Rectangle<int> getTooltipBounds(const String& tipText, Point<int> screenPos, juce::Rectangle<int> parentArea) override;

    void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;
    static void createTabTextLayout(const TabBarButton& button, float length, float depth,
        Colour colour, TextLayout& textLayout);

    void drawTabAreaBehindFrontButton(TabbedButtonBar& bar, Graphics& g, int w, int h) override;

    void drawComboBox(Graphics& g, int width, int height, bool /*isButtonDown*/,
        int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;
    Font getComboBoxFont(ComboBox& box) override;

    Font getPopupMenuFont() override;
    void drawPopupMenuBackground(Graphics& g, int width, int height) override;
    void drawPopupMenuUpDownArrow(Graphics& g, int width, int height, bool isScrollUpArrow) override;
    int getMenuWindowFlags() override;

    void drawTickBox(Graphics& g, Component& component,
                     float x, float y, float w, float h,
                     bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override;
    void drawToggleButton(Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawScrollbar(Graphics& g,
         ScrollBar& scrollbar,
         int x, int y,
         int width, int height,
         bool isScrollbarVertical,
         int thumbStartPosition,
         int thumbSize,
         bool /*isMouseOver*/,
         bool /*isMouseDown*/) override;

    void fillResizableWindowBackground(Graphics& g, int w, int h,
        const BorderSize<int>& /*border*/, ResizableWindow& window) override;
    void drawResizableFrame(Graphics& /*g*/, int /*w*/, int /*h*/, const BorderSize<int>& /*border*/) override;

    int getDefaultScrollbarWidth() override;

    void drawDrawableButton(Graphics& g, DrawableButton& button,
        bool isMouseOverButton, bool /*isButtonDown*/) override;

    void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;


    Font getTextButtonFont(TextButton&, int buttonHeight) override;
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override;

    void drawProgressBar(Graphics& g, ProgressBar& progressBar,
        int width, int height,
        double progress, const String& textToShow) override;


    void drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
        int w, int h, int titleSpaceX, int titleSpaceW, const Image* icon, bool drawTitleTextOnLeft) override;
    /* Button* createDocumentWindowButton(int buttonType); */


private:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppLookAndFeel);

};



#endif  // __PLUGINLOOKANDFEEL_H_C4BCEF54__
