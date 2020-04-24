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
#include "../Music/Misc.h"

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
    sld_1,
    sld_2,
    sld_3,
    sld_4,
    // Windows
    wnd_effect_list,
    wnd_instrument_list,
    wnd_set_synth,
    wnd_set_guitar,
    wnd_set_band,
    wnd_set_audio,
};

// Tag numbers
enum Tags {
    no_tag = 0,
    // Chorus
    ch_start = 100,
    ch_delayTag,
    ch_voicesTag,
    // Flange
    fl_start = 200,
    fl_delayTag,
    fl_offsetTag,
    fl_depthTag,
    fl_rateTag,
    // Reverb
    rv_start = 300,
    rv_timeTag,
    // Freeverb
    fr_start = 400,
    fr_sizeTag,
    fr_dampingTag,
    // Envelope
    en_start = 500,
    en_delayTag,
    en_attackTag,
    en_holdTag,
    en_decayTag,
    en_sustainTag,
    en_releaseTag,
    en_releaseNoteOnTag,
    // Delay
    dl_start = 600,
    dl_1Tag,
    dl_2Tag,
    dl_3Tag,
    dl_4Tag,
    dl_5Tag,
    dl_6Tag,
    dl_7Tag,
    dl_8Tag,
    // Bitcrusher
    bc_start = 700,
    bc_bitsTag,
    bc_rateTag,
    // Waveshaper
    ws_start = 800,
    // Granular
    gr_start = 900,
    gr_ratioTag,
    gr_freezeTag,
    gr_shiftTag,
    // EffectSettings
    ef_start = 1000,
    ef_effectTypeTag,
    ef_effectNameTag,
    // InputSettings
    in_start = 1100,
    in_guitarInputTag,
    in_synthInputTag,
    in_effectsTag,
    in_effect1Tag,
    in_effect2Tag,
    // AudioSettings
    au_start = 1200,
    au_audioTag,
    au_inputTag,
    au_micGainTag,
    au_lineInLevelTag,
    // GuiSettings
    gu_start = 1300,
    gu_guiTag,
    gu_windowColorTag,
    gu_borderColorTag,
    gu_textColorTag,
    gu_textSizeTag,
    // SynthSettings
    sn_start = 1400,
    sn_synthTag,
    sn_instrumentTag,
    sn_instrumentNameTag,
};

//=================================================================================================
// Effect settings
struct Chorus {
    #define ChorusDelayMin 1
    #define ChorusDelayMax 64
    int _delay{16}; // Delay length, in AUDIO_BLOCK_SAMPLES, max 64
    #define ChorusVoicesMin 2
    #define ChorusVoicesMax 5
    int _voices{2}; // Number of voices

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Chorus\n", title.c_str());
        Serial.printf("delay=%d\n", _delay);
        Serial.printf("voices=%d\n", _voices);
    }

    void random() {
        _delay = randomInt();
        _voices = randomInt();
    }

    bool operator != (const Chorus& r) {
        return _delay != r._delay ||
               _voices != r._voices;
    }
};

//=================================================================================================
struct Flange {
    int _delay{16}; // Delay length, in AUDIO_BLOCK_SAMPLES, max 64
    int _offset{16}; // how far back the flanged sample is from the original voice, in AUDIO_BLOCK_SAMPLES, max 64
    int _depth{16}; // modulation depth (larger values give a greater variation), in AUDIO_BLOCK_SAMPLES, max 64
    float _rate{0.5}; // modulation frequency, in Hertz.

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Flange\n", title.c_str());
        Serial.printf("delay=%d\n", _delay);
        Serial.printf("offset=%d\n", _offset);
        Serial.printf("depth=%d\n", _depth);
        Serial.printf("rate=%0.2f\n", _rate);
    }

    void random() {
        _delay = randomInt();
        _offset = randomInt();
        _depth = randomInt();
        _rate = randomFloat();
    }

    bool operator != (const Flange& r) {
        return _delay != r._delay ||
               _offset != r._offset ||
               _depth != r._depth ||
               _rate != r._rate;
    }
};

//=================================================================================================
struct Reverb {
    float _time{0.3};

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Reverb\n", title.c_str());
        Serial.printf("time=%0.2f\n", _time);
    }

    void random() {
        _time = randomFloat();
    }

    bool operator != (const Reverb& r) {
        return _time != r._time;
    }
};

