#pragma once

class Effects;
class Settings;

// Data for one synth voice
struct SynthVoice {
    SynthVoice() {}
    ~SynthVoice() {
        delete _cord;
        delete _mixer;
        Serial.printf(">>>>> deleted in SynthVoice\n");
    }

    AudioSynthWavetable _sound; // Sound loaded
    AudioConnection* _cord{nullptr}; // Connection from this voice to one of the voice mixers
    byte _note{0}; // Note currently played
    AudioMixer4* _mixer{nullptr};
    uint32_t _startTime{0}; // Time this note was started
};

class Synth {
#define MAX_MIXERS 4
public:
    Synth();
    ~Synth();
    bool init(Instrument instrument, Settings* settings);
    bool setInstrument(Instrument instrument);
    void reset();
    void noteOn(byte note, byte velocity);
    void noteOff(byte note);
    void controlChange(byte channel, byte control, byte value);
    bool updateEffects();

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
