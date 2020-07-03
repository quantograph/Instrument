// Lead.h

#pragma once

class Band;

class Lead {
    // Definitions ------------------------------------------------------------
public:

    // Data members -----------------------------------------------------------
public:
    Band* _band; // The band composer
    Settings* _settings; // Application settings

    // Functions --------------------------------------------------------------
public:
    Lead(Band* band);
    virtual ~Lead();
    int Make();
    int MakeMeasure(Chord& chord, Note& lastNote, NoteList& notes, NoteList& prevNotes);
    int RepeatRhythm(NoteList& notes, NoteList& prevNotes);
    int MakeRhythm(NoteList& notes);
    int RepeatMelody(Chord& chord, Note& lastNote, NoteList& notes, NoteList& prevNotes);
    int MakeMelody(NoteList& notes, Note& lastNote);
    int CheckRange(Note& note, Note& lastNote);
    int MoveToRange(Note& note);
};
