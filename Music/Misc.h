#ifndef Misc_h
#define Misc_h

#include <Arduino.h>
#include <Audio.h>
#include <list>
#include <vector>
#include <float.h>
#include "../Devices/AudioBoard.h"
#include "../Sound/Sound.h"
#include "MusicDef.h"

extern const char* _instrumentNames[];
extern const char* _drumNames[];

class Synth;

// Saxophone fingering
struct Fingering {
    Fingering(const char* buttons) : _buttonString(buttons) {} // List of button numbers
    String _buttonString;
    uint32_t _keys; // Bitmask with key numbers
    uint32_t _mappedKeys; // Keys mapped to sensors
    bool _mapped;
    uint16_t _note; // Note number, starting with 1 for the lowest
    int _buttons[24]; // Button numbers pressed
    int _buttonCount; // Number of buttons in the list
};

struct InstrumentInfo {
    ~InstrumentInfo();
    const AudioSynthWavetable::instrument_data* _sample{nullptr};
    INSTRUMENT _instrument = INSTRUMENT::NONE;
    String _name = "";
    //uint8_t _midiInstrument = 0;
    int _startNote = 0; // MIDI note number for the first fingering
    int _fingeringCount = 0; // Number of fingerings in _fingerings
    Fingering* _fingerings = nullptr;
    int _buttonCount = 0; // Number of buttons mapped
    int* _buttonMap = nullptr; // Touch sensors, array
    int* _keyMap = nullptr; // Instrument keys, array
    uint8_t* _noteMap = nullptr; // MIDI notes, array
    uint32_t _keyMask = 0; // Bitmask for the 'unwanted' keys
    AudioMixer4* _mixer{nullptr};
    uint8_t _mixerChannel{0};
    Synth* _synth{nullptr}; // Synthesizer for this instrument
    int _voices = 0; // Number of simultaneous voices for the synthesizer
};

typedef std::list<InstrumentInfo> InstrumentInfoList;

void reverse2Bytes(char* data);
void reverse4Bytes(char* data);
//void ShowBits(uint32_t value, String& s);
//void LogScreen(const char* line);
bool getInstrument(INSTRUMENT instrument, AudioBoard* audio, InstrumentInfo& info);

#endif
