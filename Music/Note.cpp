#include <algorithm>
#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"

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
    _instrument = NONE;
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

void sortNotes(NoteList* notes) {
    std::sort(notes->begin(), notes->end(), sortNoteTime);
}

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
