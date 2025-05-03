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

    MidiSequence.h
    Created: 21 Jun 2015 12:55:08pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#ifndef MIDISEQUENCE_H_INCLUDED
#define MIDISEQUENCE_H_INCLUDED

#include "JuceHeader.h"
#include "ExtendedMidiMessage.h"
#include "ExtendedMidiMessageSequence.h"
#include <memory>



//==============================================================================
/** Class for holding information about a trigger. Need to know both which note the trigger was 
    and which MIDI channel it send to. 
*/
class TriggerNote
{
public:
    TriggerNote();

    TriggerNote(const int note, const String& midiName);

    int noteNumber;
    juce::String midiOutName;

    inline bool isEqual(const TriggerNote& other) const
    {
        if (this->noteNumber == other.noteNumber &&
            this->midiOutName == other.midiOutName)
        {
            return true;
        }
        else
            return false;

    }


};

inline bool operator== (const TriggerNote& lhs, const TriggerNote& rhs)
{
    return lhs.isEqual(rhs);
}

inline bool operator!= (const TriggerNote& lhs, const TriggerNote& rhs)
{
    return !operator==(lhs, rhs);
}


/** This class manages a whole midi file, i.e. several midi sequences.
*/
class MidiSequence : private ActionListener
{
public:
    friend class MidiStreamLoader;
    friend class ProjectFileLoader;

    /** Helper class to load a Midi stream.
    */
    class MidiStreamLoader : public Thread, public ActionBroadcaster
    {
    public:
        /** Default Constructor. Do not use it, because it does nothing.
        */
        //MidiStreamLoader::MidiStreamLoader();

        /** Constructor. Set the input stream from which to read from and the destination MidiSequence.
        */
        MidiStreamLoader(InputStream* input, MidiSequence& dest);

        /** Starts the loading of the Midi stream.
        */
        virtual void run() override;

        /** The actual Midi stream loader.
        */
        bool loadMidiStream();

        static const String loadingFinishedSuccessfully;
        static const String loadingFinishedWithError;

    protected:
        /** Input Stream to load Midi from.
        */
        std::unique_ptr<InputStream> midiInputStream;

