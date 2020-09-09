#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "Track.h"
#include "Scale.h"
#include "Song.h"

//-----------------------------------------------------------------------------
// Default constructor
Song::Song() {
}

//-----------------------------------------------------------------------------
Song::~Song() {
    reset();
}

//-----------------------------------------------------------------------------
void Song::reset() {
    // Delete all track pointers
    for(auto track : _tracks) {
        delete track;
    }

    _tracks.clear();
}

//-----------------------------------------------------------------------------
void Song::show(bool showAll) {
    // Show song info
    Serial.printf("\n\n===== Song '%s', tempo=%d, time=%6.2f, beatTime=%6.2f, %d/%d\n",
                  _name.c_str(), _tempo, _songTime, _beatTime, _measureBeats, _beatNotes);

    // Show all tracks
    Serial.printf("%d tracks:\n", _tracks.size());
    for(auto track : _tracks) {
        Serial.printf("----------------------------\n");
        track->show(showAll);
    }

    Serial.printf("----------------------------\n");
}

void Song::getAllNotes(NoteList* notes, InstrumentList* instruments) {
    for(auto track : _tracks) {
        track->getAllNotes(notes);
        instruments->push_back(track->_instrument);
    }

    sortNotes(notes);

    // Show all notes
    Serial.printf("\n===== All notes:\n", _tracks.size());
    for(auto note : *notes) {
        note.show();
    }
    Serial.printf("\n");
}
