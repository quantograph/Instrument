#include "AudioBoard.h"
#include "AudioInput.h"
#include "../Music/Effects.h"

//=================================================================================================
AudioInput::AudioInput(AudioBoard* audio) : _audio(audio) {
}

//=================================================================================================
void AudioInput::init() {
    attachInterrupt(GUITAR_PLUG, onPlug, CHANGE);

    //_audio->_audioControl.inputSelect(AUDIO_INPUT_MIC);
    _audio->_audioControl.inputSelect(AUDIO_INPUT_LINEIN);

    _audio->_audioControl.lineInLevel(22); // Potentiometer pin
    //_audio->_audioControl.inputLevel(0.2);
    _audio->_audioControl.micGain(40); // 0 - 63

    //passthrough();
    chorus();
}

//=================================================================================================
void AudioInput::onPlug() {
    int value;

    delay(200);
    value = digitalRead(GUITAR_PLUG);
    Serial.printf(">>>> Guitar plug: %s\n", value == HIGH ? "high" : "low");
}

//=================================================================================================
void AudioInput::reset() {
    delete _effect1;
    _effect1 = nullptr;

    delete _effect2;
    _effect2 = nullptr;

    delete _passthrough1;
    _passthrough1 = nullptr;

    delete _passthrough2;
    _passthrough2 = nullptr;
}

//=================================================================================================
void AudioInput::passthrough() {
    reset();

    _passthrough1 = new AudioConnection(_input, 0, _audio->_mixer1, 1);
    _passthrough2 = new AudioConnection(_input, 1, _audio->_mixer4, 1);
}

//=================================================================================================
void AudioInput::flange(double freq) {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 1);
    _effect1->flange(freq);
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 1);
    _effect2->flange(freq);
}

//=================================================================================================
void AudioInput::chorus() {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 1);
    _effect1->chorus();
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 1);
    _effect2->chorus();
}
