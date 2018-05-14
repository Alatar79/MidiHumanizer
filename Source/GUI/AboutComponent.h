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

    AboutComponent.h
    Created: 6 Sep 2015 6:09:51pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef ABOUTCOMPONENT_H_INCLUDED
#define ABOUTCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "../AppData.h"

/* This class takes care of counting which notes have been associated with which trigger(s).
*/
class AboutComponent : public Component
{
public:

    AboutComponent();

    void paint(Graphics& g) override;
    void resized() override;

private:

    /** The Midi Humanizer logo
    */
    ImageComponent appLogo;
    /** Header text.
    */
    Label titleText;
    /** The date text.
    */
    Label dateText;
    /** The author text.
    */
    Label authorText;
    /** The info text.
    */
    Label infoText;

    //Background image for the whole component
    Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutComponent);
};



#endif  // ABOUTCOMPONENT_H_INCLUDED
