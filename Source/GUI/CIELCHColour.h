/*
==============================================================================

This file is part of the CIELCH Colour JUCE module
Copyright 2016 by Alatar79

The CIELCH JUCE module is hosted on github: https://github.com/Alatar79/JUCE-CIELCH

------------------------------------------------------------------------------

The CIELCH Colour JUCE module is provided under the terms of The MIT License (MIT):

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

==============================================================================
*/

//==============================================================================
/**
This class extends the JUCE Colour class by adding support for the CIE LCH perceptual colour space. 
The CIE LCH colour space is a colour space, which reflects how humans perceive colours. 
Therefore interpolating colours in CIE LCH gives smoother variations than when using HSV/HSL. 
LCH is an abbreviation for Lightness, Chroma(Saturation) and Hue.
Note that the LCH colour space contains more colours than humans can see.
Therefore it is possible, that a LCH colour cannot be converted to an exact RGB colour.
In such a case, the closest RGB approximation is used. 

Internally The CIE LCH colour space uses the CIE Lab and the CIE XYZ colour spaces. So you find functions for 
those as well. But not that many, because the focus is on CIE LCH.

See also https://en.wikipedia.org/wiki/CIE_1931_color_space and https://en.wikipedia.org/wiki/Lab_color_space.

Here an example on how to use the class:
@code
CIELCHColour colour = Colours::yellow;
float l, c, h;
colour.getCIELCH(l, c, h);
float newSaturation = c / 2.0f;
bool imaginary;
CIELCHColour newColour = CIELCHColour::fromCIELCH(l, newSaturation, h, colour.getAlpha(), imaginary);
if (imaginary)
   std::cout << "Imaginary colour. RGB values have been clipped to the visible range";
@endcode

Because the CIELCHColour class is subclassing the JUCE::Colour class, it has all its functionality. 
It is e.g. perfectly OK to write:
@code
CIELCHColour newColour  = CIELCHColour::fromHSV(h, s,  v);
CIELCHColour newColour2 = Colours::yellow;
@endcode

*/

#ifndef CIELCHColour_H_INCLUDED
#define CIELCHColour_H_INCLUDED

#include "JuceHeader.h"
#include <memory.h>

class CIELCHColour
{
public:

    /** Creates a transparent black colour. */
    CIELCHColour() noexcept;

    /** Creates a CIELCHColour object from a Colour objet 
    */
    CIELCHColour(const Colour& colour) noexcept;

    /** Creates a copy of another CIELCHColour object. */
    CIELCHColour(const CIELCHColour& other) noexcept;

    /** Creates a colour from a 32-bit ARGB value.

    The format of this number is:
    ((alpha << 24) | (red << 16) | (green << 8) | blue).

    All components in the range 0x00 to 0xff.
    An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.

    @see getPixelARGB
    */
    explicit CIELCHColour(uint32 argb) noexcept;

    /** Creates an opaque colour using 8-bit red, green and blue values */
    CIELCHColour(uint8 red,
        uint8 green,
        uint8 blue) noexcept;

    /** Creates a colour using 8-bit red, green, blue and alpha values. */
    CIELCHColour(uint8 red,
        uint8 green,
        uint8 blue,
        uint8 alpha) noexcept;

    /** Creates a colour from 8-bit red, green, and blue values, and a floating-point alpha.

    Alpha of 0.0 is transparent, alpha of 1.0f is opaque.
    Values outside the valid range will be clipped.
    */
    CIELCHColour(uint8 red,
        uint8 green,
        uint8 blue,
        float alpha) noexcept;

    /** Creates a colour using the HSV colour space. Floating point hue, saturation and brightness values, and an 8-bit alpha.

    The floating point values must be between 0.0 and 1.0.
    An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.
    Values outside the valid range will be clipped.
    */
    CIELCHColour(float hue,
        float saturation,
        float brightness,
        uint8 alpha) noexcept;

    /** Creates a colour using using the HSV colour space. Floating point hue, saturation, brightness and alpha values.

    All values must be between 0.0 and 1.0.
    Numbers outside the valid range will be clipped.
    */
    CIELCHColour(float hue,
        float saturation,
        float brightness,
        float alpha) noexcept;

    /** Creates a colour using a PixelARGB object. This function assumes that the argb pixel is
    not premultiplied.
    */
    CIELCHColour(PixelARGB argb) noexcept;

    /** Creates a colour using a PixelRGB object.
    */
    CIELCHColour(PixelRGB rgb) noexcept;

    /** Creates a colour using a PixelAlpha object.
    */
    CIELCHColour(PixelAlpha alpha) noexcept;


    //==============================================================================
    /** Copies another CIELCHColour object. */
    CIELCHColour& operator= (const CIELCHColour& other) noexcept;

    /** Copies from a Colour object. */
    CIELCHColour& operator= (const Colour& other) noexcept;

    //==============================================================================

    /** get the base JUCE colour, which this object works with */
    Colour getJuceColour() const; 

