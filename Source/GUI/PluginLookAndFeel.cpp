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

    AppLookAndFeel.cpp
    Created: 5 Jun 2012 9:58:19pm
    Author:  Martin

  ==============================================================================
*/

#include "PluginLookAndFeel.h"
#include "../AppData.h"
#include "GuiHelpers.h"

AppLookAndFeel::AppLookAndFeel()
:LookAndFeel_V3()
{
    
}

void AppLookAndFeel::drawAlertSign(Graphics& g, const float x, const float y, const float width, const float height,
    const CIELCHColour& c )
{

    float stop = height / 10.0f;
    for (float i = 0; i < stop; i++)
    {
        Path outerShadow;
        outerShadow.addTriangle(x + width / 2, y + i, x + width - i, y + height - i, x + i, y + height - i);
        outerShadow = outerShadow.createPathWithRoundedCorners(width / 5.0f);
        g.setColour(Colour(0x01000000 * (int)(5.0f * i/stop)));
        g.fillPath(outerShadow);
    }

    float cHue = c.getCIEHue();
    float cLight = c.getCIELightness();
    float cSat = c.getCIEChroma();

    bool imaginary;
    CIELCHColour foregroundC1 = CIELCHColour::fromCIELCH(0.75f * cLight, 1.2f * cSat, cHue - 0.02f,   1.0f, imaginary);
    CIELCHColour foregroundC2 = CIELCHColour::fromCIELCH(1.2f *  cLight, 0.75f * cSat, cHue + 0.02f, 1.0f, imaginary);

    float fgC1Hue = foregroundC1.getCIEHue();
    float fgC1Light = foregroundC1.getCIELightness();
    float fgC1Sat = foregroundC1.getCIEChroma();

    float fgC2Hue = foregroundC2.getCIEHue();
    float fgC2Light = foregroundC2.getCIELightness();
    float fgC2Sat = foregroundC2.getCIEChroma();

    Colour backgroundC1 = CIELCHColour::fromCIELCH(1.0f * fgC1Light, 0.6f * fgC1Sat, fgC1Hue,  1.0f, imaginary).getJuceColour();
    Colour backgroundC2 = CIELCHColour::fromCIELCH(1.15f * fgC2Light, 0.6f * fgC2Sat, fgC2Hue,  1.0f, imaginary).getJuceColour();

    Colour shadowC1 = CIELCHColour::fromCIELCH(0.8f * fgC1Light, 1.1f * fgC1Sat, fgC1Hue,   1.0f, imaginary).getJuceColour();
    Colour shadowC2 = CIELCHColour::fromCIELCH(0.8f * fgC2Light, 1.1f * fgC2Sat, fgC2Hue,   1.0f, imaginary).getJuceColour();

    float bottomY = 0.895f * height;

    float gradientStartX = x + 0.065f * width;
    float gradientEndX = x + 0.783f * width;
    float gradientStartY = y + 0.842f * height;
    float gradientEndY = y + 0.395f * height;

    Path background;
    background.addTriangle(x + width / 2, y, x + 0.935f * width, y + bottomY, x + 0.065f * width, y + bottomY);
    background = background.createPathWithRoundedCorners(width / 5.75f);
    ColourGradient gr(backgroundC1, gradientStartX, gradientStartY,
                      backgroundC2, gradientEndX, gradientEndY, true);
    g.setGradientFill(gr);
    g.fillPath(background);


    Path innerShadow;
    innerShadow.addTriangle(x + width / 2, y + 0.084f * height, 
                            x + 0.9f * width, y + bottomY, 
                            x + 0.1f * width, y + bottomY);
    innerShadow = innerShadow.createPathWithRoundedCorners(width / 8.0f);
    ColourGradient gr3(shadowC1, gradientStartX, gradientStartY,
                       shadowC2, gradientEndX, gradientEndY, true);
    g.setGradientFill(gr3);
    g.fillPath(innerShadow);

    float foregroundBottomY = 0.965f * bottomY;
    Path foreground;
    foreground.addTriangle(x + width / 2, y + 0.063f * height, x + 0.891f * width, y + foregroundBottomY, 
                           x + 0.109f * width, y + foregroundBottomY);
    foreground = foreground.createPathWithRoundedCorners(width / 8.0f);
    ColourGradient gr2(foregroundC1.getJuceColour(), gradientStartX, gradientStartY,
                       foregroundC2.getJuceColour(), gradientEndX, gradientEndY, true);
    g.setGradientFill(gr2);
    g.fillPath(foreground);

    g.setColour(Colours::black);
    PathStrokeType stroke(height / 50.0f);
    g.strokePath(background, stroke);

    g.setColour(foregroundC1.getJuceColour());
    ColourGradient grr2(foregroundC1.getJuceColour(), x + width, y + height,
        foregroundC2.getJuceColour(), x + width+100, y + height + 50, true);

}

