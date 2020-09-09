// Band.h

#pragma once

class Lead;
class Rhythm;
class Bass;
class Drums;

class Band {
public:
    // Definitions ------------------------------------------------------------

    // Data members -----------------------------------------------------------
public:
    Settings* _settings; // Application settings
    Track _inTrack; // Track with notes defining the new song
    Song _outSong; // The composed song
    double _measureTime; // Duration of one measure, in seconds
    double _noteTime; // Time of one note in the time signature (1/4 or 1/8)
    Note _minNote; // Minimum note number in the song's range
    Note _maxNote; // Maximum note number in the song's range
    IntList _chordProg; // Chord progressions
    Lead* _lead; // Composer for lead track
    Rhythm* _rhythm; // Composer for rhythm track
    Bass* _bass; // Composer for bass track
    Drums* _drums; // Composer for drums track

    // Functions --------------------------------------------------------------
public:
    Band(Settings* settings);
    virtual ~Band();
    void Reset();
    void test();
    bool Make();
    bool MakeChords();
};