    /** Create the colour from the CIE LCH colour space.
    The CIE LCH colour space is a colour space, which corresponds to
    the way humans perceive colours. It uses the components Lightness,
    Chroma (Saturation) and Hue. So in that way it is similar to the HSV
    colour space. But the HSV colour space is *not* perceptually correct.
    Note that the LCH colour space contains more colours than humans can see.
    These "imaginary" colours cannot be represented by RGB colours.
    Therefore certain LCH colours cannot be converted to a RGB colour.
    In such a case, the returned RGB colour is the closest match to the CIE LCH colour. And
    the parameter "imaginary" is set to true.

    @param[in] L = lightness in the range [0...1]
    @param[in] C = chroma, relative saturation in the range [0...1]
    @param[in] H = hue in the range [0...1]
    @param[in] alpha = the alpha value for the colour.
    @param[out] imaginary = true if the LCH colour is imaginary (see above). False otherwise.
    */
    static CIELCHColour fromCIELCH(float L, float C, float H, float alpha, bool& imaginary) noexcept;

    /** Returns the CIE lightness component.
    The value is in the range 0...1
    */
    float getCIELightness() const noexcept;

    /** Returns the CIE chroma/saturation component.
    The value is in the range 0...1
    */
    float getCIEChroma() const noexcept;

    /** Returns the CIE hue component.
    The value is in the range 0...1
    */
    float getCIEHue() const noexcept;

    /** Returns the CIE lightness, chroma/saturation and hue.
    All values are in the range 0...1.
    */
    void getCIELCH(float& L, float& C, float& H) const noexcept;

    //==============================================================================
    /** Returns a copy of this colour with a different CIE brightness.
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see CIELighter, CIEDarker, withMultipliedCIELightness
    @see fromCIELCH for a better explanation on imaginary.
    */
    CIELCHColour withCIELightness(float newLightness, bool& imaginary) const noexcept;

    /** Returns a copy of this colour with a different CIE chroma/saturation.
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see fromCIELCH for a better explanation on imaginary.
    */
    CIELCHColour withCIEChroma(float newChroma, bool& imaginary) const noexcept;

    /** Returns a copy of this colour with a different CIE hue.
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see fromCIELCH for a better explanation on imaginary.
    */
    CIELCHColour withCIEHue(float newHue, bool& imaginary) const noexcept;

    /** Returns a copy of this colour with its CIE Lightness multiplied by the given value.
    The new colour's brightness is (this->getCIELightness() * multiplier)
    (the result is clipped to legal limits).
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see fromCIELCH for a better explanation on imaginary.
    Note: in a CIELCH space, the only way to create a truly black colour is by setting *both*
    lightness and chroma to zero. If you only set lightness to zero, the colour might still be
    visible.
    */
    CIELCHColour withMultipliedCIELightness(float amount, bool& imaginary) const noexcept;

    /** Returns a copy of this colour with its CIE chroma/saturation multiplied by the given value.
    The new colour's chroma is (this->getCIEChroma() * multiplier)
    (the result is clipped to legal limits).
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see withMultipliedCIEChroma, withRotatedCIEHue
    @see fromCIELCH for a better explanation on imaginary.
    */
    CIELCHColour withMultipliedCIEChroma(float multiplier, bool& imaginary) const noexcept;

    /** Returns a copy of this colour with its CIE hue rotated.
    The new colour's hue is ((this->getCIEHue() + amountToRotate) % 1.0)
    If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see withMultipliedCIELightness, withMultipliedCIEChroma
    @see fromCIELCH for a better explanation on imaginary.
    */
    CIELCHColour withRotatedCIEHue(float amountToRotate, bool& imaginary) const noexcept;


    //==============================================================================
    /** Returns a brighter version of this colour.
    @param amountBrighter   how much brighter to make it - a value from 0 to 1.0 where 0 is
    unchanged, and higher values make it brighter
    @param    imaginary If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    @see fromCIELCH for a better explanation on imaginary.
    @see withMultipliedCIELightness
    */
    CIELCHColour CIELighter(bool& imaginary, float amountBrighter = 0.4f) const noexcept;

    /** Returns a darker version of this colour.
    @param amountDarker     how much darker to make it - a value from 0 to 1.0 where 0 is
    unchanged, and higher values make it darker
    @param    imaginary If the returned colour is an imaginary colour, then the "imaginary" parameter is set to true.
    Note: in a CIELCH space, the only way to create a real truly black colour is by setting *both*
    lightness and chroma to zero. If you only set lightness to zero, the colour might still be
    visible.
    @see fromCIELCH for a better explanation on imaginary.
    @see withMultipliedCIELightness
    */
    CIELCHColour CIEDarker(bool& imaginary, float amountDarker = 0.4f) const noexcept;

    //==============================================================================
    /** Convert from the sRGB colour space to the XYZ colour space.
    The transformations are calculated for Observer = 2° and Illuminant = D65.
    For explanation and formulas see https://en.wikipedia.org/wiki/SRGB

    @param[in] c = the (RGB) colour to convert
    @param[out] X = in the range 0....95.047
    @param[out] Y = in the range 0....100.000
    @param[out] Z = in the range 0....108.883
    */
    static void RGBtoXYZ(CIELCHColour c, float& X, float&Y, float& Z) noexcept;

