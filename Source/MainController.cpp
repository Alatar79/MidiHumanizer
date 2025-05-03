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

    MainController.cpp
    Created: 14 Jan 2018 2:50:36pm
    Author:  MartinG

  ==============================================================================
*/

#include "MainController.h"
#include "GUI/MainTab/MainTab.h"

MainController::MainController(MainTab& mainTabIn, MidiHelpers& midiHelperIn) :
    mainTab(mainTabIn),
    midiHelper(midiHelperIn),
    midiThread(midiHelper)
{
    //TODO: should we make this a real-time thread?
    midiThread.startThread(Thread::Priority::highest);
    //start GUI timer with 30ms intervall
    startTimer(30);
}

MainController::~MainController()
{
    //Logger::setCurrentLogger(nullptr);
    stopTimer();
    midiThread.stopThread(60);
}

void MainController::timerCallback()
{
    const ScopedTryLock sl(lock);

    if (sl.isLocked())
    {
        if (midiHelper.getMidiPlayer()->midiTimeHasChanged())
        {
            mainTab.getCurrentMidiPositionTextField().setText(String(midiHelper.getMidiPlayer()->getMidiCounter(), 3), juce::NotificationType::dontSendNotification);

            int xOffset = mainTab.getPianoRoll().getXOffsetFromMidiTime(midiHelper.getMidiPlayer()->getMidiCounter());
            juce::Rectangle<int> viewArea = mainTab.getPianoRollViewport().getViewArea();
            if (xOffset + 50 > viewArea.getBottomRight().getX() ||
                xOffset - 50 < viewArea.getBottomLeft().getX())
                mainTab.getPianoRollViewport().setViewPosition(-100 + mainTab.getPianoRoll().getXOffsetFromMidiTime(midiHelper.getMidiPlayer()->getMidiCounter()), 0);

            midiHelper.getMidiPlayer()->clearMidiTimeHasChangedFlag();
        }

        //FIXME ? Should we check first, if any key really is pressed down? Could save 10 microseconds ever timer call.     
        mainTab.getPianoRoll().highlightNotes();

    }
    else
    {
        DBG("Main Component - timer callback was busy.");
    }
}