void AppLookAndFeel::drawAlertIcon(Graphics& g, const float x, const float y, const float width, const float height,
    AlertWindow::AlertIconType iconType)
{

    Colour colour;
    char character;

    if (iconType == AlertWindow::WarningIcon)
    {
        colour = Colour(0xfff9503f);
        character = 'x';
    }
    else if (iconType == AlertWindow::InfoIcon)
    {
        colour = Colour(0xfff8b741);
        character = 'i';
    }
    else //Question icon
    {
        colour = Colour(0xff48b0fa);
        character = '?';
    }

    drawAlertSign(g, x, y, width, height, colour);
    //g.setColour(Colours::red);
    //g.drawRect(iconRect.getX(), iconRect.getY(), iconRect.getWidth(), iconRect.getHeight());

    GlyphArrangement ga;
    ga.addFittedText(Font(height * 0.65f, Font::bold),
        String::charToString((juce_wchar)(uint8)character),
        x, y + height * 0.05f,
        width, height,
        Justification::centred, false);
    Path glyphPath;
    ga.createPath(glyphPath);

    g.setColour(Colours::white);
    g.fillPath(glyphPath);
}


void AppLookAndFeel::drawAlertBox(Graphics& g, AlertWindow& alert,
    const Rectangle<int>& textArea, TextLayout& textLayout)
{
    g.fillAll(alert.findColour(AlertWindow::backgroundColourId));

    int iconSpaceUsed = 0;

    const int iconWidth = 80;
    int iconSize = jmin(iconWidth + 0, alert.getHeight() + 20);

    if (alert.containsAnyExtraComponents() || alert.getNumButtons() > 2)
        iconSize = jmin(iconSize, textArea.getHeight() + 50);

    const Rectangle<float> iconRect(0, (alert.getHeight() - iconSize * 0.8f * 1.2f) / 2.0f,
        iconSize * 1.2f, iconSize * 0.8f * 1.2f)  ;

    if (alert.getAlertType() != AlertWindow::NoIcon)
    {
        drawAlertIcon(g, iconRect.getX(), iconRect.getY(), iconRect.getWidth(), iconRect.getHeight(), alert.getAlertType());
        iconSpaceUsed = iconSize;
    }

    g.setColour(alert.findColour(AlertWindow::textColourId));
    
    textLayout.draw(g, Rectangle<int>(textArea.getX() + iconSpaceUsed,
        textArea.getY(),
        textArea.getWidth() - iconSpaceUsed,
        textArea.getHeight()).toFloat());
        

    g.setColour(alert.findColour(AlertWindow::outlineColourId));
    g.drawRect(0, 0, alert.getWidth(), alert.getHeight());

    //g.setColour(Colours::red);
    //g.drawRect(textArea.getX() + 0, textArea.getY(), textArea.getWidth() - 0, textArea.getHeight());

}


TextLayout AppLookAndFeel::layoutTooltipText (const String& text, const Colour& colour) noexcept
{
    const int maxToolTipWidth = 700;

    AttributedString s;
    s.setJustification (Justification::centred);
    //s.append (text, Font (tooltipFontSize, Font::plain), colour);
    s.append (text, Font (AppData::fontName, AppData::scalef(AppData::tooltipFontSize), Font::plain), colour);

    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
}

void AppLookAndFeel::drawTooltip (Graphics& g, const String& text, int width, int height)
{
    //g.fillAll (findColour (TooltipWindow::backgroundColourId));
    g.fillAll (Colours::lightgrey);

    const TextLayout tl (layoutTooltipText (text, findColour (TooltipWindow::textColourId)));
    tl.draw (g, Rectangle<float> ((float) width, (float) height));
}

