#pragma once

class Song {
public:
    TrackList _tracks{}; // Song's tracks (pointers)
    String _name{""}; // Song's name
    float _songTime{};
    float _tempo{};
    float _beatTime{}; // Time for a quarter note, in seconds
    int16_t _measureBeats{}; // Time signature numerator - number of beats in one measure (3 for 3/4 song)
    int16_t _beatNotes{};
    int _beatNote{}; // Time signature denominator - note duration for one beat, 4 is for a quarter note (4 for 3/4 song)
    KEY_SIGNATURE _key{KEY_NONE}; // Key signature (flats / sharps)
    SCALE _scaleType{SCALE_NONE}; // Scale type
    Scale _scale{}; // Song's scale
    int _measures{}; // Number of measures in the song

    Song();
    virtual ~Song();
    void reset();
    void show();
    void getAllNotes(NoteList* notes, InstrumentList* instruments);
};
