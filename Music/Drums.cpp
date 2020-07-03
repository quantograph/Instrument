// Drums.cpp
// Composer for the lead guitar track

#include <algorithm>
#include "../Devices/Devices.h"
#include "Misc.h"
#include "MusicDef.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"
#include "Track.h"
#include "Song.h"
#include "Band.h"
#include "Drums.h"

//-----------------------------------------------------------------------------
// Default constructor
Drums::Drums(Band* band) : _band(band), _settings(band->_settings) {
}

//-----------------------------------------------------------------------------
Drums::~Drums() {
}

//-----------------------------------------------------------------------------
// Makes a drum track
int Drums::Make() {
    Track* track{new Track()};
    NoteList measureNotes;
    double timeShift = 0.0;
    NoteListIter iter;
    Note* inNote;
    Note note;

    // Track info
    track->_trackName = "Drums";
    track->_instrumentName = "Drums";
    track->_instrument = PERCUSSION;
    track->_channel = 9;
    track->_volume = _settings->_composer._drums._volume;

    // Notes
    MakeMeasure(measureNotes);
    for(iter = _band->_inTrack._notes.begin(); iter != _band->_inTrack._notes.end(); iter++) {
        inNote = &(*iter);
        if(inNote->_type == Note::MEASURE) {
            if((inNote->_flags & FLAG_LAST_NOTE) != 0) { // The last measure
                note = *inNote;
                measureNotes.clear();
                note._start = 0; // Start of the measure

                note._midiNote = DRUM_BASS1;
                measureNotes.push_back(note);

                note._midiNote = DRUM_SNARE;
                measureNotes.push_back(note);

                note._midiNote = DRUM_CRASH2;
                note._volume *= 0.7;
                measureNotes.push_back(note);
            }

            track->addNotes(measureNotes, timeShift);
            timeShift += inNote->_duration; // Measure time
        }
    }

    _band->_outSong._tracks.push_back(track);

    return 0;
}

//-----------------------------------------------------------------------------
// Makes one drum measure
int Drums::MakeMeasure(NoteList& notes) {
    Note note;
    double start = 0;
    int i;
    int number;
    double duration;

    note._duration = _band->_measureTime;

    // Hi-hat
    start = 0;
    note._volume = 0.5;
    number = 4;
    duration = getNoteTime(_band->_outSong._beatTime, number);
    i = 0;
    while(start < _band->_measureTime) {
        if(i == 0) // Accent the first beat
            note._volume = 1.0;
        else
            note._volume = 0.5;

        note._midiNote = DRUM_HIHAT_FOOT;
        note._start = start;
        notes.push_back(note);
        start += duration;
        i++;
    }

    MakeOne(notes, _settings->_composer._drums, DRUM_BASS1, 0.35);
    MakeOne(notes, _settings->_composer._drums, DRUM_SNARE, -0.35);
    //MakeOneDrum(notes, _settings->_composer._drums, DRUM_TOM_6, 0.3);

    std::sort(notes.begin(), notes.end(), sortNoteTime); // Sort notes by start time

    return 0;
}

//-----------------------------------------------------------------------------
// Makes one drum measure for one drum
int Drums::MakeOne(NoteList& notes, InstrumentSettings& settings, int instrument, double pan) {
    Note note;
    bool pause;

    note._start = 0;
    note._midiNote = instrument;
    while(note._start < _band->_measureTime) {
        // Note or pause
        if(random(1, 100) <= settings._density)
            pause = false;
        else
            pause = true;

        // Duration
        note.setRandDuration(settings._lengthFrom, settings._lengthTo, _band->_outSong._beatTime, _band->_measureTime);

        note._volume = randomDouble(0.7, 1.0);

        if(!pause)
            notes.push_back(note);

        note._start += note._duration;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Plays all drums
void Drums::Test() {
    Note note;
    NoteList notes;
    Track* track{new Track()};

    // Track info
    track->_trackName = "Drums";
    track->_instrumentName = "Drums";
    track->_instrument = PERCUSSION;
    track->_channel = 9;
    track->_volume = _settings->_composer._drums._volume;

    // All drums
    note._start = 0.0;
    note._volume = 1.0;
    note._duration = 0.5;

    note._midiNote = DRUM_BASS2;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_BASS1;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_SNARE;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_HIHAT_CLOSED;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_HIHAT_FOOT;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_HIHAT_OPEN;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_TOM_LOW;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_TOM_LOW_MID;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_TOM_HIGH_MID;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_TOM_HIGH;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_RIDE;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_CHINESE;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_RIDE_BELL;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_SPLASH1;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_CRASH1;
    notes.push_back(note);
    note._start += 0.5;

    note._midiNote = DRUM_CRASH2;
    notes.push_back(note);
    note._start += 0.5;

    track->addNotes(notes, 0.0);
    _band->_outSong._tracks.push_back(track);
}
