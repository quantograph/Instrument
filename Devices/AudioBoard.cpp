#include "../Music/Misc.h"
#include "AudioBoard.h"
#include "../Sound/drumHat_samples.h"
#include "../Sound/drumBass_samples.h"
#include "../Sound/drumSnare_samples.h"
#include "../Sound/drumTomLow_samples.h"
#include "../Sound/drumTomHigh_samples.h"
#include "../Sound/drumCrash_samples.h"
#include "../Sound/drumRide_samples.h"
#include "../Sound/drumBlock_samples.h"
#include "../Sound/steelstrgtr_samples.h"
#include "../Sound/nylonstrgtr_samples.h"
#include "../Sound/steelstrgtr_samples.h"
#include "../Sound/overdrivegt_samples.h"
#include "../Sound/piano_samples.h"
#include "../Sound/bassoon_samples.h"


//=================================================================================================
void AudioBoard::Init() {
    AudioMemory(120);

    AudioNoInterrupts();

    _patchCord12 = new AudioConnection(_drumHat, 0, _mixer1, 1);
    _patchCord13 = new AudioConnection(_drumBass, 0, _mixer3, 1);
    _patchCord14 = new AudioConnection(_drumSnare, 0, _mixer2, 2);
    _patchCord15 = new AudioConnection(_drumTomHigh, 0, _mixer2, 2);
    _patchCord16 = new AudioConnection(_drumTomLow, 0, _mixer3, 3);
    _patchCord17 = new AudioConnection(_drumCrash, 0, _mixer1, 2);
    _patchCord18 = new AudioConnection(_drumRide, 0, _mixer4, 1);
    _patchCord19 = new AudioConnection(_drumBlock, 0, _mixer4, 3);
    // 2 output mixers
    _patchCord01 = new AudioConnection(_mixer1, 0, _outMixer1, 0);
    _patchCord02 = new AudioConnection(_mixer1, 0, _outMixer2, 0);
    _patchCord03 = new AudioConnection(_mixer2, 0, _outMixer1, 1);
    _patchCord04 = new AudioConnection(_mixer2, 0, _outMixer2, 1);
    _patchCord05 = new AudioConnection(_mixer3, 0, _outMixer1, 2);
    _patchCord06 = new AudioConnection(_mixer3, 0, _outMixer2, 2);
    _patchCord07 = new AudioConnection(_mixer4, 0, _outMixer1, 3);
    _patchCord08 = new AudioConnection(_mixer4, 0, _outMixer2, 3);
    _patchCord09 = new AudioConnection(_outMixer1, 0, _audioOutput, 0);
    _patchCord10 = new AudioConnection(_outMixer2, 0, _audioOutput, 1);

    // Rhytm guitar
    _rhythmGuitar1.setInstrument(steelstrgtr);
    _rhythmGuitar2.setInstrument(steelstrgtr);
    _rhythmGuitar3.setInstrument(steelstrgtr);
    _rhythmGuitar4.setInstrument(steelstrgtr);
    _rhythmGuitar5.setInstrument(steelstrgtr);
    _rhythmGuitar6.setInstrument(steelstrgtr);
    _rhythmGuitar1.amplitude(1);
    _rhythmGuitar2.amplitude(1);
    _rhythmGuitar3.amplitude(1);
    _rhythmGuitar4.amplitude(1);
    _rhythmGuitar5.amplitude(1);
    _rhythmGuitar6.amplitude(1);

    // Lead guitar
    _leadGuitar.setInstrument(nylonstrgtr);
    _leadGuitar.amplitude(1);

    // Bass guitar
    _bassGuitar.setInstrument(nylonstrgtr);
    _bassGuitar.amplitude(1);

    // Sampled drums
    _drumHat.setInstrument(drumHat);
    _drumBass.setInstrument(drumBass);
    _drumSnare.setInstrument(drumSnare);
    _drumTomHigh.setInstrument(drumTomHigh);
    _drumTomLow.setInstrument(drumTomLow);
    _drumCrash.setInstrument(drumCrash);
    _drumRide.setInstrument(drumRide);
    _drumBlock.setInstrument(drumBlock);
    _drumHat.amplitude(1);
    _drumBass.amplitude(1);
    _drumSnare.amplitude(1);
    _drumTomHigh.amplitude(1);
    _drumTomLow.amplitude(1);
    _drumCrash.amplitude(1);
    _drumRide.amplitude(1);
    _drumBlock.amplitude(1);

    _outMixer1.gain(0, 1.0f);
    _outMixer2.gain(0, 0.2f);
    _outMixer1.gain(1, 1.0f);
    _outMixer2.gain(1, 0.5f);
    _outMixer1.gain(2, 0.5f);
    _outMixer2.gain(2, 1.0f);
    _outMixer1.gain(3, 0.2f);
    _outMixer2.gain(3, 1.0f);

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
void AudioBoard::NoteOn(Instrument instrument, uint8_t note, uint8_t volume, uint8_t string) {
    sprintf(_string, "On: %d in=%d vol=%d", note, instrument, volume);
    //LogScreen(_string);

    uint8_t mappedNote = GetNote(instrument, note);
    AudioSynthWavetable* mappedInstrument = GetInstrument(instrument, note, string);
    if(mappedInstrument) {
        mappedInstrument->playNote(mappedNote, volume);
        _playingNote = note;
    } else {
        _playingNote = 0;
    }
}

//=================================================================================================
void AudioBoard::NoteOff(Instrument instrument, uint8_t n, uint8_t string) {
    sprintf(_string, "Off");
    //LogScreen(_string);

    if(n == 0)
        n = _playingNote;

    AudioSynthWavetable* mapped = GetInstrument(instrument, n, string);
    if(mapped) {
        mapped->stop();
    }
}

//=================================================================================================
AudioSynthWavetable* AudioBoard::GetInstrument(Instrument instrument, uint8_t note, uint8_t string) {
    switch(instrument) {
        case Instrument::leadGuitar: return &_leadGuitar;
        case Instrument::rhythmGuitar: 
            switch(string) {
                case 1: return &_rhythmGuitar1;
                case 2: return &_rhythmGuitar2;
                case 3: return &_rhythmGuitar3;
                case 4: return &_rhythmGuitar4;
                case 5: return &_rhythmGuitar5;
                case 6: return &_rhythmGuitar6;
                default:
                    sprintf(_string, "No string: %d", string);
                    //LogScreen(_string);
                    return NULL;
            }
        
        case Instrument::bassGuitar: return &_bassGuitar;
        case Instrument::drums: return GetDrum(note);
        default: 
            sprintf(_string, "No instr: %d", instrument);
            //LogScreen(_string);
            return NULL;
    }
}

//=================================================================================================
AudioSynthWavetable* AudioBoard::GetDrum(uint8_t note) {
    switch(note) {
        case DrumNotes::hat: return &_drumHat;
        case DrumNotes::bass: return &_drumBass;
        case DrumNotes::snare: return &_drumSnare;
        case DrumNotes::tomHigh: return &_drumTomHigh;
        case DrumNotes::tomLow: return &_drumTomLow;
        case DrumNotes::crash: return &_drumCrash;
        case DrumNotes::ride: return &_drumRide;
        case DrumNotes::woodblock: return &_drumBlock;
        default: 
            sprintf(_string, "No drum: %d", note);
            //LogScreen(_string);
            return NULL;
    }
}

//=================================================================================================
uint8_t AudioBoard::GetNote(Instrument instrument, uint8_t note) {
    if(instrument == Instrument::drums)
        return _drumMidiNotes[note];
    else
        return note;
}

//=================================================================================================
void AudioBoard::Test(int test) {
    int pause = 300;
    int strumPause = 150;

    NoteOn(Instrument::rhythmGuitar, 50, 90, 1);
    delay(strumPause);
    NoteOn(Instrument::rhythmGuitar, 54, 90, 2);
    delay(strumPause);
    NoteOn(Instrument::rhythmGuitar, 57, 90, 3);
    delay(strumPause);
    NoteOn(Instrument::rhythmGuitar, 62, 90, 4);
    delay(strumPause);
    NoteOn(Instrument::rhythmGuitar, 66, 90, 5);
    delay(strumPause);
    NoteOn(Instrument::rhythmGuitar, 69, 90, 6);
    delay(pause*3);
    NoteOff(Instrument::rhythmGuitar, 50, 1);
    NoteOff(Instrument::rhythmGuitar, 54, 2);
    NoteOff(Instrument::rhythmGuitar, 57, 3);
    NoteOff(Instrument::rhythmGuitar, 62, 4);
    NoteOff(Instrument::rhythmGuitar, 66, 5);
    NoteOff(Instrument::rhythmGuitar, 69, 6);

    NoteOn(Instrument::leadGuitar, 60, 90);
    delay(pause);
    NoteOff(Instrument::leadGuitar);
    NoteOn(Instrument::leadGuitar, 62, 90);
    delay(pause);
    NoteOff(Instrument::leadGuitar);
    NoteOn(Instrument::leadGuitar, 64, 90);
    delay(pause);
    NoteOff(Instrument::leadGuitar);

    NoteOn(Instrument::bassGuitar, 30, 90);
    delay(pause);
    NoteOff(Instrument::bassGuitar);
    NoteOn(Instrument::bassGuitar, 32, 90);
    delay(pause);
    NoteOff(Instrument::bassGuitar);
    NoteOn(Instrument::bassGuitar, 34, 90);
    delay(pause);
    NoteOff(Instrument::bassGuitar);

    // Drums
    TestDrums(pause);

    delay(1000);
}

//=================================================================================================
void AudioBoard::TestDrums(int pause) {
    NoteOn(Instrument::drums, DrumNotes::bass, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::snare, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::hat, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::tomHigh, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::tomLow, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::ride, 90); delay(pause);
    NoteOn(Instrument::drums, DrumNotes::crash, 90); delay(pause);
    //NoteOn(Instrument::drums, DrumNotes::woodblock, 90); delay(pause);
}

//=================================================================================================
// Audio loop
void AudioBoard::PeakMeter() {
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
void AudioBoard::Process() {
    //NoteFrequency();
    //PeakMeter();
    //Test();

    //delay(10);
}

//=================================================================================================
void AudioBoard::NoteFrequency() {
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
            NoteDetected(frequency);
            _noteDetected = true;
        }
    } else {
        //envelope1.noteOff();
    }
}

//=================================================================================================
void AudioBoard::NoteDetected(float frequency) {
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
