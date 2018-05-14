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

#ifndef __MAINCOMPONENT
#define __MAINCOMPONENT

// Issue warning 4373 only once (new style of overrides).
#pragma warning( once : 4373 )

#include "JuceHeader.h"
#include "../AppData.h"
#include "MainTab/MainTab.h"
#include "AboutComponent.h"
#include "ExtendedGUI.h"
#include "../Midi/MidiSequence.h"
#include "../Midi/MidiThread.h"
#include "StartUpDialog.h"
#include "../MainWindow.h"
#include <memory.h>

//==============================================================================
class MainComponent :   public Component,
                        public DragAndDropContainer


{
public:    
    //==============================================================================
    /** Main Component of this application. It contains all the GUI.
        Needs global AppData and also needs access to the MainWindow for e.g. 
        changing the window title. 
    */
    MainComponent(MainAppWindow& appWindowRefIn );
    ~MainComponent();
    
    void resized();

    /** Returns the aboutTab Component. */
    AboutComponent& getAboutComponent();
    /** Returns a reference to the the Midi Helper*/
    MidiHelpers& getMidiHelper();
    /* Returns a reference to the the main Tab */
    MainTab& getMainTab(); 

    /** Dim the component. I.e. add a transparent black overlay.
    Calls MainWindow::dimOn().
    For more info see there. */
    void dimOn();

    /** Turn off the dimming. I.e. show the component normally.
    Calls MainWindow::dimOff().
    For more info see there. */
    void dimOff();

private:
    //==============================================================================

    /** Show the startup dialogue*/
    void showStartupDialogue();

    /** Tab component, which acts as the main menu */
    ExtendedTabbedComponent tabbedComponent;    

    /** Reference to the main application window. */    
    MainAppWindow& mainWindowRef;

    std::unique_ptr<TooltipWindow> myTooltip;

    /** Collection of common midi functions */
    MidiHelpers midiHelper;

    /**Tab for the main application screen */
    MainTab mainTab;
    /**Tab for the About information */
    AboutComponent aboutTab;

    /** OpenGL context. This is used in order to speed up 2D-drawing. I.e. also
    the GUI elements benefit from it.
    */
    OpenGLContext openGLContext;

  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};


#endif  // __MAINCOMPONENT
