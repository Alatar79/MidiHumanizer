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

    MainTab.cpp
    Created: 29 Apr 2012 6:07:45pm
    Author:  Martin

  ==============================================================================
*/

#include "MainTab.h"
#include "../GuiHelpers.h"
#include "../../Midi/MidiPlayer.h"

//==============================================================================
MainTab::MainTab(MidiHelpers& midiHelperIn, MainAppWindow& appWindowIn)
    : midiHelper(midiHelperIn),
      mainAppWindowRef(appWindowIn),
      midiKeyboard(midiHelperIn.virtualKeyboardState, MidiKeyboardComponent::horizontalKeyboard),
      loadMidiButton("LoadMidiButton", DrawableButton::ButtonStyle::ImageFitted),
      loadPrjButton("LoadProjectButton", DrawableButton::ButtonStyle::ImageStretched),
      savePrjButton("SaveProjectButton", DrawableButton::ButtonStyle::ImageFitted),
      savePrjAsButton("SaveProjectAsButton", DrawableButton::ButtonStyle::ImageFitted),
      helpButton("helpButton", DrawableButton::ButtonStyle::ImageFitted),
      masterPanel(midiHelperIn, appWindowIn, *this),
      pianoRoll(midiHelperIn),
      pianoRollZoom(pianoRoll, pianoRollViewport)
      
{
    reset();
    setOpaque(true);

    currentProjectFileString = "";

    // add the virtual midi keyboard component..
    midiKeyboard.setPlayingRange(0,59);
    midiKeyboard.setLowestVisibleKey(60-36-6);
    addAndMakeVisible (&midiKeyboard);
    midiKeyboard.setOpaque(true);


    Font labelFont(18.0f, Font::bold);
    Font textBoxFont(16.0f, Font::plain);
    juce::BorderSize<int> labelBorder(0, 0, 0, 0);

    addAndMakeVisible(&currentMidiPositionLabel);
    currentMidiPositionLabel.setFont(labelFont);
    currentMidiPositionLabel.setText("Midi Position:", juce::NotificationType::dontSendNotification);
    currentMidiPositionLabel.setBorderSize(labelBorder);

    addAndMakeVisible(&chordThresholdLabel);
    chordThresholdLabel.setFont(labelFont);
    chordThresholdLabel.setText("Chord Threshold:", juce::NotificationType::dontSendNotification);
    chordThresholdLabel.setBorderSize(labelBorder);

    addAndMakeVisible(&currentMidiPositionTextField);
    currentMidiPositionTextField.setFont(textBoxFont);
    currentMidiPositionTextField.addListener(this);
    currentMidiPositionTextField.setTooltip("Change the playing position.");

    addAndMakeVisible(&chordThresholdTextField);
    chordThresholdTextField.setFont(textBoxFont);
    chordThresholdTextField.addListener(this);
    chordThresholdTextField.setText("0.0001", juce::NotificationType::dontSendNotification);
    chordThresholdTextField.setTooltip("Threshold in seconds for two notes in the midi file to be treated\n" 
                                        "as a chord. Only change if you know what you are doing!");


    addAndMakeVisible(&midiInputLabel);
    midiInputLabel.setFont(labelFont);
    midiInputLabel.setText("Midi Input:", juce::NotificationType::dontSendNotification);
    midiInputLabel.setBorderSize(labelBorder);

    midiInComboBox.addItem(AppData::noMidiSelected, -1);
    //midiInComboBox.addSeparator();
    Array<MidiDeviceInfo> midiDevices = juce::MidiInput::getAvailableDevices();
    for (int i = 0; i < midiDevices.size(); i++)
        midiInComboBox.addItem(midiDevices[i].name, i+1);

    midiInComboBox.setSelectedId(AppData::noMidiSelected, juce::NotificationType::dontSendNotification);
    midiInComboBox.addListener(this);
    midiInComboBox.setTooltip("The application needs midi input. Please select one.");
    midiInComboBox.setWantsKeyboardFocus(true);
    addAndMakeVisible(&midiInComboBox);

    pianoRollViewport.setViewedComponent(&pianoRoll, false);
    addAndMakeVisible(&pianoRollViewport);
    addAndMakeVisible(&pianoRollZoom);

    trackListComp = new TrackListComp(midiHelperIn, mainAppWindowRef, pianoRoll);
    trackListComp->setBounds(0, 0, 200, 600);
    tracksViewport.setViewedComponent(trackListComp);
    addAndMakeVisible(&tracksViewport);

    addAndMakeVisible(&masterPanel);

    GuiHelpers::creatDrawableButtonFromFileString("save_project_icon.svg", "Save \nProject", savePrjButton);
    addAndMakeVisible(savePrjButton);
    savePrjButton.addListener(this);
    savePrjButton.setTooltip("Save Project.");

    GuiHelpers::creatDrawableButtonFromFileString("open_project_icon.svg", "Load \nProject", loadPrjButton);
    addAndMakeVisible(loadPrjButton);
    loadPrjButton.addListener(this);
    loadPrjButton.setTooltip("Load Project.");

    GuiHelpers::creatDrawableButtonFromFileString("open_midi_file_icon.svg", "Load \nMidi", loadMidiButton);
    addAndMakeVisible(loadMidiButton);
    loadMidiButton.addListener(this);
    loadMidiButton.setTooltip("Load Midi File.");

    GuiHelpers::creatDrawableButtonFromFileString("save_project_as_icon.svg", "Save \nProject As", savePrjAsButton);
    addAndMakeVisible(savePrjAsButton);
    savePrjAsButton.addListener(this);
    savePrjAsButton.setTooltip("Save Project As.");


    setFramesPerSecond(64);
}

