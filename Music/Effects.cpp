#include "../Devices/Devices.h"
#include "Effects.h"

//=================================================================================================
Effects::Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
                 AudioStream* dest, uint8_t destInput) :
    _settings(settings), _source(source), _sourceOutput(sourceOutput), _dest(dest), 
    _destInput(destInput) {
}

//=================================================================================================
Effects::~Effects() {
    reset();
}

//=================================================================================================
void Effects::reset() {
    delete _inConnection;
    _inConnection = nullptr;

    delete _outConnection;
    _outConnection = nullptr;

    memset(_delayLine, 0, DELAY_LINE_LENGTH);

    delete _chorus;
    _chorus = nullptr;

    delete _flange;
    _flange = nullptr;

    delete _reverb;
    _reverb = nullptr;

    delete _freeverb;
    _freeverb = nullptr;

    delete _envelope;
    _envelope = nullptr;

    delete _delay;
    _delay = nullptr;

    delete _bitcrusher;
    _bitcrusher = nullptr;

    delete _waveshaper;
    _waveshaper = nullptr;

    delete _granular;
    _granular = nullptr;

    free(_granularMemory);
    _granularMemory = nullptr;
}

//=================================================================================================
void Effects::connect(AudioStream* stream) {
    _inConnection = new AudioConnection(*_source, _sourceOutput, *stream, 0);
    _outConnection = new AudioConnection(*stream, 0, *_dest, _destInput);
}

//=================================================================================================
// Update settings for the active effect
bool Effects::init() {
    Serial.printf("Effects::init: %s (%d)\n", _settings->_effectName.c_str(), _settings->_effectType);
    _effectType = _settings->_effectType;

    switch(_settings->_effectType) {
        case EffectType::eff_clean:
            clean();
            break;

        case EffectType::eff_chorus:
            chorus();
            break;

        case EffectType::eff_flange:
            flange();
            break;

        case EffectType::eff_reverb:
            reverb();
            break;

        case EffectType::eff_freeverb:
            freeverb();
            break;

        case EffectType::eff_envelope:
            envelope();
            break;

        case EffectType::eff_delay:
            delay();
            break;

        case EffectType::eff_bitcrusher:
            bitcrusher();
            break;

        case EffectType::eff_waveshaper:
            waveshaper();
            break;

        case EffectType::eff_granular:
            granular();
            break;

        default:
            Serial.printf("##### ERROR: Unknown effect type: %d\n", _settings->_effectType);
            return false;
    }

    update(); // Set all parameters

    return true;
}

//=================================================================================================
// Update settings for the active effect
void Effects::update() {
    Serial.printf("Effects::update: effectType=%d =======================\n", _effectType);
    const char* title{"Effects::update"};

    if(_effectType == EffectType::eff_clean) {
        Serial.printf("Effects::update: clean\n");
        return;
    }

    if(_chorus) {
        _settings->_chorus.show(title);
        _chorus->begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_chorus._delay * AUDIO_BLOCK_SAMPLES), 
                       _settings->_chorus._voices);
        _chorus->voices(_settings->_chorus._voices);
    } else if(_flange) {
        _settings->_flange.show(title);
        _flange->begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_flange._delay * AUDIO_BLOCK_SAMPLES), 
                       max(DELAY_LINE_LENGTH, _settings->_flange._offset * AUDIO_BLOCK_SAMPLES), 
                       max(DELAY_LINE_LENGTH, _settings->_flange._depth * AUDIO_BLOCK_SAMPLES), 
                       _settings->_flange._rate);
    } else if(_reverb) {
        _settings->_reverb.show(title);
        _reverb->reverbTime(_settings->_reverb._time);
    } else if(_freeverb) {
        _settings->_freeverb.show(title);
        _freeverb->roomsize(_settings->_freeverb._roomSize);
        _freeverb->damping(_settings->_freeverb._damping);
    } else if(_envelope) {
        _settings->_envelope.show(title);
	    _envelope->delay(_settings->_envelope._delay);
	    _envelope->attack(_settings->_envelope._attack);
	    _envelope->hold(_settings->_envelope._hold);
	    _envelope->decay(_settings->_envelope._decay);
	    _envelope->sustain(_settings->_envelope._sustain);
	    _envelope->release(_settings->_envelope._release);
	    _envelope->releaseNoteOn(_settings->_envelope._releaseNoteOn);
    } else if(_delay) {
        _settings->_delay.show(title);
        for(int i = 0; i < 8; ++i) {
            float delay = _settings->_delay._delays[i];
            if(delay > 0.0)
                _delay->delay(i, delay);
        }
    } else if(_bitcrusher) {
        _settings->_bitcrusher.show(title);
    	_bitcrusher->bits(_settings->_bitcrusher._bits);
        _bitcrusher->sampleRate(_settings->_bitcrusher._rate);
    } else if(_waveshaper) {
        _settings->_waveshaper.show(title);
        _waveshaper->shape(_settings->_waveshaper._waveshape, _settings->_waveshaper._length);
    } else if(_granular) {
        _settings->_granular.show(title);
        _granular->begin(_granularMemory, GRANULAR_MEMORY_SIZE);
	    _granular->setSpeed(_settings->_granular._ratio);
	    _granular->beginFreeze(_settings->_granular._freeze);
	    _granular->beginPitchShift(_settings->_granular._shift);
    } else {
        Serial.printf("##### ERROR, Effects::update: no effect to update\n");
    }
}

//=================================================================================================
void Effects::clean() {
    Serial.printf("New effect: clean\n");
    reset();
    _outConnection = new AudioConnection(*_source, _sourceOutput, *_dest, _destInput);
}

//=================================================================================================
void Effects::chorus() {
    Serial.printf("New effect: chorus\n");
    reset();

    _chorus = new AudioEffectChorus();

    connect(_chorus);
}

//=================================================================================================
void Effects::flange() {
    Serial.printf("New effect: flange\n");
    reset();

    _flange = new AudioEffectFlange();

    connect(_flange);
}

//=================================================================================================
void Effects::reverb() {
    Serial.printf("New effect: reverb\n");
    reset();

    _reverb = new AudioEffectReverb();

    connect(_reverb);
}

//=================================================================================================
void Effects::freeverb() {
    Serial.printf("New effect: freeverb\n");
    reset();

    _freeverb = new AudioEffectFreeverb();

    connect(_freeverb);
}

//=================================================================================================
void Effects::envelope() {
    Serial.printf("New effect: envelope\n");
    reset();

    _envelope = new AudioEffectEnvelope();

    connect(_envelope);
}

//=================================================================================================
void Effects::delay() {
    Serial.printf("New effect: delay\n");
    reset();

    _delay = new AudioEffectDelay();

    connect(_delay);
}

//=================================================================================================
void Effects::bitcrusher() {
    Serial.printf("New effect: bitcrusher\n");
    reset();

    _bitcrusher = new AudioEffectBitcrusher();

    connect(_bitcrusher);
}

//=================================================================================================
void Effects::waveshaper() {
    Serial.printf("New effect: waveshaper\n");
    reset();

    _waveshaper = new AudioEffectWaveshaper();

    connect(_waveshaper);
}

//=================================================================================================
void Effects::granular() {
    Serial.printf("New effect: granular\n");
    reset();

    _granular = new AudioEffectGranular();
    _granularMemory = (int16_t*)malloc(GRANULAR_MEMORY_SIZE);

    connect(_granular);
}
