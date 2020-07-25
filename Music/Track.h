#pragma once

class Track {
public:
    NoteList _notes{}; // Track's notes
    String _instrumentName{""}; // Instrument name
    Instrument _instrument{NONE}; // MIDI instument ID
    String _name{""}; // Track's name
    uint8_t _trackNumber{0}; // Track number
    float _volume{1.0}; // Volume: 1.0f - full volume, 0.0f - silence
    uint32_t _midiProgram{0};	// Program number. 0-127 in the file, but 1-128 in the specifications
    float _pan{0.0};
    int _channel{0}; // MIDI channel

    Track();
    Track(const Track& source);
    Track& operator = (const Track& source);
    virtual ~Track();
    void show(bool showAll = true);
    void getAllNotes(NoteList* notes);
    void reset();
    void sort();
    void addNotes(NoteList& notes, double timeShift);
};

typedef std::list<Track*> TrackList;
