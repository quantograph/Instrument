#include "Effects.h"

//=================================================================================================
Effects::Effects(AudioStream* source, uint8_t sourceOutput, AudioStream* dest, uint8_t destInput) :
    _source(source), _sourceOutput(sourceOutput), _dest(dest), _destInput(destInput) {
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

    delete _flange;
    _flange = nullptr;

    delete _chorus;
    _chorus = nullptr;

    delete _reverb;
    _reverb = nullptr;

    delete _freeReverb;
    _freeReverb = nullptr;
}

//=================================================================================================
void Effects::flange(double freq) {
    reset();
    _flange = new AudioEffectFlange();

    int s_idx = FLANGE_DELAY_LENGTH/4;
    int s_depth = FLANGE_DELAY_LENGTH/4;
    _flange->begin(_delayLine, FLANGE_DELAY_LENGTH, s_idx, s_depth, freq);
    _flange->voices(s_idx, s_depth, freq);

    _inConnection = new AudioConnection(*_source, _sourceOutput, *_flange, 0);
    _outConnection = new AudioConnection(*_flange, 0, *_dest, _destInput);
}

//=================================================================================================
void Effects::chorus() {
    reset();
    _chorus = new AudioEffectChorus();

    int n_chorus = 2;
    _chorus->begin(_delayLine, CHORUS_DELAY_LENGTH, n_chorus);
    _chorus->voices(n_chorus);

    _inConnection = new AudioConnection(*_source, _sourceOutput, *_chorus, 0);
    _outConnection = new AudioConnection(*_chorus, 0, *_dest, _destInput);
}

//=================================================================================================
void Effects::reverb() {
    reset();
    _reverb = new AudioEffectReverb();

    _reverb->reverbTime(0.3);

    _inConnection = new AudioConnection(*_source, _sourceOutput, *_reverb, 0);
    _outConnection = new AudioConnection(*_reverb, 0, *_dest, _destInput);

}


//=================================================================================================
void Effects::freeReverb() {
    reset();
    _freeReverb = new AudioEffectFreeverb();

    _freeReverb->roomsize(0.7);
    _freeReverb->damping(0.1);

    _inConnection = new AudioConnection(*_source, _sourceOutput, *_freeReverb, 0);
    _outConnection = new AudioConnection(*_freeReverb, 0, *_dest, _destInput);

}
