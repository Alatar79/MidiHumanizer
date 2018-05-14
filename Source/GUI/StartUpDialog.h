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

    StartUpDialog.h
    Created: 31 Aug 2015 2:28:47pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef STARTUPDIALOG_H_INCLUDED
#define STARTUPDIALOG_H_INCLUDED

#include "JuceHeader.h"
#include "MainTab/MainTab.h"

/** This class is shown, when the user starts the application.
    It offers the user to either open a  midi file or a project file. 
*/
class StartUpDialog : public Component,
    public Button::Listener
{

public:
    StartUpDialog(MainTab& mainTabComp, MainComponent& newMainCompRef);

    virtual ~StartUpDialog();

    void paint(Graphics &g) override;
    void resized() override;


    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;



private:

    /** Button for loading a Midi file.
    */
    DrawableButton loadMidiButton;
    /** Button for loading a Huminzer project.
    */
    DrawableButton loadPrjButton;
    /** Text label on the load Midi button
    */
    Label loadMidiLabel;
    /** Text label on the load project button
    */
    Label loadPrjLabel;
    /** the appImage Component
    */
    ImageComponent appImageComp; 

    /** Reference to a MainTab object.
        Needed for loading Midi file or project.
        FIXME: Not very nice to include MainTab here, or is it?
    */
    MainTab& mainTabRef;
    /** Reference to the mainComponent. Needed in order to switch on of black overlay
    */
    MainComponent& mainCompRef;
    
    /** Indicates, that the user has loaded Midi or project file*/
    bool couldLoadMidi = false; 

    /** The note logo 
    */
    GroupComponent theGroup;
    Label infoTextTitle;
    Label infoText;
    Label questionText;
    TextButton cancelButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StartUpDialog);
};





#endif  // STARTUPDIALOG_H_INCLUDED