MainTab::~MainTab()
{
    //FIXME is this needed, or is it removed automatically upon Object destruction?
    currentMidiPositionTextField.removeListener(this);

}

//==============================================================================
void MainTab::paint (Graphics& g)
{
  
    g.setColour(Colour(AppData::mainBackgroundColour));
    g.fillAll();

    //draw higlight, if no midi has been selected. 
    if (midiInComboBox.getText() == AppData::noMidiSelected)
    {
        uint8 alphaValue = (getFrameCounter() % 192);
        if (alphaValue > 128)
            alphaValue = 128;
        if (alphaValue > 64)
            alphaValue = 128 - alphaValue;
        alphaValue = (64 - alphaValue) * 2;

        g.setColour(Colour(AppData::defaultTextColour).withAlpha(alphaValue));
        Rectangle<float> midInHighlightBounds = midiInComboBox.getBounds().toFloat().expanded(AppData::scalef(1.0));
        g.drawRoundedRectangle(midInHighlightBounds, AppData::scalef(midInHighlightBounds.getHeight() / 12.5), AppData::scalef(3.5));
    }

}

void MainTab::resized()
{

   
    //Edge indent for the background highlight to have some border.
    int indentFactor = 7;
    savePrjButton.setBounds(AppData::scalei(AppData::mainTabMenuStartX), 
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabIconSize), 
        AppData::scalei(AppData::mainTabIconSize));
    savePrjButton.setEdgeIndent(savePrjButton.getWidth() / indentFactor);

    savePrjAsButton.setBounds(savePrjButton.getRight() + AppData::scalei(AppData::mainTabIconDistance),
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabIconSize),
        AppData::scalei(AppData::mainTabIconSize));
    savePrjAsButton.setEdgeIndent(savePrjButton.getWidth() / indentFactor);

    loadPrjButton.setBounds(savePrjAsButton.getRight() + AppData::scalei(AppData::mainTabIconDistance),
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabIconSize),
        AppData::scalei(AppData::mainTabIconSize));
    loadPrjButton.setEdgeIndent(savePrjButton.getWidth() / indentFactor);

    loadMidiButton.setBounds(loadPrjButton.getRight() + AppData::scalei(AppData::mainTabIconDistance),
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabIconSize),
        AppData::scalei(AppData::mainTabIconSize));
    loadMidiButton.setEdgeIndent(savePrjButton.getWidth() / indentFactor);

    helpButton.setBounds(loadMidiButton.getRight() + AppData::scalei(AppData::mainTabIconDistance),
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabIconSize),
        AppData::scalei(AppData::mainTabIconSize));
    helpButton.setEdgeIndent(savePrjButton.getWidth() / indentFactor);

    Font menuFont((float) AppData::scalei(AppData::mainFontSize), Font::plain);

    midiInputLabel.setFont(menuFont);
    currentMidiPositionLabel.setFont(menuFont);
    chordThresholdLabel.setFont(menuFont);
    currentMidiPositionTextField.applyFontToAllText(menuFont);
    chordThresholdTextField.applyFontToAllText(menuFont);


    int labelWidth = (int) menuFont.getStringWidthFloat(midiInputLabel.getText());
    midiInputLabel.setBounds(loadMidiButton.getRight() + AppData::scalei(AppData::mainTabMenuDistanceIconsBoxes),
        AppData::scalei(AppData::mainTabMenuStartY), 
        labelWidth,
        AppData::scalei(AppData::mainTabMenuHeight));

    midiInComboBox.setBounds(midiInputLabel.getRight() + AppData::scalei(AppData::mainTabMenuDistanceLabelsComboboxes),
        AppData::scalei(AppData::mainTabMenuStartY), 
        AppData::scalei(AppData::mainTabMenuMidiInComboBoxSize), 
        AppData::scalei(AppData::mainTabMenuHeight));

    labelWidth = (int)menuFont.getStringWidthFloat(currentMidiPositionLabel.getText());
    currentMidiPositionLabel.setBounds(midiInComboBox.getRight() + AppData::scalei(AppData::mainTabMenuDistanceComboboxesLabels),
        AppData::scalei(AppData::mainTabMenuStartY),
        labelWidth,
        AppData::scalei(AppData::mainTabMenuHeight));
        
    currentMidiPositionTextField.setBounds(currentMidiPositionLabel.getRight() + AppData::scalei(AppData::mainTabMenuDistanceLabelsComboboxes),
        AppData::scalei(AppData::mainTabMenuStartY),
        AppData::scalei(AppData::mainTabMenuMidiPositionTextFieldSize),
        AppData::scalei(AppData::mainTabMenuHeight));
    currentMidiPositionTextField.recenter();
        
    labelWidth = (int)menuFont.getStringWidthFloat(chordThresholdLabel.getText());
    chordThresholdLabel.setBounds(currentMidiPositionTextField.getRight() + AppData::scalei(AppData::mainTabMenuDistanceComboboxesLabels),
        AppData::scalei(AppData::mainTabMenuStartY),
        labelWidth,
        AppData::scalei(AppData::mainTabMenuHeight));

    int startX = chordThresholdLabel.getRight() + AppData::scalei(AppData::mainTabMenuDistanceLabelsComboboxes);
    chordThresholdTextField.setBounds(startX,
        AppData::scalei(AppData::mainTabMenuStartY),
        getWidth() - startX - AppData::scalei(AppData::pianoRollRightBorder),
        AppData::scalei(AppData::mainTabMenuHeight));
    chordThresholdTextField.recenter();

    //set the bounds of currentMidiPositionTextField once more, to make it as long as chordThresholdTextField
    currentMidiPositionTextField.setBounds(currentMidiPositionTextField.getX(), currentMidiPositionTextField.getY(),
        chordThresholdTextField.getWidth(), currentMidiPositionTextField.getHeight());
    


    midiKeyboard.setKeyWidth((float) AppData::scalei(AppData::mainTabKeyboardKeyWidth));
    midiKeyboard.setBounds(AppData::scalei(AppData::mainTabKeyboarOuterMargin),
        getHeight() - AppData::scalei(AppData::mainTabKeyboardHeight) - AppData::scalei(AppData::mainTabKeyboarOuterMargin),
        getWidth() -  AppData::scalei(2 * AppData::mainTabKeyboarOuterMargin),
        AppData::scalei(AppData::mainTabKeyboardHeight));

    masterPanel.setBounds(AppData::scalei(AppData::masterTrackPanelStartX),
        AppData::scalei(AppData::iconMenuHeight),
        AppData::scalei(AppData::masterTrackPanelWidth),
        AppData::scalei(AppData::masterTrackPanelHeight));

    tracksViewport.setScrollBarThickness(AppData::scalei(AppData::scrollBarThickness));
    int tracksViewportStartY = masterPanel.getBottom() + AppData::scalei(AppData::masterTrackPanelBottomBorder);
    Point<int> tracksViewPos = tracksViewport.getViewPosition();
    tracksViewport.setBounds(AppData::scalei(AppData::trackPanelStartX),
        tracksViewportStartY,
        AppData::scalei(AppData::trackPanelWidth) + AppData::scalei(AppData::scrollBarThickness) + AppData::scalei(AppData::scrollBarBorder),
        midiKeyboard.getY() - AppData::scalei(AppData::distanceTrackPanelKeyboard) - tracksViewportStartY);
    jassert(tracksViewport.getViewedComponent() != nullptr);
    tracksViewport.getViewedComponent()->resized();
    tracksViewport.setViewPosition(tracksViewPos);

    //AppData::scalei(AppData::)
    pianoRollViewport.setScrollBarThickness(AppData::scalei(AppData::scrollBarThickness));
    int pianoRollViewPortStartX = tracksViewport.getRight() + AppData::scalei(AppData::trackPanelRightBorder);
    int pianoRollHeight = tracksViewport.getHeight();
    int pianoRollWidth = getWidth() - pianoRollViewPortStartX - AppData::scalei(AppData::pianoRollRightBorder);
    Point<int> pianoRollViewPos = pianoRollViewport.getViewPosition();
    pianoRollViewport.setBounds(pianoRollViewPortStartX,
        tracksViewport.getY(),
        pianoRollWidth, 
        pianoRollHeight);
    pianoRollViewport.setViewPosition(pianoRollViewPos);



    int height = AppData::scalei(AppData::mainTabMenuHeight);
    int width = AppData::scalei(AppData::zoomComponentWidth);
    pianoRollZoom.setBounds(getWidth() - AppData::scalei(AppData::pianoRollRightBorder) - width,
        masterPanel.getY() + (masterPanel.getHeight() - height  ) / 2,
        width,
        height);



}

