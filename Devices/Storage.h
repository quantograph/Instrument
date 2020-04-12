#pragma once

class TouchScreen;
class Gui;
class AudioBoard;
class SdCard;
class MidiInput;
class Bluetooth;
class Player;
class Synth;

class Storage {
public:
    // Markers ----------
    #define startTag "ACDC|" // Start marker
    // Audio
    #define inputTag "aud-inpt"  // _input
    #define micGainTag  "aud-mcgn" // _micGain
    #define lineInLevelTag "au-inlv"  // _lineInLevel
    // Guitar
    #define effect1Tag "gtr-eff1"  // _guitarEffect1
    #define effect2Tag "gtr-eff2"  // _guitarEffect2
    #define endTag "####"

    char _marker[6] = "ACDC|";
    uint16_t _size = 0;
    
    Storage();
    bool read();
    bool write();
    void show(const char* title);
    bool readBuffer(int address, uint8_t* buffer, int size);
    bool writeBuffer(int address, const uint8_t* buffer, int size);
    void putValue(String& string, const char* tag, int16_t value);
    void putValue(String& string, const char* tag, float value);
    bool getValue(char* pair);

    // Add new members to the end of this:
    Settings _settings{};
};