Rectangle<int> AppLookAndFeel::getTooltipBounds(const String& tipText, Point<int> screenPos, Rectangle<int> parentArea)
{
    const TextLayout tl(layoutTooltipText(tipText, Colours::black));

    const int w = (int)(tl.getWidth() + 14.0f);
    const int h = (int)(tl.getHeight() + 6.0f);

    return Rectangle<int>(screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
        screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6) : screenPos.y + 6,
        w, h)
        .constrainedWithin(parentArea);
}




void AppLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    const Rectangle<int> activeArea(button.getActiveArea());

    const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

    const Colour bkg(button.getTabBackgroundColour());

    if (button.getToggleState())
    {
        g.setColour(Colour(AppData::mainBackgroundColour));
    }
    else
    {
        g.setColour(bkg);
    }

    g.fillRect(activeArea);

    Rectangle<int> r(activeArea);

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

    Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

    if (TabbedButtonBar* bar = button.findParentComponentOfClass<TabbedButtonBar>())
    {
        TabbedButtonBar::ColourIds colID = button.isFrontTab() ? TabbedButtonBar::frontTextColourId
            : TabbedButtonBar::tabTextColourId;

        if (bar->isColourSpecified(colID))
            col = bar->findColour(colID);
        else if (isColourSpecified(colID))
            col = findColour(colID);
    }

    const Rectangle<float> area(button.getTextArea().toFloat());

    float length = area.getWidth();
    float depth = area.getHeight();

    if (button.getTabbedButtonBar().isVertical())
        std::swap(length, depth);

    TextLayout textLayout;
    createTabTextLayout(button, length, depth, col, textLayout);

    AffineTransform t;

    switch (o)
    {
    case TabbedButtonBar::TabsAtLeft:   t = t.rotated(float_Pi * -0.5f).translated(area.getX(), area.getBottom()); break;
    case TabbedButtonBar::TabsAtRight:  t = t.rotated(float_Pi *  0.5f).translated(area.getRight(), area.getY()); break;
    case TabbedButtonBar::TabsAtTop:
    case TabbedButtonBar::TabsAtBottom: t = t.translated(area.getX(), area.getY()); break;
    default:                            jassertfalse; break;
    }

    g.addTransform(t);

    textLayout.draw(g, Rectangle<float>(length, depth));
}


void AppLookAndFeel::createTabTextLayout(const TabBarButton& button, float length, float /*depth*/,
    Colour colour, TextLayout& textLayout)
{
    //Font font(depth * 0.5f);
    juce::Font font(AppData::fontName, AppData::scalef(AppData::tabMenuFontSize), juce::Font::bold);

    AttributedString s;
    s.setJustification(Justification::centred);
    s.append(button.getButtonText().trim(), font, colour);

    textLayout.createLayout(s, length);
}



void AppLookAndFeel::drawTabAreaBehindFrontButton(TabbedButtonBar& /*bar*/, Graphics& /*g*/, int /*w*/, int /*h*/)
{

}


void AppLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool /*isButtonDown*/,
    int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{

    g.setColour(box.findColour(ComboBox::backgroundColourId));
    

    if (box.isMouseOver(true))
        //g.setColour(Colour(AppData::componentHighlightColourLighter));
        g.setColour(GuiHelpers::makeBrighter(box.findColour(ComboBox::backgroundColourId)));

    g.fillRoundedRectangle(0.0f, 0.0f, (float)width, (float)height, (float)height / 12.5f);


    const Colour buttonColour(box.findColour(ComboBox::buttonColourId));

    Path p;

    float cornerSize = (float)buttonH / 12.5f;

    if (box.isEnabled() && !box.isPopupActive())
    {

        /*
        p.addRoundedTriangle(buttonX + buttonW * 0.3f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.7f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.5f, buttonY + buttonH * 0.7f, 
           cornerSize, cornerSize, cornerSize);
           */
        p.addTriangle(buttonX + buttonW * 0.3f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.7f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.5f, buttonY + buttonH * 0.7f);
        p = p.createPathWithRoundedCorners(cornerSize);

    }
    else
    {

        /*
        p.addRoundedTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.3f, buttonY + buttonH * 0.7f,
            buttonX + buttonW * 0.7f, buttonY + buttonH * 0.7f, 
            cornerSize, cornerSize, cornerSize);
            */
        p.addTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * 0.333f,
            buttonX + buttonW * 0.3f, buttonY + buttonH * 0.7f,
            buttonX + buttonW * 0.7f, buttonY + buttonH * 0.7f);
        p = p.createPathWithRoundedCorners(cornerSize);

    }


    g.setColour(box.findColour(ComboBox::arrowColourId).withMultipliedAlpha(box.isEnabled() ? 1.0f : 0.3f));
    g.fillPath(p);
}

