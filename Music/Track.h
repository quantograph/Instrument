#pragma once

class Track {
public:
    NoteList _notes{}; // Track's notes
    String _instrumentName{""}; // Instrument name
    Instrument _instrument{NONE}; // MIDI instument ID
    String _trackName{""}; // Track's name
    uint8_t _trackNumber; // Track number
    float _volume{}; // Volume: 1.0f - full volume, 0.0f - silence
    String _name{""};
    uint32_t _midiProgram{0};	// Program number. 0-127 in the file, but 1-128 in the specifications
    float _pan{};
    int _channel{}; // MIDI channel

    Track();
    virtual ~Track();
    void show();
    void getAllNotes(NoteList* notes);
    void reset();
    void sort();
    void addNotes(NoteList& notes, double timeShift);
};

typedef std::list<Track*> TrackList;
