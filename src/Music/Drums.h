// Drums.h

#pragma once

class Band;

class Drums {
    // Definitions ------------------------------------------------------------
public:

    // Data members -----------------------------------------------------------
public:
    Band* _band; // The band composer
    Settings* _settings; // Application settings

    // Functions --------------------------------------------------------------
public:
    Drums(Band* band);
    virtual ~Drums();
    int Make();
    int MakeMeasure(NoteList& notes);
    int MakeOne(NoteList& notes, InstrumentSettings& settings, int instrument, double pan);
    void Test();
};
