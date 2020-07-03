// Scale.cpp
// Musical scale

#include <algorithm>
#include "../Devices/Devices.h"
#include "MusicDef.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"

//-----------------------------------------------------------------------------
// Default constructor
Scale::Scale() : _type(SCALE_NONE) {
    Reset();
}

//-----------------------------------------------------------------------------
// Copy constructor
Scale::Scale(const Scale& source) {
    *this = source;
}

//-----------------------------------------------------------------------------
Scale::~Scale() {
}

//-----------------------------------------------------------------------------
// Copy operator
Scale& Scale::operator = (const Scale& source) {
    _notes = source._notes;
    _root = source._root;
    _type = source._type;
    _name = source._name;
    _key = source._key;
    _steps = source._steps;

    return *this;
}

//-----------------------------------------------------------------------------
// Resets everything
void Scale::Reset() {
    _notes.clear();
    _root.reset();
    _type = SCALE_NONE;
    _name = "";
    _key = "";
    _steps.clear();
}

//-----------------------------------------------------------------------------
// Prints scale info to the console
void Scale::Show() {
    Serial.printf("===== Scale '%s'\n", _name.c_str());

    Serial.printf("Root: %s", _name.c_str());
    //_root.Show();

    //ShowNotes("Scale", _notes);
}

//-----------------------------------------------------------------------------
// Sets scale's name by it's type
int Scale::SetName() {
     switch(_type) {
         case SCALE_CHROMATIC: _name = SCALE_NAME_CHROMATIC; break;
         case SCALE_MAJOR: _name = SCALE_NAME_MAJOR; break;
         case SCALE_IONIAN: _name = SCALE_NAME_IONIAN; break;
         case SCALE_DORIAN: _name = SCALE_NAME_DORIAN; break;
         case SCALE_PHRYGIAN: _name = SCALE_NAME_PHRYGIAN; break;
         case SCALE_LYDIAN: _name = SCALE_NAME_LYDIAN; break;
         case SCALE_MIXOLYDIAN: _name = SCALE_NAME_MIXOLYDIAN; break;
         case SCALE_AEOLIAN: _name = SCALE_NAME_AEOLIAN; break;
         case SCALE_LOCRIAN: _name = SCALE_NAME_LOCRIAN; break;
         case SCALE_NAT_MINOR: _name = SCALE_NAME_NATURAL_MINOR; break;
         case SCALE_HARM_MINOR: _name = SCALE_NAME_HARMONIC_MINOR; break;
         case SCALE_MEL_MINOR: _name = SCALE_NAME_MELODIC_MINOR; break;
         case SCALE_MEL_MINOR_DESC: _name = SCALE_NAME_MELODIC_MINOR_DESC; break;
         case SCALE_PENT_MAJOR: _name = SCALE_NAME_PENTATONIC_MAJOR; break;
         case SCALE_PENT_MINOR: _name = SCALE_NAME_PENTATONIC_MINOR; break;
         case SCALE_BLUES: _name = SCALE_NAME_BLUES; break;
         case SCALE_BEBOP: _name = SCALE_NAME_BEBOP; break;
         case SCALE_DIM: _name = SCALE_NAME_DIMINISHED; break;
         case SCALE_ENIGM: _name = SCALE_NAME_ENIGMATIC; break;
         case SCALE_NEAP: _name = SCALE_NAME_NEAPOLITANIAN; break;
         case SCALE_NEAP_MIN: _name = SCALE_NAME_NEAPOLITANIAN_MINOR; break;
         case SCALE_HUNG_MIN: _name = SCALE_NAME_HUNGARIAN_MINOR; break;
         case SCALE_C_MAJOR: _name = SCALE_NAME_C_MAJOR; break;
         case SCALE_A_MINOR: _name = SCALE_NAME_A_MINOR; break;
         case SCALE_G_MAJOR: _name = SCALE_NAME_G_MAJOR; break;
         case SCALE_E_MINOR: _name = SCALE_NAME_E_MINOR; break;
         case SCALE_D_MAJOR: _name = SCALE_NAME_D_MAJOR; break;
         case SCALE_B_MINOR: _name = SCALE_NAME_B_MINOR; break;
         case SCALE_A_MAJOR: _name = SCALE_NAME_A_MAJOR; break;
         case SCALE_Fs_MINOR: _name = SCALE_NAME_Fs_MINOR; break;
         case SCALE_E_MAJOR: _name = SCALE_NAME_E_MAJOR; break;
         case SCALE_Cs_MINOR: _name = SCALE_NAME_Cs_MINOR; break;
         case SCALE_B_MAJOR: _name = SCALE_NAME_B_MAJOR; break;
         case SCALE_Gs_MINOR: _name = SCALE_NAME_Gs_MINOR; break;
         case SCALE_Fs_MAJOR: _name = SCALE_NAME_Fs_MAJOR; break;
         case SCALE_Ds_MINOR: _name = SCALE_NAME_Ds_MINOR; break;
         case SCALE_Cs_MAJOR: _name = SCALE_NAME_Cs_MAJOR; break;
         case SCALE_As_MINOR: _name = SCALE_NAME_As_MINOR; break;
         case SCALE_F_MAJOR: _name = SCALE_NAME_F_MAJOR; break;
         case SCALE_D_MINOR: _name = SCALE_NAME_D_MINOR; break;
         case SCALE_Bb_MAJOR: _name = SCALE_NAME_Bb_MAJOR; break;
         case SCALE_G_MINOR: _name = SCALE_NAME_G_MINOR; break;
         case SCALE_Eb_MAJOR: _name = SCALE_NAME_Eb_MAJOR; break;
         case SCALE_C_MINOR: _name = SCALE_NAME_C_MINOR; break;
         case SCALE_Ab_MAJOR: _name = SCALE_NAME_Ab_MAJOR; break;
         case SCALE_F_MINOR: _name = SCALE_NAME_F_MINOR; break;
         case SCALE_Db_MAJOR: _name = SCALE_NAME_Db_MAJOR; break;
         case SCALE_Bb_MINOR: _name = SCALE_NAME_Bb_MINOR; break;
         case SCALE_Gb_MAJOR: _name = SCALE_NAME_Gb_MAJOR; break;
         case SCALE_Eb_MINOR: _name = SCALE_NAME_Eb_MINOR; break;
         case SCALE_Cb_MAJOR: _name = SCALE_NAME_Cb_MAJOR; break;
         case SCALE_Ab_MINOR: _name = SCALE_NAME_Ab_MINOR; break;
         default: _name = ""; return -1;
     }

     return 0;
 }

