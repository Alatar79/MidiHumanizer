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

    PianoRollZoomComponent.h
    Created: 16 Aug 2015 3:42:22pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef PIANOROLLZOOMCOMPONENT_H_INCLUDED
#define PIANOROLLZOOMCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "../../GUI/MainTab/PianoRoll.h"
#include "../../GUI/ExtendedGUI.h"

/** Class, which provides GUI for zooming in/out a PianoRoll. 
*/
class PianoRollZoomComponent : public Component,
                               public Button::Listener,
                               public TextEditor::Listener
{
public:
    /** Constructor. Takes the piano roll, which we want to zoom on. 
    */
    PianoRollZoomComponent(PianoRoll& pianoRoll, PianoRollViewport& pianoRollViewport);

    void paint(Graphics& g) override;
    void resized() override;

    /** Return the current zoom factor.
    */
    double getZoomFactor() const;

    /** Set the current zoom factor and apply it to the piano roll.
    */
    void setZoomFactor(const double zoom);

    /** Reset this objects variables. 
    */
    void reset();

private:

    void buttonClicked(Button* button) override;
    //void buttonStateChanged(Button* button) override;
    void textEditorReturnKeyPressed(TextEditor& editor) override;
    //void textEditorTextChanged(TextEditor &) override;

    /** Convert the zoom text field text to floating point value.
    */
    double convertZoomTextToDouble();

    /** set the zoom text from double. 
    */
    void createZoomTextFromDouble(double zoom);


    /** Editor to manually enter zoom level. 
    */
    ExtendedTextEditor zoomTextField;

    /** Reference to the piano roll, which this component can zoom.
    */
    PianoRoll& pianoRollRef;
    /** Reference to the viewport, which contiains the piano roll. 
    */
    PianoRollViewport& pianoRollViewportRef;
    /** Keep track of the zoom factor, in case the user enters nonsense. 
    */
    DrawableButton zoomInButton;
    DrawableButton zoomOutButton;
    double zoomFactor; 
    

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollZoomComponent);
};


#endif  // PIANOROLLZOOMCOMPONENT_H_INCLUDED
