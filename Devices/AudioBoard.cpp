#include "Devices.h"
#include "../GUI/Gui.h"
#include "../Music/Effects.h"
#include "AudioBoard.h"

//=================================================================================================
AudioBoard::AudioBoard() {
}

//=================================================================================================
void AudioBoard::init(Gui* gui, Settings* settings) {
    //Serial.printf("AudioBoard::init\n");
    _gui = gui;
    _settings = settings;

    AudioMemory(120);
    attachInterrupt(GUITAR_PLUG, onPlug, CHANGE);
    //attachInterrupt(6, onVolume, CHANGE);

    _audioControl.enable();
    _audioControl.volume(0.6);

    // Input
    if(_settings->_audioSettings._input == Inputs::mic) {
        _audioControl.inputSelect(AUDIO_INPUT_MIC);
        setMicGain();
    } else if(_settings->_audioSettings._input == Inputs::line) {
        _audioControl.inputSelect(AUDIO_INPUT_LINEIN);
        setLineInLevel();
    }

    // Peak meters
    _cords.push_back(new AudioConnection(_input, 0, _peakLeft, 0));
    _cords.push_back(new AudioConnection(_input, 1, _peakRight, 0));

    setupMixers();

    //notefreq.begin(.15);

    // Make effects
    /*AVB _effect1 = new Effects(&_settings->_guitarInput._effect1, &_input, 0, &_settings->_audioBoard->_mixer1, 0);
    _effect2 = new Effects(&_settings->_guitarInput._effect2, &_input, 0, &_settings->_audioBoard->_mixer4, 0);*/
    updateEffects();
}

//=================================================================================================
bool AudioBoard::updateEffects() {
    /*Serial.printf("----- AudioBoard::updateEffects: Effect1: %s (%d). Effect2: %s (%d)\n", 
                  _settings->_guitarInput._effect1._effectName.c_str(), 
                  _settings->_guitarInput._effect1._effectType,
                  _settings->_guitarInput._effect2._effectName.c_str(), 
                  _settings->_guitarInput._effect2._effectType);

    _effect1->update();
    _effect2->update();*/

    return true;
}

//=================================================================================================
void AudioBoard::setLineInLevel() {
    uint16_t value = (uint16_t)(_settings->_audioSettings._lineInLevel * LINE_IN_MAX + 0.5);
    _settings->_audioBoard->_audioControl.lineInLevel(value);
    //Serial.printf("AudioBoard::setLineInLevel: %0.2f (%d)\n", _settings->_lineInLevel, value);
}

//=================================================================================================
void AudioBoard::setMicGain() {
    uint16_t value = (uint16_t)(_settings->_audioSettings._micGain * MIC_GAIN_MAX + 0.5);
    _settings->_audioBoard->_audioControl.micGain(value);
    //Serial.printf("AudioBoard::setMicGain: %0.2f (%d)\n", _settings->_micGain, value);
}

//=================================================================================================
void AudioBoard::setupMixers() {
    // Connect 4 input mixers to 2 output mixers
    _cords.push_back(new AudioConnection(_mixer1, 0, _outMixer1, 0));
    _cords.push_back(new AudioConnection(_mixer1, 0, _outMixer2, 0));
    _cords.push_back(new AudioConnection(_mixer2, 0, _outMixer1, 1));
    _cords.push_back(new AudioConnection(_mixer2, 0, _outMixer2, 1));
    _cords.push_back(new AudioConnection(_mixer3, 0, _outMixer1, 2));
    _cords.push_back(new AudioConnection(_mixer3, 0, _outMixer2, 2));
    _cords.push_back(new AudioConnection(_mixer4, 0, _outMixer1, 3));
    _cords.push_back(new AudioConnection(_mixer4, 0, _outMixer2, 3));

    // Spread the 4 mixers in a stereo panorama
    _outMixer1.gain(0, 1.0f);
    _outMixer2.gain(0, 0.1f);
    _outMixer1.gain(1, 1.0f);
    _outMixer2.gain(1, 0.5f);
    _outMixer1.gain(2, 0.5f);
    _outMixer2.gain(2, 1.0f);
    _outMixer1.gain(3, 0.1f);
    _outMixer2.gain(3, 1.0f);

    // 2 (stereo) output mixers go to the audio output
    _cords.push_back(new AudioConnection(_outMixer1, 0, _audioOutput, 0));
    _cords.push_back(new AudioConnection(_outMixer2, 0, _audioOutput, 1));
}

//=================================================================================================
// Audio loop
void AudioBoard::process() {
    //noteFrequency();
    peakMeter();
    //test();

    //delay(10);
}

//=================================================================================================
void AudioBoard::noteFrequency() {
    float peak = 0.0;
    float frequency = 0.0;
    float highPeak = 0.1;
    float lowPeak = 0.05;

    // Get the peak value
    if(_peakLeft.available() && _peakRight.available()) {
        float left = _peakLeft.read();
        float right = _peakRight.read();
        peak = (left + right) / 2.0f;
    } else {
        return;
    }

    // See if the note is on or off
    if(peak > highPeak && !_noteOn) {
        _noteStartTime = micros();
        _noteOn = true;
        _noteDetected = false;
        //drums4.playNote(48);
        //envelope1.noteOn();
        Serial.printf("%4d Note start: peak=%0.2f, %d\n", ++_sequence, peak, _noteStartTime);
    } else if(peak < lowPeak && _noteOn) {
        _noteStopTime = micros();
        _noteOn = false;
        //envelope1.noteOff();
        //g_led.Play(0);
        //g_keys.ShowNote(_noteNumber, false);
        Serial.printf("%4d Note end\n", ++_sequence);
    } else {
        //return;
    }

    // Get the new note frequency
    if(notefreq.available()) {
        if(!_noteDetected) {
            noteDetected(frequency);
            _noteDetected = true;
        }
    } else {
        //envelope1.noteOff();
    }
}

//=================================================================================================
void AudioBoard::noteDetected(float frequency) {
    _noteDetectTime = micros();
    frequency = notefreq.read();
    _noteNumber = AudioSynthWavetable::freqToNote(frequency);

    //drums4.playNote(48);

    //voice.playFrequency(frequency);
    //envelope1.noteOn();

    //prob = notefreq.probability();
    //Serial.printf("%4d Note on. Detect: %d-%d=%d, freq=%3.0f, prob=%.2f\n", ++_sequence, _noteStartTime, _noteDetectTime, _noteDetectTime - _noteStartTime, frequency, prob);
    Serial.printf("%4d %3.0f\n", ++_sequence, frequency);

    //g_keys.ShowNote(_noteNumber, true);
    //sprintf(_string, "%3.0f", frequency);
    //LogScreen(_string);
    //g_led.Play(1);
}

//=================================================================================================
// Audio loop
void AudioBoard::peakMeter() {
    if (_peakLeft.available() && _peakRight.available()) {
        float left = _peakLeft.read();
        float right = _peakRight.read();
        _gui->onPeakMeter(left, right);

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
void AudioBoard::onPlug() {
    int value;

    delay(200);
    value = digitalRead(GUITAR_PLUG);
    Serial.printf(">>>> Guitar plug: %s\n", value == HIGH ? "high" : "low");
}

//=================================================================================================
void AudioBoard::onVolume() {
    /*int value;

    value = analogRead(VOLUME_PIN);*/
    //Serial.printf(">>>> Volume change: %d\n", value);
}
