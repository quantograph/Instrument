#pragma once

class TouchScreen;
class Gui;
class AudioBoard;
class SdCard;
class MidiInput;
class Bluetooth;
class Player;
class Synth;

#include "../Music/MusicDef.h"

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
    txt_instrument,
    // Sliders
    sld_velel,
    // Windows
    wnd_effect_list,
    wnd_instrument_list
};

// Effect settings
struct Chorus {
    #define ch_delayTag "crdl"
    #define ch_voicesTag "crvs"

    int _delay{16}; // Delay length, in AUDIO_BLOCK_SAMPLES, max 64
    int _voices{2}; // Number of voices

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Chorus -----\n", title.c_str());
        Serial.printf("delay=%d\n", _delay);
        Serial.printf("voices=%d\n", _voices);
    }
};

struct Flange {
    #define fl_delayTag "fldl"
    #define fl_offsetTag "flof"
    #define fl_depthTag "fldp"
    #define fl_rateTag "flrt"

    int _delay{16}; // Delay length, in AUDIO_BLOCK_SAMPLES, max 64
    int _offset{16}; // how far back the flanged sample is from the original voice, in AUDIO_BLOCK_SAMPLES, max 64
    int _depth{16}; // modulation depth (larger values give a greater variation), in AUDIO_BLOCK_SAMPLES, max 64
    float _rate{0.5}; // modulation frequency, in Hertz.

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Flange -----\n", title.c_str());
        Serial.printf("delay=%d\n", _delay);
        Serial.printf("offset=%d\n", _offset);
        Serial.printf("depth=%d\n", _depth);
        Serial.printf("rate=%0.2f\n", _rate);
    }
};

struct Reverb {
    #define rv_timeTag "rvtm"

    float _time{0.3};

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Reverb -----\n", title.c_str());
        Serial.printf("time=%0.2f\n", _time);
    }
};

struct Freeverb {
    #define fr_sizeTag "frsz"
    #define fr_dampingTag "frdp"

    float _roomSize{0.7}; // 0.0 - 1.0
    float _damping{0.1}; // 0.0 - 1.0

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Freeverb -----\n", title.c_str());
        Serial.printf("roomSize=%0.2f\n", _roomSize);
        Serial.printf("damping=%0.2f\n", _damping);
    }
};

struct Envelope {
    #define en_delayTag "endl"
    #define en_attackTag "enat"
    #define en_holdTag "enhl"
    #define en_decayTag "endc"
    #define en_sustainTag "ensu"
    #define en_releaseTag "enrl"
    #define en_releaseNoteOnTag "enrn"

	float _delay{0.0};
	float _attack{10.5}; // Max: 11880
	float _hold{2.5}; // Max: 11880
	float _decay{35}; // Max: 11880
	float _sustain{0.8}; // 0 to 1.0
	float _release{300}; // Max: 11880
	float _releaseNoteOn{5};

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Envelope -----\n", title.c_str());
        Serial.printf("delay=%0.2f\n", _delay);
        Serial.printf("attack=%0.2f\n", _attack);
        Serial.printf("hold=%0.2f\n", _hold);
        Serial.printf("decay=%0.2f\n", _decay);
        Serial.printf("sustain=%0.2f\n", _sustain);
        Serial.printf("release=%0.2f\n", _release);
        Serial.printf("releaseNoteOn=%0.2f\n", _releaseNoteOn);
    }
};

struct Delay {
    float _delays[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Delay -----\n", title.c_str());
        char tag[32];
        for(int i = 0; i < 8; ++i) {
            sprintf(tag, "delay%d", i);
            Serial.printf("%s=%0.2f\n", tag, _delays[i]);
        }
    }
};

struct Bitcrusher {
    #define bc_bitsTag "bcbt"
    #define bc_rateTag "bcrt"

	uint8_t _bits{1}; // 1 - 16 (16 - passthough)
    float _rate{}; // Hz, 1 - 44100Hz

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Bitcrusher -----\n", title.c_str());
        Serial.printf("bits=%0.2f\n", _bits);
        Serial.printf("rate=%0.2f\n", _rate);
    }
};

struct Waveshaper {
    float* _waveshape{};
    int _length{};

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Waveshaper -----\n", title.c_str());
    }
};

struct Granular {
    #define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
    #define gr_ratioTag "grrt"
    #define gr_freezeTag "grfr"
    #define gr_shiftTag "grsh"

