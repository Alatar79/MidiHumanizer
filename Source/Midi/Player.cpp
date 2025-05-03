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

    Player.cpp
    Created: 21 Jun 2015 12:54:16pm
    Author:  Martin Schuppius

  ==============================================================================
*/

#include "Player.h"
#include "../AppData.h"


//#include "../../Common/Dbg.h"


Player::Player()
{
    reset();
}

Player::~Player()
{
    reset();
}

bool Player::addWithoutPlaying(const Array<ExtendedMidiMessage>& notes, const MidiMessage& triggerEvent)
{
    const ScopedLock sl(lock);
    int triggerNoteNumber = triggerEvent.getNoteNumber();
    if (triggerNoteNumber >= 0 && triggerNoteNumber < maxMidiNotes)
    {
        //remember all notes, so we can turn them off, when player releases the key
        notesByTrigger[triggerNoteNumber] = notes;

        //Loop collection of notes and remember the note-trigger.
        //Because of chord playing we can have several trigger notes for every played note.
        for (int i = 0; i < notes.size(); i++)
        {
            //add trigger to the note. 
            noteCounter.add(notes[i], triggerEvent);
        }
        return true;
    }

    return false;
}

void Player::sendMidiCCs(Array<ExtendedMidiMessage>& midiCCs)
{
    for (int i = 0; i < midiCCs.size(); i++)
    {
        if (midiCCs[i].getMidiOut()!=nullptr)
            midiCCs[i].getMidiOut()->sendMessageNow(midiCCs[i]);
        //DBG("Sending CC: " + String(midiCCs[i].getControllerNumber()));
        //DBG("Sending CC Value: " + String(midiCCs[i].getControllerValue()));
    }
}

void Player::play(Array<ExtendedMidiMessage>& notes, const MidiMessage& triggerEvent)
{

    if (addWithoutPlaying(notes, triggerEvent))
    {
         //Loop collection of notes and send them to midi device
        for (int i = 0; i < notes.size(); i++)
        {
            float vel = triggerEvent.getVelocity();
            notes.getReference(i).setVelocity(vel / 127.0f);
            if (notes[i].getMidiOut() != nullptr)
            {
                NoteCounter::MidiLaneEntry *midiLane = noteCounter.getMidiLaneFromNote(notes[i]);
                jassert(midiLane != nullptr);

                //Is the note already playing?
                //if the size is 2 or greater that means, the note was already playing (and has also been
                //added by addWithoutPlaying above). If that is the case, stop the note first, before sending 
                //out a new note on event. It is important to always stop a note with a note off event,
                //otherwise DAWs like Cubase get confused. 
                if (midiLane->triggerColletion[notes[i].getNoteNumber()].size() > 1)
                {
                    //notes[i].getMidiOut()->sendMessageNow(notes[i].noteOffObject->message);
                    notes[i].getMidiOut()->sendMessageNow(MidiMessage::noteOff(notes[i].getChannel(), notes[i].getNoteNumber()));
                }

                notes[i].getMidiOut()->sendMessageNow(notes[i]);
            }
/*
#ifdef DEBUG
            int dbg_no = notes[i].message->getNoteNumber();
            DBG(String("Playing Note ") + notes[i].message->getMidiNoteName(dbg_no, true, true, 4) + " - " + *(notes[i].getMidiOutName()));
#endif
            */
         }

/*
#ifdef DEBUG
        double dbg_playFinishTime = Time::getMillisecondCounterHiRes();
        //DBG("Render Time:" + String(dbg_playFinishTime - dbg_audioRenderStartTime));
        DBG("Play Time: " + String(dbg_playFinishTime - dbg_midiInTime));
#endif
*/
        
    }
}

bool Player::existsMidiOutInNotes(const String& midiOut, Array<TriggerNote>& notes) const
{
    for (int i = 0; i < notes.size(); i++)
    {
        if (notes[i].midiOutName == midiOut)
            return true;
    }

    return false;
}

void Player::stop(const MidiMessage& triggerEvent)
{
    const ScopedLock sl(lock);

    int triggerNoteNumber = triggerEvent.getNoteNumber();
    if (triggerNoteNumber >= 0 && triggerNoteNumber < maxMidiNotes)
    {
        //Get all notes, which this trigger is associated with.
        Array<ExtendedMidiMessage> notes = notesByTrigger[triggerNoteNumber];
        notesByTrigger[triggerNoteNumber].clear();

        for (int i = 0; i < notes.size(); i++)
        {
           //remove the trigger from the notes
            noteCounter.remove(notes[i], triggerEvent);
            //are there any triggers left for the note?
            //FIXME: Is removing if below really working? It generates a note-off event for every trigger which is up
            //Do we want that? What in caes of a piano? Would it stop playing prematurely?
            if (noteCounter.isClear(notes[i]))
            {
                if (notes[i].getMidiOut() != nullptr)
                {
                    notes[i].getMidiOut()->sendMessageNow(MidiMessage::noteOff(notes[i].getChannel(), notes[i].getNoteNumber()));
                }
/*
#ifdef DEBUG
                int dbg_no = notes[i].noteOffObject->message.getNoteNumber();
                DBG(String("Stopping Note ") + notes[i].noteOffObject->message.getMidiNoteName(dbg_no, true, true, 4) + " - " + *(notes[i].getMidiOutName()));
#endif
*/
            }
        }
        notes.clear();
    }
}

Array<double> Player::getCurrentlyActiveChordTimes() const
{
    //Must lock! Otherwise the notesByTrigger array could have been modified after execution of 
    //the if-statement below, resulting in a crash. 
    const ScopedLock sl(lock);

    Array<double> chordTimes;

    for (int i= 0; i<128; i++)
    {
        if(notesByTrigger[i].size() > 0)
            chordTimes.add(notesByTrigger[i].getFirst().getTimeStamp());
    }

    return chordTimes;
}



void Player::reset()
{
    const ScopedLock sl(lock);

    noteCounter.reset();

    for (int i = 0; i < maxMidiNotes; i++)
        notesByTrigger[i].clear();
}
