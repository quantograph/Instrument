#ifndef Track_h
#define Track_h

class Track {
public:
public:
    NoteList _notes; // Track's notes
    String _trackName; // Track's name
    String _instrumentName; // Instrument name
    INSTRUMENT _instrument; // MIDI instument ID
    uint8_t _trackNumber; // Track number
    float _volume; // Volume: 1.0f - full volume, 0.0f - silence

    Track();
    virtual ~Track();
};

typedef std::list<Track*> TrackList;

#endif // Track_h
