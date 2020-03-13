#include "AudioBoard.h"
#include "AudioInput.h"
#include "../Music/Effects.h"

//=================================================================================================
AudioInput::AudioInput(AudioBoard* audio) : _audio(audio) {
}

//=================================================================================================
void AudioInput::init() {
    attachInterrupt(GUITAR_PLUG, onPlug, CHANGE);

    // Peak meters
    _cords.push_back(new AudioConnection(_input, 0, _peakLeft, 0));
    _cords.push_back(new AudioConnection(_input, 1, _peakRight, 0));

    passthrough();
    //flange(0.5);
    //chorus();
    //reverb();
    freeReverb();
}

//=================================================================================================
// Audio loop
void AudioInput::peakMeter() {
    float width = 60.0;
    int cnt = 0;

    if (_peakLeft.available() && _peakRight.available()) {
        float left = _peakLeft.read();
        float right = _peakRight.read();
        Serial.printf("%0.3f %0.3f\n", left, right);
        uint8_t leftPeak = left * width;
        uint8_t rightPeak = right * width;

        /*for (cnt = 0; cnt < width - leftPeak; cnt++) {
            Serial.print(" ");
        }
        while (cnt++ < width) {
            Serial.print("<");
        }
        Serial.print("||");
        for (cnt = 0; cnt < rightPeak; cnt++) {
            Serial.print(">");
        }
        while (cnt++ < width) {
            Serial.print(" ");
        }
        Serial.println();*/
    }
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

    _passthrough1 = new AudioConnection(_input, 0, _audio->_mixer1, 0);
    _passthrough2 = new AudioConnection(_input, 1, _audio->_mixer4, 0);
}

//=================================================================================================
void AudioInput::flange(double freq) {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 0);
    _effect1->flange(freq);
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 0);
    _effect2->flange(freq);
}

//=================================================================================================
void AudioInput::chorus() {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 0);
    _effect1->chorus();
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 0);
    _effect2->chorus();
}

//=================================================================================================
void AudioInput::reverb() {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 0);
    _effect1->reverb();
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 0);
    _effect2->reverb();
}

//=================================================================================================
void AudioInput::freeReverb() {
    reset();

    _effect1 = new Effects(&_input, 0, &_audio->_mixer1, 0);
    _effect1->freeReverb();
    _effect2 = new Effects(&_input, 1, &_audio->_mixer4, 0);
    _effect2->freeReverb();
}
