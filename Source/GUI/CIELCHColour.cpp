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


#include "CIELCHColour.h"

CIELCHColour::CIELCHColour() noexcept
{
    juceColour = std::make_unique<Colour>();
}


CIELCHColour::CIELCHColour(const Colour& colour) noexcept
{
    juceColour = std::make_unique<Colour>(colour.getRed(), colour.getGreen(), colour.getBlue(), colour.getAlpha());
}


CIELCHColour::CIELCHColour(const CIELCHColour& other) noexcept
{
    juceColour = std::make_unique<Colour>(other.getJuceColour());
}

CIELCHColour::CIELCHColour(uint32 argb) noexcept
{
    juceColour = std::make_unique<Colour>(argb);
}

CIELCHColour::CIELCHColour(uint8 red, uint8 green, uint8 blue) noexcept
{
    juceColour = std::make_unique<Colour>(red, green, blue);
}

CIELCHColour::CIELCHColour(uint8 red, uint8 green, uint8 blue, uint8 alpha) noexcept
{
    juceColour = std::make_unique<Colour>(red, green, blue, alpha);
}

CIELCHColour::CIELCHColour(uint8 red, uint8 green, uint8 blue, float alpha) noexcept
{
    juceColour = std::make_unique<Colour>(red, green, blue, alpha);
}

CIELCHColour::CIELCHColour(float hue, float saturation, float brightness, uint8 alpha) noexcept
{
    juceColour = std::make_unique<Colour>(hue, saturation, brightness, alpha);
}

CIELCHColour::CIELCHColour(float hue, float saturation, float brightness, float alpha) noexcept
{
    juceColour = std::make_unique<Colour>(hue, saturation, brightness, alpha);
}

CIELCHColour::CIELCHColour(PixelARGB argb) noexcept
{
    juceColour = std::make_unique<Colour>(argb);
}

CIELCHColour::CIELCHColour(PixelRGB rgb) noexcept
{
    juceColour = std::make_unique<Colour>(rgb);
}

CIELCHColour::CIELCHColour(PixelAlpha alpha) noexcept
{
    juceColour = std::make_unique<Colour>(alpha);
}

//==============================================================================
CIELCHColour& CIELCHColour::operator= (const CIELCHColour& other) noexcept
{
    juceColour = std::make_unique<Colour>(other.getJuceColour());
    return *this; 
}

CIELCHColour& CIELCHColour::operator= (const Colour& other) noexcept
{
    juceColour = std::make_unique<Colour>(other);
    return *this;

}

Colour CIELCHColour::getJuceColour() const
{
    return *juceColour;
}

//==============================================================================
CIELCHColour CIELCHColour::fromCIELCH(float L, float C, float H, float alpha, bool& imaginary) noexcept
{
    CIELCHColour colour;
    CIELCHtoRGB(L, C, H, colour, imaginary);
    colour = colour.getJuceColour().withAlpha(alpha);
    return colour; 
}

float CIELCHColour::getCIELightness() const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return L;
}

float CIELCHColour::getCIEChroma() const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return C;
}

float CIELCHColour::getCIEHue() const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return H;
}

void CIELCHColour::getCIELCH(float& L, float& C, float& H) const noexcept
{
    RGBtoCIELCH(*this, L, C, H);
}

//==============================================================================
CIELCHColour CIELCHColour::withCIELightness(float newLightness, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(newLightness, C, H, this->getJuceColour().getFloatAlpha(), imaginary);
}

CIELCHColour CIELCHColour::withCIEChroma(float newChroma, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(L, newChroma, H, this->getJuceColour().getFloatAlpha(), imaginary);
}

CIELCHColour CIELCHColour::withCIEHue(float newHue, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(L, C, newHue, this->getJuceColour().getFloatAlpha(), imaginary);
}

CIELCHColour CIELCHColour::withMultipliedCIELightness(float multiplier, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(L * multiplier, C, H, this->getJuceColour().getFloatAlpha(), imaginary);
}

CIELCHColour CIELCHColour::withMultipliedCIEChroma(float multiplier, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(L, C * multiplier, H, this->getJuceColour().getFloatAlpha(), imaginary);
}

CIELCHColour CIELCHColour::withRotatedCIEHue(float amountToRotate, bool& imaginary) const noexcept
{
    float L, C, H;
    this->getCIELCH(L, C, H);
    return fromCIELCH(L, C, H + amountToRotate, this->getJuceColour().getFloatAlpha(), imaginary);
}

//==============================================================================
CIELCHColour CIELCHColour::CIELighter(bool& imaginary, float amountBrighter) const noexcept
{
    amountBrighter = 1.0f / (1.0f + amountBrighter);
    float newLightness = 1.0f - (amountBrighter * (1.0f - getCIELightness()));
    return withCIELightness(newLightness, imaginary);
}

CIELCHColour CIELCHColour::CIEDarker(bool& imaginary, float amountDarker) const noexcept
{
    amountDarker = 1.0f / (1.0f + amountDarker);
    float newLightness = amountDarker * getCIELightness();
    return withCIELightness(newLightness, imaginary);
}

//==============================================================================
float CIELCHColour::invGammaCorrection(float g) noexcept
{
    if (g > 0.04045f)
        return std::pow(((g + 0.055f) / 1.055f), 2.4f);
    else
        return g / 12.92f;
}

void CIELCHColour::RGBtoXYZ(CIELCHColour c, float& X, float&Y, float& Z) noexcept
{
    float R = c.getJuceColour().getFloatRed();
    float G = c.getJuceColour().getFloatGreen();
    float B = c.getJuceColour().getFloatBlue();

    R = invGammaCorrection(R) * 100.0f;
    G = invGammaCorrection(G) * 100.0f;
    B = invGammaCorrection(B) * 100.0f;

    //Observer = 2°, Illuminant = D65
    X = R * 0.4124f + G * 0.3576f + B * 0.1805f;
    Y = R * 0.2126f + G * 0.7152f + B * 0.0722f;
    Z = R * 0.0193f + G * 0.1192f + B * 0.9505f;
}

