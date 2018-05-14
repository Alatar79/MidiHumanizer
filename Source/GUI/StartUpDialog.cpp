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

    StartUpDialog.cpp
    Created: 31 Aug 2015 2:28:47pm
    Author:  MartinG

  ==============================================================================
*/

#include "StartUpDialog.h"
#include "GuiHelpers.h"
#include "Paths.h"
#include "MainComponent.h"

StartUpDialog::StartUpDialog(MainTab& newMainTabRef, MainComponent& newMainCompRef)
    : loadMidiButton("LoadMidiButtonStartUp", DrawableButton::ButtonStyle::ImageFitted),
      loadPrjButton("LoadProjectButtonStartUp", DrawableButton::ButtonStyle::ImageFitted),
      mainTabRef(newMainTabRef),
      mainCompRef(newMainCompRef)

{

    setOpaque(false);

    appImageComp.setImage(AppData::getAppIcon());
    appImageComp.setImagePlacement(juce::RectanglePlacement::centred);
    addAndMakeVisible(appImageComp);

    infoTextTitle.setText("Welcome to the Midi Humanizer", juce::NotificationType::dontSendNotification);
    infoTextTitle.setJustificationType(juce::Justification::centred);
    infoTextTitle.setMinimumHorizontalScale(1.0f);
    infoTextTitle.setFont(Font(AppData::scalef(AppData::headingFontSize), Font::bold));
    addAndMakeVisible(infoTextTitle);

    infoText.setText("In order to work with the Midi Humanizer, you need to load a Midi "
        "file or a previously saved project file. "
        "Or you can click on cancel in order to load a default midi file.",
        juce::NotificationType::dontSendNotification);
    infoText.setMinimumHorizontalScale(1.0f);
    infoText.setJustificationType(juce::Justification::centred);
    infoText.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::plain));
    addAndMakeVisible(infoText);

    questionText.setText("What do you want to do?", juce::NotificationType::dontSendNotification);
    questionText.setJustificationType(juce::Justification::centred);
    questionText.setMinimumHorizontalScale(1.0f);
    questionText.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::bold));
    addAndMakeVisible(questionText);

    GuiHelpers::creatDrawableButtonFromFileString("open_project_icon.svg", "Load \nProject", loadPrjButton);
    addAndMakeVisible(loadPrjButton);
    loadPrjButton.addListener(this);

    GuiHelpers::creatDrawableButtonFromFileString("open_midi_file_icon.svg", "Load \nMidi", loadMidiButton);
    addAndMakeVisible(loadMidiButton);
    loadMidiButton.addListener(this);

    juce::BorderSize<int> labelBorder(0, 0, 0, 0);

    loadPrjLabel.setText("Load Project", juce::NotificationType::dontSendNotification);
    loadPrjLabel.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::plain));
    loadPrjLabel.setBorderSize(labelBorder);
    addAndMakeVisible(loadPrjLabel);

    loadMidiLabel.setText("Load Midi File", juce::NotificationType::dontSendNotification);
    loadMidiLabel.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::plain));
    loadMidiLabel.setBorderSize(labelBorder);
    addAndMakeVisible(loadMidiLabel);

    cancelButton.setButtonText("Cancel");
    cancelButton.setColour(TextButton::buttonColourId, Colour(AppData::optionBackgroundColour));
    cancelButton.setColour(TextButton::buttonOnColourId, Colour(AppData::optionBackgroundColour));
    cancelButton.addListener(this);
    addAndMakeVisible(cancelButton);

    mainCompRef.dimOn();

}

StartUpDialog::~StartUpDialog()
{
    if (!couldLoadMidi)
        mainTabRef.loadDefaultMidi();
    mainCompRef.dimOff();
}

void StartUpDialog::paint(Graphics& g)
{

    g.setColour(Colour(AppData::componentBackgroundColour));
    g.fillRect(0.0f, 0.0f, (float)getWidth(), (float)getHeight());

}

void StartUpDialog::resized()
{
    appImageComp.setSize(AppData::scalei(166), AppData::scalei(166));
    //appImageComp.setSize(getHeight()/3, getHeight() / 3);
    appImageComp.setCentreRelative(0.5f, 0.15f);

    infoTextTitle.setSize((int)(getWidth() * 0.8), AppData::scalei(50));
    infoTextTitle.setCentreRelative(0.5f, 0.375f);
    infoText.setSize((int)(getWidth() * 0.8), AppData::scalei(150));
    infoText.setCentreRelative(0.5f, 0.475f);
    questionText.setSize((int)(getWidth() * 0.8), AppData::scalei(300));
    questionText.setCentreRelative(0.5f, 0.575f);

    //Edge indent for the background highlight to have some border.
    int indentFactor = 7;
    loadMidiButton.setSize(AppData::scalei(AppData::mainTabIconSize), AppData::scalei(AppData::mainTabIconSize));
    loadMidiButton.setEdgeIndent(loadMidiButton.getWidth() / indentFactor);
    loadMidiButton.setCentreRelative(0.333f, 0.725f);
    loadPrjButton.setSize(AppData::scalei(AppData::mainTabIconSize), AppData::scalei(AppData::mainTabIconSize));
    loadPrjButton.setEdgeIndent(loadPrjButton.getWidth() / indentFactor);
    loadPrjButton.setCentreRelative(0.666f, 0.725f);

    int labelWidth = (int)loadMidiLabel.getFont().getStringWidthFloat(loadMidiLabel.getText());
    loadMidiLabel.setSize(labelWidth, 25);
    loadMidiLabel.setCentreRelative(0.333f, 0.8f);
    labelWidth = (int)loadPrjLabel.getFont().getStringWidthFloat(loadPrjLabel.getText());
    loadPrjLabel.setSize(labelWidth, 25);
    loadPrjLabel.setCentreRelative(0.666f, 0.8f);
    //43
    cancelButton.setSize(AppData::scalei(113), AppData::scalei(AppData::DialogButtonHeight));
    cancelButton.setCentreRelative(0.9f, 0.95f);

}

void StartUpDialog::buttonClicked(Button* button)
{
    if (button == &loadMidiButton)
    {
        couldLoadMidi = mainTabRef.loadMidi();
        
        //juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Could not aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaccess file:");
        //juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::InfoIcon, "Error", "Could not aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaccess file:");
        //juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::QuestionIcon, "Error", "Could not aaaaaaaaa aaaaaaaaa aaaaaaaaaaaaa aaaaaaaaaaaa ccess file:");
        
    }
    else if (button == &loadPrjButton)
    {
        couldLoadMidi = mainTabRef.loadProject();
    }


    //in any case, exit this dialog window
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>())
    {
        dw->exitModalState(0);
    }
   
}

void StartUpDialog::buttonStateChanged(Button* /*button*/)
{}