void MainTab::update()
{
}

PianoRoll& MainTab::getPianoRoll()
{
    return pianoRoll;
}

ExtendedTextEditor& MainTab::getCurrentMidiPositionTextField()
{
    return currentMidiPositionTextField;
}

PianoRollViewport& MainTab::getPianoRollViewport()
{
    return pianoRollViewport;
}

OwnedArray<TrackPanel>& MainTab::getTrackPanelList()
{
    return trackListComp->getTrackPanelList();
}

bool MainTab::keyStateChanged(bool /*isKeyDown*/)
{
    Component *currentFocusComp = Component::getCurrentlyFocusedComponent();
    if (currentFocusComp == this)
    {
        DBG("The Main Tab has the focus!");
    }
    else
    {
        DBG(String("Focus is on: X=") + String(currentFocusComp->getX()) + String(", Y=") + String(currentFocusComp->getY()) + String(", W=") + String(currentFocusComp->getWidth()) + String(", H=") + String(currentFocusComp->getHeight()));
    }

    return false;
}

void MainTab::saveProject(const String& saveFileString)
{

    File file(saveFileString);
    if (file.exists() && !file.deleteFile())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Could not access file: " + saveFileString);
        return;
    }

    juce::ZipFile::Builder zipBuilder;
    bool saveOK = midiHelper.getMidiPlayer()->saveProjectFile(zipBuilder);
    if (!saveOK)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Something went wrong while saving the project data.");
        return;
    }

    juce::XmlElement xml("GENERAL_DATA");
    xml.setAttribute("MidiIn", midiInComboBox.getText());
    xml.setAttribute("MidiPosition", currentMidiPositionTextField.getText().getDoubleValue());
    xml.setAttribute("ChordThreshold", chordThresholdTextField.getText().getDoubleValue());
    xml.setAttribute("ZoomFactor", pianoRollZoom.getZoomFactor());
    MemoryOutputStream xmlOutStream;
    xml.writeToStream(xmlOutStream, "");
    MemoryBlock xmlMemoryBlock = xmlOutStream.getMemoryBlock();
    zipBuilder.addEntry(new juce::MemoryInputStream(xmlMemoryBlock, false), 9, "GeneralData.xml", Time::getCurrentTime());

    FileOutputStream os(file);
    if (os.failedToOpen())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Could not open file: " + saveFileString);
        return;
    }

    double progressDummy;
    if (!zipBuilder.writeToStream(os, &progressDummy))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Could not write project file: " + saveFileString);
        os.flush();
        return;
    }
    os.flush();

    setAppName(saveFileString);

}

