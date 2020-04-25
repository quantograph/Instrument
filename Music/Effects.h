#pragma once

struct EffectSettings;

class Effects {
    #define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    #define FLANGE_DELAY_LENGTH (6*AUDIO_BLOCK_SAMPLES)

    #define DELAY_LINE_LENGTH (64*AUDIO_BLOCK_SAMPLES)
    short _delayLine[DELAY_LINE_LENGTH];

public:
    Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
            AudioStream* dest, uint8_t destInput);
    ~Effects();
    void reset();
    bool init(bool reset);
    void connect(AudioStream* stream);

    EffectSettings* _settings{};
    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    AudioConnection* _inConnection{nullptr};
    AudioConnection* _outConnection{nullptr};
    int16_t* _granularMemory{nullptr};
    // Effects
    AudioEffectChorus _chorus;
    AudioEffectFlange _flange;
    AudioEffectReverb _reverb;
    AudioEffectFreeverb _freeverb;
    AudioEffectEnvelope _envelope;
    AudioEffectDelay _delay;
    AudioEffectBitcrusher _bitcrusher;
    AudioEffectWaveshaper _waveshaper;
    AudioEffectGranular _granular;
};
