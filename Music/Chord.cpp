// Chord.cpp
// Music chord

#include "../Devices/Devices.h"
#include "Misc.h"
#include "Note.h"
#include "Chord.h"
#include "Scale.h"

//-----------------------------------------------------------------------------
// Default constructor
Chord::Chord() {
    Reset();
}

//-----------------------------------------------------------------------------
// Copy constructor
Chord::Chord(const Chord& source) {
    *this = source;
}

//-----------------------------------------------------------------------------
Chord::~Chord() {
}

//-----------------------------------------------------------------------------
// Resets everything
void Chord::Reset() {
    _root.reset();
    _notes.clear();
    _type = CHORD_NONE;
    _name = "";
}

//-----------------------------------------------------------------------------
void Chord::Show() {
    //Serial.printf("Chord '%s', ", (const char*)_name);
    //ShowNotes("chord notes", _notes);
}

//-----------------------------------------------------------------------------
// Copy operator
Chord& Chord::operator = (const Chord& source) {
    _root = source._root;
    _notes = source._notes;
    _type = source._type;
    _name = source._name;

    return *this;
}

//=============================================================================================
// Gets chord's notes
// scale - scale for the chord
// startNote - MIDI number of the lowest note in the scale from which to start looking for the interval
// rootInterval - scale's interval number for the chord root
// type - chord type
int Chord::Make(Scale& scale, int startNote, int rootInterval, CHORD type) {
    Note* scaleNote;
    NoteListIter iter;
    bool start = false; // Found the starting note in the scale for this chord
    int chordInterval = 1;
    int ret;

    Reset();

    _type = type;
    SetName();

    // Get chord's intervals
    ret = GetIntervals(type);
    if(ret)
        return ret;

    // Find chord's notes in the scale
    for(iter = scale._notes.begin(); iter != scale._notes.end(); iter++) {
        scaleNote = &(*iter);
        if(scaleNote->_midiNote < startNote) // Ignore root intervals before this note
            continue;

        // Find chord's number - root note's interval number in the scale
        if(scaleNote->_scaleInterval == rootInterval)
            start = true;

        if(start) {
            SetScaleNote(chordInterval, *scaleNote);

            // Stop after 14 intervals
            chordInterval++;
            if(chordInterval > 14)
                break;
        }
    }

    _root = _notes[0];

    return 0;
}

//=============================================================================================
// Copies scale's note into chord's interval note
// number - interval number for the chord note
int Chord::SetScaleNote(int chordInterval, Note& scaleNote) {
    Note* chordNote;
    NoteListIter iter;

    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        chordNote = &(*iter);
        if(chordNote->_chordInterval == chordInterval) { // There is a chord note with this scale interval
            *chordNote = scaleNote;
            chordNote->_chordInterval = chordInterval; // Keep the chord's interval
            return 0;
        }
    }

    return -1;
}

//=============================================================================================
// Changes chord's notes to sound like a guitar strum
// down - strum up or down
// start - chord's start time
// delay - delay between notes start
// duration - duration of the chord
// volume - chor's volume
void Chord::Strum(bool down, double start, double delay, double duration, double volume) {
    Note* note;
    NoteListIter iter;
    double noteStart;

    if(down)
        noteStart = start;
    else
        noteStart = start + delay * (_notes.size() - 1);

    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        note = &(*iter);
        note->_start = noteStart;
        note->_duration = duration - (noteStart - start);
        note->_volume = volume;

        if(down)
            noteStart += delay;
        else
            noteStart -= delay;
    }
}

//=============================================================================================
// Adds a new interval note
void Chord::AddInterval(int interval) {
    Note note;

    note._chordInterval = interval;
    _notes.push_back(note);
}