bool MainTab::loadProject()
{
    juce::FileChooser myChooser("Please select a project file to load...", getSpecialLocation(), "*.hpr");
    if (!myChooser.browseForFileToOpen())
    {
        return false; 
    }
    else
    {
        fileString = myChooser.getResult().getFullPathName();

        File file(fileString);
        if (!file.exists())
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Could not find the file: " + fileString);
            return false;
        }

        ZipFile zipFile(file);


        if (zipFile.getNumEntries() != 3 ||
            zipFile.getIndexOfFileName("MidiData.mid") == -1 ||
            zipFile.getIndexOfFileName("TrackData.xml") == -1 ||
            zipFile.getIndexOfFileName("GeneralData.xml") == -1)
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "The project file has a wrong format: " + fileString);
            return false;
        }

        loadProgressBar = createProgressBar("Loading Project...");

        midiHelper.getMidiPlayer()->loadProjectFile(file, *this);
    }

    return true;
}

void MainTab::loadProjectPart2()
{
    File file(fileString);
    ZipFile zipFile(file);

    const ZipFile::ZipEntry* zipXml = zipFile.getEntry("GeneralData.xml");
    auto xmlStream = std::unique_ptr<InputStream>(zipFile.createStreamForEntry(*zipXml));
    if (xmlStream == nullptr)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Something went wrong while loading general data for project: " + fileString);
        return;
    }
    XmlDocument xmlDoc(xmlStream->readEntireStreamAsString());
    std::unique_ptr<XmlElement> xml = xmlDoc.getDocumentElement();
    if (xml == nullptr ||
        !xml->hasTagName("GENERAL_DATA"))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Something went wrong while loading general data for project: " + fileString);
        return;
    }

    double zoom = xml->getDoubleAttribute("ZoomFactor", 1.0);
    double chordThreshold = xml->getDoubleAttribute("ChordThreshold", MidiHelpers::midiDoubleThreshold);
    double midiPosition = xml->getDoubleAttribute("MidiPosition", 0);
    String midiIn = xml->getStringAttribute("MidiIn", AppData::noMidiSelected);

    pianoRollViewport.newFileLoaded();
    trackListComp->newFileLoaded();


    midiHelper.setMidiChordThreshold(chordThreshold);
    chordThresholdTextField.setText(String(chordThreshold), true);

    currentMidiPositionTextField.setText(String(midiPosition), true);
    midiHelper.getMidiPlayer()->setMidiCounter(midiPosition);

    midiInComboBox.setSelectedId(midiIn, juce::NotificationType::sendNotification);

    reset();
    pianoRollZoom.setZoomFactor(zoom);

    setAppName(fileString);
    checkMidiOutChannelAvailability();
}

