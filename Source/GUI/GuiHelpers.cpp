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

    GuiHelpers.cpp
    Created: 16 Aug 2015 3:20:20pm
    Author:  MartinG

  ==============================================================================
*/

#include "GuiHelpers.h"
#include "../AppData.h"
#include <memory>


StringArray GuiHelpers::iconNames;
OwnedArray<Drawable> GuiHelpers::iconsFromZipFile;


void GuiHelpers::creatDrawableButtonFromPath(Path& path, const String&, DrawableButton& button)
{

    path.applyTransform(juce::AffineTransform::verticalFlip(0));

    DrawablePath itemDrawable;
    itemDrawable.setStrokeThickness(1.0f);
    itemDrawable.setPath(path);
    itemDrawable.setFill(juce::Colour(AppData::iconColour));
    itemDrawable.setStrokeFill(juce::Colour(AppData::iconColour));

    button.setImages(&itemDrawable);

}

// This is a little utility to create a button with one of the SVG images in
// our embedded ZIP file "icons.zip"
std::unique_ptr<Drawable> GuiHelpers::createDrawableFromZipFileSVG(const String& filename)
{
    if (iconsFromZipFile.size() == 0)
    {
        // If we've not already done so, load all the images from the zip file..
        MemoryInputStream iconsFileStream(BinaryData::SvgIcons_zip, BinaryData::SvgIcons_zipSize, false);
        ZipFile icons(&iconsFileStream, false);

        for (int i = 0; i < icons.getNumEntries(); ++i)
        {
            std::unique_ptr<InputStream> svgFileStream(icons.createStreamForEntry(i));

            if (svgFileStream != 0)
            {
                iconNames.add(icons.getEntry(i)->filename);
                iconsFromZipFile.add(Drawable::createFromImageDataStream(*svgFileStream));
            }
        }
    }

    return iconsFromZipFile [iconNames.indexOf (filename)]->createCopy();
}

void GuiHelpers::creatDrawableButtonFromFileString(const String& fileName, const String&, DrawableButton& button)
{

    std::unique_ptr<Drawable> drawableFromFile = std::unique_ptr<Drawable>(createDrawableFromZipFileSVG( fileName ));

    button.setImages(drawableFromFile.get());
    button.setButtonStyle(DrawableButton::ImageFitted);

}

void GuiHelpers::clear()
{
    iconsFromZipFile.clear();
    iconNames.clear();

    
}

Colour GuiHelpers::makeBrighter(CIELCHColour c)
{
    //return Colour(AppData::componentHighlightColourLighter);
    
    CIELCHColour textColor(AppData::defaultTextColour);
    float deltaL = textColor.getCIELightness() - c.getCIELightness();
    float deltaC = textColor.getCIEChroma() - c.getCIEChroma();
    float deltaH = textColor.getCIEHue() - c.getCIEHue();
    bool imaginary;
    return CIELCHColour::fromCIELCH(c.getCIELightness() + 0.2f * deltaL, c.getCIEChroma() + 0.2f * deltaC, c.getCIEHue() + 0.2f * deltaH, 1.0f, imaginary).getJuceColour();
    
    //return c.withMultipliedCIEChroma(0.000085f, imaginary); // .withMultipliedCIELightness(1.15f, imaginary);
    //return c.withMultipliedCIELightness(1.25f, imaginary);
    ///return c.withCIELightness(c.getCIELightness() + 0.075f, imaginary);
    //return Colours::red;
    //return c.withMultipliedSaturation(1.1f).withMultipliedBrightness(1.2f);
}


ExtendedPath::ExtendedPath()
{
}


/** Add a rounded triangle to the path.

    p1,p2,p3 : Vertices of the triangle
    cornerSize1, cornerSize2, cornerSize3 : corner sizes of each triangle corner. 

*/
void ExtendedPath::addRoundedTriangle(Point<float> p1, Point<float> p2, Point<float> p3, 
                                      float cornerSize1, float cornerSize2, float cornerSize3)
{
    //get Vectors along every edge:
    //we are using 3D vectors with z=0, because JUCE has no 2D vectors
    Vector3D<float> vec12 (p2.getX() - p1.getX(), p2.getY() - p1.getY(), 0);
    Vector3D<float> vec23 (p3.getX() - p2.getX(), p3.getY() - p2.getY(), 0);
    Vector3D<float> vec31 (p1.getX() - p3.getX(), p1.getY() - p3.getY(), 0);

    //calculate lenghts of all triangle edges
    float dist12 = vec12.length();
    float dist23 = vec23.length();
    float dist31 = vec31.length();

    //lengths are calcualated => normalize the edge vectors:
    vec12 = vec12.normalised();
    vec23 = vec23.normalised();
    vec31 = vec31.normalised();

    //corner size should not be larger than half of the smallest edge length of that corner:
    cornerSize1 = jmin(cornerSize1, dist12 * 0.5f);
    cornerSize1 = jmin(cornerSize1, dist31 * 0.5f);
    cornerSize2 = jmin(cornerSize2, dist12 * 0.5f);
    cornerSize2 = jmin(cornerSize2, dist23 * 0.5f);
    cornerSize3 = jmin(cornerSize3, dist23 * 0.5f);
    cornerSize3 = jmin(cornerSize3, dist31 * 0.5f);

    //substract corner size from edge length => new start/end points for every edge. 
    Vector3D<float> e1p1(p1.getX(), p1.getY(), 0);
    e1p1 = e1p1 + vec12 * cornerSize1;
    Vector3D<float> e1p2(p2.getX(), p2.getY(), 0);
    e1p2 = e1p2 - vec12 * cornerSize2;
    Vector3D<float> e2p1(p2.getX(), p2.getY(), 0);
    e2p1 = e2p1 + vec23 * cornerSize2;
    Vector3D<float> e2p2(p3.getX(), p3.getY(), 0);
    e2p2 = e2p2 - vec23 * cornerSize3;
    Vector3D<float> e3p1(p3.getX(), p3.getY(), 0);
    e3p1 = e3p1 + vec31 * cornerSize3;
    Vector3D<float> e3p2(p1.getX(), p1.getY(), 0);
    e3p2 = e3p2 - vec31 * cornerSize1;

    path.startNewSubPath(e1p1.x, e1p1.y);
    path.lineTo(e1p2.x, e1p2.y);
    path.quadraticTo(p2.getX(), p2.getY(), e2p1.x, e2p1.y);
    path.lineTo(e2p2.x, e2p2.y);
    path.quadraticTo(p3.getX(), p3.getY(), e3p1.x, e3p1.y);
    path.lineTo(e3p2.x, e3p2.y);
    path.quadraticTo(p1.getX(), p1.getY(), e1p1.x, e1p1.y);
    path.closeSubPath();
}


void ExtendedPath::addRoundedTriangle(float x1, float y1, float x2, float y2, float x3, float y3, 
                                      float cornerSize1, float cornerSize2, float cornerSize3)
{
    addRoundedTriangle(Point<float>(x1, y1),
        Point<float>(x2, y2),
        Point<float>(x3, y3), 
        cornerSize1, cornerSize2, cornerSize3);
}
