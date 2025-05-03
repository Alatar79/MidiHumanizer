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

    MainComponent.cpp
    Created: 29 Apr 2012 12:04:21pm
    Author:  Martin

  ==============================================================================
*/

#include "MainComponent.h"

//----------------------------------------------------------------------

MainComponent::MainComponent(MainAppWindow& newAppWindowRefIn)
    : tabbedComponent(TabbedButtonBar::TabsAtTop),
      mainWindowRef(newAppWindowRefIn),
      mainTab(midiHelper, newAppWindowRefIn)
{

    addAndMakeVisible (&tabbedComponent);
    tabbedComponent.addTab(AppData::tabNames[AppData::TabIndices::Home],
                           Colour(AppData::tabBackgroundColour), 
                           &mainTab, 
                           false,
                           AppData::TabIndices::Home);

    tabbedComponent.addTab(AppData::tabNames[AppData::TabIndices::About],
        Colour(AppData::tabBackgroundColour),
        &aboutTab,
        false,
        AppData::TabIndices::About);

    tabbedComponent.addTab(AppData::tabNames[AppData::TabIndices::Help],
        Colour(AppData::tabBackgroundColour),
        nullptr,
        false,
        AppData::TabIndices::Help);

    myTooltip = std::unique_ptr<TooltipWindow>(new TooltipWindow());
    myTooltip->setMillisecondsBeforeTipAppears(400);
    myTooltip->setColour(TooltipWindow::backgroundColourId, Colours::lightgreen);
    myTooltip->setColour(TooltipWindow::outlineColourId, Colours::grey);

    // Add OpenGL renderer to our main Component. 
    // Used automatically by all our 2D menues in the application. 
    // Very important to do this. Gives big 
    // performance gain, especially on Retina displays. 
    openGLContext.attachTo(*this);

    showStartupDialogue();
}

MainComponent::~MainComponent()
{
    openGLContext.detach();
    myTooltip = nullptr;
}

void MainComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();

    tabbedComponent.setBounds (0, 0, w, h );
}

AboutComponent& MainComponent::getAboutComponent()
{
    return aboutTab;
}

MidiHelpers& MainComponent::getMidiHelper()
{
    return midiHelper; 
}

MainTab& MainComponent::getMainTab()
{
    return mainTab;
}

void MainComponent::dimOn()
{
    mainWindowRef.dimOn();
}

void MainComponent::dimOff()
{
    mainWindowRef.dimOff();
}

void MainComponent::showStartupDialogue()
{

    DialogWindow::LaunchOptions launchStartupDialog;
    launchStartupDialog.dialogTitle = "";
    launchStartupDialog.dialogBackgroundColour = Colour(AppData::componentBackgroundColour);
    bool pointerTakesOwnership = true;
    StartUpDialog* startupDlg = new StartUpDialog(mainTab, *this);
    startupDlg->setBounds(0, 0, AppData::scalei(AppData::DialogWindowWidth), AppData::scalei(AppData::DialogWindowHeight));
    launchStartupDialog.content.set(startupDlg, pointerTakesOwnership);
    launchStartupDialog.componentToCentreAround = this;
    launchStartupDialog.escapeKeyTriggersCloseButton = true;
    launchStartupDialog.useNativeTitleBar = false;
    launchStartupDialog.resizable = true;
    juce::DialogWindow* dlg = launchStartupDialog.launchAsync();
    dlg->centreWithSize(AppData::scalei(AppData::DialogWindowWidth), AppData::scalei(AppData::DialogWindowHeight));
    dlg->setColour(DocumentWindow::textColourId, Colour(AppData::defaultTextColour));
    //dlg->getCloseButton()->setVisible(false);


}
