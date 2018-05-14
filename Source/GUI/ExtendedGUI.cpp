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

    ExtendedTabbedComponent.cpp
    Created: 29 Jun 2015 11:10:22am
    Author:  MartinG

  ==============================================================================
*/

#include "ExtendedGUI.h"
#include "../AppData.h"


ProgressBarComp::ProgressBarComp (double& progress_, int width_, int height_)
: ProgressBar(progress_)
{
    width = width_;
    height = height_;
}

void ProgressBarComp::resized()
{
   //setBounds(0, 0, width, height);
    //int w = getWidth();
    //int h = getHeight();
    //int i = 0;
}

ExtendedProgressBar::ExtendedProgressBar(String text, double& progress, MainAppWindow& newMainAppWindowRef, int width, int height)
    : mainAppWindowRef(newMainAppWindowRef)
{
    mainAppWindowRef.dimOn();

    ProgressBarComp* progressBar = new ProgressBarComp(progress, width, height);
    progressBar->setTextToDisplay(text);
    progressBar->setBounds(0, 0, width, height);
    progressBar->setColour(ProgressBar::backgroundColourId, Colour(AppData::componentBackgroundColour));
    progressBar->setColour(ProgressBar::foregroundColourId, Colour(AppData::componentHighlightColour));
    progressBar->setOpaque(false);

    DialogWindow::LaunchOptions launch;
    launch.dialogTitle = String::empty;
    launch.dialogBackgroundColour = Colour(0xff151520);
    launch.content.set(progressBar, /*takeOwnhership*/ true);
    launch.componentToCentreAround = &newMainAppWindowRef;
    launch.escapeKeyTriggersCloseButton = false;
    launch.useNativeTitleBar = false;
    launch.resizable = false;

    dialogWindow = launch.launchAsync();

    dialogWindow->setTitleBarHeight(0);
    dialogWindow->setOpaque(true);
    dialogWindow->setSize(width, height);
    //dialogWindow->centreWithSize(width, height);



}
ExtendedProgressBar::~ExtendedProgressBar()
{
    mainAppWindowRef.dimOff();
    dialogWindow->exitModalState(0);
}


ExtendedComboBox::ExtendedComboBox()
    :ComboBox()
{
    addMouseListener(this, true);
}

void ExtendedComboBox::mouseEnter(const MouseEvent &event)
{
    ComboBox::mouseEnter(event);
    repaint();
}

void ExtendedComboBox::mouseExit(const MouseEvent &event)
{
    ComboBox::mouseExit(event);
    repaint();
}



ExtendedTabbedComponent::ExtendedTabbedComponent(TabbedButtonBar::Orientation orientation)
    :TabbedComponent(orientation)
{

}

void ExtendedTabbedComponent::currentTabChanged(int , const String &newCurrentTabName)
{
    unfocusAllComponents();

    if (newCurrentTabName == AppData::tabNames[AppData::TabIndices::Help])
    {
        juce::File file;
#if JUCE_WINDOWS
        file = juce::File::getSpecialLocation(juce::File::invokedExecutableFile).getSiblingFile("Help.pdf");
#else
        file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/Help.pdf");
#endif
        if (file.existsAsFile())
        {
            if (!juce::Process::openDocument("file:" + file.getFullPathName(), juce::String::empty))
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon, "Error", "PDF Reader must be installed to show the help file.", juce::String::empty, nullptr);
            }
        }
        else
        {
            juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon, "Error", "Help file 'Help.pdf' does not exist!\n" + file.getFullPathName(), juce::String::empty, nullptr);
        }

        setCurrentTabIndex(AppData::TabIndices::Home);
    }

}

ExtendedTextEditor::ExtendedTextEditor()
    :TextEditor()
{

}

void ExtendedTextEditor::recenter()
{
    int fontHeight = (int) getFont().getHeight();
    int newTopIndent = (getHeight() - fontHeight) / 2;
    int newLeftIndent = fontHeight / 2;
    setBorder(BorderSize<int>(0, 0, 0, 0));
    setIndents(newLeftIndent, newTopIndent);
    applyFontToAllText(getFont()); //workaround to make sure, the text is refitted. 
}

void ExtendedTextEditor::mouseEnter(const MouseEvent &/*event*/)
{
    repaint();
}

void ExtendedTextEditor::mouseExit(const MouseEvent &/*event*/)
{
    repaint();
}



ExtendedLabel::ExtendedLabel()
    :Label()
{
}

int ExtendedLabel::getWidthFromFont()
{
    Font f = getFont();
    return (int)f.getStringWidthFloat(getText()) + 1;
}

//=========================================================================

ExtendedMidiKeyboardComponent::ExtendedMidiKeyboardComponent(MidiKeyboardState& state,
    Orientation orientation)
    : MidiKeyboardComponent(state, orientation)
{
    minAvaiableNote = 0;
    maxAvaiableNote = 127;

    for (int i = 0; i<128; i++)
        velocity[i] = 0;
}

