#include "Misc.h"
#include "Note.h"
#include "Track.h"
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
void Song::show() {
    // Show song info
    Serial.printf("Song '%s', tempo=%d, time=%6.2f, beatTime=%6.2f, %d/%d\n",
                  _name.c_str(), _tempo, _songTime, _beatTime, _measureBeats, _beatNotes);

    // Show all tracks
    Serial.printf("%d tracks:\n", _tracks.size());
    for(auto track : _tracks) {
        Serial.printf("----------------------------\n");
        track->show();
    }

    Serial.printf("----------------------------\n");
}
