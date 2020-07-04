#include "../Devices/Devices.h"
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
#include "Misc.h"
#include "Note.h"
#include "Track.h"
#include "Scale.h"
#include "Song.h"
#include "Synth.h"
#include "MidiFile.h"
#include "Player.h"

//=================================================================================================
// Default constructor
Player::Player() {
}

//=================================================================================================
Player::~Player() {
    reset();
}

//=================================================================================================
// Mixer1-0 audioInput1
// Mixer1-1 synth1
// Mixer1-2 drumHat
// Mixer1-3 drumCrash
// --------
// Mixer2-0 guitarBass
// Mixer2-1 guitarRhythm
// Mixer2-2 drumSnare
// Mixer2-3 drumTomHigh
// --------
// Mixer3-0 guitarLead
// Mixer3-1 drumBass
// Mixer3-2 soloInstrument1
// Mixer3-3 drumRide
// --------
// Mixer4-0 audioInput2
// Mixer4-1 synth2
// Mixer4-2 
// Mixer4-3 drumBlock
bool Player::init(Settings* settings) {
    _settings = settings;
    setupDrums();

    for(int i = 0; i < MAX_INSTRUMENT; ++i) {
        _instruments[i] = nullptr;
    }

    return true;
}

//=================================================================================================
void Player::reset() {
    // Delete all synthesizers
    for(int i = 0; i < MAX_INSTRUMENT; ++i) {
        delete _instruments[i];
        _instruments[i] = nullptr;
    }
}

//=================================================================================================
bool Player::setupDrums() {
    AudioSynthWavetable* drum;
    
    for(int i = 0; i < MAX_DRUMS; ++i) {
        _drums[i] = nullptr;
    }

    // High-hat
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumHat);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer1, 1));
    _drums[DRUM_HIHAT_CLOSED] = drum;

    // Bass
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumBass);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer3, 1));
    _drums[DRUM_BASS1] = drum;

    // Snare
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumSnare);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer2, 2));
    _drums[DRUM_SNARE] = drum;

    // Tom high
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumTomHigh);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer2, 2));
    _drums[DRUM_TOM_HIGH] = drum;

    // Crash
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumCrash);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer1, 2));
    _drums[DRUM_CRASH1] = drum;

    // Ride
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumRide);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer4, 1));
    _drums[DRUM_RIDE] = drum;

    // Wooden block
    drum = new AudioSynthWavetable();
    drum->setInstrument(drumBlock);
    drum->amplitude(1);
    _cords.push_back(new AudioConnection(*drum, 0, _settings->_audioBoard->_mixer4, 3));
    _drums[DRUM_HI_WOOD_BLOCK] = drum;

    return true;
}

//=================================================================================================
bool Player::play(Song* song) {
    InstrumentList instruments;
    Serial.printf("Player::play\n");

    // Get all notes to play
    _notes.clear();
    song->getAllNotes(&_notes, &instruments);
    _iter = _notes.begin();
    _startTime = millis();

    // Make a synth for each instrument (Track) in this song. 
    Serial.printf("Player play notes, %d instruments\n", instruments.size());
    for(auto instrument : instruments) {
        if(instrument == PERCUSSION) {
            Serial.printf("Skipping percussion\n");
            continue;
        }

        Serial.printf("Making synth for instrument %d\n", instrument);
        Synth* synth = new Synth();
        if(synth->init(instrument, _settings)) {
            _instruments[instrument] = synth;
        }
    }

    Serial.printf("Player::play end\n");
    return true;
}

//=================================================================================================
void Player::process() {
    double time{(double)(millis() - _startTime) / 1000.0};

    // See if it's time to play the current note
    while(time > _iter->_start && _iter != _notes.end()) {
        Note* note{&(*_iter++)};
        if(note->_instrument == Instrument::PERCUSSION) {
            Serial.printf("Play drum (%6.3f): ", time); note->show();
            AudioSynthWavetable* drum = _drums[note->_midiNote];
            if(drum) {
                Serial.printf("Play drum (%6.3f): ", time); note->show();
                drum->playNote(note->_midiNote, 90 /*note->_volume*/);
            } else {
                Serial.printf("No drum for note (%6.3f): ", time); note->show();
            }
        } else {
            //Serial.printf("Play note (%6.3f): ", time); note->show();
            playNote(note);
        }
    }
}

//=================================================================================================
bool Player::playNote(Note* note) {
    Synth* synth{_instruments[note->_instrument]};
    if(!synth) {
        Serial.printf("ERROR (Player::playNote): No synth for instrument %d", note->_instrument);
        return false;
    }

    if(note->_volume > 0.0) {
        synth->noteOn(note->_midiNote, note->_volume);
    } else {
        synth->noteOff(note->_midiNote);
    }

    return true;
}

//=================================================================================================
void Player::testMidiFile(SdCard* sdCard) {
    MidiFile file;
    Song song;
    char* data;
    uint32_t dataSize;
    
    //g_sdCard.readFile("Test1.mid", data, dataSize);
    sdCard->readFile("TEST2.mid", data, dataSize);
    file.read(data, dataSize, &song);
    free(data);
    song.show();
    play(&song);
}

//=================================================================================================
void Player::testDrums() {
    int notes[] = {
    DRUM_HIHAT_CLOSED,
    DRUM_BASS1,
    DRUM_SNARE,
    DRUM_TOM_HIGH,
    DRUM_CRASH1,
    DRUM_RIDE,
    DRUM_HI_WOOD_BLOCK};

    int interval = 500;
    int volume = 90;

    for(size_t i = 0; i < (sizeof(notes) / sizeof(int)); ++i) {
        int note = notes[i];
        AudioSynthWavetable* drum = _drums[note];
        if(drum) {
            Serial.printf("Play drum %d, %d\n", note, volume);
            drum->playNote(note, volume);
        } else {
            Serial.printf("No drum note %d, %d\n", note, volume);
        }
        delay(interval);
    }
}
