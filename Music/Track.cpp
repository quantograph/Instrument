#include <algorithm>
#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "Track.h"

//-----------------------------------------------------------------------------
// Default constructor
Track::Track() {
}

//-----------------------------------------------------------------------------
Track::~Track() {
}

void Track::show() {
    // Show track info
    Serial.printf("Track %d '%s', intrument=(%d)'%s', %d notes\n", 
                  _trackNumber, _name.c_str(), _instrument, _instrumentName.c_str(), _notes.size());

    // Show all notes
    for(auto note : _notes) {
        note.show();
    }
}

void Track::getAllNotes(NoteList* notes) {
    for(auto note : _notes) {
        // Add the starting note
        note._instrument = _instrument;
        notes->push_back(note);
        
        // Add the ending note
        Note endNote{note};
        endNote._start = note._start + note._duration;
        endNote._volume = 0.0;
        notes->push_back(endNote);
    }
}

//-----------------------------------------------------------------------------
// Resets everything
void Track::reset() {
    _notes.clear();
    _trackName = "";
    _instrumentName = "";
    _instrument = Instrument::NONE;
    _trackNumber = 0;
    _channel = 0;
    _pan = 0.0;
    _volume = 1.0;
}

//-----------------------------------------------------------------------------
// Sorts track's notes by their start time
void Track::sort() {
    std::sort(_notes.begin(), _notes.end(), sortNoteTime); // Sort notes by start time
}

//-----------------------------------------------------------------------------
// Adds notes to the and of track, shifting their start times
void Track::addNotes(NoteList& notes, double timeShift) {
    ::addNotes(notes, _notes, timeShift);
}