float CIELCHColour::gammaCorrection(float g) noexcept
{
    if (g > 0.0031308f)
        return 1.055f * std::pow(g, (1.0f / 2.4f)) - 0.055f;
    else
        return 12.92f * g;
}

void CIELCHColour::XYZtoRGB(float X, float Y, float Z, CIELCHColour& c, bool& imaginary) noexcept
{
    /* (Observer = 2°, Illuminant = D65)
    X from 0 to  95.047
    Y from 0 to 100.000
    Z from 0 to 108.883 */

    X = X / 100.0f;
    Y = Y / 100.0f;
    Z = Z / 100.0f;

    float R = X *  3.2406f + Y * -1.5372f + Z * -0.4986f;
    float G = X * -0.9689f + Y *  1.8758f + Z *  0.0415f;
    float B = X *  0.0557f + Y * -0.2040f + Z *  1.0570f;

    R = gammaCorrection(R);
    G = gammaCorrection(G);
    B = gammaCorrection(B);

    // If R,G or B are out of bounds, we have an imaginary colour. 
    if ((R < 0.0 || R > 1.0) ||
        (G < 0.0 || G > 1.0) ||
        (B < 0.0 || B > 1.0))
    {
        imaginary = true;
    }
    else
    {
        imaginary = false;
    }

    //perform clipping. 
    if (R < 0) R = 0;
    else if (R > 1) R = 1;
    if (G < 0) G = 0;
    else if (G > 1) G = 1;
    if (B < 0) B = 0;
    else if (B > 1) B = 1;

    c = CIELCHColour((uint8)std::round(R * 255.0f), (uint8)std::round(G * 255.0f), (uint8)std::round(B * 255.0f));
}

float CIELCHColour::f(float t) noexcept
{
    if (t > 0.008856f)
        return std::pow(t, (1.0f / 3.0f));
    else
        return (7.787f * t) + (16.0f / 116.0f);
}

void CIELCHColour::XYZtoCIELab(float X, float Y, float Z, float& L, float& a, float& b) noexcept
{
    /* Observer = 2°, Illuminant = D65
    ref_X =  95.047
    ref_Y = 100.000
    ref_Z = 108.883 */
    X = X / 95.047f;
    Y = Y / 100.000f;
    Z = Z / 108.883f;

    X = f(X);
    Y = f(Y);
    Z = f(Z);

    L = (116.0f * Y) - 16.0f;
    a = 500.0f * (X - Y);
    b = 200.0f * (Y - Z);

}

float CIELCHColour::fInverse(float t) noexcept
{
    if (t > 6.0f / 29.0f)
        return std::pow(t, 3.0f);
    else
        return (t - 16.0f / 116.0f) / 7.787f;
}

void CIELCHColour::CIELabtoXYZ(float L, float a, float b, float& X, float& Y, float& Z) noexcept
{
    Y = (L + 16.0f) / 116.0f;
    X = a / 500.0f + Y;
    Z = Y - b / 200.0f;

    X = fInverse(X);
    Y = fInverse(Y);
    Z = fInverse(Z);

    /* Observer= 2°, Illuminant= D65
    ref_X =  95.047
    ref_Y = 100.000
    ref_Z = 108.883 */
    X = 95.047f * X;
    Y = 100.000f * Y;
    Z = 108.883f * Z;
}

void CIELCHColour::CIELabtoCIELCH(float L, float a, float b, float& Lout, float& C, float& H) noexcept
{
    H = std::atan2(b, a);

    if (H > 0)
        H = (H / MathConstants<float>::pi) * 180.0f;
    else
        H = 360.0f - (std::abs(H) / MathConstants<float>::pi) * 180.0f;

    Lout = L;
    C = std::sqrt(a*a + b*b);

}

void CIELCHColour::CIELCHtoCIELab(float L, float C, float H, float& Lout, float& a, float& b) noexcept
{
    Lout = L;
    a = std::cos(H * MathConstants<float>::pi / 180.0f) * C;
    b = std::sin(H * MathConstants<float>::pi / 180.0f) * C;
}

void CIELCHColour::CIELCHtoRGB(float L, float C, float H, CIELCHColour& c, bool& imaginary) noexcept
{
    //Lightness from 0 to 100
    L = L * 100.0f;
    if (L > 100.0f) L = 100.0f;
    if (L < 0.0f) L = 0.0f;
    //Chroma from 0 to 134.0
    C = C * 134.0f;
    if (C > 134.0f) C = 134.0f;
    if (C < 0.0f) C = 0.0f;
    //Hue in degrees. 
    H = H * 360.0f;

    float L_ab, a, b;
    CIELCHtoCIELab(L, C, H, L_ab, a, b);
    float X, Y, Z;
    CIELabtoXYZ(L_ab, a, b, X, Y, Z);
    XYZtoRGB(X, Y, Z, c, imaginary);
}

void CIELCHColour::RGBtoCIELCH(CIELCHColour c, float& L, float& C, float& H) noexcept
{
    float X, Y, Z;
    RGBtoXYZ(c, X, Y, Z);
    float L_ab, a, b;
    XYZtoCIELab(X, Y, Z, L_ab, a, b);
    CIELabtoCIELCH(L_ab, a, b, L, C, H);
    //rescale values 
    L = L / 100.0f;
    C = C / 134.0f;
    H = H / 360.0f;
}