Font AppLookAndFeel::getComboBoxFont(ComboBox& /*box*/)
{
    return Font(AppData::scalef(AppData::mainFontSize));
}

Font AppLookAndFeel::getPopupMenuFont()
{
    return Font(AppData::scalef(AppData::mainFontSize));
}

void AppLookAndFeel::drawPopupMenuBackground(Graphics& g, int /*width*/, int /*height*/)
{
    const Colour background(findColour(PopupMenu::backgroundColourId));

    g.fillAll(background);
}

void AppLookAndFeel::drawPopupMenuUpDownArrow(Graphics& g, int width, int height, bool isScrollUpArrow)
{
    const Colour background(findColour(PopupMenu::backgroundColourId));

    g.setGradientFill(ColourGradient(background, 0.0f, height * 0.5f,
        background.withAlpha(0.0f),
        0.0f, isScrollUpArrow ? ((float)height) : 0.0f,
        false));

    g.fillRect(1, 1, width - 2, height - 2);

    const float hw = width * 0.5f;
    const float arrowW = height * 0.3f;
    const float y1 = height * (isScrollUpArrow ? 0.6f : 0.3f);
    const float y2 = height * (isScrollUpArrow ? 0.3f : 0.6f);

    Path p;
    p.addTriangle(hw - arrowW, y1,
        hw + arrowW, y1,
        hw, y2);

    g.setColour(findColour(PopupMenu::highlightedTextColourId).withAlpha(0.75f));
    g.fillPath(p);
}

int AppLookAndFeel::getMenuWindowFlags()
{
    //return ComponentPeer::windowHasDropShadow;
    return 0;
}

/*
void AppLookAndFeel::drawTickBox(Graphics& g, Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    const bool isEnabled,
    const bool isMouseOverButton,
    const bool isButtonDown)
{
    const float boxSize = w * 0.7f;

    drawGlassSphere(g, x, y + (h - boxSize) * 0.5f, boxSize,
        LookAndFeelHelpers::createBaseColour(component.findColour(TextButton::buttonColourId)
        .withMultipliedAlpha(isEnabled ? 1.0f : 0.5f),
        true, isMouseOverButton, isButtonDown),
        isEnabled ? ((isButtonDown || isMouseOverButton) ? 1.1f : 0.5f) : 0.3f);

    if (ticked)
    {
        Path tick;
        tick.startNewSubPath(1.5f, 3.0f);
        tick.lineTo(3.0f, 6.0f);
        tick.lineTo(6.0f, 0.0f);

        g.setColour(isEnabled ? Colours::black : Colours::grey);

        const AffineTransform trans(AffineTransform::scale(w / 9.0f, h / 9.0f)
            .translated(x, y));

        g.strokePath(tick, PathStrokeType(2.5f), trans);
    }
}
*/

void AppLookAndFeel::drawTickBox(Graphics& g, Component& component,
    float x, float y, float w, float h,
    bool ticked,
    bool isEnabled,
    bool /*isMouseOverButton*/,
    bool /*isButtonDown*/)
{
    Path box;
    box.addRoundedRectangle(0.0f, 0.0f, 8.0f, 8.0f, 1.0f);

    AffineTransform trans(AffineTransform::scale(w / 9.0f, h / 9.0f).translated(x, y));

    g.setColour(Colour(AppData::componentBackgroundColour));
    g.fillPath(box, trans);


    g.setColour(component.findColour(ToggleButton::textColourId));
    g.strokePath(box, PathStrokeType(2.0f), trans);

    if (ticked)
    {
        Path tick;
        tick.startNewSubPath(1.5f, 3.5f);
        tick.lineTo(3.5f, 5.5f);
        tick.lineTo(6.5f, 1.5f);

        g.setColour(isEnabled ? Colour(AppData::toggleButtonTickColour) : Colour(AppData::toggleButtonTickColour).darker());
        g.strokePath(tick, PathStrokeType(2.0f), trans);
    }
}


void AppLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    if (button.hasKeyboardFocus(true))
    {
        g.setColour(button.findColour(TextEditor::focusedOutlineColourId));
        g.drawRect(0, 0, button.getWidth(), button.getHeight());
    }

    float fontSize = jmin(15.0f, button.getHeight() * 0.75f);
    float tickWidth = fontSize * 1.1f;
    tickWidth = jmin(tickWidth, (float) button.getHeight());
    tickWidth = jmin(tickWidth, (float) button.getWidth());

    drawTickBox(g, button, 1.0f, (button.getHeight() - tickWidth) * 0.5f,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        isMouseOverButton,
        isButtonDown);

    g.setColour(button.findColour(ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    const int textX = (int)tickWidth + 5;

    if (button.getButtonText() != "")
    {
        g.drawFittedText(button.getButtonText(),
            textX, 0,
            button.getWidth() - textX - 2, button.getHeight(),
            Justification::centredLeft, 10);
    }
}

void AppLookAndFeel::drawScrollbar(Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height,
    bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown)
{

    //g.fillAll(scrollbar.findColour(ScrollBar::backgroundColourId));

    Path thumbPath;
    Path slotPath;

    if (thumbSize > 0)
    {
        const float thumbIndent = (isScrollbarVertical ? width : height) * 0.0f;
        const float thumbIndentx2 = thumbIndent * 2.0f;

        if (isScrollbarVertical)
        {
            thumbPath.addRoundedRectangle(x + thumbIndent, thumbStartPosition + thumbIndent,
                width - thumbIndentx2, thumbSize - thumbIndentx2, 
                (width - thumbIndentx2) * 0.5f);

            
            slotPath.addRoundedRectangle(x + thumbIndent, y + thumbIndent,
            width - thumbIndentx2, height - thumbIndentx2,
            (width - thumbIndentx2) * 0.5f);
            

            /*
            slotPath.addRoundedRectangle(x + thumbIndent,
                y + thumbIndent,
                width - thumbIndentx2,
                height - thumbIndentx2,
                (width - thumbIndentx2) * 0.5f);
                */

        }
        else
        {
            thumbPath.addRoundedRectangle(thumbStartPosition + thumbIndent, y + thumbIndent,
                thumbSize - thumbIndentx2, height - thumbIndentx2, (height - thumbIndentx2) * 0.5f);

            slotPath.addRoundedRectangle(x + thumbIndent,
                y + thumbIndent,
                width - thumbIndentx2,
                height - thumbIndentx2,
                (height - thumbIndentx2) * 0.5f);

        }
    }

    Colour thumbCol(scrollbar.findColour(ScrollBar::thumbColourId, true));
    Colour slotCol(scrollbar.findColour(ScrollBar::backgroundColourId, true));

    if (isMouseOver || isMouseDown)
        thumbCol = GuiHelpers::makeBrighter(thumbCol);

    g.setColour(slotCol);
    g.fillPath(slotPath);

    g.setColour(thumbCol);
    g.fillPath(thumbPath);


    //g.setColour(thumbCol.contrasting((isMouseOver || isMouseDown) ? 0.2f : 0.1f));
    //g.strokePath(thumbPath, PathStrokeType(1.0f));
}


void AppLookAndFeel::fillResizableWindowBackground(Graphics& g, int /*w*/, int /*h*/,
    const BorderSize<int>& /*border*/, ResizableWindow& window)
{
    g.fillAll(window.getBackgroundColour());

    /*
    juce::Rectangle<float> rect(1, 1, (float)w - 2, (float)h - 2);
    g.setColour(window.getBackgroundColour());
    g.fillRoundedRectangle(rect, 13);
    */
}

void AppLookAndFeel::drawResizableFrame(Graphics& /*g*/, int /*w*/, int /*h*/, const BorderSize<int>& /*border*/)
{
}

int AppLookAndFeel::getDefaultScrollbarWidth()
{
    return 9;
}


void AppLookAndFeel::drawDrawableButton(Graphics& g, DrawableButton& button,
    bool isMouseOverButton, bool /*isButtonDown*/)
{
    bool toggleState = button.getToggleState();

    g.setColour(button.findColour(isMouseOverButton ? DrawableButton::backgroundOnColourId
        : DrawableButton::backgroundColourId));
    g.fillRoundedRectangle(0.0f ,0.0f, (float) button.getWidth(), (float) button.getHeight(), (float) button.getWidth() / 12.5f);

    const int textH = (button.getStyle() == DrawableButton::ImageAboveTextLabel)
        ? jmin(16, button.proportionOfHeight(0.25f))
        : 0;

    if (textH > 0)
    {
        g.setFont((float)textH);

        g.setColour(button.findColour(toggleState ? DrawableButton::textColourOnId
            : DrawableButton::textColourId)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.4f));

        g.drawFittedText(button.getButtonText(),
            2, button.getHeight() - textH - 1,
            button.getWidth() - 4, textH,
            Justification::centred, 1);
    }
}

void AppLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
    Colour background = textEditor.findColour(TextEditor::backgroundColourId);
    if (textEditor.isMouseOver() && !textEditor.hasKeyboardFocus(true))
        //g.setColour(Colour(AppData::componentHighlightColourLighter));
        background = GuiHelpers::makeBrighter(background);

    g.setColour(background);


    g.fillRoundedRectangle(0.0f, 0.0f, (float)width, (float)height, (float)height / 12.5f);
}

Font AppLookAndFeel::getTextButtonFont(TextButton&, int buttonHeight)
{
    return Font(buttonHeight * 0.5f);
}

void AppLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
    bool isMouseOverButton, bool isButtonDown)
{
    Colour baseColour(backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 1.0f)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

    if (isButtonDown)
        baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.1f);
    if (isMouseOverButton)
        baseColour = GuiHelpers::makeBrighter(backgroundColour);

    const bool flatOnLeft = button.isConnectedOnLeft();
    const bool flatOnRight = button.isConnectedOnRight();
    const bool flatOnTop = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

    const float width = (float) button.getWidth();
    const float height = (float) button.getHeight();

    if (width > 0 && height > 0)
    {
        const float cornerSize = (float)button.getHeight() / 12.5f;

        Path outline;
        outline.addRoundedRectangle(0.0f, 0.0f, width, height, cornerSize, cornerSize,
            !(flatOnLeft || flatOnTop),
            !(flatOnRight || flatOnTop),
            !(flatOnLeft || flatOnBottom),
            !(flatOnRight || flatOnBottom));

        g.setColour(baseColour);
        g.fillPath(outline);


        //drawButtonShape(g, outline, baseColour, height);
    }
}

void AppLookAndFeel::drawProgressBar(Graphics& g, ProgressBar& progressBar,
    int width, int height,
    double progress, const String& textToShow)
{
    const Colour background(progressBar.findColour(ProgressBar::backgroundColourId));
    const Colour foreground(progressBar.findColour(ProgressBar::foregroundColourId));

    g.setColour(background);
    g.fillRoundedRectangle(0.0f, 0.0f, (float)width - 0.0f, (float)height - 0.0f, (float)height / 12.5f);

    g.setColour(foreground);
    g.fillRoundedRectangle(0.0f, 0.0f,
        (float)jlimit(0.0, (double)width - 0.0, progress * (width - 0.0)),
        (float)height - 0.0f,
        (float)height / 12.5f);

    if (textToShow.isNotEmpty())
    {
        g.setColour(Colour::contrasting(background, foreground));
        g.setFont(height * 0.6f);

        g.drawText(textToShow, 0, 0, width, height, Justification::centred, false);
    }
}

