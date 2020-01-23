#ifndef Synth_h
#define Synth_h

#include <Audio.h>
#include "../Devices/AudioBoard.h"

#define SYNTH_VOICES 10
#define SYNTH_MIXERS 3

struct SynthVoice {
    SynthVoice() {}
    ~SynthVoice() { delete _cord; }

    AudioSynthWavetable _sound; // Sound loaded
    AudioConnection* _cord{nullptr}; // Connection from this voice to one of the voice mixers
    byte _note{0}; // Note currently played
    uint32_t _startTime{0}; // Time this note was started
};

class Synth {
public:
    Synth(AudioBoard* audio);
    void Init(int voices);
    void NoteOn(byte note, byte velocity);
    void NoteOff(byte note);

    AudioBoard* _audio;
    SynthVoice* _synthVoices[SYNTH_VOICES]; // Synth voices
    AudioMixer4 _voiceMixers[SYNTH_MIXERS]; // 4-channel mixers for all synth voices
    AudioConnection* _mixerCords[SYNTH_MIXERS]; // Connections from voice mixers to the synth output mixer
    AudioMixer4 _outMixer; // Synth output mixer going to one of 4 main audio mixers
    AudioConnection* _outCord; // Connection from synth output mixer to one of 4 main audio mixers
    int _voices; // Number of symultanios voices
};

#endif