void ExtendedMidiKeyboardComponent::setPlayingRange(int lowestNote, int highestNote)
{
    jassert(lowestNote >= 0 && lowestNote <= 127);
    jassert(highestNote >= 0 && highestNote <= 127);

    minAvaiableNote = lowestNote;
    maxAvaiableNote = highestNote;
}

void ExtendedMidiKeyboardComponent::handleNoteOn(MidiKeyboardState* source, int channel, int midiNoteNumber, float vel)
{
    this->velocity[midiNoteNumber] = vel;
    MidiKeyboardComponent::handleNoteOn(source, channel, midiNoteNumber, vel);
}

void ExtendedMidiKeyboardComponent::drawWhiteNote(int midiNoteNumber,
    Graphics& g, Rectangle<float> area,
    bool isDown, bool isOver,
    Colour lineColour,
    Colour textColour)
{
    //(over)paint background
    g.setColour(findColour(upDownButtonArrowColourId));
    g.fillRect(area.expanded(1.0f, 0.0f));

    //key is smaller than the whole size given to drawWhiteNote
    int realHeight = (int)round(((double)area.getHeight() * 6.0) / 7.0);

    if (isDown)
        realHeight = (realHeight * 49) / 50;

    int cornerSize = 0;
    int frontShadowSize = 0;
    int backShadowSize = 0;
    float lineSize = 1;

    switch (getOrientation())
    {
    case horizontalKeyboard: 
        cornerSize = (int)round((double)area.getHeight() / 40.0); 
        frontShadowSize = (int)round((double)area.getHeight() / 40.0);
        backShadowSize = (int)round((double)area.getHeight() / 30.0);
        lineSize = jmax(1.0f, (float) round((double)area.getHeight() / 140.0));
        break;
    case verticalKeyboardFacingLeft:   
    case verticalKeyboardFacingRight:   
        cornerSize = (int)round((double)area.getWidth() / 40.0);
        frontShadowSize = (int)round((double)area.getWidth() / 40.0);
        backShadowSize = (int)round((double)area.getWidth() / 30.0);
        lineSize = (float)round((double)area.getWidth() / 140.0);
        break;
    default: break;
    }

    if (isDown)
        backShadowSize = (backShadowSize * 3) / 2;

    //Front shadow colour
    //g.setColour((Colours::black).withAlpha(0.1f));
    g.setColour(Colour(AppData::pianoFrontShadowColour));
    
    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRoundedRectangle((float) area.getX(), (float) realHeight - frontShadowSize, (float) area.getWidth(), (float) 2 * frontShadowSize, (float) cornerSize);
        break;
    case verticalKeyboardFacingLeft:    break; // todo
    case verticalKeyboardFacingRight:   break; // todo
    default: break;
    }
    
    Colour c(findColour(whiteNoteColourId));

    if (isDown)
    {

        if ( midiNoteNumber <= maxAvaiableNote)
            c = Colour(AppData::pianoKeyRedColour);
        else
            c= Colour(AppData::pianoKeyGreenColour);
        c = c.withBrightness((velocity[midiNoteNumber])*0.5f+ sqrt(velocity[midiNoteNumber])*0.5f);
    }
    if (isOver && !isDown)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

    //main note colour
    g.setColour(c);

    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRoundedRectangle((float) area.getX()+ lineSize, (float) area.getY() - cornerSize, (float) area.getWidth()- lineSize, (float) realHeight + cornerSize, (float) cornerSize);
        break;
    case verticalKeyboardFacingLeft:    /* todo*/ break;
    case verticalKeyboardFacingRight:   /* todo*/ break;
    default: break;
    }


    const String text(getWhiteNoteText(midiNoteNumber));

    if (text.isNotEmpty())
    {
        const float fontHeight = jmin(12.0f, getKeyWidth() * 0.9f);

        g.setColour(textColour);
        g.setFont(Font(fontHeight).withHorizontalScale(0.8f));

        switch (getOrientation())
        {
        case horizontalKeyboard:            g.drawText(text, (int)area.getX() + 1, (int)area.getY(), (int)area.getWidth() - 1, realHeight - 2, Justification::centredBottom, false); break;
        case verticalKeyboardFacingLeft:    g.drawText(text, (int)area.getX() + 2, (int)area.getY() + 2, (int)area.getWidth() - 4, (int)area.getHeight() - 4, Justification::centredLeft, false); break;
        case verticalKeyboardFacingRight:   g.drawText(text, (int)area.getX() + 2, (int)area.getY() + 2, (int)area.getWidth() - 4, (int)area.getHeight() - 4, Justification::centredRight, false); break;
        default: break;
        }
    }
    

    if (!lineColour.isTransparent())
    {
        g.setColour(lineColour);

        switch (getOrientation())
        {
        case horizontalKeyboard:            
            g.drawRoundedRectangle((float) area.getX(), (float) area.getY()- cornerSize, (float) area.getWidth(), (float) realHeight + cornerSize, (float) cornerSize, lineSize);
            break;
        case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
        case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
        default: break;
        }

        if (midiNoteNumber == getRangeEnd())
        {
            switch (getOrientation())
            {
            case horizontalKeyboard:            g.fillRect(area.getX() + area.getWidth(), area.getY(), 1.0f, (float)realHeight); break;
            case verticalKeyboardFacingLeft:    g.fillRect(area.getX(), area.getY() + area.getHeight(), area.getWidth(), 1.0f); break;
            case verticalKeyboardFacingRight:   g.fillRect(area.getX(), area.getY() - 1, area.getWidth(), 1.0f); break;
            default: break;
            }
        }
    }

    //back shadow
    g.setColour(Colour(AppData::pianobackShadowColour));
    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRect(area.getX(), area.getY(), area.getWidth(), (float)backShadowSize);
        break;
    case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
    case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
    default: break;
    }

}

