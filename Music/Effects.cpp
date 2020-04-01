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
}

//=================================================================================================
void Effects::connect(AudioStream* stream) {
    _inConnection = new AudioConnection(*_source, _sourceOutput, *stream, 0);
    _outConnection = new AudioConnection(*stream, 0, *_dest, _destInput);
}

//=================================================================================================
// Update settings for the active effect
bool Effects::init(EffectType type) {
    switch(type) {
        case cleanType:
            clean();
            break;

        case chorusType:
            chorus();
            break;

        case flangeType:
            flange();
            break;

        case reverbType:
            reverb();
            break;

        case freeverbType:
            freeverb();
            break;

        default:
            Serial.printf("##### ERROR: Unknown effect type: %d\n", type);
            return false;
    }

    update();

    return true;
}

//=================================================================================================
// Update settings for the active effect
void Effects::update() {
    if(_chorus) {
        _chorus->voices(_settings->_chorus._chorus);
    } else if(_flange) {
        int idx = FLANGE_DELAY_LENGTH/4;
        int depth = FLANGE_DELAY_LENGTH/4;
        _flange->voices(idx, depth, _settings->_flange._rate);
        Serial.printf("_flange._rate: %0.2f\n", _settings->_flange._rate);
    } else if(_reverb) {
        _reverb->reverbTime(_settings->_reverb._reverbTime);
        Serial.printf("_reverb._reverbTime: %0.2f\n", _settings->_reverb._reverbTime);
    } else if(_freeverb) {
        _freeverb->roomsize(_settings->_freeverb._roomsize);
        _freeverb->damping(_settings->_freeverb._damping);
        Serial.printf("_freeverb._roomsize: %0.2f, _damping: %0.2f\n",
                      _settings->_freeverb._roomsize, _settings->_freeverb._damping);
    }
}

//=================================================================================================
void Effects::clean() {
    Serial.printf("Effect: clean\n");
    reset();
    _outConnection = new AudioConnection(*_source, _sourceOutput, *_dest, _destInput);
}

//=================================================================================================
void Effects::chorus() {
    Serial.printf("Effect: chorus\n");
    reset();
    _chorus = new AudioEffectChorus();
    _chorus->begin(_delayLine, CHORUS_DELAY_LENGTH, _settings->_chorus._chorus);
    connect(_chorus);
}

//=================================================================================================
void Effects::flange() {
    Serial.printf("Effect: flange\n");
    reset();
    _flange = new AudioEffectFlange();

    int idx = FLANGE_DELAY_LENGTH/4;
    int depth = FLANGE_DELAY_LENGTH/4;
    _flange->begin(_delayLine, FLANGE_DELAY_LENGTH, idx, depth, _settings->_flange._rate);

    connect(_flange);
}

//=================================================================================================
void Effects::reverb() {
    Serial.printf("Effect: reverb\n");
    reset();
    _reverb = new AudioEffectReverb();
    connect(_reverb);
}

//=================================================================================================
void Effects::freeverb() {
    Serial.printf("Effect: freeverb\n");
    reset();
    _freeverb = new AudioEffectFreeverb();
    connect(_freeverb);
}
