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
}

//=================================================================================================
void Effects::connect(AudioStream* stream) {
    _inConnection = new AudioConnection(*_source, _sourceOutput, *stream, 0);
    _outConnection = new AudioConnection(*stream, 0, *_dest, _destInput);
}

//=================================================================================================
// Update settings for the active effect
bool Effects::init() {
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

    return true;
}

//=================================================================================================
// Update settings for the active effect
void Effects::update() {
    if(_effectType == EffectType::eff_clean) {
        //Serial.printf("Effects::update: clean\n");
        return;
    }

    if(_chorus) {
        //Serial.printf("Effects::update: _chorus=%p\n", _chorus);
        _chorus->voices(_settings->_chorus._chorus);
    } else if(_flange) {
        //Serial.printf("Effects::update: _flange._rate: %0.2f\n", _settings->_flange._rate);
        int idx = FLANGE_DELAY_LENGTH/4;
        int depth = FLANGE_DELAY_LENGTH/4;
        _flange->voices(idx, depth, _settings->_flange._rate);
    } else if(_reverb) {
        //Serial.printf("Effects::update: _reverb._reverbTime: %0.2f\n", _settings->_reverb._reverbTime);
        _reverb->reverbTime(_settings->_reverb._reverbTime);
    } else if(_freeverb) {
        //Serial.printf("Effects::update: _freeverb._roomsize: %0.2f, _damping: %0.2f\n", _settings->_freeverb._roomsize, _settings->_freeverb._damping);
        _freeverb->roomsize(_settings->_freeverb._roomsize);
        _freeverb->damping(_settings->_freeverb._damping);
    } else if(_envelope) {
        //Serial.printf("Effects::update: _envelope\n");
	    _envelope->delay(_settings->_envelope._delay);
	    _envelope->attack(_settings->_envelope._attack);
	    _envelope->hold(_settings->_envelope._hold);
	    _envelope->decay(_settings->_envelope._decay);
	    _envelope->sustain(_settings->_envelope._sustain);
	    _envelope->release(_settings->_envelope._release);
	    _envelope->releaseNoteOn(_settings->_envelope._releaseNoteOn);
    } else if(_delay) {
        //Serial.printf("Effects::update: _envelope\n");
        for(int i = 0; i < 8; ++i) {
            if(_settings->_delay._delays[i] > 0)
                _delay->delay(i, _settings->_delay._delays[i]);
        }
    } else if(_bitcrusher) {
        //Serial.printf("Effects::update: _bitcrusher\n");
    	_bitcrusher->bits(_settings->_bitcrusher._bits);
        _bitcrusher->sampleRate(_settings->_bitcrusher._sampleRate);
    } else if(_waveshaper) {
        //Serial.printf("Effects::update: _waveshaper\n");
        _waveshaper->shape(_settings->_waveshaper._waveshape, _settings->_waveshaper._length);
    } else if(_granular) {
        //Serial.printf("Effects::update: _granular\n");
    } else {
        Serial.printf("##### ERROR, Effects::update: no effect to update\n");
    }
}

//=================================================================================================
void Effects::clean() {
    //Serial.printf("Effect: clean\n");
    reset();
    _outConnection = new AudioConnection(*_source, _sourceOutput, *_dest, _destInput);
}

//=================================================================================================
void Effects::chorus() {
    //Serial.printf("Effect: chorus\n");
    reset();

    _chorus = new AudioEffectChorus();
    _chorus->begin(_delayLine, CHORUS_DELAY_LENGTH, _settings->_chorus._chorus);

    connect(_chorus);
}

//=================================================================================================
void Effects::flange() {
    //Serial.printf("Effect: flange\n");
    reset();

    _flange = new AudioEffectFlange();

    int idx = FLANGE_DELAY_LENGTH/4;
    int depth = FLANGE_DELAY_LENGTH/4;
    _flange->begin(_delayLine, FLANGE_DELAY_LENGTH, idx, depth, _settings->_flange._rate);

    connect(_flange);
}

//=================================================================================================
void Effects::reverb() {
    //Serial.printf("Effect: reverb\n");
    reset();

    _reverb = new AudioEffectReverb();

    connect(_reverb);
}

//=================================================================================================
void Effects::freeverb() {
    //Serial.printf("Effect: freeverb\n");
    reset();

    _freeverb = new AudioEffectFreeverb();

    connect(_freeverb);
}

//=================================================================================================
void Effects::envelope() {
    //Serial.printf("Effect: envelope\n");
    reset();

    _envelope = new AudioEffectEnvelope();

    connect(_envelope);
}

//=================================================================================================
void Effects::delay() {
    //Serial.printf("Effect: delay\n");
    reset();

    _delay = new AudioEffectDelay();

    connect(_delay);
}

//=================================================================================================
void Effects::bitcrusher() {
    //Serial.printf("Effect: bitcrusher\n");
    reset();

    _bitcrusher = new AudioEffectBitcrusher();

    connect(_bitcrusher);
}

//=================================================================================================
void Effects::waveshaper() {
    //Serial.printf("Effect: waveshaper\n");
    reset();

    _waveshaper = new AudioEffectWaveshaper();

    connect(_waveshaper);
}

//=================================================================================================
void Effects::granular() {
    //Serial.printf("Effect: granular\n");
    reset();

    _granular = new AudioEffectGranular();

    connect(_granular);
}