//=================================================================================================
// Gets a list of intervals for a chord type
int Chord::GetIntervals(CHORD type) {
    AddInterval(1);
    switch(type) {
        case CHORD_1245:
            AddInterval(2);
            AddInterval(4);
            AddInterval(5);
            break;
        case CHORD_125:
            AddInterval(2);
            AddInterval(5);
            break;
        case CHORD_1257:
            AddInterval(2);
            AddInterval(5);
            AddInterval(7);
            break;
        case CHORD_135:
            AddInterval(3);
            AddInterval(5);
            break;
        case CHORD_1356:
            AddInterval(3);
            AddInterval(5);
            AddInterval(6);
            break;
        case CHORD_13569:
            AddInterval(3);
            AddInterval(5);
            AddInterval(6);
            AddInterval(9);
            break;
        case CHORD_1357:
            AddInterval(3);
            AddInterval(5);
            AddInterval(7);
            break;
        case CHORD_13579:
            AddInterval(3);
            AddInterval(5);
            AddInterval(7);
            AddInterval(9);
            break;
        case CHORD_13579_11:
            AddInterval(3);
            AddInterval(5);
            AddInterval(7);
            AddInterval(9);
            AddInterval(11);
            break;
        case CHORD_13579_11_13:
            AddInterval(3);
            AddInterval(5);
            AddInterval(7);
            AddInterval(9);
            AddInterval(11);
            AddInterval(13);
            break;
        case CHORD_1359:
            AddInterval(3);
            AddInterval(5);
            AddInterval(9);
            break;
        case CHORD_145:
            AddInterval(4);
            AddInterval(5);
            break;
        case CHORD_1457:
            AddInterval(4);
            AddInterval(5);
            AddInterval(7);
            break;
        case CHORD_14579:
            AddInterval(4);
            AddInterval(5);
            AddInterval(7);
            AddInterval(9);
            break;
        case CHORD_15:
            AddInterval(5);
            break;
        case CHORD_179:
            AddInterval(7);
            AddInterval(9);
            break;
        default:
            return -1;
    }

    return 0;
}

//=================================================================================================
// Sets chord's name
int Chord::SetName() {
    switch(_type) {
        case CHORD_1245:
            _name = CHORD_NAME_1245;
            break;
        case CHORD_125:
            _name = CHORD_NAME_125;
            break;
        case CHORD_1257:
            _name = CHORD_NAME_1257;
            break;
        case CHORD_135:
            _name = CHORD_NAME_135;
            break;
        case CHORD_1356:
            _name = CHORD_NAME_1356;
            break;
        case CHORD_13569:
            _name = CHORD_NAME_13569;
            break;
        case CHORD_1357:
            _name = CHORD_NAME_1357;
            break;
        case CHORD_13579:
            _name = CHORD_NAME_13579;
            break;
        case CHORD_13579_11:
            _name = CHORD_NAME_13579_11;
            break;
        case CHORD_13579_11_13:
            _name = CHORD_NAME_13579_11_13;
            break;
        case CHORD_1359:
            _name = CHORD_NAME_1359;
            break;
        case CHORD_145:
            _name = CHORD_NAME_145;
            break;
        case CHORD_1457:
            _name = CHORD_NAME_1457;
            break;
        case CHORD_14579:
            _name = CHORD_NAME_14579;
            break;
        case CHORD_15:
            _name = CHORD_NAME_15;
            break;
        case CHORD_179:
            _name = CHORD_NAME_179;
            break;
        default:
            _name = "";
            return -1;
    }

    return 0;
}

//=============================================================================================
// Gets a string with chord's note names
int Chord::GetNoteNames(String& names) {
    int i = 0;
    Note* chordNote;
    NoteListIter iter;
    char buffer[1024];

    for(iter = _notes.begin(); iter != _notes.end(); iter++) {
        chordNote = &(*iter);

        if(i > 0)
            names += ", ";

        names += chordNote->GetName();
        sprintf(buffer, "(%d)", chordNote->_chordInterval);
        names += buffer;

        i++;
    }

    return 0;
}

//=============================================================================================
// Gets full name of the chord: root note and chord type
int Chord::GetFullName(String& name) {
    Note root = _notes[0];

    name = root._name;
    name += " ";
    name += _name;

    return 0;
}

//=============================================================================================
// Gets the root note
int Chord::GetRoot(Note& note) {
    note = _notes[0];

    return 0;
}

