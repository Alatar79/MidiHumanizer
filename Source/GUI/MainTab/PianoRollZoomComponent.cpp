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

    PianoRollZoomComponent.cpp
    Created: 16 Aug 2015 3:42:22pm
    Author:  MartinG

  ==============================================================================
*/

#include "PianoRollZoomComponent.h"
#include "../GuiHelpers.h"
#include "../Paths.h"
#include "../../Midi/MidiPlayer.h"


PianoRollZoomComponent::PianoRollZoomComponent( PianoRoll& pianoRoll, PianoRollViewport& pianoRollViewport)
    : pianoRollRef(pianoRoll),
    pianoRollViewportRef(pianoRollViewport),
    zoomInButton("ZoomInButton", DrawableButton::ButtonStyle::ImageFitted),
    zoomOutButton("ZoomOutButton", DrawableButton::ButtonStyle::ImageFitted)
{

    reset();

    juce::Path plusSignPath = Drawable::parseSVGPath(Paths::plusSign);
    GuiHelpers::creatDrawableButtonFromPath(plusSignPath, "Zoom In", zoomInButton);
    addAndMakeVisible(zoomInButton);
    zoomInButton.addListener(this);
    zoomInButton.setTooltip("Zoom In.");
    zoomInButton.setColour(DrawableButton::backgroundColourId, Colour(AppData::optionBackgroundColour));
    zoomInButton.setColour(DrawableButton::backgroundOnColourId, GuiHelpers::makeBrighter(Colour(AppData::optionBackgroundColour)));

   

    juce::Path minusSignPath = Drawable::parseSVGPath(Paths::minusSign);
    GuiHelpers::creatDrawableButtonFromPath(minusSignPath, "Zoom Out", zoomOutButton);
    addAndMakeVisible(zoomOutButton);
    zoomOutButton.addListener(this);
    zoomOutButton.setTooltip("Zoom Out.");
    zoomOutButton.setColour(DrawableButton::backgroundColourId, Colour(AppData::optionBackgroundColour));
    zoomOutButton.setColour(DrawableButton::backgroundOnColourId, GuiHelpers::makeBrighter(Colour(AppData::optionBackgroundColour)));


    addAndMakeVisible(&zoomTextField);
    zoomTextField.addListener(this);
    createZoomTextFromDouble(100.0);
    zoomTextField.setTooltip("Zoom level in percent.");


}

void PianoRollZoomComponent::paint(Graphics& /*g*/)
{


}

void PianoRollZoomComponent::resized()
{
    zoomOutButton.setBounds(0, 0, getHeight(), getHeight());
    zoomOutButton.setEdgeIndent(zoomOutButton.getWidth() / 3);

    zoomInButton.setBounds(getWidth() - getHeight(), 0, getHeight(), getHeight());
    zoomInButton.setEdgeIndent(zoomOutButton.getWidth() / 3);

    zoomTextField.applyFontToAllText(Font(AppData::scalef(AppData::mainFontSize)));
    int gap = zoomInButton.getX() - zoomOutButton.getRight();
    int fieldWidth = AppData::scalei(AppData::zoomTextFieldWidth);
    zoomTextField.setBounds(zoomOutButton.getRight() + (gap - fieldWidth) /2, 0,
        fieldWidth, getHeight());
    zoomTextField.recenter();


}

double PianoRollZoomComponent::getZoomFactor() const
{
    return zoomFactor;
}

void PianoRollZoomComponent::setZoomFactor(const double zoom)
{
    
    if (zoom >= 0.01 && zoom <= 100)
        zoomFactor = zoom;
    createZoomTextFromDouble(zoomFactor);

    double currentViewportScale = (double)pianoRollViewportRef.getViewPositionX() / (double)pianoRollRef.getWidth();
    pianoRollRef.scale(zoomFactor);
    pianoRollViewportRef.setViewPosition((int)(pianoRollRef.getWidth() * currentViewportScale), 0);
    
}

void PianoRollZoomComponent::reset()
{
    zoomFactor = 1.0;
    createZoomTextFromDouble(zoomFactor);
}

double PianoRollZoomComponent::convertZoomTextToDouble()
{
    String zoomText = zoomTextField.getText().removeCharacters("%");
    return zoomText.getDoubleValue() / 100.0;
}

void PianoRollZoomComponent::createZoomTextFromDouble(double zoom)
{
    zoomTextField.setText(String(round(zoom * 100.0)) + "%", true);
}

void PianoRollZoomComponent::buttonClicked(Button* button)
{
    if (button == &zoomOutButton)
    {
        setZoomFactor(convertZoomTextToDouble()*0.8);
   }
    else if (button == &zoomInButton)
    {
        setZoomFactor(convertZoomTextToDouble()*1.25);
    }
}

void PianoRollZoomComponent::textEditorReturnKeyPressed(TextEditor& editor)
{
    if (&editor == &zoomTextField)
    {
        setZoomFactor(convertZoomTextToDouble());
    }

    editor.unfocusAllComponents();
}
