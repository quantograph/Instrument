#ifndef Effects_h
#define Effects_h

#include <Audio.h>

struct EffectSettings;

class Effects {
    #define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    #define DELAY_LINE_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    #define FLANGE_DELAY_LENGTH (6*AUDIO_BLOCK_SAMPLES)
    short _delayLine[DELAY_LINE_LENGTH];

public:
    Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
            AudioStream* dest, uint8_t destInput);
    ~Effects();
    void reset();
    bool init(EffectType type);
    void connect(AudioStream* stream);
    void update();
    void clean();
    void chorus();
    void flange();
    void reverb();
    void freeverb();

    EffectSettings* _settings{};
    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    AudioConnection* _inConnection{nullptr};
    AudioConnection* _outConnection{nullptr};
    AudioEffectChorus* _chorus{nullptr};
    AudioEffectFlange* _flange{nullptr};
    AudioEffectReverb* _reverb{nullptr};
    AudioEffectFreeverb* _freeverb{nullptr};
};

#endif