    /** Convert from the XYZ colour space to the sRGB colour space.
    Note that the XYZ colour space contains more colours than humans can see.
    These "imaginary" colours cannot be represented by standard RGB colours.
    Therefore certain XYZ colours cannot be converted to a RGB colour.
    In such a case, the returned RGB colour is the closest match to the XYZ colour. And 
    the parameter "imaginary" is set to true.

    The transformations are calculated for Observer = 2° and Illuminant = D65.
    For explanation and formulas see https://en.wikipedia.org/wiki/SRGB

    @param[in] X = in the range 0....95.047
    @param[in] Y = in the range 0....100.000
    @param[in] Z = in the range 0....108.883
    @param[out] c = the resulting (RGB) colour
    @param[out] imaginary: true if the XYZ colour is imaginary (see above). False otherwise.
    */
    static void XYZtoRGB(float X, float Y, float Z, CIELCHColour& c, bool& imaginary) noexcept;

    /** Convert from the XYZ colour space to the CIE Lab colour space.
    The transformations are calculated for Observer = 2° and Illuminant = D65.
    For epxlanation and formulas see https://en.wikipedia.org/wiki/Lab_color_space

    @param[in] X = in the range 0....95.047
    @param[in] Y = in the range 0....100.000
    @param[in] Z = in the range 0....108.883
    @param[out] L = lightness in the range 0...100
    @param[out] a = range 0...500
    @param[out] b = range 0...200
    */
    static void XYZtoCIELab(float X, float Y, float Z, float& L, float& a, float& b) noexcept;

    /** Convert from the CIE Lab colour space to the XYZ colour space.
    The transformations are calculated for Observer = 2° and Illuminant = D65.
    For epxlanation and formulas see https://en.wikipedia.org/wiki/Lab_color_space

    @param[in] L = lightness in the range 0...100
    @param[in] a = range 0...500
    @param[in] b = range 0...200
    @param[out] X = in the range 0....95.047
    @param[out] Y = in the range 0....100.000
    @param[out] Z = in the range 0....108.883
    */
    static void CIELabtoXYZ(float L, float a, float b, float& X, float& Y, float& Z) noexcept;

    /** Convert from the CIE Lab colour space to the CIE LCH colour space.
    Basically, this is a conversion to cylindrical coordinates.
    For explanation and details see https://en.wikipedia.org/wiki/Lab_color_space.

    @param[in] L = lightness in the range 0...100
    @param[in] a = range 0...500
    @param[in] b = range 0...200
    @param[out] Lout = lightness in the range 0...100
    @param[out] C = chroma, relative saturation in the range 0...134
    @param[out] H = hue in the range 0...360

    */
    static void CIELabtoCIELCH(float L, float a, float b, float& Lout, float& C, float& H) noexcept;
    /** Convert from the CIE LCH colour space to the CIE Lab colour space.
    Basically, this is a conversion from cylindrical to carthesian coordinates.
    For explanation and details see https://en.wikipedia.org/wiki/Lab_color_space.

    @param[in] L = lightness in the range 0...100
    @param[in] C = chroma, relative saturation in the range 0...134
    @param[in] H = hue in the range 0...360
    @param[out] Lout = lightness in the range 0...100
    @param[out] a = range 0...500
    @param[out] b = range 0...200

    */
    static void CIELCHtoCIELab(float L, float C, float H, float& Lout, float& a, float& b) noexcept;

    //==============================================================================
    /** Convert from the CIE LCH colour space to the sRGB colour space.
    The CIE LCH colour space is a colour space, which corresponds to
    the way humans perceive colours. It uses the components Lightness,
    Chroma (Saturation) and Hue. So in that way it is similar to the HSV
    colour space. But the HSV colour space is *not* perceptually correct.
    Note that the LCH colour space contains more colours than humans can see.
    These "imaginary" colours cannot be represented by standard RGB colours.
    Therefore it is possible, that a LCH colour cannot be converted to a RGB colour.

    @param[in] L = lightness in the range [0...1]
    @param[in] C = chroma, relative saturation in the range [0...1]
    @param[in] H = hue in the range [0...1]
    @param[out] c = a JUCE colour component.
    @param[out] imaginary: true if the LCH colour is imaginary (see above). False otherwise.
    */
    static void CIELCHtoRGB(float L, float C, float H, CIELCHColour& c, bool& imaginary) noexcept;

    /** Convert from the sRGB colour space to the CIE LCH colour space.
    The CIE LCH colour space is a colour space, which corresponds to
    the way humans perceive colours. In contrast the HSV colour space is *not*
    perceptually correct.
    @param[in] c = a JUCE colour component.
    @param[out] L = lightness in the range [0...1]
    @param[out] C = chroma, relative saturation in the range [0...1]
    @param[out] H = hue in the range [0...1]
    */
    static void RGBtoCIELCH(CIELCHColour c, float& L, float& C, float& H) noexcept;




private:

    static float gammaCorrection(float g) noexcept;
    static float invGammaCorrection(float g) noexcept;

    static float f(float t) noexcept;
    static float fInverse(float t) noexcept;

    std::unique_ptr<Colour> juceColour;


};



#endif  // CIELCHColour_H_INCLUDED
