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
    struct Value {
        uint16_t _parent{};
        uint16_t _parent2{};
        uint16_t _tag{};
        float _value{};
        bool operator == (Value &r) {
            return _parent == r._parent &&
                   _parent2 == r._parent2 &&
                   _tag == r._tag &&
                   _value == r._value;
        }
    };

    SettingsFile();
    bool read(Settings& settings);
    bool write(Settings& settings);
    void show(const char* title);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, int16_t value);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, float value);
    static void putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getValue(Settings& settings, char* pair);
    bool split(char* pair, uint16_t& parent, uint16_t& parent2, uint16_t& tag, const char*& value);
    bool getInputSettings(InputSettings& settings, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getEffectsSettings(EffectSettings& settings, uint16_t parent, uint16_t parent2, uint16_t tag, const char* value);
    bool getSynthSettings(SynthSettings& settings, uint16_t& parent, uint16_t& parent2, uint16_t& tag, 
                          const char*& value);
    bool test();

    SdCard _sdCard;
    uint32_t _size{0};
    static uint16_t _count;
    Settings _settings{};
    String _path{"/system/settings.txt"};
    std::list<String> _writtenValues{};
    std::list<String> _readValues{};
};
