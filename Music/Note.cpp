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
    _instrument = PERCUSSION;
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
    Serial.printf("Note: start=%6.2f, durat=%6.2f, note=%d, volume=6.2%\n", 
                  _start, _duration, _midiNote, _volume);
}
