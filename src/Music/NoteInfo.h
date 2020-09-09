// NoteInfo.h

#pragma once

class NoteInfo {
public:
    // Definitions ------------------------------------------------------------

    // Data members -----------------------------------------------------------
public:
    NoteList _notes; // All notes
    int _count; // Count of all notes

    // Functions --------------------------------------------------------------
public:
    NoteInfo();
    virtual ~NoteInfo();
    int Init();
    int GetNoteInfo(int midiNote, Note* note);
};