bool MainTab::loadMidi()
{

    juce::FileChooser myChooser("Please select a MIDI file...", getSpecialLocation(), "*.mid");
    if (!myChooser.browseForFileToOpen())
    {
        return false;
    }
    else
    {
        loadProgressBar = createProgressBar("Loading Midi...");
        midiHelper.getMidiPlayer()->loadMidiFile(myChooser.getResult().getFullPathName(), *this);
    }

    return true; 

}

void MainTab::afterMidiLoad()
{

    setAppName("");
    reset();
    pianoRollViewport.newFileLoaded();
    trackListComp->newFileLoaded();
    masterPanel.setAllTracksToCurrentMidiout();

}

void MainTab::loadDefaultMidi()
{

    loadProgressBar = createProgressBar("Default Midi...");

    reset();
    ScopedPointer<MemoryInputStream> memoryStream;
    memoryStream = new MemoryInputStream(BinaryData::Bach_Prelude_1_in_C_mid,
        BinaryData::Bach_Prelude_1_in_C_midSize,
        false);

    midiHelper.getMidiPlayer()->loadMidiStream(memoryStream, *this);
}




std::unique_ptr<ExtendedProgressBar> MainTab::createProgressBar(String text)
{

    std::unique_ptr<ExtendedProgressBar> progressBar (new ExtendedProgressBar(text,
        midiHelper.getMidiPlayer()->getRefToLoadMidiProgressIndicator(),
        mainAppWindowRef,
        AppData::scalei(600), AppData::scalei(65)));

    return progressBar;

}



//==============================================================================


// This is our Slider::Listener callback, when the user drags a slider.
void MainTab::sliderValueChanged(Slider*)
{

}

void MainTab::setAppName(const String fullFilePath)
{

    String fileName;
    String modTime;
    if (fullFilePath != "")
    {
        fileName = File(fullFilePath).getFileName();
        modTime = File(fullFilePath).getLastModificationTime().toString(true, true, false, true);
        mainAppWindowRef.setName(JUCEApplication::getInstance()->getApplicationName() + " - " + fileName + "   ("+modTime+")");
    }
    else
    {
        fileName = "Untitled.hpr";
        modTime = "";
        mainAppWindowRef.setName(JUCEApplication::getInstance()->getApplicationName() + " - " + fileName);
    }
    currentProjectFileString = fullFilePath;

}