//=================================================================================================
struct Freeverb {
    float _roomSize{0.7}; // 0.0 - 1.0
    float _damping{0.1}; // 0.0 - 1.0

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Freeverb\n", title.c_str());
        Serial.printf("roomSize=%0.2f\n", _roomSize);
        Serial.printf("damping=%0.2f\n", _damping);
    }

    void random() {
        _roomSize = randomFloat();
        _damping = randomFloat();
    }

    bool operator != (const Freeverb& r) {
        return _roomSize != r._roomSize ||
               _damping != r._damping;
    }
};

//=================================================================================================
struct Envelope {
    float _delay{0.0};
    float _attack{10.5}; // Max: 11880
    float _hold{2.5}; // Max: 11880
    float _decay{35}; // Max: 11880
    float _sustain{0.8}; // 0 to 1.0
    float _release{300}; // Max: 11880
    float _releaseNoteOn{5};

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Envelope\n", title.c_str());
        Serial.printf("delay=%0.2f\n", _delay);
        Serial.printf("attack=%0.2f\n", _attack);
        Serial.printf("hold=%0.2f\n", _hold);
        Serial.printf("decay=%0.2f\n", _decay);
        Serial.printf("sustain=%0.2f\n", _sustain);
        Serial.printf("release=%0.2f\n", _release);
        Serial.printf("releaseNoteOn=%0.2f\n", _releaseNoteOn);
    }

    void random() {
        _delay = randomFloat();
        _attack = randomFloat();
        _hold = randomFloat();
        _decay = randomFloat();
        _sustain = randomFloat();
        _release = randomFloat();
        _releaseNoteOn = randomFloat();
    }

    bool operator != (const Envelope& r) {
        return _delay != r._delay ||
               _attack != r._attack ||
               _hold != r._hold ||
               _decay != r._decay ||
               _sustain != r._sustain ||
               _release != r._release ||
               _releaseNoteOn != r._releaseNoteOn;
    }
};

//=================================================================================================
struct Delay {
    float _delays[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Delay \n", title.c_str());
        for(int i = 0; i < 8; ++i)
            Serial.printf("%d=%0.2f\n", i, _delays[i]);
    }

    void random() {
        for(int i = 0; i < 8; ++i)
            _delays[i] = randomFloat();
    }

    bool operator != (const Delay& r) {
        for(int i = 0; i < 8; ++i)
            if(_delays[i] != r._delays[i])
                return true;

        return false;
    }
};

//=================================================================================================
struct Bitcrusher {
    uint8_t _bits{1}; // 1 - 16 (16 - passthough)
    float _rate{}; // Hz, 1 - 44100Hz

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Bitcrusher\n", title.c_str());
        Serial.printf("bits=%0.2f\n", _bits);
        Serial.printf("rate=%0.2f\n", _rate);
    }

    void random() {
        _bits = randomInt();
        _rate = randomFloat();
    }

    bool operator != (const Bitcrusher& r) {
        return _bits != r._bits ||
               _rate != r._rate;
    }
};

//=================================================================================================
struct Waveshaper {
    float* _waveshape{};
    int _length{};

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Waveshaper\n", title.c_str());
    }

    void random() {
    }

    bool operator != (const Waveshaper& r) {
        return false;
            //_waveshape != r._waveshape ||
            //_length != r._length;
    }
};

//=================================================================================================
struct Granular {
    #define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
    float _ratio{1.0}; // 0.125 - 8.0
    float _freeze{100.0}; // milliseconds
    float _shift{200.0}; // milliseconds

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s Granular\n", title.c_str());
        Serial.printf("ratio=%0.2f\n", _ratio);
        Serial.printf("freeze=%0.2f\n", _freeze);
        Serial.printf("shift=%0.2f\n", _shift);
    }

    void random() {
        _ratio = randomFloat();
        _freeze = randomFloat();
        _shift = randomFloat();
    }

    bool operator != (const Granular& r) {
        return _ratio != r._ratio ||
               _freeze != r._freeze ||
               _shift != r._shift;
    }
};

