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

    MainTab.h
    Created: 29 Apr 2012 6:07:45pm
    Author:  Martin

  ==============================================================================
*/

#ifndef __MAINTAB_H_E8E6BA6__
#define __MAINTAB_H_E8E6BA6__

#include "JuceHeader.h"
#ifdef IS_AUDIO_PLUGIN
#include "../Source/PluginProcessor.h"
#endif
#include "../../AppData.h"
#include "../../GUI/Paths.h"
#include "../../GUI/MainTab/PianoRoll.h"
#include "../../GUI/MainTab/TrackListComp.h"
#include "../../GUI/MainTab/MasterTrackPanel.h"
#include "../../Midi/MidiSequence.h"
#include "../../Midi/MidiHelpers.h"
#include "../../Midi/from_juce/juce_MidiMessageCollector_mod.h"
#include "../../MainWindow.h"
#include "../../GUI/MainTab/PianoRollZoomComponent.h"
#include "../../GUI/ExtendedGUI.h"


//forward declaration
class MainComponent; 

//==============================================================================
/** This is the editor component that our filter will display.
*/
class MainTab  : public AnimatedAppComponent,
                 public Slider::Listener,
                 public Button::Listener,
                 public TextEditor::Listener,
                 public ComboBox::Listener,
                 public ActionListener
{

public:

    MainTab (MidiHelpers& midiHelperIn, MainAppWindow& appWindowIn);
    ~MainTab();

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    void update() override;

    /** Get the piano roll component.
    */
    PianoRoll& getPianoRoll();
    /** Get the current Midi Position Text Field
    */
    ExtendedTextEditor& getCurrentMidiPositionTextField();
    /** Get the piano roll view port
    */
    PianoRollViewport& getPianoRollViewport();

    /** get a pointer to the list of track panels from the trackListComp component. 
    */
    OwnedArray<TrackPanel>& getTrackPanelList();

    /** check for keyboard, so we can forward the strokes to the midi keyboard. 
    */
    bool keyStateChanged(bool isKeyDown) override;

    /** Save the projcect as files string.
    */
    void saveProject(const String& fileString);

    /** load a project
        Returns true, if a project could successfully be loaded. False otherwise. 
    */
    bool loadProject();

    /** Second part of the project load. Why is this needed? The Midi file is loaded on a separate thread and
        takes the most time to load. After it has finished loading (asynchronusly) this part 2 has to be called.
    */
    void loadProjectPart2();

    /** load a Midi file.
        Returns true, if a Midi file could successfully be loaded. False otherwise.
    */
    bool loadMidi();

    /** Wrapper, which performs some final actions, after a Midi file is loaded.
    */
    void afterMidiLoad();

    /* Load default Midi file */
    void loadDefaultMidi();

    /** create a progress bar, used by load project and load midi
    */
    std::unique_ptr<ExtendedProgressBar> createProgressBar(String text);

private:
    /** Set the title bar of the Application Window to the name of the
        current project. The full file path to the project name is required.
        If the file path should be empty, the title bar is set to "Untitled.hpr"
    */
    void setAppName(const String fullFilePath);
    /** Check if all Midi channels from the midi tracks
        are available. If not, warn about it. 
    */
    void checkMidiOutChannelAvailability();
    /** Small helper function, which determines, which directory
        to show, when loading/saving a file. 
    */
    File getSpecialLocation();


    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;
    void textEditorReturnKeyPressed(TextEditor& editor) override;
    void textEditorTextChanged(TextEditor &) override;
    void comboBoxChanged(ComboBox* comboBox) override;
    void actionListenerCallback(const String &message) override;

    /** Collection of common midi function */
    MidiHelpers& midiHelper; 
    /** Reference to the main Window. Needed e.g. for changing the title after loading a project. */
    MainAppWindow& mainAppWindowRef;

    /* The virtual midi keyboard. */
    ExtendedMidiKeyboardComponent midiKeyboard;
    /** Button for loading a Midi file. */
    DrawableButton loadMidiButton;
    /** Button for loading a Humanizer project. */
    DrawableButton loadPrjButton;
    /** Button for saving a Humanizer project. */
    DrawableButton savePrjButton;
    /** Button for Save As. */
    DrawableButton savePrjAsButton;
    /** Button for getting help. */
    DrawableButton helpButton;

    Label currentMidiPositionLabel;
    Label midiInputLabel;
    Label chordThresholdLabel;
    MidiDeviceComboBox midiInComboBox;
    /* Displays, where in the midi track we are. 
    */
    ExtendedTextEditor currentMidiPositionTextField;
    /* Displays the note threshold, for two notes to be treated as in a chord.
    */
    ExtendedTextEditor chordThresholdTextField;
    /* Only on Mac: Name for new Midi Output to create.
    */
    ExtendedTextEditor createMidiOutTextField;

    PianoRollViewport pianoRollViewport;
    Viewport tracksViewport;
    /** From the master panel the user can change the parameters in all
     other panels at the same time.
     */
    MasterTrackPanel masterPanel;
    /** Piano roll, which displays all midi notes.
    */
    PianoRoll pianoRoll;
    /** Zoom Gui for the pianoRoll
    */
    PianoRollZoomComponent pianoRollZoom;
    /** Component, which includes all track panels. 
    */
    TrackListComp* trackListComp;

    /** String with the file path of the current project file name. 
    */
    String currentProjectFileString;
    /** the midi input, which the application is working with
    */
    ScopedPointer<MidiInput> midiInput;
    /** Reset this component.
    */
    /** File string used by the loadProjectFunction. Must be globabl, because the function is split in two parts .
    */
    String fileString;
    /** Progress bar shown during loading.
    */
    std::unique_ptr<ExtendedProgressBar> loadProgressBar;

    /** Reset all parameters of this MainTab
    */
    void reset();





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainTab);
};




#endif  // __MAINTAB_H_E8E6BA6__