//-----------------------------------------------------------------------------
void Scale::SetKey() {
    switch(_type) {
        case SCALE_G_MAJOR:  _key = "1#"; break;
        case SCALE_E_MINOR:  _key = "1#"; break;
        case SCALE_D_MAJOR:  _key = "2#"; break;
        case SCALE_B_MINOR:  _key = "2#"; break;
        case SCALE_A_MAJOR:  _key = "3#"; break;
        case SCALE_Fs_MINOR: _key = "3#"; break;
        case SCALE_E_MAJOR:  _key = "4#"; break;
        case SCALE_Cs_MINOR: _key = "4#"; break;
        case SCALE_B_MAJOR:  _key = "5#"; break;
        case SCALE_Gs_MINOR: _key = "5#"; break;
        case SCALE_Fs_MAJOR: _key = "6#"; break;
        case SCALE_Ds_MINOR: _key = "6#"; break;
        case SCALE_Cs_MAJOR: _key = "7#"; break;
        case SCALE_As_MINOR: _key = "7#"; break;
        case SCALE_F_MAJOR:  _key = "1b"; break;
        case SCALE_D_MINOR:  _key = "1b"; break;
        case SCALE_Bb_MAJOR: _key = "2b"; break;
        case SCALE_G_MINOR:  _key = "2b"; break;
        case SCALE_Eb_MAJOR: _key = "3b"; break;
        case SCALE_C_MINOR:  _key = "3b"; break;
        case SCALE_Ab_MAJOR: _key = "4b"; break;
        case SCALE_F_MINOR:  _key = "4b"; break;
        case SCALE_Db_MAJOR: _key = "5b"; break;
        case SCALE_Bb_MINOR: _key = "5b"; break;
        case SCALE_Gb_MAJOR: _key = "6b"; break;
        case SCALE_Eb_MINOR: _key = "6b"; break;
        case SCALE_Cb_MAJOR: _key = "7b"; break;
        case SCALE_Ab_MINOR: _key = "7b"; break;
        default: _key = ""; // Not an error, not all scales have a key
    }
}