//=================================================================================================
// Settings for all possible effects for one instrument (synth or guitar)
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

    void putValues(String& string, uint16_t parent, uint16_t parent2);

    void show(String title) const {
        Serial.printf("---------- %s EffectSettings\n", title.c_str());
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

    void random() {
        _effectType = (EffectType)randomInt();
        _effectName = randomString();
        _chorus.random();
        _flange.random();
        _reverb.random();
        _freeverb.random();
        _envelope.random();
        _delay.random();
        _bitcrusher.random();
        _waveshaper.random();
        _granular.random();
    }

    bool operator != (const EffectSettings& r) {
        return _effectType != r._effectType ||
               _effectName != r._effectName ||
               _chorus != r._chorus ||
               _flange != r._flange ||
               _reverb != r._reverb ||
               _freeverb != r._freeverb ||
               _envelope != r._envelope ||
               _delay != r._delay ||
               _bitcrusher != r._bitcrusher ||
               _waveshaper != r._waveshaper ||
               _granular != r._granular;
    }

    bool compare(const EffectSettings& r) {
        if(_effectType != r._effectType) {
            Serial.printf("##### ERROR, mismatch _effectType: %d %d\n", _effectType, r._effectType);
            return false;
        }

        if(_effectName != r._effectName) {
            Serial.printf("##### ERROR, mismatch _effectType: '%s' '%s'\n", 
                          _effectName.c_str(), r._effectName.c_str());
            return false;
        }

        if(_chorus != r._chorus) {
            Serial.printf("##### ERROR, _chorus mismatch:\n");
            _chorus.show("<--"), 
            r._chorus.show("-->");
            return false;
        }

        if(_flange != r._flange) {
            Serial.printf("##### ERROR, _flange mismatch:\n");
            _flange.show("<--"), 
            r._flange.show("-->");
            return false;
        }

        if(_reverb != r._reverb) {
            Serial.printf("##### ERROR, _reverb mismatch:\n");
            _reverb.show("<--"), 
            r._reverb.show("-->");
            return false;
        }

        if(_freeverb != r._freeverb) {
            Serial.printf("##### ERROR, _freeverb mismatch:\n");
            _freeverb.show("<--"), 
            r._freeverb.show("-->");
            return false;
        }

        if(_envelope != r._envelope) {
            Serial.printf("##### ERROR, _envelope mismatch:\n");
            _envelope.show("<--"), 
            r._envelope.show("-->");
            return false;
        }

        if(_delay != r._delay) {
            Serial.printf("##### ERROR, _delay mismatch:\n");
            _delay.show("<--"), 
            r._delay.show("-->");
            return false;
        }

        if(_bitcrusher != r._bitcrusher) {
            Serial.printf("##### ERROR, _bitcrusher mismatch:\n");
            _bitcrusher.show("<--"), 
            r._bitcrusher.show("-->");
            return false;
        }

        if(_waveshaper != r._waveshaper) {
            Serial.printf("##### ERROR, _waveshaper mismatch:\n");
            _waveshaper.show("<--"), 
            r._waveshaper.show("-->");
            return false;
        }

        if(_granular != r._granular) {
            Serial.printf("##### ERROR, _granular mismatch:\n");
            _granular.show("<--"), 
            r._granular.show("-->");
            return false;
        }

        return true;
    }
};

// Input settings for synth or guitar
struct InputSettings {
    uint16_t _effects{1}; // Number of effects (single or double)
    EffectSettings _effect1{};
    EffectSettings _effect2{};

    void putValues(String& string, uint16_t parent);

    void show(String title) const {
        String title2 = title + " InputSettings";
        Serial.printf("---------- %s\n", title2.c_str());
        Serial.printf("effects=%d\n", _effects);
        _effect1.show(title2 + " effect1");
        _effect2.show(title2 + " effect2");
    }

    void random() {
        _effects = randomInt();
        _effect1.random();
        _effect2.random();
    }

    bool operator != (const InputSettings& r) {
        return _effects != r._effects ||
               _effect1 != r._effect1 ||
               _effect2 != r._effect2;
    }

    bool compare(const InputSettings& r) {
        if(_effects != r._effects) {
            Serial.printf("##### ERROR, mismatch _effects: %d %d\n", _effects, r._effects);
            return false;
        }

        if(!_effect1.compare(r._effect1)) {
            Serial.printf("##### ERROR, _effect1 mismatch:\n");
            _effect1.show("<--"), 
            r._effect1.show("-->");
            return false;
        }

        if(!_effect2.compare(r._effect2)) {
            Serial.printf("##### ERROR, _effect2 mismatch:\n");
            _effect2.show("<--"), 
            r._effect2.show("-->");
            return false;
        }

        return true;
    }
};

// Audio setting
struct AudioSettings {
    Inputs _input{Inputs::mic}; // Mic or line in
    float _micGain{0.5}; // Microphone gain (0 - 63)
    float _lineInLevel{0.5}; // 0 - 15

    void putValues(String& string);

