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
    _Tab.Reset();
    _nChannel = 0;
    _instrument = PERCUSSION;
}

Note& Note::operator = (const Note& note) {
    _midiNote = note._midiNote;
    _volume = note._volume;
    _start = note._start;
    _duration = note._duration;
    _state = note._state;
    _Tab = note._Tab;
    _nChannel = note._nChannel;
    _instrument = note._instrument;
    return *this;
}

void Note::show() {
    Serial.printf("Note: start=%6.2f, durat=%6.2f, note=%d, volume=6.2%\n", 
                  _start, _duration, _midiNote, _volume);
}
