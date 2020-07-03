// Rhythm.h

#pragma once

class Band;

class Rhythm {
    // Definitions ------------------------------------------------------------
public:

    // Data members -----------------------------------------------------------
public:
    Band* _band; // The band composer
    Settings* _settings; // Application settings

    // Functions --------------------------------------------------------------
public:
    Rhythm(Band* band);
    virtual ~Rhythm();
    int Make();
    int MakePattern(NoteList& notes);
    int MakeMeasure(Chord& chord, NoteList& patternNotes, NoteList& measureNotes);
};
