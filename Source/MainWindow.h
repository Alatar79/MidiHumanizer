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

This file is part of the MidiHumanizer application. 
Copyright 2017 by Alatar79

==============================================================================
*/

#ifndef __MAINWINDOW_H_2A2929F1__
#define __MAINWINDOW_H_2A2929F1__


#include "JuceHeader.h"
#include "AppData.h"
#include "Midi/MidiSequence.h"
#include "Midi/MidiHelpers.h"
#include "MainController.h"

//==============================================================================
class MainAppWindow   : public DocumentWindow
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();

    void resized() override;
    void paintOverChildren(Graphics &g) override;

    void closeButtonPressed() override;

    /** Dim the component. I.e. add a transparent black overlay.
    Used in order to put focus on dialog windows.
    */
    void dimOn();

    /** Turn off the dimming. I.e. show the component normally.
        Note: if another component still wants to dim, dimming is not turned off, until the
        other component also turns it off. 
    */
    void dimOff();



    /* Note: Be careful when overriding DocumentWindow methods - the base class
       uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in you content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       implementation calls the superclass's method.
    */

private:
    //==============================================================================

    /** It is possible, that several components want to dim the main windwow.
        Keep track of the number of dimOn/dimOff calls on continue draw dimmed, as long
        as not all requests for diming have been switched of by dimOff.
    */
    int dimCounter = 0;

    /** Main Controller object. Controls e.g. the gui 
    */
    std::unique_ptr<MainController> mainController; 
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};


#endif  // __MAINWINDOW_H_2A2929F1__
