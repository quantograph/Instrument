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
    #define startTag "ACDC|" // Start marker
    char _marker[6] = "ACDC|";
    #define endTag "####" // End marker
    uint16_t _size = 0;
    
    Storage();
    bool read();
    bool write();
    void show(const char* title);
    bool readBuffer(int address, uint8_t* buffer, int size);
    bool writeBuffer(int address, const uint8_t* buffer, int size);
    static void putValue(String& string, const char* parent, const char* parent2, const char* tag, int16_t value);
    static void putValue(String& string, const char* parent, const char* parent2, const char* tag, float value);
    static void putValue(String& string, const char* parent, const char* parent2, const char* tag, const char* value);
    bool getValue(char* pair);
    bool split(char* pair, const char*& parent, const char*& parent2, const char*& tag, const char*& value);

    // Add new members to the end of this:
    Settings _settings{};
};
