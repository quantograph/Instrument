// Bass.cpp
// Composer for the bass guitar track

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
#include "Bass.h"

//-----------------------------------------------------------------------------
// Default constructor
Bass::Bass(Band* band) : _band(band), _settings(band->_settings) {
}

//-----------------------------------------------------------------------------
Bass::~Bass() {
}

//-----------------------------------------------------------------------------
// Makes a bass track
int Bass::Make() {
    Track* track{new Track()};
    NoteList patternNotes;
    NoteList measureNotes;
    NoteListIter iter;
    Note* inNote;
    Note note;

    // Track info
    track->_trackName = "Bass";
    track->_channel = 3;
    track->_instrument = _settings->_composer._bass._instrument;
    track->_instrumentName = _instrumentNames[track->_instrument];
    track->_pan = _settings->_composer._bass._pan;
    track->_volume = _settings->_composer._bass._volume;

    // Notes
    MakePattern(3, patternNotes);
    //ShowNotes("MakeBassPattern", patternNotes);
    for(iter = _band->_inTrack._notes.begin(); iter != _band->_inTrack._notes.end(); iter++) {
        inNote = &(*iter);
        if(inNote->_type == Note::CHORD) {
            if((inNote->_flags & FLAG_LAST_NOTE) != 0) { // The last chord
                note = *inNote;
                patternNotes.clear();
                note._start = 0; // Start of the measure
                note._index = 1;
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
// Makes chord interval pattern for one bass measure
// count - number of notes in the chord
int Bass::MakePattern(int count, NoteList& notes) {
    Note note;
    int i = 0;
    bool pause;

    note._start = 0;
    while(note._start < _band->_measureTime) {
        // Note or pause
        if(random(1, 100) <= _settings->_composer._bass._density ||
           i == 0) // First note of the measure
            pause = false;
        else
            pause = true;

        // Interval
        if(i == 0) { // Root must be first
            note._index = 0;
            note._volume = 1.0;
        } else {
            note._index = random(0, count - 1);
            note._volume = randomDouble(0.7, 1.0);
        }

        // Duration
        note.setRandDuration(_settings->_composer._bass._lengthFrom, _settings->_composer._bass._lengthTo,
                             _band->_outSong._beatTime, _band->_measureTime);

        if(!pause)
            notes.push_back(note);

        // For the next note
        note._start += note._duration;
        i++;
    }

    std::sort(notes.begin(), notes.end(), sortNoteTime); // Sort notes by start time

    return 0;
}

//-----------------------------------------------------------------------------
// Makes one bass measure
int Bass::MakeMeasure(Chord& chord, NoteList& patternNotes, NoteList& measureNotes) {
    NoteListIter iter;
    Note note;
    Note* patterNote;

    measureNotes.clear();
    for(iter = patternNotes.begin(); iter != patternNotes.end(); iter++) {
        patterNote = &(*iter);

        // Find this interval's note in the chord
        note = chord._notes[patterNote->_index];

        note._start = patterNote->_start;
        note._duration = patterNote->_duration;
        note._volume = patterNote->_volume;

        measureNotes.push_back(note);
    }

    return 0;
}
