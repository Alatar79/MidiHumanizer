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

    Data.cpp
    Created: 1 May 2012 11:59:59am
    Author:  Martin

  ==============================================================================
*/

#include "AppData.h"

double dbg_midiInTime;
double dbg_audioRenderStartTime;

const String AppData::noMidiSelected = "None Selected";
const String AppData::fontName = "Arial";
double AppData::appGlobalScaleFactor = 0.8;
const char* AppData::tabNames[] = { "Home", "About", "Help" };

AppData::AppData()
{

}

int AppData::scalei(int value)
{
    return (int) round(scalef(value));
}

float AppData::scalef(double value)
{
    return (float)(value * appGlobalScaleFactor);
}

Image AppData::getAppIcon()
{
    if (Desktop::getInstance().getGlobalScaleFactor() < 1.5)
        return ImageCache::getFromMemory(BinaryData::AppIcon_128x128_png, BinaryData::AppIcon_128x128_pngSize);
    else
        return ImageCache::getFromMemory(BinaryData::AppIcon_256x256_png, BinaryData::AppIcon_256x256_pngSize);

}
