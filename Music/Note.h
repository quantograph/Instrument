#ifndef Note_h
#define Note_h

class Note {
public:
    uint8_t _midiNote{NO_MIDI_NOTE}; // MIDI note number
    float _volume{0.0}; // Volume: 1.0 - full volume, 0.0 - silence
    float _start{0.0}; // Start time, in seconds
    float _duration{0.0}; // Duration of the note, in seconds

    Note();
    virtual ~Note();
};

typedef std::list<Note> NoteList;

#endif // Note_h
