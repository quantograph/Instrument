#ifndef TouchScreen_h
#define TouchScreen_h

#include <XPT2046_Touchscreen.h>
#include <ILI9341_t3.h>

// Store: https://www.pjrc.com/store/display_ili9341_touch.html
// Screen library: https://github.com/PaulStoffregen/ILI9341_t3
// Touch screen: https://github.com/PaulStoffregen/XPT2046_Touchscreen

class TouchScreen {
public:
    // Connections:
    // Screen ---
    // VCC: (+3.3V  1) connected to TFT_RST (RESET 4)
    // GND (GND  2)
    #define TFT_CS 14 // (Scrn-CS 3)
    #define TFT_RST 255 // (RESET 4) connected to VCC (+3.3V 1)
    #define TFT_DC 5 // (Scrn-DC 5)
    #define TFT_MOSI 11 // (6) connected to T_DIN (ScrT-DIN 12)
    #define TFT_SCK 13 // (7) connected to T_CLK (ScrT-CLK 10)
    #define TFT_LED 4 // (Scrn-LED 8) 100 Ohm resistor
    #define TFT_SDO 12 // (9) connected to T_DO (ScrT-DO 13)
    // Touch ---
    // T_CLK (ScrT-CLK 10) connected to TFT_SCK (7)
    #define T_CS 9 // (ScrT-CS 11)
    // T_DIN (ScrT-DIN 12) connected to TFT_MOSI (6)
    // T_DO (ScrT-DO 13) connected to TFT_SDO (9)
    #define T_IRQ 6 // (ScrT-IRQ 14)

    TouchScreen();
    void Init();
    bool Read();
    void Clear();
    static void OnTouched();

    XPT2046_Touchscreen _touch;
    ILI9341_t3 _screen;
    boolean _touched = false;
    TS_Point _touchPoint;
    int16_t _width = 0;
    int16_t _height = 0;
};

#endif
