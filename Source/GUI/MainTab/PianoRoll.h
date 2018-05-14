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

    PianoRoll.h
    Created: 22 Jun 2015 5:11:10pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#ifndef PIANOROLL_H_INCLUDED
#define PIANOROLL_H_INCLUDED

#include "JuceHeader.h"
#include "../../AppData.h"
#include "../../Midi/MidiSequence.h"
#include "../../Midi/MidiHelpers.h"
#include "NoteButton.h"

class PianoRollViewport : public Viewport
{
public:

    PianoRollViewport(const String& name = String::empty);

    void setViewedComponent(PianoRoll *newPianoRoll, bool deleteComponentWhenNoLongerNeeded = true);

    void paint(Graphics& g) override;
    //void resized() override;
    //void visibleAreaChanged(const Rectangle< int > &newVisibleArea) override;
    void setBounds(int x, int y, int width, int height);
    /** Set the bounds for the piano roll. They must be smaller than the viewport, to make room for the scrollbar.
    */
    void setBoundsForPianoRoll();
    /** Perform all necessary steps, after a new file/project has been loaded. 
    */
    void newFileLoaded();

    /** find out the height of the piano roll. 
    */
    int pianoRollHeight();


private:
    PianoRoll* pianoRollRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollViewport);
};


class PianoRoll : public Component
{
public:
    PianoRoll(MidiHelpers& midiHelperIn);

    ~PianoRoll();

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent &event) override;

    /** Scale width of this component. Afterwards resize and redraw. 
    */
    void scale(double scaleFactor);

    /** Indicates that a new file is laoded and we have to change the piano roll
    */
    void newFileLoaded();
    /** Calculate X-Offset in pixels from the relative midi time in seconds. 
        Used in the viewport to set the correct viewing position.
    */
    int getXOffsetFromMidiTime(double time);
    /** Chekc if the track is enabled. 
    */
    bool isEnabled(const int track) const;
    /** Set the focus to the track selected.
    */
    void setFocusTrack(const int track);
    /** Enable the track
    */
    void enableTrack(const int track);
    /** Enable the track
    */
    void disableTrack(const int track);
    /** Set the state of the track (i.e. selected) and bring it to front, if needed. 
    */
    void setTrackState(const int track, const NoteButton::NoteButtonState state, const bool bringToFront = false);
    /** Highlight the notes, which are currently "pressed down" in the Midi file. 
    */
    void highlightNotes();
    /** Sets the midi time of the midi player to the given value
    */
    void setMidiTime(const double time);
    /** Reset object data.
    */
    void reset();


private:

    /** Collection of common midi functions*/
    MidiHelpers& midiHelper; 
    int xOffset; 
    /** The track, which is currently in focus. */
    int focusTrack;
    /** Array with all notes as buttons */
    OwnedArray<NoteButton> noteButtons;
    /** Start of the noteButtons for each midi track */
    Array<int> noteButtonStart;
    /** End of the noteButtons for each midi track */
    Array<int> noteButtonEnd;
    /** Indices of currently higlighted notes.
    */
    Array<int> higlightedNotes;
    /** X-Coordinate of the klicked marker
    */
    int markerLineX;



JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRoll);
};




#endif  // PIANOROLL_H_INCLUDED
