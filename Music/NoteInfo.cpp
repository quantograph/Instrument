// NoteInfo.cpp
// All functions for notes the their info

#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "NoteInfo.h"

//-----------------------------------------------------------------------------
// Constructor
NoteInfo::NoteInfo() {
    Init();
}

//-----------------------------------------------------------------------------
NoteInfo::~NoteInfo() {
}

//-----------------------------------------------------------------------------
// Initializes everything
int NoteInfo::Init() {
    int octave = 0; // Current octave
    int octaveNote = 0;// Current note in the octave
    Note note; // note info
    int midiNote = 0; // MIDI note number
    double shift;
    int base = -12 * 5 - 9; // 5 octaves below A4, then move to C.

    _count = 0;
    shift = 0.5 * pow(2.0, 1.0 / 12.0);
    for(octave = -1; octave < 10; octave++) {
        for(octaveNote = 0; octaveNote < 12; octaveNote++) {
            note._frequency = (float)(440.0 * pow(2.0, (double)base / 12.0));
            note._freqFrom = (float)(440.0 * pow(2.0, ((double)base - (1.0 - shift)) / 12.0));
            note._freqTo = (float)(440.0 * pow(2.0, ((double)base + shift) / 12.0));
            note._octave = octave;
            note._name = Note::_names[octaveNote];
            note._nameSharp = Note::_namesSharp[octaveNote];
            note._nameFlat = Note::_namesFlat[octaveNote];
            note._midiNote = midiNote;
            _notes.push_back(note);
            //note.Show();
            midiNote++;
            base++;
            _count++;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Gets note info by it's MIDI number
int NoteInfo::GetNoteInfo(int midiNote, Note* note) {
    Note* listNote; // note from the list of all notes

    if(midiNote == NO_MIDI_NOTE)
        return 0;

    // Check the note number
    if(midiNote < 0 || midiNote >= _count) {
        return -1;
    }

    // Get the note
    listNote = &(_notes[midiNote]);
    note->CopyTone(*listNote);

    return 0;
}
