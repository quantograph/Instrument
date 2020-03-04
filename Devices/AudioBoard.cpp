#include "AudioBoard.h"

//=================================================================================================
void AudioBoard::init() {
    AudioMemory(120);

    AudioNoInterrupts();

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

    // Waveform
    //waveform1.frequency(440);
    //waveform1.amplitude(1.0);
    //waveform1.begin(WAVEFORM_SINE);

    // Pitch detection
    /*voice.setInstrument(steelstrgtr); 
    voice.amplitude(1);
    envelope1.attack(30);
    envelope1.decay(100);
    envelope1.sustain(0.5);
    envelope1.release(500);*/

    _audioControl.enable();

    // Volume and input levels
    _audioControl.volume(0.8);

    //notefreq.begin(.15);

    /*c1.disconnect();
    c2.disconnect();
    c3.disconnect();
    c4.disconnect();*/

    /*c31.connect();
    c32.connect();
    c33.connect();
    c34.connect();*/

    AudioInterrupts();
}

//=================================================================================================
// Audio loop
void AudioBoard::peakMeter() {
    float width = 60.0;

    if (peak_L.available() && peak_R.available()) {
        float left = peak_L.read();
        float right = peak_R.read();
        Serial.printf("%0.3f %0.3f\n", left, right);
        uint8_t leftPeak = left * width;
        uint8_t rightPeak = right * width;

        for (cnt = 0; cnt < width - leftPeak; cnt++) {
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
        Serial.println();
    }
}

//=================================================================================================
// Audio loop
void AudioBoard::process() {
    //noteFrequency();
    //peakMeter();
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
    if(peak_L.available() && peak_R.available()) {
        float left = peak_L.read();
        float right = peak_R.read();
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
