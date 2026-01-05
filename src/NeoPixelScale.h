#include <Arduino.h>

// use a namespace to keep the default namespace clean
namespace NeoPixel
{

    // Define a structure to hold RGB values
    struct RgbColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    // For ESP32 const globals should be in the code space automatically

    // A lookup table (array) of 16 RgbColor structures representing a full spectrum.
    // The values are in the standard 0-255 range.
    constexpr const RgbColor SpectrumLookupTable[] = {
        //   R    G    B   // Color Description
        {255, 0, 0},     // 1: Red
        {255, 64, 0},    // 2: Orange-Red
        {255, 128, 0},   // 3: Orange
        {255, 191, 0},   // 4: Golden Yellow
        {255, 255, 0},   // 5: Yellow
        {191, 255, 0},   // 6: Lime Green
        {128, 255, 0},   // 7: Green-Yellow
        {0, 255, 0},     // 8: Green
        {0, 255, 128},   // 9: Teal
        {0, 255, 255},   // 10: Cyan
        {0, 191, 255},   // 11: Light Blue
        {0, 128, 255},   // 12: Blue
        {0, 0, 255},     // 13: Deep Blue
        {128, 0, 255},   // 14: Violet
        {255, 0, 255},   // 15: Magenta
        {255, 0, 128},   // 16: Hot Pink
        {255, 255, 255}, // 17: White
        {0, 0, 0}        // 18: Black}
    };

    // Matching color names RgbColor
    constexpr const char *const colorNames[] = {
        "Red",
        "Orange-Red",
        "Orange",
        "Golden Yellow",
        "Yellow",
        "Lime Green",
        "Green-Yellow",
        "Green",
        "Teal",
        "Cyan",
        "Light Blue",
        "Blue",
        "Deep Blue",
        "Violet",
        "Magenta",
        "Hot Pink",
        "White",
        "Black"};

    // Matching enum to RgbColor
    enum NeoPixColor
    {
        Red = 0,
        OrangeRed,
        Orange,
        GoldenYellow,
        Yellow,
        LimeGreen,
        GreenYellow,
        Green,
        Teal,
        Cyan,
        LightBlue,
        Blue,
        DeepBlue,
        Violet,
        Magenta,
        HotPink,
        White,
        Black,
        NumColors
    };

    constexpr const uint8_t SpectrumLookupTableSize = sizeof(SpectrumLookupTable) / sizeof(RgbColor);

    /// @brief Display the next color using the builtin NeoPixel
    /// @param pin
    /// @param brightness
    /// @return
    inline const char *DisplayColorSpectrum(const uint8_t pin, float brightness)
    {
        if (brightness > 1.0f)
            brightness = 1.0F;
        else if (brightness < 0.0f)
            brightness = 0.0f;
        static uint8_t currentColorIndex = 0;
        // the table is 0 to 255.
        // brightness should be 0 to 1.
        float scale = RGB_BRIGHTNESS * brightness / 255;
        Serial.println("Color index: " + String(currentColorIndex));
        neopixelWrite(pin, SpectrumLookupTable[currentColorIndex].r * scale, SpectrumLookupTable[currentColorIndex].g * scale, SpectrumLookupTable[currentColorIndex].b * scale);
        int res = currentColorIndex;
        if (++currentColorIndex >= SpectrumLookupTableSize)
            currentColorIndex = 0;
        return colorNames[res];
    }

    /// @brief Display a color on builtin NeoPixel
    /// @param pin
    /// @param color  A value from enum NeoPixelColor
    /// @param brightness
    inline void DisplayColor(const uint8_t pin, const enum NeoPixColor color, float brightness = 64.0f)
    {
        if (brightness > 1.0f)
            brightness = 1.0F;
        else if (brightness < 0.0f)
            brightness = 0.0f;
        if ((uint32_t)color >= (uint32_t)NumColors)
            return;

        struct RgbColor rgb = SpectrumLookupTable[color];
        neopixelWrite(pin, rgb.r * brightness, rgb.g * brightness, rgb.b * brightness);
    }
}