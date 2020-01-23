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
    void Flange(double freq);
    void Chorus();
    void Reset();

    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    AudioEffectFlange* _flange{nullptr};
    AudioEffectChorus* _chorus{nullptr};
    AudioConnection* _inConnection{nullptr};
    AudioConnection* _outConnection{nullptr};
};

#endif
