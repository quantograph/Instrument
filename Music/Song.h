#ifndef Song_h
#define Song_h

class Song {
public:
    TrackList _tracks; // Song's tracks (pointers)
    String _name; // Song's name
    float _songTime{};
    float _tempo{};
    float _beatTime{}; // Time for a quarter note, in seconds
    int16_t _measureBeats{}; // Time signature numerator - number of beats in one measure (3 for 3/4 song)
    int16_t _beatNotes{};
    KEY_SIGNATURE _key{KEY_NONE}; // Key signature (flats / sharps)
    SCALE _scaleType{SCALE_NONE}; // Scale type

    Song();
    virtual ~Song();
    void reset();
    void show();
    void getAllNotes(NoteList* notes, InstrumentList* instruments);
};

#endif // Song_h