    void show(String title) const {
        Serial.printf("---------- %s AudioSettings\n", title.c_str());
        Serial.printf("input=%d\n", _input);
        Serial.printf("micGain=%0.2f\n", _micGain);
        Serial.printf("lineInLevel=%0.2f\n", _lineInLevel);
    }

    void random() {
        _input = (Inputs)randomInt();
        _micGain = randomFloat();
        _lineInLevel = randomFloat();
    }

    bool operator != (const AudioSettings& r) {
        return _input != r._input ||
               _micGain != r._micGain ||
               _lineInLevel != r._lineInLevel;
    }
};

// GUI settings
struct GuiSettings {
    uint16_t _windowColor{ILI9341_WHITE}; // Window background color
    uint16_t _borderColor{ILI9341_BLACK}; // Control border color
    uint16_t _textColor{ILI9341_NAVY}; // Text color
    int _textSize{2}; // Text size

    void putValues(String& string);

    void show(String title) const {
        Serial.printf("---------- %s GUI\n", title.c_str());
        Serial.printf("windowColor=%d\n", _windowColor);
        Serial.printf("borderColor=%d\n", _borderColor);
        Serial.printf("textColor=%d\n", _textColor);
        Serial.printf("textSize=%d\n", _textSize);
    }

    void random() {
        _windowColor = randomInt();
        _borderColor = randomInt();
        _textColor = randomInt();
        _textSize = randomInt();
    }

    bool operator != (const GuiSettings& r) {
        return _windowColor != r._windowColor ||
               _borderColor != r._borderColor ||
               _textColor != r._textColor ||
               _textSize != r._textSize;
    }
};

// Settings for synth
struct SynthSettings {
    uint16_t _instrument{};
    String _instrumentName{};

    void putValues(String& string);

    void show(String title) const {
        Serial.printf("---------- %s SynthSettings\n", title.c_str());
        Serial.printf("instrument=%d\n", _instrument);
        Serial.printf("instrumentName=%s\n", _instrumentName.c_str());
    }

    void random() {
        _instrument = randomInt();
        _instrumentName = randomString();
    }

    bool operator != (const SynthSettings& r) {
        return _instrument != r._instrument ||
               _instrumentName != r._instrumentName;
    }
};

// All settings
struct Settings {
    AudioSettings _audioSettings{};
    GuiSettings _guiSettings{};
    InputSettings _guitarInput{};
    InputSettings _synthInput{};
    SynthSettings _synthSettings{};

    void show(String title) const {
        Serial.printf("---------- %s Settings\n", title.c_str());
        _audioSettings.show(title);
        _guiSettings.show(title);
        _guitarInput.show(title + " Guitar");
        _synthInput.show(title + " Synth");
        _synthSettings.show(title);
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

    void random() {
        _audioSettings.random();
        _guiSettings.random();
        _guitarInput.random();
        _synthInput.random();
        _synthSettings.random();
    }

    bool operator != (const Settings& r) {
        return _audioSettings != r._audioSettings ||
               _guiSettings != r._guiSettings ||
               _guitarInput != r._guitarInput ||
               _synthInput != r._synthInput ||
               _synthSettings != r._synthSettings;
    }

    bool compare(const Settings& r) {
        if(_audioSettings != r._audioSettings) {
            Serial.printf("##### ERROR, _audioSettings mismatch:\n");
            _audioSettings.show("<--");
            r._audioSettings.show("-->");
            return false;
        }

        if(_guiSettings != r._guiSettings) {
            Serial.printf("##### ERROR, _guiSettings mismatch:\n");
            _guiSettings.show("<--");
            r._guiSettings.show("-->");
            return false;
        }

        if(!_guitarInput.compare(r._guitarInput)) {
            Serial.printf("##### ERROR, _guitarInput mismatch:\n");
            _guitarInput.show("<--");
            r._guitarInput.show("-->");
            return false;
        }

        if(!_synthInput.compare(r._synthInput)) {
            Serial.printf("##### ERROR, _synthInput mismatch:\n");
            _synthInput.show("<--");
            r._synthInput.show("-->");
            return false;
        }

        if(_synthSettings != r._synthSettings) {
            Serial.printf("##### ERROR, _synthSettings mismatch:\n");
            _synthSettings.show("<--");
            r._synthSettings.show("-->");
            return false;
        }

        return true;
    }
};

#define MIC_GAIN_MAX 63 // 0 - 63
#define LINE_IN_MAX 15 // 0 - 15
