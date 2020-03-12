#ifndef Player_h
#define Player_h

#include <list>
#include <Audio.h>
#include "../Devices/AudioBoard.h"
#include "Song.h"
#include "Synth.h"

class Player {
#define MAX_DRUMS 128

private:
    AudioBoard* _audio{nullptr};
    NoteList _notes;
    NoteListIter _iter;
    uint32_t _startTime{};
    std::list<AudioConnection*> _cords; // Connections from all instruments to the output mixers
    Synth _rhythm;
    AudioSynthWavetable* _drums[MAX_DRUMS]; // Map of drum notes to their instruments
    void reset();
    bool setupDrums();
    void testDrums();
    Synth* _instruments[MAX_INSTRUMENT]; // Synthesizers for all instruments (Tracks)

public:
    Player(AudioBoard* audio);
    virtual ~Player();
    bool init();
    bool play(Song* song);
    void process();
    void testMidiFile();
    bool playNote(Note* note);
};

#endif // Player_h
