#include <vector>
#include <list>
#include <XPT2046_Touchscreen.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>
#include "TouchScreen.h"
#include "../Music/Effects.h"
#include "../GUI/Settings.h"
#include "../GUI/Gui.h"
#include "AudioBoard.h"

//=================================================================================================
AudioBoard::AudioBoard(Gui* gui) : _gui(gui) {
}

//=================================================================================================
void AudioBoard::init() {
    bool mic = true;

    AudioMemory(120);
    attachInterrupt(GUITAR_PLUG, onPlug, CHANGE);

    _audioControl.enable();
    _audioControl.volume(0.6);

    // Input
    if(mic) {
        _audioControl.inputSelect(AUDIO_INPUT_MIC);
        _audioControl.micGain(40); // 0 - 63
    } else {
        _audioControl.inputSelect(AUDIO_INPUT_LINEIN);
        _audioControl.lineInLevel(15); // 0 - 15
    }

    // Peak meters
    _cords.push_back(new AudioConnection(_input, 0, _peakLeft, 0));
    _cords.push_back(new AudioConnection(_input, 1, _peakRight, 0));

    setupMixers();

    //notefreq.begin(.15);

    // Effects
    passthrough();
    //flange(0.5);
    //chorus();
    //reverb();
    //freeReverb();
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
    _outMixer2.gain(0, 0.2f);
    _outMixer1.gain(1, 1.0f);
    _outMixer2.gain(1, 0.5f);
    _outMixer1.gain(2, 0.5f);
    _outMixer2.gain(2, 1.0f);
    _outMixer1.gain(3, 0.2f);
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
void AudioBoard::reset() {
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
void AudioBoard::passthrough() {
    reset();

    _passthrough1 = new AudioConnection(_input, 0, _mixer1, 0);
    _passthrough2 = new AudioConnection(_input, 1, _mixer4, 0);
}

//=================================================================================================
void AudioBoard::flange(double freq) {
    reset();

    _effect1 = new Effects(&_input, 0, &_mixer1, 0);
    _effect1->flange(freq);
    _effect2 = new Effects(&_input, 1, &_mixer4, 0);
    _effect2->flange(freq);
}

//=================================================================================================
void AudioBoard::chorus() {
    reset();

    _effect1 = new Effects(&_input, 0, &_mixer1, 0);
    _effect1->chorus();
    _effect2 = new Effects(&_input, 1, &_mixer4, 0);
    _effect2->chorus();
}

//=================================================================================================
void AudioBoard::reverb() {
    reset();

    _effect1 = new Effects(&_input, 0, &_mixer1, 0);
    _effect1->reverb();
    _effect2 = new Effects(&_input, 1, &_mixer4, 0);
    _effect2->reverb();
}

//=================================================================================================
void AudioBoard::freeReverb() {
    reset();

    _effect1 = new Effects(&_input, 0, &_mixer1, 0);
    _effect1->freeReverb();
    _effect2 = new Effects(&_input, 1, &_mixer4, 0);
    _effect2->freeReverb();
}
