// Lead.cpp
// Composer for the lead guitar track

#include "../Devices/Devices.h"
#include "Misc.h"
#include "MusicDef.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"
#include "Track.h"
#include "Song.h"
#include "Band.h"
#include "Lead.h"

//-----------------------------------------------------------------------------
// Default constructor
Lead::Lead(Band* band) : _band(band), _settings(band->_settings) {
}

//-----------------------------------------------------------------------------
Lead::~Lead() {
}

//-----------------------------------------------------------------------------
// Makes a lead guitar track
int Lead::Make() {
    int i = 0;
    Track* track{new Track};
    NoteList measureNotes;
    NoteList prevNotes;
    Note lastNote;
    Note* inNote;
    NoteListIter inIter;

    // Track info
    track->_trackNumber = 1;
    track->_name = "Lead";
    track->_channel = 1;
    track->_instrument = _settings->_composer._lead._instrument;
    track->_instrumentName = _instrumentNames[track->_instrument];
    track->_pan = _settings->_composer._lead._pan;
    track->_volume = _settings->_composer._lead._volume;

    // Notes
    for(inIter = _band->_inTrack._notes.begin(); inIter != _band->_inTrack._notes.end(); inIter++) {
        inNote = &(*inIter);

        if((inNote->_flags & FLAG_LAST_NOTE) != 0) // The last note
            continue;

        if(inNote->_type == Note::CHORD) {
            MakeMeasure(*inNote->_chord, lastNote, measureNotes, prevNotes);

            // Save this measure's notes, they may be repeated in the next measure
            prevNotes.clear();
            if(i % 2 == 0) // Repeat every 2 measures only
                addNotes(measureNotes, prevNotes, 0.0);

            track->addNotes(measureNotes, inNote->_start);
            i++;
        }
    }

    //track->Show();
    if(isBass(track->_instrument)) {
        //shiftNotes(track->_notes, 0);
    } else {
        if(track->_instrument == VIOLIN)
            shiftNotes(track->_notes, 24);
        else
            shiftNotes(track->_notes, 12);
    }

    _band->_outSong._tracks.push_back(track);

    return 0;
}

//-----------------------------------------------------------------------------
// Makes one lead measure
int Lead::MakeMeasure(Chord& chord, Note& lastNote, NoteList& notes, NoteList& prevNotes) {
    notes.clear();

    // Rhythm
    RepeatRhythm(notes, prevNotes);
    MakeRhythm(notes);
    //showNotes("LeadRhythm", notes);

    // Melody
    RepeatMelody(chord, lastNote, notes, prevNotes);
    MakeMelody(notes, lastNote);
    showNotes("LeadMelody", notes);

    return 0;
}

//-----------------------------------------------------------------------------
// Makes notes with only duration (no pitch) for one lead measure
int Lead::RepeatRhythm(NoteList& notes, NoteList& prevNotes) {
    Note note;
    Note* prevNote;
    NoteListIter iter;
    int currTime;

    if(prevNotes.size() == 0) // No notes to repeat
        return 0;

    for(iter = prevNotes.begin(); iter != prevNotes.end(); iter++) {
        prevNote = &(*iter);
        currTime = (int)((double)prevNote->_start / _band->_measureTime * 100.0);
        if(currTime >= _settings->_composer._repeatRhythm)
            break;

        // Repeat the start and duration only
        note._start = prevNote->_start;
        note._duration = prevNote->_duration;
        notes.push_back(note);
    }

    //showNotes("Rhythm repeated", notes);

    return 0;
}

