#include <algorithm>
#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "Chord.h"
#include "NoteInfo.h"

const char* Note::_names[] = { "C",  "C#", "D", "D#", "E",  "F",  "F#", "G", "G#", "A", "A#", "B" }; // Note names
const char* Note::_namesSharp[] = { "B#", "C#", "D", "D#", "E",  "E#", "F#", "G", "G#", "A", "A#", "B"  }; // Note names, wiht #
const char* Note::_namesFlat[] = { "C",  "Db", "D", "Eb", "Fb", "F",  "Gb", "G", "Ab", "A", "Bb", "Cb" }; // Note names, with b

//-----------------------------------------------------------------------------
// Default constructor
Note::Note() {
}

//-----------------------------------------------------------------------------
Note::~Note() {
}

void Note::reset() {
    _midiNote = NO_MIDI_NOTE;
    _volume = 0.0;
    _start = 0.0;
    _duration = 0.0;
    _state = STATE_NONE;
    _tab.reset();
    _channel = 0;
    _type = TYPE::NOTE;
    _scaleIndex = -1;
    _scaleInterval = -1;
    _chordInterval = -1;
    _step = -1;
    _rootOffset = -1;
    _name = "";
    _nameSharp = "";
    _nameFlat = "";
    _octave = NO_INT;
    _showShift = SHIFT::NONE;
    _shift = NONE;
    _measure = 0;
    _flags = 0;
    _instrument = Instrument::NONE;

    if(_chord) {
        delete _chord;
        _chord = NULL;
    }
}

Note& Note::operator = (const Note& note) {
    _midiNote = note._midiNote;
    _volume = note._volume;
    _start = note._start;
    _duration = note._duration;
    _state = note._state;
    _tab = note._tab;
    _channel = note._channel;
    _instrument = note._instrument;
    return *this;
}

void Note::show() {
    Serial.printf("Note: instr=%3d, start=%6.3f, durat=%6.3f, note=%2d, volume=%3.2f\n", 
                  _instrument, _start, _duration, _midiNote, _volume);
}

//-----------------------------------------------------------------------------
// Returns note's proper name for the current scale
const char* Note::GetName() {
    switch(_showShift) {
        case NONE: return _name.c_str();
        case SHARP: return _nameSharp.c_str();
        case FLAT: return _nameFlat.c_str();
        default: return "?";
    }
}

//-----------------------------------------------------------------------------
// Sets all note names for the current MIDI note number
int Note::SetNames() {
    int index;

    if(_midiNote < 0)
        return -1;

    index = _midiNote % 12;
    _name = _names[index];
    _nameSharp = _namesSharp[index];
    _nameFlat = _namesFlat[index];

    return 0;
}

//-----------------------------------------------------------------------------
// Copies note's tone info
void Note::CopyTone(Note note) {
    _octave = note._octave;
    _name = note._name;
    _nameSharp = note._nameSharp;
    _nameFlat = note._nameFlat;
    _midiNote = note._midiNote;
    _scaleIndex = note._scaleIndex;
    _scaleInterval = note._scaleInterval;
    if(note._chordInterval != -1)
        _chordInterval = note._chordInterval;
}

//-----------------------------------------------------------------------------
// Sets random duration
// lengthFrom - shortest note (1/16)
// lengthTo - longest note (1/4)
// beatTime - for 1/4
void Note::setRandDuration(int lengthFrom, int lengthTo, double beatTime, double measureTime) {
    int fastest = 32; // Fastest note (1/32)
    int length; // Note length
    int parts; // Number of duration parts for the current note
    double duration = getNoteTime(beatTime, fastest); // Duration of the fastest note (one part)
    int from;
    int to;

    // Get power of 2 for the from and to notes
    from = log2(lengthFrom);
    to = log2(lengthTo);
    length = (int)round(pow(2.0, random(to, from)));
    parts = fastest / length;
    _duration = parts * duration;
    if(_start + _duration > measureTime) // Don't get outside of the measure
        _duration = measureTime - _start;
}

//-----------------------------------------------------------------------------
void sortNotes(NoteList* notes) {
    std::sort(notes->begin(), notes->end(), sortNoteTime);
}

//-----------------------------------------------------------------------------
// Comparison function for sorting notes by their MIDI numbers
bool sortNoteNumber(Note note1, Note note2) {
    return (note1._midiNote < note2._midiNote);
}

//-----------------------------------------------------------------------------
bool sortNoteTime(Note note1, Note note2) {
    if(note1._start > note2._start)
        return false;
    else if(note1._start < note2._start)
        return true;
    else { // The start times are equal
        // Make sure that ends of notes go before starts of note
        if(note1._volume < note2._volume)
            return true;
        else
            return false;
    }
}

//-----------------------------------------------------------------------------
// Adds notes to the and of a note list, shifting their start times
void addNotes(NoteList& from, NoteList& to, double timeShift) {
    NoteListIter iter;
    Note* fromNote;
    Note toNote;

    for(iter = from.begin(); iter != from.end(); iter++) {
        fromNote = &(*iter);
        toNote = *fromNote;
        toNote._start += timeShift;
        to.push_back(toNote);
    }
}

//-----------------------------------------------------------------------------
// Shifts note numbers
void shiftNotes(NoteList& notes, int shift) {
    NoteListIter iter;
    Note* note;

    for(iter = notes.begin(); iter != notes.end(); iter++) {
        note = &(*iter);
        note->_midiNote += shift;
    }
}

//-----------------------------------------------------------------------------
// Gets duration of the note: 8 for 1/8, in seconds
double getNoteTime(double beatTime, int note) {
    return beatTime * (4.0 / note);
}
