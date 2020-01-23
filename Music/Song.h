#ifndef Song_h
#define Song_h

class Song {
public:
    TrackList _tracks; // Song's tracks (pointers)
    String _name; // Song's name
    int _tempo; // Tempo, quarter notes per minute
    double _beatTime; // Time for a quarter note, in seconds
    int _measureBeats; // Time signature numerator - number of beats in one measure (3 for 3/4 song)
    int _beatNote; // Time signature denominator - note duration for one beat, 4 is for a quarter note (4 for 3/4 song)

    Song();
    virtual ~Song();
    void reset();
};

#endif // Song_h
