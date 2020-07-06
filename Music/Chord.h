// Chord.h

#pragma once

class Scale;

class Chord {
public:
    // Definitions ------------------------------------------------------------
    // Chord types - lists of intervals
    enum CHORD {
        // ############ NOTE! Don't insert, only append ###########
        CHORD_NONE,
        CHORD_1245,
        CHORD_125,
        CHORD_1257,
        CHORD_135,
        CHORD_1356,
        CHORD_13569,
        CHORD_1357,
        CHORD_13579,
        CHORD_13579_11,
        CHORD_13579_11_13,
        CHORD_1359,
        CHORD_145,
        CHORD_1457,
        CHORD_14579,
        CHORD_15,
        CHORD_179,
        // ############ NOTE! Don't insert, only append ###########
    };

    // Chord names
    #define CHORD_NAME_1245 "sus2sus4"
    #define CHORD_NAME_125 "sus2"
    #define CHORD_NAME_1257 "7sus2"
    #define CHORD_NAME_135 "Triad"
    #define CHORD_NAME_1356 "6"
    #define CHORD_NAME_13569 "6add9"
    #define CHORD_NAME_1357 "7"
    #define CHORD_NAME_13579 "9"
    #define CHORD_NAME_13579_11 "11"
    #define CHORD_NAME_13579_11_13 "13"
    #define CHORD_NAME_1359 "add9"
    #define CHORD_NAME_145 "sus4"
    #define CHORD_NAME_1457 "7sus4"
    #define CHORD_NAME_14579 "9sus4"
    #define CHORD_NAME_15 "5"
    #define CHORD_NAME_179 "9+5"

    // Chord progressions
    enum CHORD_PROG {
        // ############ NOTE! Don't insert, only append ###########
        CHORD_PROG_NONE, // Not set
        CHORD_PROG_5151,
        CHORD_PROG_2525,
        CHORD_PROG_2511,
        CHORD_PROG_1545,
        CHORD_PROG_1625,
        CHORD_PROG_1645,
        CHORD_PROG_1245,
        CHORD_PROG_1415,
        CHORD_PROG_1451,
        CHORD_PROG_1563,
        CHORD_PROG_5411,
        CHORD_PROG_1141,
        CHORD_PROG_1346,
        CHORD_PROG_1355,
        CHORD_PROG_1346_1355,
        CHORD_PROG_1346_1515,
        CHORD_PROG_BLUES1,
        CHORD_PROG_BLUES2,
        CHORD_PROG_GOOD, // "Good" random chords
        CHORD_PROG_RANDOM  // Totally random chords
        // ############ NOTE! Don't insert, only append ###########
    };

    // Chord progression names
    #define CHORD_PROG_NAME_5151 "5-1-5-1"
    #define CHORD_PROG_NAME_2525 "2-5-2-5"
    #define CHORD_PROG_NAME_2511 "2-5-1-1"
    #define CHORD_PROG_NAME_1545 "1-5-4-5"
    #define CHORD_PROG_NAME_1625 "1-6-2-5"
    #define CHORD_PROG_NAME_1645 "1-6-4-5"
    #define CHORD_PROG_NAME_1245 "1-2-4-5"
    #define CHORD_PROG_NAME_1415 "1-4-1-5"
    #define CHORD_PROG_NAME_1451 "1-6-5-1"
    #define CHORD_PROG_NAME_1563 "1-5-6-3"
    #define CHORD_PROG_NAME_5411 "5-4-1-1"
    #define CHORD_PROG_NAME_1141 "1-1-4-1"
    #define CHORD_PROG_NAME_1346 "1-3-4-6"
    #define CHORD_PROG_NAME_1355 "1-3-5-5"
    #define CHORD_PROG_NAME_1346_1355 "1-3-4-6 1-3-5-5"
    #define CHORD_PROG_NAME_1346_1515 "1-3-4-6 1-5-1-5"
    #define CHORD_PROG_NAME_BLUES1 "12-bar Blues 1"
    #define CHORD_PROG_NAME_BLUES2 "12-bar Blues 2"
    #define CHORD_PROG_NAME_GOOD "Generate"
    #define CHORD_PROG_NAME_RANDOM "Random"

    // Data members -----------------------------------------------------------
public:
    Note _root{}; // Root note
    NoteList _notes{}; // Chord notes
    CHORD _type{}; // Chord type
    String _name{""}; // Chord name

    // Functions --------------------------------------------------------------
public:
    Chord();
    Chord(const Chord& source);
    virtual ~Chord();
    void Reset();
    Chord& operator = (const Chord& source);
    void Show();
    int Make(Scale& scale, int startNote, int rootInterval, CHORD type);
    int SetScaleNote(int chordInterval, Note& scaleNote);
    void Strum(bool down, double start, double delay, double duration, double volume);
    void AddInterval(int interval);
    int GetIntervals(CHORD type);
    int SetName();
    int GetNoteNames(String& names);
    int GetFullName(String& name);
    int GetRoot(Note& note);
    static const char* GetChordProgName(CHORD_PROG prog);
    static int GetChordProgNumbers(CHORD_PROG prog, IntList& intervals);
    static int GetGoodProg(IntList& prog);
    static int GetRandomProg(IntList& prog);
    static int GetNextChord(int current);
    static int GetGoodChords(int current, IntList& intervals);
};

typedef std::vector<Chord> ChordList;
typedef ChordList::iterator ChordListIter;