String ExtendedMidiKeyboardComponent::getWhiteNoteText(int /*midiNoteNumber*/)
{

    return String();
}

void ExtendedMidiKeyboardComponent::drawBlackNote(int /*midiNoteNumber*/,
    Graphics& g, Rectangle<float> area,
    bool isDown, bool isOver,
    Colour noteFillColour)
{

    int cornerSize = 5;
    int frontShadowSize = 0;
    float lineSize = 1;
    int backShadowSize = 0;

    int x = (int)area.getX(); 
    int y = (int)area.getY();
    int h = (int)area.getHeight();
    int w = (int)area.getWidth();

    switch (getOrientation())
    {
    case horizontalKeyboard:
        w = (int)round((double)w * 0.9);
        x = x + (int)round((double)w * 0.1);
        cornerSize = (int)round((double)h / 30.0);
        frontShadowSize = (int)round((double)h / 30.0);
        backShadowSize = (int)round((double)h / 30.0);
        lineSize = jmax(1.0f, (float)round((double)h / 140.0));
        break;
    case verticalKeyboardFacingLeft:
    case verticalKeyboardFacingRight:
        h = (int)round((double)h * 0.9);
        y = y + (int)round((double)h * 0.1);
        cornerSize = (int)round((double)w / 30.0);
        frontShadowSize = (int)round((double)w / 30.0);
        backShadowSize = (int)round((double)w / 30.0);
        lineSize = (float)round((double)w / 140.0);
        break;
    default: break;
    }


    //key is smaller than the whole size given to drawBlackNote
    int realHeight = (int)round(((double)h * 6.0) / 7.0);

    if (isDown)
    {
        backShadowSize = (backShadowSize * 3) / 2;
    }


    //Front shadow colour
    //g.setColour((Colours::black).withAlpha(0.1f));
    g.setColour(Colour(AppData::pianoFrontShadowColour));

    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRoundedRectangle((float)x, (float)realHeight - frontShadowSize, (float)w, (float)2 * frontShadowSize, (float)cornerSize);
        break;
    case verticalKeyboardFacingLeft:    /* todo*/ break;
    case verticalKeyboardFacingRight:   /* todo*/ break;
    default: break;
    }

    //main colour
    Colour c(noteFillColour);
    
    //if (isDown)  c = c.overlaidWith(findColour(keyDownOverlayColourId));
    if (isDown)  c = c.overlaidWith(Colour(AppData::pianoKeyGreenColour).withAlpha(0.4f));
    else if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

    g.setColour(c);
    switch (getOrientation())
    {
    case horizontalKeyboard:            g.fillRoundedRectangle((float)x, (float)y - (float)cornerSize, (float)w, (float)realHeight + (float)cornerSize, (float)cornerSize); break;
    case verticalKeyboardFacingLeft:    /*g.fillRect(x + w / 8, y + xIndent, w - w / 8, h - xIndent * 2);*/ break;
    case verticalKeyboardFacingRight:   /*g.fillRect(x, y + xIndent, 7 * w / 8, h - xIndent * 2);*/ break;
    default: break;
    }

    Colour lineColour = findColour(keySeparatorLineColourId).darker();

    if (!lineColour.isTransparent())
    {
        g.setColour(lineColour);

        switch (getOrientation())
        {
        case horizontalKeyboard:
            g.drawRoundedRectangle((float)x, (float)y - cornerSize, (float)w, (float)realHeight + cornerSize, (float)cornerSize, lineSize);
            break;
        case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
        case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
        default: break;
        }
    }


    //back shadow
    g.setColour(Colour(AppData::pianobackShadowColour));
    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRect((float)x, (float)y, (float)w, (float)backShadowSize);
        break;
    case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
    case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
    default: break;
    }

    /*
    if (isDown)
    {
        g.setColour(noteFillColour);
        g.drawRect(x, y, w, h);
    }
    else
    {
        g.setColour(c.brighter());
        const int xIndent = jmax(1, jmin(w, h) / 8);

        switch (getOrientation())
        {
        case horizontalKeyboard:            g.fillRect(x + xIndent, y, w - xIndent * 2, 7 * h / 8); break;
        case verticalKeyboardFacingLeft:    g.fillRect(x + w / 8, y + xIndent, w - w / 8, h - xIndent * 2); break;
        case verticalKeyboardFacingRight:   g.fillRect(x, y + xIndent, 7 * w / 8, h - xIndent * 2); break;
        default: break;
        }
    }
    */
}