//-----------------------------------------------------------------------------
// Gets a string with scale's note names, for one octave, to show scales notes.
int Scale::GetNoteNames(String& names) {
    int i = 0;
    NoteListIter iter;
    Note* note;

    names = "";
    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        note = &(*iter);
        if(note->_midiNote < _root._midiNote || note->_midiNote > _root._midiNote + 11)
            continue;

        if(i > 0)
            names += ", ";

        names += note->GetName();
        //names += " " + note._midiNote + "+" + note._step;

        i++;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Sets the root note for major and minor scales
// 60 C, 61 C#/Db, 62 D, 63 D#/Eb, 64 E, 65 F, 66 F#/Gb, 67 G, 68 G#/Ab, 69 A, 70 A#/Bb, 71 B
int Scale::SetRoot() {
    switch(_type) {
        case SCALE_Cb_MAJOR: _root._midiNote = 59; _root._shift = Note::FLAT; break;
         case SCALE_C_MAJOR:
         case SCALE_C_MINOR: _root._midiNote = 60; _root._shift = Note::NONE; break;
         case SCALE_Cs_MAJOR:
         case SCALE_Cs_MINOR: _root._midiNote = 61; _root._shift = Note::SHARP; break;
         case SCALE_Db_MAJOR: _root._midiNote = 61; _root._shift = Note::FLAT; break;
         case SCALE_D_MAJOR:
         case SCALE_D_MINOR: _root._midiNote = 62; _root._shift = Note::NONE; break;
         case SCALE_Ds_MINOR: _root._midiNote = 63; _root._shift = Note::SHARP; break;
         case SCALE_Eb_MAJOR:
         case SCALE_Eb_MINOR: _root._midiNote = 63; _root._shift = Note::FLAT; break;
         case SCALE_E_MAJOR:
         case SCALE_E_MINOR: _root._midiNote = 64; _root._shift = Note::NONE; break;
         case SCALE_F_MAJOR:
         case SCALE_F_MINOR: _root._midiNote = 65; _root._shift = Note::NONE; break;
         case SCALE_Fs_MAJOR:
         case SCALE_Fs_MINOR: _root._midiNote = 66; _root._shift = Note::SHARP; break;
         case SCALE_Gb_MAJOR: _root._midiNote = 66; _root._shift = Note::FLAT; break;
         case SCALE_G_MAJOR:
         case SCALE_G_MINOR: _root._midiNote = 67; _root._shift = Note::NONE; break;
         case SCALE_Gs_MINOR: _root._midiNote = 68; _root._shift = Note::SHARP; break;
         case SCALE_Ab_MAJOR:
         case SCALE_Ab_MINOR: _root._midiNote = 68; _root._shift = Note::FLAT; break;
         case SCALE_A_MAJOR:
         case SCALE_A_MINOR: _root._midiNote = 69; _root._shift = Note::NONE; break;
         case SCALE_As_MINOR: _root._midiNote = 70; _root._shift = Note::SHARP; break;
         case SCALE_Bb_MAJOR:
         case SCALE_Bb_MINOR: _root._midiNote = 70; _root._shift = Note::FLAT; break;
         case SCALE_B_MAJOR:
         case SCALE_B_MINOR: _root._midiNote = 71; _root._shift = Note::NONE; break;
         default: _root._shift = Note::NONE; return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Makes the scale
int Scale::Make(SCALE type, int root) {
    Note note;
    Note* listNote;
    int start;
    int offset;
    int interval;
    int index;
    int step;
    int midiNote;
    IntListIter intIter;
    NoteListIter noteIter;

    _notes.clear();

    _type = type;
    _root._midiNote = root;
    SetRoot(); // Set root note, if the scale's type defines it
    SetName();
    SetKey();
    GetSteps(_steps);

    // Above root
    start = _root._midiNote;
    while(start <= 127) {
        interval = 1;
        offset = 0;

        // Root
        note._midiNote = start;
        note._scaleInterval = interval;
        note._step = 0;
        note._rootOffset = offset;
        _notes.push_back(note);

        for(intIter = _steps.begin(); intIter != _steps.end(); intIter++) {
            step = *intIter;
            offset += step;
            interval++;
            midiNote = start + offset;
            if(midiNote > 127)
                break;

            note._midiNote = midiNote;
            note._step = step;
            note._rootOffset = offset;
            note._scaleInterval = interval;
            _notes.push_back(note);
        }

        start += 12;
    }

    // Below root
    start = _root._midiNote - 12;
    while(start >= 0) {
        interval = 1;
        offset = 0;

        // Root
        note._midiNote = start;
        note._scaleInterval = interval;
        note._step = 0;
        note._rootOffset = offset;
        _notes.push_back(note);

        for(intIter = _steps.begin(); intIter != _steps.end(); intIter++) {
            step = *intIter;
            offset += step;
            interval++;
            midiNote = start + offset;
            if(midiNote < 0)
                break;

            note._midiNote = midiNote;
            note._step = step;
            note._rootOffset = offset;
            note._scaleInterval = interval;
            _notes.push_back(note);
        }

        start -= 12;
    }

    std::sort(_notes.begin(), _notes.end(), sortNoteNumber); // Sort notes by MIDI number

    // Set all note parameters
    index = 0;
    for(noteIter = _notes.begin(); noteIter != _notes.end(); noteIter++) {
        listNote = &(*noteIter);
        if(_type >= SCALE_G_MAJOR && _type <= SCALE_As_MINOR) // Sharps
            listNote->_showShift = Note::SHARP;
        if(_type >= SCALE_F_MAJOR && _type <= SCALE_Ab_MINOR) // Flats
            listNote->_showShift = Note::FLAT;
        else
            listNote->_showShift = Note::NONE;

        listNote->SetNames();
        listNote->_scaleIndex = index;
        index++;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Gets scale steps in half-tones
int Scale::GetSteps(IntList& steps) {
    switch(_type) {
        case SCALE_C_MAJOR:
        case SCALE_G_MAJOR:
        case SCALE_D_MAJOR:
        case SCALE_A_MAJOR:
        case SCALE_E_MAJOR:
        case SCALE_B_MAJOR:
        case SCALE_Fs_MAJOR:
        case SCALE_Cs_MAJOR:
        case SCALE_F_MAJOR:
        case SCALE_Bb_MAJOR:
        case SCALE_Eb_MAJOR:
        case SCALE_Ab_MAJOR:
        case SCALE_Db_MAJOR:
        case SCALE_Gb_MAJOR:
        case SCALE_Cb_MAJOR: {
            int add[] = { 2, 2, 1, 2, 2, 2 }; // Major scale
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_A_MINOR:
        case SCALE_E_MINOR:
        case SCALE_B_MINOR:
        case SCALE_Fs_MINOR:
        case SCALE_Cs_MINOR:
        case SCALE_Gs_MINOR:
        case SCALE_Ds_MINOR:
        case SCALE_As_MINOR:
        case SCALE_D_MINOR:
        case SCALE_G_MINOR:
        case SCALE_C_MINOR:
        case SCALE_F_MINOR:
        case SCALE_Bb_MINOR:
        case SCALE_Eb_MINOR:
        case SCALE_Ab_MINOR: {
            int add[] = { 2, 1, 2, 2, 1, 2 }; // Minor scale
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_CHROMATIC: { // Chromatic
            int add[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_MAJOR: { // Major
            int add[] = { 2, 2, 1, 2, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_IONIAN: { // Ionian mode of the major scale, starts at it's 1 step
            int add[] = { 2, 2, 1, 2, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_DORIAN: { // Dorian mode of the major scale, starts at it's 2 step
            int add[] = { 2, 1, 2, 2, 2, 1 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_PHRYGIAN: { // Phrygian mode of the major scale, starts at it's 3 step
            int add[] = { 1, 2, 2, 2, 1, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_LYDIAN: { // Lydian mode of the major scale, starts at it's 4 step
            int add[] = { 2, 2, 2, 1, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_MIXOLYDIAN: { // Mixolydian mode of the major scale, starts at it's 5 step
            int add[] = { 2, 2, 1, 2, 2, 1 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_AEOLIAN: { // Aeolian mode of the major scale, starts at it's 6 step
            int add[] = { 2, 1, 2, 2, 1, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_LOCRIAN: { // Locrian mode of the major scale, starts at it's 7 step
            int add[] = { 1, 2, 2, 1, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_NAT_MINOR: { // Natural minor
            int add[] = { 2, 1, 2, 2, 1, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_HARM_MINOR: { // Harmonic minor
            int add[] = { 2, 1, 2, 2, 1, 3 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_MEL_MINOR: { // Melodic minor (ascending)
            int add[] = { 2, 1, 2, 2, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_MEL_MINOR_DESC: { // Melodic minor (descending), same as natural minor
            int add[] = { 2, 1, 2, 2, 1, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_PENT_MAJOR: { // Pentatonic major
            int add[] = { 2, 2, 3, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_PENT_MINOR: { // Pentatonic minor
            int add[] = { 3, 2, 2, 3 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_BLUES: { // Blues
            int add[] = { 3, 2, 1, 1, 3 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_BEBOP: { // Bebop
            int add[] = { 2, 2, 1, 2, 2, 1, 1 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_DIM: { // Diminished
            int add[] = { 2, 1, 2, 1, 2, 1, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_ENIGM: { // Enigmatic
            int add[] = { 1, 3, 2, 2, 2, 1 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_NEAP: { // Neapolitanian
            int add[] = { 1, 2, 2, 2, 2, 2 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_NEAP_MIN: { // Neapolitanian minor
            int add[] = { 1, 2, 2, 2, 1, 3 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        case SCALE_HUNG_MIN: { // Hungarian minor
            int add[] = { 2, 1, 3, 1, 1, 3 };
            addIntList(steps, add, sizeof(add) / sizeof(int));
            break;
        }

        default: // Unknown scale
            return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Gets a note which stands away from the 'from' note by the given intervals number
int Scale::GetIntervalNote(Note& from, Note& to, int interval) {
    NoteListIter iter;
    Note* note;
    int pos = 0;

    if(interval == 0) {
        to.CopyTone(from);
        return 0;
    }


    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        note = &(*iter);

        // Find the 'from' note
        if(note->_midiNote >= from._midiNote)
            break;

        pos++;
    }

    pos += interval;
    if(pos >= 0) {
        to.CopyTone(_notes[pos]);
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Gets a note which stands away from the 'from' note by the given index number
int Scale::GetIndexNote(Note& from, Note& to, int index) {
    Note note;
    int pos = from._scaleIndex + index;

    if(index == 0 || pos < 0 || pos > (int)_notes.size() - 1) {
        to.CopyTone(from);
        return -1;
    }

    note = _notes[pos];
    to.CopyTone(note);

    return 0;
}

//-----------------------------------------------------------------------------
// Moves the note to the closest one to the chord's notes
int Scale::MoveToChordNote(Note& note, Chord& chord) {
    Note scaleNote;
    Note* chordNote;
    NoteListIter iter;
    int min = -1;
    int toNote = -1;
    int dist;
    int retNote = note._midiNote;
    int i = 0;

    for(iter = chord._notes.begin(); iter != chord._notes.end(); iter++) {
        chordNote = &(*iter);
        toNote = FindClosest(note._midiNote, chordNote->_midiNote);
        dist = abs(note._midiNote - toNote);
        if(min == -1 || dist < min) {
            retNote = toNote;
            min = dist;
        }

        i++;
        if(i == 3) { // Move to one of the first 3 notes of the chord
            break;
        }
    }

    // Find this note in the scale and get it's tone info
    FindNote(retNote, scaleNote);
    note.CopyTone(scaleNote);

    return -1; // Not found
}

//-----------------------------------------------------------------------------
// Finds closest note to the given one, in all octaves
// Returns the closest note
int Scale::FindClosest(int inNote, int chordNote) {
    int min = -1;
    int dist;
    int outNote = inNote;

    outNote = inNote;
    for(int i = chordNote % 12; i <= 127; i +=12) {
        dist = abs(i - inNote);
        if(min == -1 || dist < min) {
            min = dist;
            outNote = i;
        }
    }

    return outNote;
}

//-----------------------------------------------------------------------------
// Finds note by it's midi number
int Scale::FindNote(int midiNote, Note& note) {
    NoteListIter iter;
    Note* scaleNote;

    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        scaleNote = &(*iter);
        if(scaleNote->_midiNote == midiNote) {
            note = *scaleNote;
            return 0;
        }
    }

    return -1; // Not found
}

//-----------------------------------------------------------------------------
// Finds note with give offset from the root
int Scale::FindRootOffset(int offset, Note& note) {
    NoteListIter iter;

    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        if(note._rootOffset == offset)
            return 0;
    }

    return -1;
}
