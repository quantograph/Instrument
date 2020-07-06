// Scale.h

#pragma once

class Chord;

class Scale {
public:
    // Definitions ------------------------------------------------------------

    // Data members -----------------------------------------------------------
public:
    NoteList _notes; // Scale notes
    Note _root; // Root note of the scale
    SCALE _type; // Chord type
    String _name{""}; // Scale name
    String _key{""}; // Scale's key name
    IntList _steps; // Half-tone steps for all notes

    // Functions --------------------------------------------------------------
public:
    Scale();
    Scale(const Scale& source);
    virtual ~Scale();
    Scale& operator = (const Scale& source);
    void Reset();
    void Show();
    int SetName();
    void SetKey();
    int GetNoteNames(String& names);
    int SetRoot();
    int GetSteps(IntList& steps);
    int Make(SCALE type, int root);
    int GetIntervalNote(Note& from, Note& to, int interval);
    int GetIndexNote(Note& from, Note& to, int index);
    int MoveToChordNote(Note& note, Chord& chord);
    int FindClosest(int inNote, int chordNote);
    int FindNote(int midiNote, Note& note);
    int FindRootOffset(int offset, Note& note);
};
