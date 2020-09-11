// LED.h - all definitions for the LED's
#ifndef Led_h
#define Led_h

#include <Arduino.h>

class Led {
public:
    // Color
    struct COLOR {
        COLOR() : _red(0), _green(0), _blue(0) {};
        COLOR(byte red, byte green, byte blue) : _red(red), _green(green), _blue(blue) {};
        byte _red;
        byte _green;
        byte _blue;
    };

    int _redPin{14};  // PWM pin for red LED
    int _greenPin{24} ;  // PWM pin for green LED
    int _bluePin{15} ; // PWM pin for blue LED
    int _hue = 0; // Current hue
    #define HUES 256 * 6 // Number of color hues
    COLOR _hues[HUES]; // All color hueues
    float _level = 1.0; // Level for all colors

    // "Pulse width to brightness" table
    const byte _ledTable[256] = {
      0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
      1,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   4,   4,
      4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,
      8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,  14,
     14,  15,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  22,
     22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,  29,  30,  30,  31,  32,
     33,  33,  34,  35,  36,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,
     46,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
     61,  62,  63,  64,  65,  67,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,
     80,  81,  82,  83,  85,  86,  87,  89,  90,  91,  93,  94,  95,  97,  98,  99,
    101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 121, 122, 124,
    125, 127, 129, 130, 132, 134, 135, 137, 139, 141, 142, 144, 146, 148, 150, 151,
    153, 155, 157, 159, 161, 163, 165, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 189, 191, 193, 195, 197, 199, 201, 204, 206, 208, 210, 212, 215, 217,
    219, 221, 224, 226, 228, 231, 233, 235, 238, 240, 243, 245, 248, 250, 253, 255 };

    bool Init(int redPin, int greenPin, int bluePin, float level = 0.5);
    void SetLevel(float level);
    bool GetAllHues();
    bool GetHues(int& index, const COLOR& color, int redMove, int greenMove, int blueMove);
    void SetHue(int hue);
    void SetRandomHue();
    void NextColor(int& color, int& increment);
    bool SetColor(byte red, byte green, byte blue);
    void Test();
    void SetPwmFrequency(int frequency);
    void Play(uint32_t touched);
};

#endif
