#ifndef MidiFile_h
#define MidiFile_h

#include "Misc.h"
#include "Note.h"
#include "Track.h"
#include "Song.h"

class MidiFile {
public:
    MidiFile();
    virtual ~MidiFile();

    // Definitions ------------------------------------------------------------
    #define	ID_FILE_HEADER "MThd"// File header
    #define	ID_TRACK_HEADER "MTrk"// Track header
    #define DRUM_CHANNEL 9 // Drum channel in MIDI

        // Marker types
    #define MARKER_CHORD "<Chord>"
    #define MARKER_SONG_PART "<SongPart>"
    #define MARKER_SONG_PART_INFO "<SongPartInfo>"
    #define MARKER_SONG_INFO "<SongInfo>"
    #define MARKER_NOTE_INFO "<NoteInfo>"
    #define MARKER_MEASURE "<Measure>"
    #define MARKER_MIX "<Mix>"

        // Market fields
    #define MARKER_FIELD_MIDI_NOTE "MidiNote="
    #define MARKER_FIELD_PITCH "Pitch="
    #define MARKER_FIELD_CHORD_TYPE "ChordType="
    #define MARKER_FIELD_CHORD_GROUP "ChordGroup="
    #define MARKER_FIELD_REPEAT "Repeat="
    #define MARKER_FIELD_SONG_LENGTH "SongLength="
    #define MARKER_FIELD_NUMBER "Number="
    #define MARKER_FIELD_SCALE "Scale="
    #define MARKER_FIELD_SCALE_ROOT "ScaleRoot="
    #define MARKER_FIELD_NAME "Name="
    #define MARKER_FIELD_INTERVAL "Interval="
    #define MARKER_FIELD_PART_MEASURE "PartMeasure="
    #define MARKER_FIELD_SONG_MEASURE "SongMeasure="
    #define MARKER_FIELD_DESCR "Description="
    #define MARKER_FIELD_STYLE "Style="
    #define MARKER_FIELD_GROOVE_NAME "GrooveName="
    #define MARKER_FIELD_GROOVE_FILE "GrooveFile="
    #define MARKER_FIELD_GROOVE_LENGTH "GrooveLength="
    #define MARKER_FIELD_PART_LENGTH "PartLength="
    #define MARKER_FIELD_INSTRUMENT "Instrument="
    #define MARKER_FIELD_VOLUME "Volume="
    #define MARKER_FIELD_PAN "Pan="
    #define MARKER_FIELD_TEMPO "Tempo="
    #define MARKER_FIELD_BEAT_TIME "BeatTime="
    #define MARKER_FIELD_MEASURES "Measures="
    #define MARKER_FIELD_BEAT_NOTES "BeatNotes="
    #define MARKER_FIELD_MEASURE_BEATS "MeasureBeats="
    #define MARKER_FIELD_STRING "String="
    #define MARKER_FIELD_FRET "Fret="
    #define MARKER_FIELD_FINGER "Finger="
    #define MARKER_FIELD_MODE "Mode="

    // Meta eventId types
    enum META_EVENT {
        META_SEQUENCE		= 0x00, // Sequence number
        META_TEXT			= 0x01, // Text eventId
        META_COPYRIGHT		= 0x02, // Copyright notice
        META_TRACK_NAME		= 0x03, // Sequence or track name
        META_INSTRUMENT		= 0x04, // Instrument name
        META_LYRICS			= 0x05, // Lyric text
        META_MARKER			= 0x06, // Marker text
        META_CUE			= 0x07, // Cue point
        META_CHANNEL		= 0x20, // MIDI channel prefix assignment
        META_END			= 0x2F, // End of track
        META_TEMPO			= 0x51, // Tempo setting
        META_OFFSET			= 0x54, // SMPTE offset
        META_TIME_SIGNATURE	= 0x58, // Time signature
        META_KEY_SIGNATURE	= 0x59, // Key signature
        META_SPECIFIC		= 0x7F, // Sequencer specific eventId
    };

    // MIDI events
    enum MIDI_EVENT {
        MIDI_NONE			= 0x00,	// Not set
        MIDI_NOTE_OFF		= 0x80, // Note Off
        MIDI_NOTE_ON		= 0x90, // Note on
        MIDI_NOTE_AFTER		= 0xA0, // Note aftertouch
        MIDI_CONTROL		= 0xB0, // Control Change
        MIDI_PROGRAM		= 0xC0, // Program change
        MIDI_CHANNEL_AFTER	= 0xD0, // Channel aftertouch
        MIDI_BEND			= 0xE0, // Pitch bend
        MIDI_SYSTEM			= 0xF0,	// System eventId
        MIDI_META			= 0xFF,	// Meta eventId
    };

    // MIDI program types
    enum {
        PROGRAM_NONE                = 255,  // Not set, all program numbers are 1-128
        PROGRAM_FINGER_BASS         = 33,   // Electric Bass (finger)
        PROGRAM_CLEAN_GUITAR        = 27,   // Electric Guitar (clean) - rhythm guitar
        PROGRAM_OVERDRIVEN_GUITAR   = 29,   // Overdriven Guitar - lead guitar
        PROGRAM_DISTORTION_GUITAR   = 30,   // Distortion Guitar - solo guitar
        PROGRAM_DRUM_SET            = 0,    // Drum set
    };

