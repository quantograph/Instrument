// Bass.h

#pragma once

class Band;

class Bass {
    // Definitions ------------------------------------------------------------
public:

    // Data members -----------------------------------------------------------
public:
    Band* _band; // The band composer
    Settings* _settings; // Application settings

    // Functions --------------------------------------------------------------
public:
    Bass(Band* band);
    virtual ~Bass();
    int Make();
    int MakePattern(int count, NoteList& notes);
    int MakeMeasure(Chord& chord, NoteList& patternNotes, NoteList& measureNotes);
};
