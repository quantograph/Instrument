#ifndef Note_h
#define Note_h

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

    uint8_t _midiNote{NO_MIDI_NOTE}; // MIDI note number
    float _volume{0.0}; // Volume: 1.0 - full volume, 0.0 - silence
    float _start{0.0}; // Start time, in seconds
    float _duration{0.0}; // Duration of the note, in seconds
    STATE _state{STATE_NONE};
    TabInfo _tab; // String and fret for this note, for one particular notation
    uint16_t _channel{0}; // MIDI channel
    INSTRUMENT _instrument{NONE}; // Instrument type

    Note();
    Note(const Note& note) { *this = note; }
    virtual ~Note();
    void reset();
    void show();
    Note& operator = (const Note& note);
};

typedef std::vector<Note> NoteList;
typedef NoteList::iterator NoteListIter;

bool sortNoteTime(Note note1, Note note2);
void sortNotes(NoteList* notes);

#endif // Note_h
