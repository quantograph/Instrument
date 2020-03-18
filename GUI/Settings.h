#ifndef Settings_h
#define Settings_h

class Settings {
public:
    struct Data {
        // GUI
        uint16_t _windowColor{ILI9341_WHITE}; // Window background color
        uint16_t _borderColor{ILI9341_BLACK}; // Control border color
        uint16_t _textColor{ILI9341_NAVY}; // Text color
        int _textSize{2}; // Text size
        // Sound
        Inputs _input{Inputs::mic}; // Mic or line in
        uint16_t _micGain{40}; // Microphone gain (0 - 63)
        uint8_t _lineInLevel{7}; // 0 - 15
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