void ExtendedMidiKeyboardComponent::drawUpDownButton(Graphics& g, int w, int h,
    const bool mouseOver,
    const bool buttonDown,
    const bool movesOctavesUp)
{
    //g.fillAll(Colours::transparentWhite);
    g.fillAll(Colour(AppData::mainBackgroundColour));

    int cornerSize = 10;
    int backShadowSize = 0;
    float lineSize = 1;

    switch (getOrientation())
    {
    case horizontalKeyboard:            
        cornerSize = (int)round((double)h / 30.0);  
        backShadowSize = (int)round((double)h / 30.0);
        lineSize = jmax(1.0f, (float)round((double)h / 140.0));
        break;
    case verticalKeyboardFacingLeft:    
    case verticalKeyboardFacingRight:   
        cornerSize = (int)round((double)w / 30.0); 
        backShadowSize = (int)round((double)w / 30.0);
        lineSize = jmax(1.0f, (float)round((double)w / 140.0));
        break;
    default:                            break;
    }

    g.setColour(findColour(upDownButtonBackgroundColourId));
    if (mouseOver)
    g.setColour(findColour(upDownButtonBackgroundColourId).withBrightness(0.47f));

    
    switch (getOrientation())
    {
    case horizontalKeyboard:            g.fillRoundedRectangle(0,  (float) -cornerSize, (float)w, (float)h+ cornerSize, (float)cornerSize);  break;
    case verticalKeyboardFacingLeft:
    case verticalKeyboardFacingRight:   break;
    default:                            break;
    }
    

    //separator line
    g.setColour(findColour(keySeparatorLineColourId).withAlpha(0.5f));
    float factor = movesOctavesUp ? 0.0f : 1.0f;
    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRect((float)((float)w-lineSize)*factor, 0.0, lineSize, (float)h);
        break;
    case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
    case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
    default: break;
    }


    //back shadow
    g.setColour(Colour(AppData::pianobackShadowColour));
    switch (getOrientation())
    {
    case horizontalKeyboard:
        g.fillRect(0.0f, 0.0f, (float)w, (float)backShadowSize);
        break;
    case verticalKeyboardFacingLeft:    /*g.fillRect(x, y, w, 1); todo*/ break;
    case verticalKeyboardFacingRight:   /*g.fillRect(x, y + realHeight - 1, w, 1); todo */ break;
    default: break;
    }

    float angle;

    switch (getOrientation())
    {
    case horizontalKeyboard:            angle = movesOctavesUp ? 0.0f : 0.5f;  break;
    case verticalKeyboardFacingLeft:    angle = movesOctavesUp ? 0.25f : 0.75f; break;
    case verticalKeyboardFacingRight:   angle = movesOctavesUp ? 0.75f : 0.25f; break;
    default:                            jassertfalse; angle = 0; break;
    }

    Path path;
    path.addTriangle(0.0f, 0.0f, 0.0f, 1.25f, 1.0f, 0.625f);
    path = path.createPathWithRoundedCorners(0.275f);
    path.applyTransform(AffineTransform::rotation(float_Pi * 2.0f * angle, 0.5f, 0.5f));

    g.setColour(findColour(upDownButtonArrowColourId));
    if (buttonDown)
        g.setColour(findColour(upDownButtonArrowColourId).withBrightness(1.0));

    g.fillPath(path, path.getTransformToScaleToFit(w * 0.15f, 0.15f * (float)h, w * 0.7f, 0.7f * (float)h, true));

}




bool ExtendedMidiKeyboardComponent::mouseDownOnKey(int, const MouseEvent &)
{
    //When the mouse is down, we want the full velocity possible.
    setVelocity(1.0f, true);
    return true;
}

void ExtendedMidiKeyboardComponent::mouseUpOnKey(int, const MouseEvent &)
{
    //When the mouse is up, limit velocity so the PC keyboard does not sound too much. 
    setVelocity(0.55f, true);
}
