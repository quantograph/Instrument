#include "Misc.h"
#include "Note.h"
#include "Track.h"
#include <algorithm>

//-----------------------------------------------------------------------------
// Default constructor
Track::Track() {
}

//-----------------------------------------------------------------------------
Track::~Track() {
}

void Track::show() {
    // Show track info
    Serial.printf("Track %d '%s', intrument='%s', %d notes\n", 
                  _trackNumber, _name.c_str(), _instrumentName.c_str(), _notes.size());

    // Show all notes
    for(auto note : _notes) {
        note.show();
    }
}

void Track::sortNotes() {
    std::sort(_notes.begin(), _notes.end(), sortNoteTime);
}

bool Track::sortNoteTime(Note note1, Note note2) {
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
