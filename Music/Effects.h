#pragma once

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
    bool init();
    void connect(AudioStream* stream);
    void update();
    
    void clean();
    void chorus();
    void flange();
    void reverb();
    void freeverb();
    void envelope();
    void delay();
    void bitcrusher();
    void waveshaper();
    void granular();

    AudioEffectChorus* _chorus{nullptr};
    AudioEffectFlange* _flange{nullptr};
    AudioEffectReverb* _reverb{nullptr};
    AudioEffectFreeverb* _freeverb{nullptr};
    AudioEffectEnvelope* _envelope{nullptr};
    AudioEffectDelay* _delay{nullptr};
    AudioEffectBitcrusher* _bitcrusher{nullptr};
    AudioEffectWaveshaper* _waveshaper{nullptr};
    AudioEffectGranular* _granular{nullptr};

    EffectType _effectType{EffectType::eff_none}; // Current effect type
    EffectSettings* _settings{};
    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    AudioConnection* _inConnection{nullptr};
    AudioConnection* _outConnection{nullptr};
   
};
