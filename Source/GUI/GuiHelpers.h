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

    GuiHelpers.h
    Created: 16 Aug 2015 3:20:20pm
    Author:  MartinG

  ==============================================================================
*/

#ifndef GUIHELPERS_H_INCLUDED
#define GUIHELPERS_H_INCLUDED

#include "JuceHeader.h"
#include "CIELCHColour.h"

class GuiHelpers
{

public:
    /** Creates a drawable button from a path and a text label. For the normal version, only the path is used.
    For the mouse-over version, a special highlighted images is created.
    */
    static void creatDrawableButtonFromPath(Path& path, const String& buttonText, DrawableButton& button);

    /** This is a little utility to create a drrawable with one of the SVG images in
        our embedded ZIP file "icons.zip". Filename is one of the files in the ZIP. */
    static Drawable* createDrawableFromZipFileSVG(const String& filename);

    /** Creates a drawable button from a file string and a text label. The file string is part of the icons.zip
        in the binary resources.
        For the normal version, only the path is used. For the mouse-over version, a special highlighted 
        images is created.
    */
    static void creatDrawableButtonFromFileString(const String& fileName, const String& buttonText, DrawableButton& button);

    /** Clear iconNames and iconsFromZipFile arrays. 
    */
    static void clear();

    /** Create a brighter colour for higlights
    */
    static Colour makeBrighter(CIELCHColour c);

private:
    static StringArray iconNames;
    static OwnedArray<Drawable> iconsFromZipFile;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiHelpers)
};

class ExtendedPath
{
public:

    ExtendedPath();
    void addRoundedTriangle(Point<float> p1, Point<float> p2, Point<float> p3, 
                            float cornerSize1, float cornerSize2, float cornerSize3);
    void addRoundedTriangle(float x1, float y1, float x2, float y2, float x3, float y3, 
                            float cornerSize1, float cornerSize2, float cornerSize3);

private:
    Path path;
    JUCE_LEAK_DETECTOR(ExtendedPath)
};



#endif  // GUIHELPERS_H_INCLUDED
