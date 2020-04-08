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
    eff_none,
    eff_clean,
    eff_chorus,
    eff_flange,
    eff_reverb,
    eff_freeverb,
    eff_envelope,
    eff_delay,
    eff_bitcrusher,
    eff_waveshaper,
    eff_granular,
    eff_last // Not an effect, for enumeration only. Insert new effects above this line.
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
    sld_velel,
    // Windows
    wnd_effect_list
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

struct Envelope {
	float _delay{0.0};
	float _attack{10.5}; // Max: 11880
	float _hold{2.5}; // Max: 11880
	float _decay{35}; // Max: 11880
	float _sustain{0.8}; // 0 to 1.0
	float _release{300}; // Max: 11880
	float _releaseNoteOn{5};
};

struct Delay {
    float _delays[8] = {-1, -1, -1, -1, -1, -1, -1, -1 };
};

struct Bitcrusher {
	uint8_t _bits{}; // 1 - 16 (16 - passthough)
    float _sampleRate{}; // Hz
};

struct Waveshaper {
    float* _waveshape{};
    int _length{};
};

struct Granular {
};

struct EffectSettings {
    EffectType _effectType{EffectType::eff_clean};
    String _effectName{};
    Chorus _chorus{};
    Flange _flange{};
    Reverb _reverb{};
    Freeverb _freeverb{};
    Envelope _envelope{};
    Delay _delay{};
    Bitcrusher _bitcrusher{};
    Waveshaper _waveshaper{};
    Granular _granular{};
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
    EffectSettings _effect1{};
    EffectSettings _effect2{};

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
