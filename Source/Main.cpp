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

#define JUCE_MODAL_LOOPS_PERMITTED 0

#include "JuceHeader.h"
#include "MainWindow.h"
#include "AppData.h"
#include "Midi/MidiSequence.h"
#include "Midi/MidiHelpers.h"
#include "GUI/GuiHelpers.h"
#include <memory>


//==============================================================================
class MidiHumanizerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    MidiHumanizerApplication()
    {
    }

    ~MidiHumanizerApplication()
    {
    }

    //==============================================================================

    void setLookAndFeel()
    {

        appLook.setColour(TextButton::buttonColourId, Colours::lightgrey);
        appLook.setColour(TextButton::buttonOnColourId, Colours::grey);
        appLook.setColour(DrawableButton::backgroundOnColourId, GuiHelpers::makeBrighter(Colour(AppData::optionBackgroundColour)));

        appLook.setColour(ToggleButton::textColourId, Colour(AppData::componentHighlightColour));
        appLook.setColour(Slider::rotarySliderFillColourId, Colour((uint8)180, (uint8)230, (uint8)255, (uint8)255));
        appLook.setColour(TooltipWindow::backgroundColourId, Colour(AppData::appToolTipBlueColour));
        appLook.setColour(TooltipWindow::outlineColourId, Colours::black);
        appLook.setColour(ComboBox::backgroundColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(ComboBox::buttonColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(ComboBox::outlineColourId, Colour(0x00ffffff));
        appLook.setColour(ComboBox::textColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(ComboBox::arrowColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(PopupMenu::backgroundColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(PopupMenu::highlightedTextColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(PopupMenu::textColourId, Colour(AppData::comboBoxTextColour));

        appLook.setColour(PopupMenu::highlightedBackgroundColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(TextEditor::backgroundColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(TextEditor::textColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(TextEditor::focusedOutlineColourId, Colour(0x00ffffff));
        appLook.setColour(TextEditor::outlineColourId, Colour(0x00ffffff));
        appLook.setColour(TextEditor::highlightColourId, Colour(AppData::defaultTextColour).withAlpha(0.5f));
        appLook.setColour(CaretComponent::caretColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(Label::textColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(TabbedComponent::backgroundColourId, Colour(AppData::tabBackgroundColour));
        appLook.setColour(TabbedButtonBar::frontTextColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(TabbedButtonBar::tabTextColourId, Colour(AppData::defaultTextColour).darker());

        appLook.setColour(MidiKeyboardComponent::upDownButtonBackgroundColourId, Colour(AppData::pianoScrollButtonColour));
        appLook.setColour(MidiKeyboardComponent::upDownButtonArrowColourId, Colour(AppData::pianoScrollButtonTriangleColour));
        appLook.setColour(MidiKeyboardComponent::whiteNoteColourId, Colour(AppData::pianoWhiteKeyColour));
        appLook.setColour(MidiKeyboardComponent::blackNoteColourId, Colour(AppData::pianoBlackKeyColour));
        appLook.setColour(MidiKeyboardComponent::keySeparatorLineColourId, Colour(AppData::pianoLineColour));
        appLook.setColour(MidiKeyboardComponent::shadowColourId, (Colours::transparentWhite));

        appLook.setColour(ScrollBar::thumbColourId, Colour(AppData::componentHighlightColour));
        appLook.setColour(ScrollBar::backgroundColourId, Colour(AppData::componentBackgroundColour));

        appLook.setColour(AlertWindow::backgroundColourId, Colour(AppData::optionBackgroundColour));
        appLook.setColour(AlertWindow::textColourId, Colour(AppData::defaultTextColour));
        appLook.setColour(AlertWindow::outlineColourId, Colours::transparentWhite);

        bool imaginary;
        CIELCHColour buttonBackground = CIELCHColour(AppData::componentHighlightColour).withMultipliedCIELightness(1.1f, imaginary);
        appLook.setColour(TextButton::buttonColourId, buttonBackground.getJuceColour());
        appLook.setColour(TextButton::buttonOnColourId, buttonBackground.getJuceColour());
        appLook.setColour(TextButton::textColourOffId, Colour(AppData::defaultTextColour));
        appLook.setColour(TextButton::textColourOnId, Colour(AppData::defaultTextColour));

        appLook.setDefaultSansSerifTypefaceName(AppData::fontName);
        LookAndFeel::setDefaultLookAndFeel(&appLook);

    }

    void initialise (const String& )
    {
        setLookAndFeel();
        mainWindow = std::make_unique<MainAppWindow>();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        ModalComponentManager::getInstance()->cancelAllModalComponents();
        GuiHelpers::clear();
        quit();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return "Midi Humanizer";
    }

    const String getApplicationVersion()
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& )
    {
        
    }

private:
    std::unique_ptr<MainAppWindow> mainWindow;
    AppLookAndFeel appLook;

};

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(MidiHumanizerApplication)
