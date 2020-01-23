#include "../Sound/flute_samples.h"
#include "Synth.h"

//=================================================================================================
Synth::Synth(AudioBoard* audio) : _audio(audio) {
}

//=================================================================================================
void Synth::Init(int voices) {
    _voices = voices;

    // Clear all mixer connections
    for(int i = 0; i < SYNTH_MIXERS; ++i)
        _mixerCords[i] = nullptr;

    for(int i = 0; i < _voices; ++i) {
        // Load the instrument
        SynthVoice* voice = new SynthVoice;
        _synthVoices[i] = voice;
        voice->_sound.setInstrument(flute);
        voice->_sound.amplitude(1);

        // Connect the voice to it's mixer
        int mixerIndex = i / 4;
        int channelIndex = i % 4;
        AudioMixer4* voiceMixer = &(_voiceMixers[mixerIndex]);
        voice->_cord = new AudioConnection(_synthVoices[i]->_sound, 0, *voiceMixer, channelIndex);
        //Serial.printf("Voice %d, mixer %d, channel %d\n", i, mixerIndex, channelIndex);

        // Connect this mixer to the synth output mixer
        AudioConnection* mixerCord{_mixerCords[mixerIndex]};
        if(mixerCord == nullptr) {
            //Serial.printf("--- New cable\n");
            mixerCord = new AudioConnection(*voiceMixer, 0, _outMixer, mixerIndex);
            _mixerCords[mixerIndex] = mixerCord;
        }
    }

    // Connect the synth output mixer to one of the main audio mixers
    _outCord = new AudioConnection(_outMixer, 0, _audio->_mixer3, 0);
}

//=================================================================================================
void Synth::NoteOn(byte note, byte velocity) {
    SynthVoice* voice{nullptr};
    SynthVoice* oldestVoice{nullptr};
    uint32_t minTime{0};

    // Find a voice to use for this note
    for(int i = 0; i < _voices; ++i) {
        voice = _synthVoices[i];
        if(voice->_note == 0) // Found a free voice
            break;

        // Find the oldest sounding voice
        if(minTime == 0 || voice->_startTime < minTime) {
            oldestVoice = voice;
            minTime = voice->_startTime;
        }
    }

    // If no free voice was found, stop the oldest sounding voice and reuse it
    if(voice == nullptr) {
        voice = oldestVoice;
        voice->_sound.stop();
    }

    voice->_sound.playNote(note, velocity);
    voice->_note = note;
    voice->_startTime = millis();
}

//=================================================================================================
void Synth::NoteOff(byte note) {
    SynthVoice* voice{nullptr};

    // Find the voice for the note to stop
    for(int i = 0; i < _voices; ++i) {
        voice = _synthVoices[i];
        if(voice->_note == note)
            break;
    }

    // Stop the note
    voice->_sound.stop();
    voice->_note = 0;
    voice->_startTime = 0;
}
