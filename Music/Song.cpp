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
Song::reset() {
    // Delete all track pointers
    for(auto track : _tracks) {
        delete track;
    }
    _tracks.clear();
}