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

    Paths.h
    Created: 21 Jul 2015 7:59:39pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#include "JuceHeader.h"

/** class for storing path and svg images.
*/
class Paths
{
public:
    static const unsigned char diskPath[];
    static const int diskPathSize;

    static const unsigned char folderOpenPath[];
    static const int folderOpenSize;

    static const unsigned char musicPath[];
    static const int musicSize;

    static const unsigned char squarePath[];
    static const int squareSize;

    static const unsigned char filePath[];
    static const int filePathSize;

    static const String warningSvg;
    
    static const String questionSvg;

    static const String arrowLeft;

    static const String minusCircle;

    static const String plusCircle;

    static const String plusSign;

    static const String minusSign;


};

#endif  // PATHS_H_INCLUDED