    // Control types
    enum CONTROL {
        CONTROL_VOLUME	    = 7,	// Volume
        CONTROL_PAN		    = 10,	// Pan
        CONTROL_TAB_STRING  = 16,   // Guitar tab - string number
        CONTROL_TAB_FRET    = 17,   // Guitar tab - fret number
        CONTROL_TAB_FINGER  = 18,   // Guitar tab - finger number
        CONTROL_TAB_MODE    = 19,   // Guitar tab - playing mode
        CONTROL_CHORD_TYPE  = 80,   // Chord type (see CHORD_TYPE)
        CONTROL_CHORD_ROOT  = 81,   // Chors's root note (MIDI note number)
        CONTROL_RESET       = 121,  // Channel Mode Message: Reset All Controllers
    };

    // Chunk header
    struct ChunkHeader {
        char _id[4]; // Chunk ID
        uint32_t _length{}; // Chunk length (4 bytes). Does not include this 8-byte chunk header.
    };

    // Chunk info
    struct ChunkInfo {
        ChunkHeader _header{}; // Chunk header
        char* _data{nullptr}; // Chunk data
    };

    // MIDI file header
    struct FileHeader {
        uint16_t _format; // MIDI format
        uint16_t _tracks; // Number of tracks
        uint16_t _division; // Time division
    };

    // Data members -----------------------------------------------------------
protected:
    char* _data{nullptr}; // File data
    char* _dataOffset{nullptr}; // Offset to the current data
    uint32_t _dataSize{}; // Data size
    uint32_t _dataRead{}; // Size of data read
    uint16_t _format{}; // MIDI format
    uint16_t _tracks{}; // Number of tracks in the file
    uint16_t _timeDivision{}; // Time division
    float _beatTime; // Note time
    std::vector<Note> _notes; // Notes of a track, which were started
    static float _volumeRate; // Logarithmic convertion rate
    Note _note{}; // Current note's info

    // Functions --------------------------------------------------------------
protected:
    float getBeatTime(float time);
    int32_t getTimeDiff(float dTimeDiff);
    float getTimeDiff(int32_t timeDiff);
    bool getChunks(Song* song);
    bool nextChunk(ChunkInfo& chunk, bool& done);
    bool getFileHeader(ChunkInfo& chunk, Song* song);
    bool getTrack(ChunkInfo& Chunk, Song* song, Track* track);
    bool getMetaEvent(char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Song* song, Track* track);
    bool getSystemEvent(char* data, uint32_t dataSize, uint32_t& processed);
    bool getMidiEvent(uint8_t eventId, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track);
    bool getNote(uint32_t channel, bool noteOn, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track);
    bool addNote(Note* pNote, float trackTime, Track* track);
    uint8_t getProgram(INSTRUMENT instrument);
    bool getProgram(uint32_t channel, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track);
    bool getControl(uint32_t channel, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track);
    bool resetNotes();
    bool PutVarLen(int32_t value, char* data, uint32_t dataSize, uint32_t& bytes);
    bool getVarLen(char* data, uint32_t dataSize, int32_t& value, uint32_t& processed);
    bool getProgramInfo(uint32_t program, String& name, INSTRUMENT& nInstrument);
    bool getControlInfo(uint32_t control, String& name);
    uint8_t getMidiDrumNote(INSTRUMENT instrument);
    bool getDrumInfo(uint32_t midiNote, String& name, INSTRUMENT& nInstrument);
    //bool GetMarker(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool GetChord(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool GetSongPart(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool GetMeasure(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool SaveHeader(Song* song);
    //bool SaveSongInfo(Song* song);
    //bool SaveAllTracks(Song* song);
    //bool SaveParts(Song* song, Buffer& sTrackData);
    //bool SaveTrack(const char* pTrackData, uint32_t nTrackLength);
    //bool SaveMetaEvent(uint32_t timeDiff, META_EVENT nMetaEvent, uint32_t channel, const char* eventData, uint32_t nEventDataSize, Buffer& sTrackData);
    //bool SaveControlEvent(uint32_t timeDiff, uint32_t channel, uint8_t type, uint8_t value, Buffer& sTrackData);
    //bool SaveNote(uint32_t timeDiff, bool bEVent, uint32_t channel, bool bOn, uint32_t midiNote, float volume, Buffer& sTrackData);
    //bool SaveTab(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool SaveMarker(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool SaveChord(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool SaveMix(uint32_t timeDiff, CMix* pMix, Buffer& sTrackData);
    //bool GetMix(const char* pMarker, Song* song);
    //bool SavePartInfo(uint32_t timeDiff, SongPart* pPart, Buffer& sTrackData);
    //bool SaveNoteInfo(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool GetPartInfo(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool GetNoteInfo(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool GetSongInfo(const char* pMarker, float trackTime, Song* song, Track* track);
    //bool SaveSongInfo(Song* song, Buffer& sTrackData);
    //bool SaveSongPart(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool SaveMeasure(uint32_t timeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool SaveEvent(uint32_t timeDiff, MIDI_EVENT eventId, uint32_t channel, const char* eventData, uint32_t nEventDataSize, Buffer& sTrackData);
    //bool SaveTrackInfo(Track* track, Buffer& sTrackData);
    //bool SaveTrackNotes(Track* track, Buffer& sTrackData);
    //bool Log(const char* pFormat, const char* pMessage = nullptr);
    //bool AddData(void* data, uint32_t size);

public:
    bool read(char* data, uint32_t size, Song* song);
    //bool SaveData(const char* pFile, Song* song);
    //bool SaveData(char*& data, uint32_t& dwDataSize, Song* song);
    static uint8_t VolumeToMidi(float volume);
    static float midiToVolume(uint8_t velocity);
};

#endif // MidiFile_h