        /** MidiSequence to write to.
        */
        MidiSequence& destSequence;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiStreamLoader);

    };

    /** Helper class to load a Project file.
    */
    class ProjectFileLoader : public MidiStreamLoader
    {
    public:
        /** Default Constructor. Do not use it, because it does nothing.
        */
        //MidiStreamLoader::MidiStreamLoader();

        /** Constructor. Set the zip file from which to read from and the destination MidiSequence.
        */
        ProjectFileLoader(File& file, MidiSequence& dest);

        /** Starts the loading of the Midi stream.
        */
        void run() override;

        /** The actual Midi stream loader.
        */
        bool loadProjectFile();

        static const String loadingFinishedSuccessfully;
        static const String loadingFinishedWithError;

    private:

        std::unique_ptr<ZipFile> zipFile;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectFileLoader);

    };



    /** Construct a midi sequence from file fileString. 
    */
    MidiSequence(String fileString);
    /** Default Constructor
    */
    MidiSequence();

    ~MidiSequence();

    /** Reset the midi sequence and fill it with new data from file fileString.
        The listeners is informed, when the load operation has finished successfully 
        via a MidiSequence::MidiStreamLoader::loadingFinishedSuccessfully message.
    */
    void loadMidiFile(const String& fileString, ActionListener& listener);

    /** Reset the midi sequence and fill it with new data from the input stream.
        The listeners is informed, when the load operation has finished successfully
        via a MidiSequence::MidiStreamLoader::loadingFinishedSuccessfully message.
    */
    void loadMidiStream(InputStream* inputStream, ActionListener& listener);

    /** Get the reference to the Midi load progress variable.
    */
    double& getRefToLoadMidiProgressIndicator();

    /** Load project from file. That includes the midi file, but also information about
    the tracks: which tracks are enabled, what midi devices are being used...
    The listeners is informed, when the load operation has finished successfully
    via a MidiSequence::projectLoadingFinishedSuccessfully message.
    */
    void loadProjectFile(File& file, ActionListener& listener);

    /** Save project to file. That includes the midi file, but also information about
    the tracks: which tracks are enabled, what midi devices are being used...
    Return true, if the project was saved successfully.
    */
    bool saveProjectFile(ZipFile::Builder& xml);

    /** Get Start time of the whole midi file/sequence 
    */
    double getStartTime();
    /** Get End time of the whole midi file/sequence 
    */
    double getEndTime();
    /** Returns the index of the first note on event on or after the given timestamp.
    If the time is beyond the end of the sequence, this will return the
    number of events.
    */
    int getNextNoteOnIndexAtTime(double time, const ExtendedMidiMessageSequence* track);
    /** Returns the index of the first note on event on or before the given timestamp.
    If the time is beyond the start of the sequence, this will return zero.
    */
    int getPreviousNoteOnIndexAtTime(double time, const ExtendedMidiMessageSequence* track);
    /** Find next note(s) at/after time atTime. 
    */
    Array<ExtendedMidiMessage> getNextChord(double startTime);

    /** Find out the time of the next note-on event in each track. Start searching from startTime.
        An array with a time for each track is returned. If the time is greater than the end-time of the
        midi sequence, no note-on event was found for that track (or the track is disabled).
    */
    Array<double> getNextNoteOnTimes(const double startTime, const bool skipDisabledTracks = true);
    /** Find out the time of the previous note-on event in each track. Start searching from startTime.
        An array with a time for each track is returned. If the time is lower than the start-time of the
        midi sequence, no note-on event was found for that track (or the track is disabled).
        Normally this function skips disabled tracks. But you can tell it not to do so.
    */
    Array<double> getPreviousNoteOnTimes(double startTime);
    /** Find time stamp of next note(s) at/after time atTime. 
        Normally this function skips disabled tracks. But you can tell it not to do so. 
    */
    double getNextChordTime(const double startTime, const bool skipDisabledTracks = true);

    /** Find all Midi CCs, which belong to the given Chord. Add them to resultMidiCCs.
    */
    void getMidiCCsFromChord(const Array<ExtendedMidiMessage>& chordNotes, Array<ExtendedMidiMessage>& resultMidiCCs);

    int getLowestNote();
    int getHighestNote();
    /** Return the number of tracks in this midi sequence.
    */
    int getNoOfTracks();
    const ExtendedMidiMessageSequence* getTrack(int idx);
    /** Find the track with the biggest number of note on events and return the number of note on events.
    */
    int maxNoOfNoteOnEvents();
    /** Find total number of chord events in this Midi sequence and return it.
        A chord event can consist of *one* or *several* note on events, which are played at the same time. 
    */
    int noOfChordEvents();

    /** Returns the name of the track, if any name has been defined in a MIDI meta event
    */
    String getTrackName(int trackNo);

    /** Enable track trackNo for midi playback
    */
    void enableTrack(int trackNo);

    /** Disable track trackNo for midi playback
    */
    void disableTrack(int trackNo);

    /** Check, if this track is enabled for playback
    */
    bool isEnabled(int trackNo);
    /** Set the Midi Output of track trackNo, which we send the midi data to
        Only the name of the string is needed. The corresponding midi device pointer 
        is figured out by the function. 
    */
    void setMidiOut(const String& midiOutName, const int trackNo);
    /** Set the midi channel of the track.
    */
    void setMidiChannel(const int midiChannel, const int trackNo);
    /** Get the midi channel of the track.
    */
    int getMidiChannel(const int trackNo) const;


    /** Get the name of the Midi output for track trackNo. 
    */
    String getMidiOutName(const int trackNo) const;

    /** Get a list with all midi outputs
    */
    Array<MidiOutput*> getMidiOutputList();


private:

    /** Listen to Action brodcasts
    */
    void actionListenerCallback(const String &message) override;

    /** Reset all variables of the object. 
    */
    void reset();

    /** MIDI file, which all tracks are read from
    */
    MidiFile midiFile;
    /** Backup file. Needed for saving the project. Because for some reason, the saved midi file is 
        corrupt, after using "convertTimestampTicksToSeconds()".
    */
    MidiFile midiFileBackup;
    /**list of all tracks in the midi file.
    */
    OwnedArray<ExtendedMidiMessageSequence> tracks;
    /** In order to make the function noOfChordEvents() faster, cache the result. 
    */
    int noOfChordEventsCached;

    /** Progress indicator, which is used when loading a midi file.
    */
    double loadMidiProgress;

    /** Loader object for loading a Midi stream. 
    */
    std::unique_ptr<MidiStreamLoader> streamLoader;

    /** Loader object for loading a Project file.
    */
    std::unique_ptr<ProjectFileLoader> projectLoader;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiSequence);
};

#endif  // MIDISEQUENCE_H_INCLUDED
