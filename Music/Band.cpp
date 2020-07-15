// Band.cpp
// Band composer

#include "../Devices/Devices.h"
#include "Misc.h"
#include "MusicDef.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"
#include "Track.h"
#include "Song.h"
#include "Lead.h"
#include "Rhythm.h"
#include "Bass.h"
#include "Drums.h"
#include "Band.h"

//=================================================================================================
Band::Band(Settings* settings) : _settings(settings), _measureTime(0.0), _noteTime(0.0),
        _lead(NULL), _rhythm(NULL), _bass(NULL), _drums(NULL) {
    _lead = new Lead(this);
    _rhythm = new Rhythm(this);
    _bass = new Bass(this);
    _drums = new Drums(this);
}

//=================================================================================================
Band::~Band() {
    Reset();
    delete _lead;
    delete _rhythm;
    delete _bass;
    delete _drums;
}

//=================================================================================================
// Resets everything
void Band::Reset() {
    _inTrack.reset();
    _outSong.reset();
    _measureTime = 0.0;
    _noteTime = 0.0;
    _minNote.reset();
    _maxNote.reset();
    _chordProg.clear();
}

//=================================================================================================
// Makes a test song
void Band::test() {
    Reset();

    _outSong._tempo = 120;
    _settings->_composer._measureBeats = 4;
    _settings->_composer._beatNote = 4;
    _settings->_composer._scaleType = SCALE_C_MAJOR;
    _settings->_composer._verseChords.push_back(Chord::CHORD_PROG_1625);
    _settings->_composer._chorusChords.push_back(Chord::CHORD_PROG_2511);
    Make();
}

//=================================================================================================
// Makes a song
bool Band::Make() {
    Reset();

    // Make sure the parameters are set
    if(!_settings->_composer._verseChords.size() && !_settings->_composer._chorusChords.size()) {
        return false;
    }

    // Copy some settings into the song
    _outSong._name = "Band song";
    _outSong._tempo = _settings->_composer._tempo;
    _outSong._measureBeats = _settings->_composer._measureBeats;
    _outSong._beatNote = _settings->_composer._beatNote;
    _outSong._measures = 4;
    _outSong._scaleType = _settings->_composer._scaleType;
    _outSong._scale.Make(_outSong._scaleType, _settings->_composer._scaleRoot);
    _outSong._scale.Show(false);

    // Measure time for the current time signature
    _outSong._beatTime = 60.0 / _outSong._tempo;
    _noteTime = _outSong._beatTime * (4.0 / _outSong._beatNote);
    _measureTime = _noteTime * _outSong._measureBeats;

    // Chord progressions
    _chordProg.clear();
    _chordProg.insert(_chordProg.end(), _settings->_composer._verseChords.begin(), _settings->_composer._verseChords.end());
    _chordProg.insert(_chordProg.end(), _settings->_composer._chorusChords.begin(), _settings->_composer._chorusChords.end());
    MakeChords();

    // Make all tracks
    if(_settings->_composer._lead._play)
        _lead->Make();

    if(_settings->_composer._rhythm._play)
        _rhythm->Make();

    if(_settings->_composer._bass._play)
        _bass->Make();

    if(_settings->_composer._drums._play)
        _drums->Make();

    _outSong.show();

    return true;
}

//=================================================================================================
// Makes a list of all chords
bool Band::MakeChords() {
    Note note;
    Note lastNote;
    IntList intervals;
    IntListIter progIter;
    IntListIter intervalIter;
    int chordCounter;
    int progCounter = 0;
    int interval;
    Chord::CHORD_PROG prog;

    Serial.printf("MakeChords ========================================\n");

    note._start = 0.0;
    note._volume = 1.0;
    note._duration = _measureTime;
    note._chord = new Chord();
    note._index = 1;

    // Make chords sequences for all progressions
    for(progIter = _chordProg.begin(); progIter != _chordProg.end(); progIter++) {
        prog = (Chord::CHORD_PROG)*progIter;
        Serial.printf("\n===== Progression '%s'\n", Chord::GetChordProgName(prog));
        intervals.clear();
        Chord::GetChordProgNumbers(prog, intervals);
        chordCounter = 0;

        // Get all chords for the current progression
        for(intervalIter = intervals.begin(); intervalIter != intervals.end(); intervalIter++) {
            interval = *intervalIter;

            // Chord
            note._type = Note::CHORD;
            note._scaleInterval = interval;
            note._chord->Make(_outSong._scale, 40, interval, Chord::CHORD_135);
            note.show();
            _inTrack._notes.push_back(note);
            if(chordCounter == 0) // Last note is the same as first one in progression
                lastNote = note;

            // Song's note range
            if(progCounter == 0 && chordCounter == 0) { // Set note range for the song based on root of the first chord
                _outSong._scale.GetIntervalNote(note._chord->_root, _minNote, -(_settings->_composer._moveRange / 2));
                _outSong._scale.GetIntervalNote(_minNote, _maxNote, _settings->_composer._moveRange);
                //Serial.printf("Song range: %d-%d\n", _minNote._midiNote, _maxNote._midiNote);
            }

            // Measure
            note._type = Note::MEASURE;
            note._measure = chordCounter;
            _inTrack._notes.push_back(note);

            note._start += note._duration;
            note._index++;
            chordCounter++;
        }

        progCounter++;
    }

    // Last note
    lastNote._flags |= FLAG_LAST_NOTE;
    lastNote._start = note._start;
    lastNote._duration = _outSong._beatTime * 2;

    // Last chord
    lastNote._type = Note::CHORD;
    note = lastNote;
    note._chord->Make(_outSong._scale, 40, lastNote._scaleInterval, Chord::CHORD_135);
    _inTrack._notes.push_back(note);

    // Last measure
    lastNote._type = Note::MEASURE;
    lastNote._measure = 1;
    _inTrack._notes.push_back(lastNote);

    _inTrack.sort();
    //_inTrack.Show("inTrack");
    Serial.printf("======================================== MakeChords\n");

    return true;
}