void MainTab::checkMidiOutChannelAvailability()
{
    StringArray missingMidOuts;

    MidiSequence& midiData(midiHelper.getMidiPlayer()->getMidiSequence());
    for (int i = 0; i < midiData.getNoOfTracks(); i++)
    {
        String midiOutName = midiData.getMidiOutName(i);
        StringArray midiDevices = juce::MidiOutput::getDevices();
        bool foundDevice = false;
        for (int j = 0; j < midiDevices.size(); j++)
        {
            if (midiOutName == AppData::noMidiSelected)
                foundDevice = true;
            else if (midiDevices.contains(midiOutName))
                foundDevice = true;
        }
        if (!foundDevice)
        {
            if (!missingMidOuts.contains(midiOutName))
                missingMidOuts.add(midiOutName);
        }
    }


    if (missingMidOuts.size() > 0)
    {
        String missingMidiOutListString = "";
        for (int i = 0; i < missingMidOuts.size(); i++)
        {
            missingMidiOutListString = missingMidiOutListString + missingMidOuts[i] + "\n";
        }


        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::InfoIcon, "The following Midi outputs are missing", missingMidiOutListString);
    }
}

File MainTab::getSpecialLocation()
{
    File currentFile;
    if (currentProjectFileString != "")
        return File(currentProjectFileString);
    else
#if JUCE_WINDOWS
        return File::getSpecialLocation(juce::File::invokedExecutableFile).getParentDirectory();
#else
        return File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources");
#endif
}


void MainTab::buttonClicked(Button* button)
{
    if (button == &loadMidiButton)
    {
        loadMidi();
    }
    else if (button == &savePrjAsButton ||
        (button == &savePrjButton && currentProjectFileString == ""))
    {
        juce::FileChooser myChooser("Please select a file for saving your project...", getSpecialLocation(), "*.hpr");
        if (myChooser.browseForFileToSave(true))
            saveProject(myChooser.getResult().getFullPathName());
    }
    else if (button == &savePrjButton)
    {
        saveProject(currentProjectFileString);
    }
    else if (button == &loadPrjButton)
    {
        loadProject();
    }
}

void MainTab::buttonStateChanged(Button*)
{

}

void MainTab::textEditorReturnKeyPressed(TextEditor& editor)
{
    if (&editor == &currentMidiPositionTextField)
    {
        midiHelper.getMidiPlayer()->setMidiCounter(editor.getText().getDoubleValue());
    }
    else if (&editor == &chordThresholdTextField)
    {
        double value = editor.getText().getDoubleValue();
        if (value > 0)
            midiHelper.setMidiChordThreshold(value);
        else
            editor.setText(String(midiHelper.getMidiChordThreshold()));
    }

    editor.unfocusAllComponents();
}

void MainTab::textEditorTextChanged(TextEditor &)
{

}

void MainTab::comboBoxChanged(ComboBox* comboBox)
{
    if (comboBox == &midiInComboBox)
    {
        StringArray midiDevices = MidiInput::getDevices();
        if (midiDevices.indexOf(comboBox->getText()) != -1)
        {
            int index = midiDevices.indexOf(comboBox->getItemText(comboBox->getSelectedId()));
            jassert(midiHelper.getMidiMessageCollector() != nullptr);
            midiInput = MidiInput::openDevice(index, midiHelper.getMidiMessageCollector());
            //if device is open in another application we cannot open it
            if (midiInput != nullptr)
                midiInput->start();
            else
                comboBox->setText("Error: Device busy.");
        }
        //else if (comboBox->getItemText(comboBox->getSelectedId()) == AppData::noMidiSelected)
        else if (comboBox->getText() == AppData::noMidiSelected)
        {
            midiInput->stop();
            midiInput = nullptr;
        }

    }
}

void MainTab::actionListenerCallback(const String &message)
{
    if (message == MidiPlayer::midiLoadingFinishedSuccessfully)
    {
        loadProgressBar = nullptr;
        afterMidiLoad();
    }
    else if (message == MidiPlayer::midiLoadingFinishedWithErrors)
    {
        loadProgressBar = nullptr;
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Something went wrong loading the MIDI file.");
    }
    else if (message == MidiPlayer::projectLoadingFinishedSuccessfully)
    {
        loadProgressBar = nullptr;
        loadProjectPart2();
    }
    else if (message == MidiPlayer::projectLoadingFinishedWithErrors)
    {
        loadProgressBar = nullptr;
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", "Something went wrong while loading the project file");

    }
}


void MainTab::reset()
{
    pianoRollZoom.reset();
}
