#ifndef Synth_h
#define Synth_h

#include <list>
#include <vector>
#include "Misc.h"

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
    bool init(INSTRUMENT instrument, AudioBoard* audio);
    void reset();
    void noteOn(byte note, byte velocity);
    void noteOff(byte note);
private:
    std::vector<SynthVoice*> _synthVoices; // Synth voices
    std::list<AudioMixer4*> _voiceMixers; // 4-channel mixers for all synth voices
    AudioConnection* _cords[MAX_MIXERS]; // Connections from voice mixers to the synth output mixer
    AudioMixer4 _outMixer; // Synth output mixer going to one of the 4 main mixers in the AudioBoard
    AudioConnection* _outCord{nullptr}; // Connection from synth output mixer to one of 4 main audio mixers
    InstrumentInfo _instrumentInfo;
};

#endif
