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

    NoteButton.h
    Created: 27 Jul 2015 5:46:12pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef NOTEBUTTON_H_INCLUDED
#define NOTEBUTTON_H_INCLUDED

#include "JuceHeader.h"
#include "../../AppData.h"
#include "TrackPanel.h"

/** GUI class, which represents a note button in the piano roll.
*/
class NoteButton : public juce::Button
{
public:

    /** Constructor
        Construct it with the midi start time, note playing duration, midi track id and note number as argument. 
    */
    NoteButton(const double startTime, const double duration,  const int track, const int noteNumber);

    enum NoteButtonState
    {
        normal = 0,
        disabled,
        higlighted,
        playing,
        totalNumParams
    };

    /** Set the state specific to this note button.
    */
    void setNoteButtonState(const NoteButtonState state);

    /** Get the button state.
    */
    NoteButtonState getNoteButtonState() const;

    /** Set, whether the note is highlighted or not. 
    */
    void setHighlightState(const bool highlight);
    /** Get the startTime of this note.
    */
    double getMidiStartTime();
    /** Get the playing duration of this note. 
    */
    double getNoteDuration();
    /** Get the note number of this note button.
    */
    int getNoteNumber();


protected:
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) override;
    void clicked() override;
    void mouseEnter(const MouseEvent &) override;
    void mouseExit(const MouseEvent &) override;

private:

    /** State of the button. 
    */
    NoteButtonState noteButtonState;
    /** The note can get higlighted, when it is currently being played. 
    */
    bool highlighted;
    /** Each button represents a midi note. Remember the start time of that note.
    */
    double midiStartTime;
    /** Playing duration of this note. 
    */
    double noteDuration;
    /* Each note belongs to a midi track. 
    */
    int midiTrack;
    /** Note number, which this note represents.
    */
    int noteNumber;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteButton)
};



#endif  // NOTEBUTTON_H_INCLUDED
