#include <algorithm>
#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "Chord.h"
#include "NoteInfo.h"

const char* Note::_names[] = { "C",  "C#", "D", "D#", "E",  "F",  "F#", "G", "G#", "A", "A#", "B" }; // Note names
const char* Note::_namesSharp[] = { "B#", "C#", "D", "D#", "E",  "E#", "F#", "G", "G#", "A", "A#", "B"  }; // Note names, wiht #
const char* Note::_namesFlat[] = { "C",  "Db", "D", "Eb", "Fb", "F",  "Gb", "G", "Ab", "A", "Bb", "Cb" }; // Note names, with b

//=================================================================================================
// Default constructor
Note::Note() {
    reset();
}

//=================================================================================================
// Copy constructor
Note::Note(const Note& source) {
    *this = source;
}

//=================================================================================================
Note::~Note() {
}

//=================================================================================================
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

//=================================================================================================
Note& Note::operator = (const Note& source) {
    _index = source._index;
    _type = source._type;
    _midiNote = source._midiNote;
    _frequency = source._frequency;
    _freqFrom = source._freqFrom;
    _freqTo = source._freqTo;
    _scaleIndex = source._scaleIndex;
    _scaleInterval = source._scaleInterval;
    _chordInterval = source._chordInterval;
    _step = source._step;
    _rootOffset = source._rootOffset;
    _start = source._start;
    _duration = source._duration;
    _volume = source._volume;
    _name = source._name;
    _nameSharp = source._nameSharp;
    _nameFlat = source._nameFlat;
    _octave = source._octave;
    _showShift = source._showShift;
    _shift = source._shift;
    _measure = source._measure;
    _flags = source._flags;
    _tab = source._tab;
    _instrument = source._instrument;
    _state = source._state;
    _channel = source._channel;

    // Copy the chord
    if(source._chord && source._type == Note::CHORD) {
        if(!_chord)
            _chord = new Chord();

        *_chord = *(source._chord);
    } else { // No chord
        if(_chord) {
            delete _chord;
            _chord = NULL;
        }
    }

    return *this;
}

//=================================================================================================
void Note::show(const char* title) {
    if(title)
        Serial.printf("%s: ", title);

    Serial.printf("note=%3d, strt=%6.3f, dur=%.3f, vol=%.1f, name=%-2s, sInd=%2d, sInt=%2d, cInt=%2d, step=%2d, "
           "offs=%2d, flgs=%d, type=%d, freq=%8.2f, octv=%d, instr=%3d\n",
           _midiNote, _start, _duration, _volume, GetName(), _scaleIndex, _scaleInterval, _chordInterval, _step, 
           _rootOffset, _flags, _type, _frequency, _octave, _instrument);

    if(_type == TYPE::CHORD && _chord) {
        Serial.printf("        ");
        _chord->show();
    }
}

//=================================================================================================
// Returns note's proper name for the current scale
const char* Note::GetName() {
    switch(_showShift) {
        case NONE: return _name.c_str();
        case SHARP: return _nameSharp.c_str();
        case FLAT: return _nameFlat.c_str();
        default: return "?";
    }
}

//=================================================================================================
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

//=================================================================================================
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

//=================================================================================================
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

//=================================================================================================
void sortNotes(NoteList* notes) {
    std::sort(notes->begin(), notes->end(), sortNoteTime);
}

//=================================================================================================
// Comparison function for sorting notes by their MIDI numbers
bool sortNoteNumber(Note note1, Note note2) {
    return (note1._midiNote < note2._midiNote);
}

//=================================================================================================
// Shows all notes in the note list
void showNotes(const char* header, NoteList& notes, const char* noteTitle) {
    Serial.printf("'%s' notes (%d) ---\n", header, (int)notes.size());
    for(auto iter : notes)
        iter.show(noteTitle);
}

//=================================================================================================
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

//=================================================================================================
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

//=================================================================================================
// Shifts note numbers
void shiftNotes(NoteList& notes, int shift) {
    NoteListIter iter;
    Note* note;

    for(iter = notes.begin(); iter != notes.end(); iter++) {
        note = &(*iter);
        note->_midiNote += shift;
    }
}

//=================================================================================================
// Gets duration of the note: 8 for 1/8, in seconds
double getNoteTime(double beatTime, int note) {
    return beatTime * (4.0 / note);
}
