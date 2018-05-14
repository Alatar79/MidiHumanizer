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

#include "MainWindow.h"
#include "GUI/MainComponent.h"
#include "GUI/GuiHelpers.h"
#include <math.h>

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colour(AppData::tabBackgroundColour),
                      DocumentWindow::allButtons)
{
    setColour(DocumentWindow::textColourId, Colour(AppData::defaultTextColour));
    

    centreWithSize (jmin (1165, (int) (0.8f * getParentWidth())), 
                    jmin (840, (int) (0.99f * getParentHeight())));
    setVisible (true);
    setResizable (true, false);
    MainComponent* mainComp = new MainComponent((*this));
    setContentOwned (mainComp, false);
    mainController = std::make_unique<MainController>(mainComp->getMainTab(), mainComp->getMidiHelper());

    setOpaque(true);
}

MainAppWindow::~MainAppWindow()
{ 
    //Manually release the mainController. Needed, because it holds a reference to MainComponent,
    //which is deleted implicitly, after deletion of this MainAppWindow object. 
    mainController.reset(nullptr); 

}

double oldScaleTime = 0;

void MainAppWindow::resized()
{
    AppData::appGlobalScaleFactor = ((double)getWidth()) / 1440.0;
    DocumentWindow::resized();
    
}

void MainAppWindow::paintOverChildren(Graphics &g)
{
    if (dimCounter > 0)
        g.fillAll(Colours::black.withAlpha(0.66f));
}

void MainAppWindow::closeButtonPressed()
{
    //Workaround to get rid of irritating asserts on quit. The application thinks we are leaking, because
    //GuiHelpers is only used static. 
    GuiHelpers::clear();

    JUCEApplication::getInstance()->systemRequestedQuit();
}


void MainAppWindow::dimOn()
{
    dimCounter++;
    repaint();
}

void MainAppWindow::dimOff()
{
    dimCounter--;
    jassert(dimCounter >= 0);
    repaint();
}