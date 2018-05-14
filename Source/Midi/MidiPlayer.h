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

    MidiPlayer.h
    Created: 28 Jun 2015 5:57:42pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef MIDIPLAYER_H_INCLUDED
#define MIDIPLAYER_H_INCLUDED

#include "JuceHeader.h"
#include "MidiSequence.h"
#include "Player.h"
#include "../AppData.h"
#include "MidiHelpers.h"

/** Small helper class which encapsulates the midi Output String and the midi channel. 
*/
class MidiCompareObject
{
public:

    MidiCompareObject(const String* name, const int channel)
    {
        midiOutName = name;
        midiChannel = channel;
    }

    inline bool isEqual(const MidiCompareObject& other) const
    {
        if (this->midiChannel == other.midiChannel &&
            *(this->midiOutName) == *(other.midiOutName))
        {
            return true;
        }
        else
            return false;
    }

    /** The midiOut String. Note: Only a reference, because the object is going to be used in an
        Array. And I am not sure, if a juce::Array can handle an object, which is composed of a string.
        A reference to a string is fine thoug. That much I know. 
    */
    const String* midiOutName;
    /** The midi channel.
    */
    int midiChannel;

};

inline bool operator== (const MidiCompareObject& lhs, const MidiCompareObject& rhs)
{
    return lhs.isEqual(rhs);
}

inline bool operator!= (const MidiCompareObject& lhs, const MidiCompareObject& rhs)
{
    return !operator==(lhs, rhs);
}


class MidiPlayer : public ActionListener, public ActionBroadcaster
{
public:

    MidiPlayer();
    ~MidiPlayer();


    /** Load Midi file.
        Note: loading is done on a separate thread. Once the loading has finished, the
        caller is informed via the listener.
        On Success: midiLoadingFinishedSuccessfully
        On Failure: midiLoadingFinishedWithErrors
    */
    void loadMidiFile(const String& fileString, ActionListener& listener);

    /** Load a Midi file from stream.
        Note: loading is done on a separate thread. Once the loading has finished, the
        caller is informed via the listener.
        On Success: midiLoadingFinishedSuccessfully
        On Failure: midiLoadingFinishedWithErrors
    */
    void loadMidiStream(InputStream* inputStream, ActionListener& listener);

    /** Load project from file. That includes the midi file, but also information about
        the tracks: which tracks are enabled, what midi devices are being used...
        Note: loading is done on a separate thread. Once the loading has finished, the 
        caller is informed via the listener.
        On Success: projectLoadingFinishedSuccessfully
        On Failure: projectLoadingFinishedWithErrors
    */
    void loadProjectFile(File& file, ActionListener& listener);

    /** Get the reference to the Midi load progress variable.
    */
    double& getRefToLoadMidiProgressIndicator();

    /** Save project to xml blob. That includes the midi file, but also information about
    the tracks: which tracks are enabled, what midi devices are being used...
    Return true, if the project was saved successfully.
    */
    bool saveProjectFile(juce::ZipFile::Builder& xml);


    /** Process midi data (from external midi source, e.g. keyboard)
    */
    void processNextMidiBlock(AudioSampleBuffer& outputBuffer,
                              const MidiBuffer& data,
                              int startSample,
                              int numSamples);

    /** process all midi data
    */
    void processNextMidiBlock(const MidiBuffer& data,
                              int startSample,
                              int numSamples);

    /**Handle midi events AND apply velocity map
    */
    void handleMidiEvent(const MidiMessage& m);

    /** Reset the midi counter currentMidiTime, which is used to step forward in the midi data.
    */
    void resetMidiCounter();
    /** Get the midi counter/time, which is used to step forward in the midi data.
    */
    double getMidiCounter();
    /** Get the relative midi counter/time. I.e. the current dime divided by the length of the Midi sequence.
    */
    double getMidiCounterInPercent();
    /** Set the midi counter/time to value specified in counter
    */
    void setMidiCounter(double counter);
    /** Return the midiTimeHasChangedFlag value.
        It is e.g. used in the GUI thread to update the values in the GUI.
    */
    bool midiTimeHasChanged();
    /** Clear the midiTimeHasChangedFlag value.
    */
    void clearMidiTimeHasChangedFlag();

    MidiSequence& getMidiSequence();

    /** Get the chord times from all currently active triggers.
     */
    Array<double> getCurrentlyActiveChordTimes() const;

    static const String midiLoadingFinishedSuccessfully;
    static const String midiLoadingFinishedWithErrors;
    static const String projectLoadingFinishedSuccessfully;
    static const String projectLoadingFinishedWithErrors;


protected:
    /** This is used to control access to the process callback. */
    CriticalSection lock;

private:
    /** Reset all relevant variables to intial values.
    */
    void resetVars();

    /** Listen to Action brodcasts
    */
    void actionListenerCallback(const String &message) override;

    Array<ExtendedMidiMessage>& removeDuplicates(Array<ExtendedMidiMessage>& notes);

	static const int maxMidiNotes = 128;

    /**Map for mapping of incoming MIDI velocity to new values.
    Needed, because different keyboards have different velocity behaviour.*/
	//FIXME: need to remove this. No velocity mapping supported anymore. 
    float velocityMap[128];

	/**	account for all keys of the Midi keyboard 
	*/
	bool keyIsDown[maxMidiNotes];

    double currentMidiTime;
    bool midiTimeHasChangedFlag;
    Array<ExtendedMidiMessage> chordNotes;
    double lastNoteOnTime;

    MidiSequence midiData;
    Player player;
    /** Time to wait for chord in ms.*/
    double chordTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiPlayer);
};



#endif  // MIDIPLAYER_H_INCLUDED
