#ifndef Synth_h
#define Synth_h

#include <list>
#include <vector>
#include "Misc.h"

class Effects;
class Settings;

// Data for one synth voice
struct SynthVoice {
    SynthVoice() {}
    ~SynthVoice() { delete _cord; }

    AudioSynthWavetable _sound; // Sound loaded
    AudioConnection* _cord{nullptr}; // Connection from this voice to one of the voice mixers
    byte _note{0}; // Note currently played
    uint32_t _startTime{0}; // Time this note was started
};

class Synth {
#define MAX_MIXERS 4
public:
    Synth();
    ~Synth();
    bool init(INSTRUMENT instrument, Settings* settings);
    void reset();
    void noteOn(byte note, byte velocity);
    void noteOff(byte note);
    void controlChange(byte channel, byte control, byte value);
    bool createEffect(Effects*& effect, EffectSettings* effectSettings, AudioMixer4* mixer, uint8_t mixerInput);
    bool setEffects();

private:
    Settings* _settings{};
    std::vector<SynthVoice*> _synthVoices; // Synth voices
    std::list<AudioMixer4*> _voiceMixers; // 4-channel mixers for all synth voices
    AudioConnection* _cords[MAX_MIXERS]; // Connections from voice mixers to the synth output mixer
    AudioMixer4 _outMixer; // Synth output mixer going to one of the 4 main mixers in the AudioBoard
    AudioConnection* _outCord{nullptr}; // Connection from synth output mixer to one of 4 main audio mixers
    InstrumentInfo _instrumentInfo;
    Effects* _effect1{nullptr};
    Effects* _effect2{nullptr};
    float _volume{0.5};
};

#endif
