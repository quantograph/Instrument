#ifndef Definitions_h
#define Definitions_h

#include <Arduino.h>

// Audio inputs
enum Inputs {
    none,
    mic,
    line
};

// Sound effects
enum EffectType {
    noneType,
    cleanType,
    chorusType,
    flangeType,
    reverbType,
    freeverbType
};

enum ControlId {
    ctl_none,
    // Buttons
    btn_back,
    btn_guitar,
    btn_synth,
    btn_band,
    btn_settings,
    // Check boxes
    chk_mic,
    chk_line,
    chk_singleEffect,
    chk_doubleEffect,
    // Text boxes
    txt_effect,
    txt_effect1,
    txt_effect2,
    // Sliders
    sld_velel
};

// Effect settings
struct Chorus {
    int _chorus{2}; // Number of voices
};

struct Flange {
    float _rate{0.5};
};

struct Reverb {
    float _reverbTime{0.3};
};

struct Freeverb {
    float _roomsize{0.7};
    float _damping{0.1};
};

struct EffectSettings {
    Chorus _chorus;
    Flange _flange;
    Reverb _reverb;
    Freeverb _freeverb;
};

class TouchScreen;
class Gui;
class AudioBoard;
class SdCard;
class MidiInput;
class Bluetooth;
class Player;
class Synth;

struct Settings {
    // GUI
    uint16_t _windowColor{ILI9341_WHITE}; // Window background color
    uint16_t _borderColor{ILI9341_BLACK}; // Control border color
    uint16_t _textColor{ILI9341_NAVY}; // Text color
    int _textSize{2}; // Text size
    // Audio
    Inputs _input{Inputs::mic}; // Mic or line in
    float _micGain{0.5}; // Microphone gain (0 - 63)
    float _lineInLevel{0.5}; // 0 - 15
    // Guitar
    uint16_t _guitarEffects{1}; // Number of guitar effects (single or double)
    EffectSettings _effect1;
    EffectSettings _effect2;

    // All devices, not saved into EEPROM, just to have them available
    TouchScreen* _screen{};
    Gui* _gui{};
    AudioBoard* _audio{};
    SdCard* _sdCard{};
    MidiInput* _midiInput{};
    Bluetooth* _bluetooth{};
    Player* _player{};
    Synth* _synth{};
};

#define MIC_GAIN_MAX 63 // 0 - 63
#define LINE_IN_MAX 15 // 0 - 15

#endif
