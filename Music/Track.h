#ifndef Track_h
#define Track_h

class Track {
public:
    NoteList _notes{}; // Track's notes
    String _instrumentName{}; // Instrument name
    INSTRUMENT _instrument{PERCUSSION}; // MIDI instument ID
    uint8_t _trackNumber; // Track number
    float _volume{}; // Volume: 1.0f - full volume, 0.0f - silence
    String _name{};
    uint32_t _midiProgram{0};	// Program number. 0-127 in the file, but 1-128 in the specifications
    float _pan{};

    Track();
    virtual ~Track();
    void sortNotes();
    static bool sortNoteTime(Note note1, Note note2);
    void show();
};

typedef std::list<Track*> TrackList;

#endif // Track_h
