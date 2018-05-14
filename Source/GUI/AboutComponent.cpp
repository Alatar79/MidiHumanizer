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

    AboutComponent.cpp
    Created: 6 Sep 2015 6:09:51pm
    Author:  MartinG

  ==============================================================================
*/

#include "AboutComponent.h"
#include "GuiHelpers.h"
#include "Paths.h"


AboutComponent::AboutComponent()
{
    setOpaque(true);

    appLogo.setImage(AppData::getAppIcon());
    appLogo.setImagePlacement(juce::RectanglePlacement::centred);
    addAndMakeVisible(appLogo);

    String versionNumber = "Version 1.0";
    String versionType = "Registered Version";
    titleText.setText("Midi Humanizer - " + versionNumber,
        juce::NotificationType::dontSendNotification);
    titleText.setMinimumHorizontalScale(1.0f);
    titleText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleText);

    dateText.setText(__DATE__, juce::NotificationType::dontSendNotification);
    dateText.setMinimumHorizontalScale(1.0f);
    dateText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(dateText);

    authorText.setText("Programmed by Martin Schuppius", juce::NotificationType::dontSendNotification);
    authorText.setMinimumHorizontalScale(1.0f);
    authorText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(authorText);

    infoText.setText(
        "The Midi Humanizer is an interactive Midi player. "
        "You can use it to play an arbitrary Midi file, "
        "interactively. For help using the application click on "
        "the help tab.",
        juce::NotificationType::dontSendNotification);
    infoText.setMinimumHorizontalScale(1.0f);
    infoText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(infoText);
}

void AboutComponent::paint(Graphics& g)
{

    g.fillAll(Colour(AppData::mainBackgroundColour));

    juce::Rectangle<float> rect(AppData::scalef(232),AppData::scalef(138),AppData::scalef(966), AppData::scalef(644));
    g.setColour(Colour(AppData::componentBackgroundColour));
    g.fillRoundedRectangle(rect, AppData::scalef(AppData::cornerSize));

}


void AboutComponent::resized()
{
    appLogo.setSize(AppData::scalei(166), AppData::scalei(166));
    appLogo.setCentrePosition(getWidth() / 2, AppData::scalei(138 + 50 + 84));


    titleText.setFont(Font(AppData::scalef(AppData::headingFontSize), Font::bold));
    titleText.setSize(getWidth(), (int)titleText.getFont().getHeight());
    titleText.setCentrePosition(getWidth() / 2, appLogo.getBottom() + AppData::scalei(64) + (int)titleText.getFont().getHeight() / 3);

    dateText.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::bold));
    dateText.setSize(getWidth(), (int)dateText.getFont().getHeight());
    dateText.setCentrePosition(getWidth() / 2, titleText.getBottom() + (int)titleText.getFont().getHeight());


    authorText.setFont(Font(AppData::scalef(AppData::subHeadingFontSize), Font::plain));
    authorText.setSize(getWidth(), (int)authorText.getFont().getHeight());
    authorText.setCentrePosition(getWidth() / 2, dateText.getBottom() + (int)dateText.getFont().getHeight() + AppData::scalei(15));

    infoText.setFont(Font(AppData::scalef(AppData::abouTextFontSize), Font::plain));
    infoText.setSize(AppData::scalei(900), (int)infoText.getFont().getHeight() * 3);
    infoText.setCentrePosition(getWidth() / 2, authorText.getBottom() + AppData::scalei(50));
  
}
