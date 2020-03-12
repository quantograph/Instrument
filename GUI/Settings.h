#ifndef Settings_h
#define Settings_h

#include <ILI9341_t3.h>
#include <EEPROM.h>
//#include <./Libraries/Teensy/SerialFlash/SerialFlash.h>

class Settings {
public:
    struct Data {
        // GUI
        uint16_t _windowColor{ILI9341_WHITE}; // Window background color
        uint16_t _borderColor{ILI9341_BLACK}; // Control border color
        uint16_t _textColor{ILI9341_NAVY}; // Text color
        int _textSize{2}; // Text size
        // Sound
        uint8_t _volume{127};
    };

    char _marker[5] = "ACDC";
    uint16_t _size = 0;
    
    Settings();
    bool read();
    bool Write();
    void Show(const char* title);
    bool ReadBuffer(int address, uint8_t* buffer, int size);
    bool WriteBuffer(int address, const uint8_t* buffer, int size);

    // Add new members to the end of this:
    Data  _data;
};

#endif