//-----------------------------------------------------------------------------
// Makes notes with only duration (no pitch) for one lead measure
int Lead::MakeRhythm(NoteList& notes) {
    Note note;
    Note oldNote;
    bool pause;
    int i = 0;

    // Start after the repeated notes, if any
    if(notes.size() > 0) {
        oldNote = notes[notes.size() - 1];
        note._start = oldNote._start + oldNote._duration;
    } else {
        note._start = 0.0;
    }

    // Make new rhythm
    while(note._start < _band->_measureTime) {
        // Note or pause
        if(random(1, 100) <= _settings->_composer._lead._density ||
           i == 0) // First note of the measure
            pause = false;
        else
            pause = true;

        // Duration
        note.setRandDuration(_settings->_composer._lead._lengthFrom,
            _settings->_composer._lead._lengthTo, _band->_outSong._beatTime, _band->_measureTime);

        if(!pause)
            notes.push_back(note);

        // To the next note
        note._start += note._duration;
        i++;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Makes sets note numbers for one lead measure
int Lead::RepeatMelody(Chord& chord, Note& lastNote, NoteList& notes, NoteList& prevNotes) {
    int interval;
    Note note;
    Note chordNote;
    int currTime;
    int size;
    int shift;
    Note prevNote1;
    Note newNote1;
    Note prevNote2;
    Note newNote2;

    if(notes.size() == 0) // No notes to set
        return 0;

    // First note of the measure
    note = notes[0];
    note._volume = 1.0;
    if(lastNote._midiNote != NO_MIDI_NOTE) {
        interval = random(-_settings->_composer._move, _settings->_composer._move);
        _band->_outSong._scale.GetIntervalNote(lastNote, note, interval);
        _band->_outSong._scale.MoveToChordNote(note, chord);
    } else { // First note of the song
        chord.GetRoot(chordNote);
        note.CopyTone(chordNote);
    }

    MoveToRange(note);

    if(prevNotes.size() == 0) // No notes to repeat
        return 0;

    // Get all intervals between the previous notes and move the new notes relative to the first new note
    size = std::min((int)notes.size(), (int)prevNotes.size());
    newNote1 = notes[0];
    prevNote1 = prevNotes[0];

    for(int i = 1; i < size; i++) {
        newNote2 = notes[i];
        prevNote2 = prevNotes[i];

        currTime = (int)((double)newNote2._start / _band->_measureTime * 100.0);
        if(currTime >= _settings->_composer._repeatMelody)
            break;

        newNote2._volume = prevNote1._volume;
        shift = prevNote2._scaleIndex - prevNote1._scaleIndex;
        _band->_outSong._scale.GetIndexNote(newNote1, newNote2, shift);

        newNote1 = newNote2;
        prevNote1 = prevNote2;
    }

    //showNotes("Melody repeated", notes);

    return 0;
}

//-----------------------------------------------------------------------------
// Sets note numbers for one lead measure
int Lead::MakeMelody(NoteList& notes, Note& lastNote) {
    int interval;
    NoteListIter iter;
    Note* note;

    if(notes.size() == 0) // No notes to set
        return 0;

    // Make new melody
    for(iter = notes.begin(); iter != notes.end(); iter++) {
        note = &(*iter);
        note->_instrument = _settings->_composer._lead._instrument;
        if(note->_midiNote != NO_MIDI_NOTE) { // Melody for this note is already set
            lastNote = *note;
            continue;
        }

        // Move the next note
        interval = random(0, _settings->_composer._move);
        _band->_outSong._scale.GetIntervalNote(lastNote, *note, interval);
        CheckRange(*note, lastNote);

        note->_volume = randomDouble(0.7, 1.0);
        lastNote = *note;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Stay within note range, random move
int Lead::CheckRange(Note& note, Note& lastNote) {
    int interval;
    Note limitNote;

    if(note._midiNote > _band->_maxNote._midiNote) {
        limitNote = _band->_maxNote;
        interval = -(random(0, _settings->_composer._move));
    } else if(note._midiNote < _band->_minNote._midiNote) {
        limitNote = _band->_minNote;
        interval = random(0, _settings->_composer._move);
    } else
        return 0;

    if(lastNote._midiNote != limitNote._midiNote) // Set to limit note, if it was't the limit note before
        note.CopyTone(limitNote);
    else
        _band->_outSong._scale.GetIntervalNote(limitNote, note, interval);

    return 0;
}

//-----------------------------------------------------------------------------
// Move this note into the note range by moving in by an octave
int Lead::MoveToRange(Note& note) {
    int step = 0;
    int newNote;
    Note scaleNote;

    if(note._midiNote > _band->_maxNote._midiNote)
        step = -12;
    else if(note._midiNote < _band->_minNote._midiNote)
        step = 12;
    else
        return 0;

    newNote = note._midiNote;
    do {
        newNote += step;
        if(newNote < 0 || newNote > 126) // Don't loop forever
            return -1;
    } while(newNote < _band->_minNote._midiNote || newNote > _band->_maxNote._midiNote);

    _band->_outSong._scale.FindNote(newNote, scaleNote);
    note.CopyTone(scaleNote);

    return 0;
}