//=================================================================================================
// Gets chord progression name from it's number
const char* Chord::GetChordProgName(CHORD_PROG prog) {
    switch(prog) {
        case CHORD_PROG_NONE:
            return "None";
        case CHORD_PROG_5151:
            return CHORD_PROG_NAME_5151;
        case CHORD_PROG_2525:
            return CHORD_PROG_NAME_2525;
        case CHORD_PROG_2511:
            return CHORD_PROG_NAME_2511;
        case CHORD_PROG_1545:
            return CHORD_PROG_NAME_1545;
        case CHORD_PROG_1625:
            return CHORD_PROG_NAME_1625;
        case CHORD_PROG_1645:
            return CHORD_PROG_NAME_1645;
        case CHORD_PROG_1245:
            return CHORD_PROG_NAME_1245;
        case CHORD_PROG_1415:
            return CHORD_PROG_NAME_1415;
        case CHORD_PROG_1451:
            return CHORD_PROG_NAME_1451;
        case CHORD_PROG_1563:
            return CHORD_PROG_NAME_1563;
        case CHORD_PROG_5411:
            return CHORD_PROG_NAME_5411;
        case CHORD_PROG_1141:
            return CHORD_PROG_NAME_1141;
        case CHORD_PROG_1346:
            return CHORD_PROG_NAME_1346;
        case CHORD_PROG_1355:
            return CHORD_PROG_NAME_1355;
        case CHORD_PROG_1346_1355:
            return CHORD_PROG_NAME_1346_1355;
        case CHORD_PROG_1346_1515:
            return CHORD_PROG_NAME_1346_1515;
        case CHORD_PROG_BLUES1:
            return CHORD_PROG_NAME_BLUES1;
        case CHORD_PROG_BLUES2:
            return CHORD_PROG_NAME_BLUES2;
        case CHORD_PROG_GOOD:
            return CHORD_PROG_NAME_GOOD;
        case CHORD_PROG_RANDOM:
            return CHORD_PROG_NAME_RANDOM;
        default:
            return "";
    }
}

//=================================================================================================
//Gets chord progression's intervals
int Chord::GetChordProgNumbers(CHORD_PROG prog, IntList& intervals) {
    switch(prog) {
        case CHORD_PROG_1545: {
            int add[] = { 1, 5, 4, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1625: {
            int add[] = { 1, 6, 2, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1645: {
            int add[] = { 1, 6, 4, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1245: {
            int add[] = { 1, 2, 4, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_5151: {
            int add[] = { 5, 1, 5, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_2525: {
            int add[] = { 2, 5, 2, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_2511: {
            int add[] = { 2, 5, 1, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1415: {
            int add[] = { 1, 4, 1, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1451: {
            int add[] = { 1, 4, 5, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1563: {
            int add[] = { 1, 5, 6, 3 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_5411: {
            int add[] = { 5, 4, 1, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1141: {
            int add[] = { 1, 1, 4, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1346: {
            int add[] = { 1, 3, 4, 6 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1355: {
            int add[] = { 1, 3, 5, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1346_1355: {
            int add[] = { 1, 3, 4, 6, 1, 3, 5, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_1346_1515: {
            int add[] = { 1, 3, 4, 6, 1, 5, 1, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_BLUES1: {
            int add[] = { 1, 4, 1, 1, 4, 4, 1, 1, 5, 5, 1, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_BLUES2: {
            int add[] = { 1, 4, 1, 1, 4, 4, 1, 1, 2, 5, 1, 2 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case CHORD_PROG_GOOD: {
            GetGoodProg(intervals);
            break; }

        case CHORD_PROG_RANDOM: {
            GetRandomProg(intervals);
            break; }

        default: {
            intervals.clear();
            break; }
    }

    return 0;
}

//=============================================================================================
int Chord::GetGoodProg(IntList& prog) {
    int curr;
    int next;

    curr = random(1, 6);
    prog.push_back(curr);

    for(int i = 0; i < 3; i++) {
        next = GetNextChord(curr);
        prog.push_back(next);
        curr = next;
    }

    return 0;
}

//=============================================================================================
int Chord::GetRandomProg(IntList& prog) {
    for(int i = 0; i < 4; i++)
        prog.push_back(random(1, 6));

    return 0;
}

//=============================================================================================
// Gets next chord for the given chord number in a chord progression.
// current - current chord's interval
int Chord::GetNextChord(int current) {
    IntList prog;
    int next;
    int index;

    GetGoodChords(current, prog);
    index = random(0, (int)prog.size() - 1);
    next = prog[index];

    return next;
}

//=============================================================================================
// Gets next chord for the given chord number in a chord progression.
// current - current chord's interval
int Chord::GetGoodChords(int current, IntList& intervals) {
    switch(current) {
        case 1: { // 1 leads to any chord
            int add[] = { 2, 3, 4, 5, 6 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 2: {
            int add[] = { 4, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 3: {
            int add[] = { 2, 4, 6 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 4: {
            int add[] = { 1, 3, 5 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 5: {
            int add[] = { 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 6: {
            int add[] = { 2, 4, 5, 1 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        case 7: {
            int add[] = { 1, 3 };
            addIntList(intervals, add, sizeof(add) / sizeof(int));
            break; }

        default: {
            intervals.clear(); }
    }

    return 0;
}
