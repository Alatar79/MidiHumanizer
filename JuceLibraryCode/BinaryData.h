/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Bach_Prelude_1_in_C_mid;
    const int            Bach_Prelude_1_in_C_midSize = 4543;

    extern const char*   Beethoven_Moonlight_Sonata_mid;
    const int            Beethoven_Moonlight_Sonata_midSize = 15665;

    extern const char*   Mozart_Requiem_07_Lacrimosa_mid;
    const int            Mozart_Requiem_07_Lacrimosa_midSize = 18870;

    extern const char*   AppIcon_128x128_png;
    const int            AppIcon_128x128_pngSize = 17598;

    extern const char*   AppIcon_256x256_png;
    const int            AppIcon_256x256_pngSize = 39588;

    extern const char*   AppIcon_512x512_2x_png;
    const int            AppIcon_512x512_2x_pngSize = 156721;

    extern const char*   Help_pdf;
    const int            Help_pdfSize = 1209080;

    extern const char*   SvgIcons_zip;
    const int            SvgIcons_zipSize = 16252;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) noexcept;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8) noexcept;
}
