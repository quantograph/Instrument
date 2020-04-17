#pragma once

class TouchScreen;
class Gui;
class AudioBoard;
class SdCard;
class MidiInput;
class Bluetooth;
class Player;
class Synth;

class SettingsFile {
public:
    SettingsFile();
    bool read();
    bool write();
    void show(const char* title);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, int16_t value);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, float value);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getValue(char* pair);
    bool split(char* pair, uint16_t& parent, uint16_t& parent2, uint16_t& tag, const char*& value);
    bool getInputSettings(InputSettings& settings, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getEffectsSettings(EffectSettings& settings, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getSynthSettings(SynthSettings& settings, uint16_t& parent, uint16_t& parent2, uint16_t& tag, 
                          const char*& value);

    SdCard _sdCard;
    uint32_t _size{0};
    Settings _settings{};
    String _path{"/system/settings.txt"};
};
