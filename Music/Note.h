#pragma once

class Chord;

class Note {
public:
    // Note state
    enum STATE {
        STATE_NONE,         // Not set
        STATE_START,        // This note is a start point in a note chain
        STATE_STOP,         // This note is a stop point in a note chain
        STATE_CHAIN,        // This note is in the middle of a note chain
        STATE_SINGLE,       // This is a single note: the mouse button was pressed and released on the same note.
        STATE_DOWN,         // Mouse button is pressed
        STATE_UP,           // Mouse button is released
        STATE_MOVE,         // Mouse is moved with the button pressed
        STATE_DELETE,       // The note or chain is marked for deletion
        STATE_SIZE_LEFT,    // Resize the note to the left
        STATE_SIZE_RIGHT,   // Resize the note to the right
        STATE_CHORD_MARK,   // Mark note for chord type
        STATE_PART_MARK,    // Mark note for song part
        STATE_MEASURE_MARK, // Mark note for measure number
    };

    // Sharp/flat
    enum SHIFT {
        // ############ NOTE! Don't insert, only append ###########
        NONE,
        SHARP,
        FLAT
        // ############ NOTE! Don't insert, only append ###########
    };

    // Note type
    enum TYPE {
        // ############ NOTE! Don't insert, only append ###########
        NOTE, // Regular note
        CHORD, // Mark note for chord type
        MEASURE // Mark note for measure number
        // ############ NOTE! Don't insert, only append ###########
    };

    // Guitar tab info: string and fret number
    struct TabInfo {
        TabInfo() :
            _string(NO_INT),
            _fret(NO_INT),
            _shift(0),
            _position(NO_INT),
            _finger(NO_INT),
            _mode(MODE_NONE) {
        }

        enum MODE { // Playing mode
            MODE_NONE        = 0x00, // Not set
            MODE_BARRE       = 0x01, // Barre
            MODE_ROOT        = 0x02, // Root note of the chord
            MODE_STRUM_START = 0x04, // Start of a chord strum
            MODE_STRUM_END   = 0x08, // End of a chord strum
            MODE_UP          = 0x10, // Up stroke, down otherwise
        };

        // Data -----
        int     _string;      // Guitar string number, from high to low (by pitch)
        int     _fret;        // Guitar fret number, 0 - open string
        int     _shift;       // Shift for the finger position withing the fret, for several fingers withing the same fret on different strings
        int     _position;    // Guitar position - fret number for the root of first finger
        int     _finger;      // Finger
        MODE    _mode;        // Playing mode

        void reset() {
            _string = NO_INT;
            _fret = NO_INT;
            _shift = 0;
            _position = NO_INT;
            _finger = NO_INT;
            _mode = MODE_NONE;
        }
    };

    // Flags
    #define FLAG_LAST_NOTE (1<<0) // Last note in a song

    int _index{}; // Note's index in the array
    static const char* _names[]; // Note names
    static const char* _namesSharp[]; // Note names, wiht #
    static const char* _namesFlat[]; // Note names, with b
    uint8_t _midiNote{NO_MIDI_NOTE}; // MIDI note number
    double _frequency{0.0}; // Note's frequency
    double _freqFrom{0.0}; // Note's frequency range from
    double _freqTo{0.0}; // Note's frequency range to
    float _volume{0.0}; // Volume: 1.0 - full volume, 0.0 - silence
    float _start{0.0}; // Start time, in seconds
    float _duration{0.0}; // Duration of the note, in seconds
    STATE _state{STATE_NONE};
    TabInfo _tab{}; // String and fret for this note, for one particular notation
    TYPE _type{TYPE::NOTE}; // Note type
    uint16_t _channel{0}; // MIDI channel
    Instrument _instrument{Instrument::NONE}; // Instrument type
    int _scaleIndex{0}; // Index in the scale's list
    int _scaleInterval{0}; // Scale's interval
    int _chordInterval{0}; // Interval in a chord
    int _step{0}; // Number of semi-tones from the previous note
    int _rootOffset{0}; // Number of semi-tones from the scale's or chord's root note
    String _name{""}; // Note's name
    String _nameSharp{""}; // Note's name wtih #, if any
    String _nameFlat{""}; // Note's name wtih b, if any
    int _octave{0}; // Note's octave number, from -1 to 9
    SHIFT _showShift{SHIFT::NONE}; // Whether to show _nameSharp or _nameFlat
    SHIFT _shift{SHIFT::NONE}; // Sharp/flat
    int _measure{0}; // Measure (bar) number, for MEASURE note type
    int _flags{0}; // Bitmask of flags
    Chord* _chord{nullptr}; // Chord info, for CHORD note type

    Note();
    Note(const Note& source);
    virtual ~Note();
    void reset();
    void show(const char* title = nullptr);
    Note& operator = (const Note& note);
    const char* GetName();
    int SetNames();
    void CopyTone(Note note);
    void setRandDuration(int lengthFrom, int lengthTo, double beatTime, double measureTime);
};

typedef std::vector<Note> NoteList;
typedef NoteList::iterator NoteListIter;

void showNotes(const char* header, NoteList& notes, const char* noteTitle = "");
bool sortNoteTime(Note note1, Note note2);
bool sortNoteNumber(Note note1, Note note2);
void sortNotes(NoteList* notes);
void addNotes(NoteList& from, NoteList& to, double timeShift);
void shiftNotes(NoteList& notes, int shift);
double getNoteTime(double beatTime, int note);