    float _ratio{1.0}; // 0.125 - 8.0
    float _freeze{100.0}; // milliseconds
    float _shift{200.0}; // milliseconds

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s Granular -----\n", title.c_str());
        Serial.printf("ratio=%0.2f\n", _ratio);
        Serial.printf("freeze=%0.2f\n", _freeze);
        Serial.printf("shift=%0.2f\n", _shift);
    }
};

// Settings for all possible effects for one instrument (synth or guitar)
struct EffectSettings {
    #define ef_effectTypeTag "eft"
    #define ef_effectNameTag "efn"

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

    void putValues(String& string, const char* parent, const char* parent2);

    void show(String title) {
        Serial.printf("%s -----\n", title.c_str());
        Serial.printf("effectType=%d\n", _effectType);
        Serial.printf("effectName=%s\n", _effectName.c_str());
        _chorus.show(title);
        _flange.show(title);
        _reverb.show(title);
        _freeverb.show(title);
        _envelope.show(title);
        _delay.show(title);
        _bitcrusher.show(title);
        _waveshaper.show(title);
        _granular.show(title);
    }
};

// Input settings for synth or guitar
struct InputSettings {
    #define in_guitarInputTag "gin"
    #define in_synthInputTag "sin"
    #define in_effect1Tag "ef1"
    #define in_effect2Tag "ef2"
    #define in_effectsTag "efs"

    uint16_t _effects{1}; // Number of effects (single or double)
    EffectSettings _effect1{};
    EffectSettings _effect2{};

    void putValues(String& string, const char* parent);

    void show(String title) {
        String title2 = title + " input";
        Serial.printf("%s -----\n", title2.c_str());
        Serial.printf("effects=%d\n", _effects);
        _effect1.show(title2 + " effect1");
        _effect2.show(title2 + " effect2");
    }
};

// Audio setting
struct AudioSettings {
    #define au_audioTag "aud"
    #define au_inputTag "inpt"
    #define au_micGainTag "mcgn"
    #define au_lineInLevelTag "inlv"

    Inputs _input{Inputs::mic}; // Mic or line in
    float _micGain{0.5}; // Microphone gain (0 - 63)
    float _lineInLevel{0.5}; // 0 - 15

    void putValues(String& string);

    void show() {
        Serial.printf("Audio -----\n");
        Serial.printf("input=%d\n", _input);
        Serial.printf("micGain=%0.2f\n", _micGain);
        Serial.printf("lineInLevel=%0.2f\n", _lineInLevel);
    }
};

// GUI settings
struct GuiSettings {
    #define gu_guiTag "gui"
    #define gu_windowColorTag "wncl"
    #define gu_borderColorTag "bdcl"
    #define gu_textColorTag "txcl"
    #define gu_textSizeTag "txsz"

    uint16_t _windowColor{ILI9341_WHITE}; // Window background color
    uint16_t _borderColor{ILI9341_BLACK}; // Control border color
    uint16_t _textColor{ILI9341_NAVY}; // Text color
    int _textSize{2}; // Text size

    void putValues(String& string);

    void show() {
        Serial.printf("GUI -----\n");
        Serial.printf("windowColor=%d\n", _windowColor);
        Serial.printf("borderColor=%d\n", _borderColor);
        Serial.printf("textColor=%d\n", _textColor);
        Serial.printf("textSize=%d\n", _textSize);
    }
};

// Settings for synth
struct SynthSettings {
    #define sn_synthTag "syn"
    #define sn_instrumentTag "snit"
    #define sn_instrumentNameTag "snin"

    #define synthTag "snth"

    uint16_t _instrument{};
    String _instrumentName{};

    void putValues(String& string);

    void show() {
        Serial.printf("Synth -----\n");
        Serial.printf("instrument=%d\n", _instrument);
        Serial.printf("instrumentName=%s\n", _instrumentName.c_str());
    }
};

// All settings
struct Settings {
    AudioSettings _audioSettings{};
    GuiSettings _guiSettings{};
    InputSettings _guitarInput;
    InputSettings _synthInput;
    SynthSettings _synthSettings;

    void show() {
        _audioSettings.show();
        _guiSettings.show();
        _guitarInput.show("Guitar");
        _synthInput.show("Synth");
        _synthSettings.show();
    }

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
