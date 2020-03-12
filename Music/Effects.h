#ifndef Effects_h
#define Effects_h

#include <Audio.h>

class Effects {
    #define DELAY_LINE_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    #define FLANGE_DELAY_LENGTH (6*AUDIO_BLOCK_SAMPLES)
    #define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    short _delayLine[DELAY_LINE_LENGTH];

public:
    Effects(AudioStream* source, uint8_t sourceOutput, AudioStream* dest, uint8_t destInput);
    ~Effects();
    void reset();
    void flange(double freq);
    void chorus();
    void reverb();
    void freeReverb();

    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    AudioConnection* _inConnection{nullptr};
    AudioConnection* _outConnection{nullptr};
    AudioEffectFlange* _flange{nullptr};
    AudioEffectChorus* _chorus{nullptr};
    AudioEffectReverb* _reverb{nullptr};
    AudioEffectFreeverb* _freeReverb{nullptr};
};

#endif
