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

    NoteButton.cpp
    Created: 27 Jul 2015 5:46:12pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#include "NoteButton.h"
#include "../../GUI/MainTab/PianoRoll.h"
#include "../../GUI/GuiHelpers.h"

NoteButton::NoteButton(const double startTime, const double duration, const int track, const int noteNumber_)
    :Button("DummyName")
{
    setOpaque(true);

    noteButtonState = normal;
    highlighted = false;
    midiStartTime = startTime;
    noteDuration = duration; 
    midiTrack = track;
    noteNumber = noteNumber_;
}

//int dbg_ctr = 0;
void NoteButton::paintButton(Graphics &g, bool isMouseOverButton, bool)
{
    Colour fillColour;

    //dbg_ctr++;
    //DBG("Paint Button!! " + String(dbg_ctr));
    
    if (noteButtonState == NoteButtonState::normal)
    {
        
        //fillColour = JUCE_LIVE_CONSTANT(Colour(0xffa2c4de)); //0xffb37676; 0xff8c4c55
        fillColour = Colour(AppData::noteNormalColour);
    }
    else if (noteButtonState == NoteButton::higlighted)
    {
        fillColour = Colour(AppData::noteHighlightColour);

    }
    else if (noteButtonState == NoteButtonState::disabled)
    {
        fillColour = Colour(AppData::noteDisabledColour);
    }
    //Highlight preceeds all the above, except disabled.
    if (highlighted && noteButtonState!=NoteButtonState::disabled)
    {
        fillColour = Colour(AppData::notePlayingColour);
        //fillColour = JUCE_LIVE_CONSTANT(Colour(0xfff0f682));
    }

    if (isMouseOverButton)
    {
        //fillColour = fillColour.withAlpha(0.5f);
        //outlineColour = outlineColour.withAlpha(0.5f);
        //fillColour = fillColour.brighter(0.8f);
        fillColour = fillColour.withMultipliedSaturation(1.1f).withMultipliedBrightness(1.2f).withMultipliedBrightness(1.7f);
    }


    //g.setColour(JUCE_LIVE_CONSTANT(Colour(0xff8b6752)));
    //g.setColour(fillColour);
    //g.fillRect(0, 0, getWidth(), getHeight()-1);
    g.fillAll(fillColour);
    //g.setColour(JUCE_LIVE_CONSTANT(Colour(0xffd9b7b2)));
    g.setColour(fillColour);
    if (noteButtonState != NoteButtonState::disabled)
    {
        g.drawRect(getWidth() - 2, 0, getWidth(), getHeight() - 1);
        g.drawRect(0, 0, 2, getHeight() - 1);
    }
    //g.drawRect(juce::Rectangle<int>(0, 0, getWidth(), getHeight()), 1);

}

void NoteButton::clicked()
{

    if (PianoRoll* pianoRoll = dynamic_cast<PianoRoll*>(getParentComponent()))
    {
        pianoRoll->setMidiTime(midiStartTime);
    }

}

void NoteButton::mouseEnter(const MouseEvent & m)
{
    Button::mouseEnter(m);
    //toFront(false);
    repaint();
}

void NoteButton::mouseExit(const MouseEvent & m)
{
    Button::mouseExit(m);
    repaint();
}

void NoteButton::setNoteButtonState(const NoteButtonState state)
{
    if (state >= 0 && state <= NoteButtonState::totalNumParams)
        noteButtonState = state;
}

NoteButton::NoteButtonState NoteButton::getNoteButtonState() const
{
    return noteButtonState;
}

void NoteButton::setHighlightState(const bool highlight)
{
    highlighted = highlight;
}

double NoteButton::getMidiStartTime()
{
    return midiStartTime;
}

double NoteButton::getNoteDuration()
{
    return noteDuration;
}


int NoteButton::getNoteNumber()
{
    return noteNumber;
}
