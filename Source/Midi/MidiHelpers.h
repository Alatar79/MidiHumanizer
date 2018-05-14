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

    MidiHelpers.h
    Created: 17 Dec 2017 7:48:07pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef MIDIHELPERS_H_INCLUDED
#define MIDIHELPERS_H_INCLUDED

#include "JuceHeader.h"

//Forward declarations. Definition see AudioEngine.h.
class MidiPlayer;
class MidiMessageCollectorMod;

extern double dbg_midiInTime;
extern double dbg_audioRenderStartTime;

class MidiHelpers
{
public:

    MidiHelpers();
    
    StringArray& getMidiInputs();

    /** set threshold for two notes to be treated as a chord */
    void setMidiChordThreshold(const double threshold);
    /** get threshold for two notes to be treated as a chord */
    double getMidiChordThreshold() const;

    MidiPlayer* getMidiPlayer();
    void setMidiPlayer(MidiPlayer* midiPlayer_);

    MidiMessageCollectorMod* getMidiMessageCollector();
    void setMidiMessageCollector(MidiMessageCollectorMod* midiMessageCollector_);

    /** The state of the virtual keyboard.
    Used to handle midi events from/to the virtual keyboard. */
    MidiKeyboardState virtualKeyboardState;

    /** Used in floating point calculation.  Minimal threshold
    for two floats to be equal in midi time comparisons. */
    static float midiFloatThreshold;
    /** Used in floating point calculation. Minimal threshold
    for two doubles to be equal in midi time comparisons. */
    static double midiDoubleThreshold;
    /** Minimum value for two doubles to be treated equal.
    Is constant for the whole application */
    static const double doubleComparisonThreshold; /* = 1e-9 */

private:
    MidiPlayer* midiPlayer;
    MidiMessageCollectorMod* midiMessageCollector;
    MidiOutput* midiOut;
    
    //workaround, because sometimes MidiOutput::getDevices() can fail, if you call it a second time.
    //therefore store the names after the first call.
    StringArray midiInputs;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiHelpers);
};



#endif  // MIDIHELPERS_H_INCLUDED
