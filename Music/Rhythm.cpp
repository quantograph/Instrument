// Rhythm.cpp
// Composer for the rhythm guitar track

#include "../Devices/Devices.h"
#include "Misc.h"
#include "MusicDef.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"
#include "Track.h"
#include "Song.h"
#include "Band.h"
#include "Rhythm.h"

//-----------------------------------------------------------------------------
// Default constructor
Rhythm::Rhythm(Band* band) : _band(band), _settings(band->_settings) {
}

//-----------------------------------------------------------------------------
Rhythm::~Rhythm() {
}

//-----------------------------------------------------------------------------
// Makes a rhythm guitar track
int Rhythm::Make() {
    Track* track{new Track()};
    NoteList patternNotes;
    NoteList measureNotes;
    Note* inNote;
    NoteListIter iter;
    Note note;

    // Track info
    track->_trackName = "Rhythm";
    track->_channel = 2;
    track->_instrument = _settings->_composer._rhythm._instrument;
    track->_instrumentName = _instrumentNames[track->_instrument];
    track->_pan = _settings->_composer._rhythm._pan;
    track->_volume = _settings->_composer._rhythm._volume;

    // Notes
    MakePattern(patternNotes);
    for(iter = _band->_inTrack._notes.begin(); iter != _band->_inTrack._notes.end(); iter++) {
        inNote = &(*iter);
        if(inNote->_type == Note::CHORD) {
            if((inNote->_flags & FLAG_LAST_NOTE) != 0) { // The last chord
                note = *inNote;
                patternNotes.clear();
                note._start = 0; // Start of the measure
                patternNotes.push_back(note);
            }

            MakeMeasure(*inNote->_chord, patternNotes, measureNotes);
            track->addNotes(measureNotes, inNote->_start);
        }
    }

    _band->_outSong._tracks.push_back(track);

    return 0;
}

//-----------------------------------------------------------------------------
// Makes a pattern for one rhythm measure
int Rhythm::MakePattern(NoteList& notes) {
    Note note;
    int i = 0;
    bool pause;

    notes.clear();
    note._start = 0;
    note._instrument = _settings->_composer._rhythm._instrument;
    while(note._start < _band->_measureTime) {
        // Note or pause
        if(random(1, 100) <= _settings->_composer._rhythm._density ||
           i == 0) // First note of the measure
            pause = false;
        else
            pause = true;

        // Volume
        if(i == 0) { // First note
            note._volume = 1.0;
        } else {
            note._volume = randomDouble(0.7, 1.0);
        }

        // Duration
        note.setRandDuration(_settings->_composer._rhythm._lengthFrom, _settings->_composer._rhythm._lengthTo,
                             _band->_outSong._beatTime, _band->_measureTime);

        if(!pause)
            notes.push_back(note);

        // For the next note
        note._start += note._duration;
        i++;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Makes one rhythm measure
int Rhythm::MakeMeasure(Chord& chord, NoteList& patternNotes, NoteList& measureNotes) {
    Note* patternNote;
    NoteListIter iter;
    Chord newChord;

    measureNotes.clear();
    for(iter = patternNotes.begin(); iter != patternNotes.end(); iter++) {
        patternNote = &(*iter);
        newChord = chord;
        newChord.Strum(true, patternNote->_start, 0.01, patternNote->_duration, patternNote->_volume);
        addNotes(newChord._notes, measureNotes, 0.0);
    }

    return 0;
}