void AppLookAndFeel::drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
    int w, int h, int titleSpaceX, int titleSpaceW,
    const Image* icon, bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();

    g.setColour(window.getBackgroundColour());
    g.fillAll();

    Font font(h * 0.65f, Font::bold);
    g.setFont(font);

    int textW = font.getStringWidth(window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr)
    {
        iconH = (int)font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = jmin(titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX
        : jmax(titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;

    if (icon != nullptr)
    {
        g.setOpacity(isActive ? 1.0f : 0.6f);
        g.drawImageWithin(*icon, textX, (h - iconH) / 2, iconW, iconH,
            RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }

    if (window.isColourSpecified(DocumentWindow::textColourId) || isColourSpecified(DocumentWindow::textColourId))
    {
        if (isActive)
            g.setColour(window.findColour(DocumentWindow::textColourId));
        else
            g.setColour(window.findColour(DocumentWindow::textColourId).darker());

    }
    else
    {
        g.setColour(window.getBackgroundColour().contrasting(isActive ? 0.7f : 0.4f));
    }

    g.drawText(window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}


/*
class AppLookAndFeel_DocumentWindowButton : public Button
{
public:
    AppLookAndFeel_DocumentWindowButton(const String& name, Colour c, const Path& normal, const Path& toggled)
        : Button(name), colour(c), normalShape(normal), toggledShape(toggled)
    {
    }

    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {
        Colour background(Colours::grey);
        Colour foreground(Colours::white);

        if (ResizableWindow* rw = findParentComponentOfClass<ResizableWindow>())
            background = rw->getBackgroundColour();

        if (DocumentWindow* dw = findParentComponentOfClass<DocumentWindow>())
        {
            if (dw->isColourSpecified(DocumentWindow::textColourId) || isColourSpecified(DocumentWindow::textColourId))
                foreground = dw->findColour(DocumentWindow::textColourId);
        }

        const float cx = getWidth() * 0.5f, cy = getHeight() * 0.5f;
        const float diam = jmin(cx, cy) * (isButtonDown ? 0.60f : 0.65f);

        g.setColour(background);
        g.fillEllipse(cx - diam, cy - diam, diam * 2.0f, diam * 2.0f);

        Colour c(background.contrasting(colour, 0.6f));
        c = foreground.brighter();

        if (!isEnabled())
            c = c.withAlpha(0.6f);
        else if (isMouseOverButton)
            c = background.contrasting(colour, 0.6f).brighter();

        g.setColour(c);
        g.drawEllipse(cx - diam, cy - diam, diam * 2.0f, diam * 2.0f, diam * 0.2f);

        Path& p = getToggleState() ? toggledShape : normalShape;

        float scale = 0.55f;
        g.fillPath(p, p.getTransformToScaleToFit(cx - diam * scale, cy - diam * scale,
            diam * 2.0f * scale, diam * 2.0f * scale, true));
    }

private:
    Colour colour;
    Path normalShape, toggledShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLookAndFeel_DocumentWindowButton)
};

Button* AppLookAndFeel::createDocumentWindowButton(int buttonType)
{
    Path shape;
    const float crossThickness = 0.25f;

    if (buttonType == DocumentWindow::closeButton)
    {
        shape.addLineSegment(Line<float>(0.0f, 0.0f, 1.0f, 1.0f), crossThickness * 1.4f);
        shape.addLineSegment(Line<float>(1.0f, 0.0f, 0.0f, 1.0f), crossThickness * 1.4f);

        return new AppLookAndFeel_DocumentWindowButton("close", Colour(0xffdd1100), shape, shape);
    }

    if (buttonType == DocumentWindow::minimiseButton)
    {
        shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        return new AppLookAndFeel_DocumentWindowButton("minimise", Colour(0xffaa8811), shape, shape);
    }

    if (buttonType == DocumentWindow::maximiseButton)
    {
        shape.addLineSegment(Line<float>(0.5f, 0.0f, 0.5f, 1.0f), crossThickness);
        shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        Path fullscreenShape;
        fullscreenShape.startNewSubPath(45.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 45.0f);
        fullscreenShape.addRectangle(45.0f, 45.0f, 100.0f, 100.0f);
        PathStrokeType(30.0f).createStrokedPath(fullscreenShape, fullscreenShape);

        return new AppLookAndFeel_DocumentWindowButton("maximise", Colour(0xff119911), shape, fullscreenShape);
    }

    jassertfalse;
    return nullptr;
}
*/
