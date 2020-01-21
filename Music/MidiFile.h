// MidiFile.h

#ifndef MUSIC_MIDI_FILE_H
#define MUSIC_MIDI_FILE_H
namespace QuantoGraph {

class MidiFile : public File {
public:
    MidiFile();
    virtual ~MidiFile(void);

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

    // Meta event types
    enum META_EVENT {
        META_SEQUENCE		= 0x00, // Sequence number
        META_TEXT			= 0x01, // Text event
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
        META_SPECIFIC		= 0x7F, // Sequencer specific event
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
        MIDI_SYSTEM			= 0xF0,	// System event
        MIDI_META			= 0xFF,	// Meta event
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
    struct CHUNK_HEADER {
        char _id[4]; // Chunk ID
        int	_length; // Chunk length. Does not include the 8 byte chunk header.
    };

    // Chunk info
    struct CHUNK_INFO {
        CHUNK_HEADER _header; // Chunk header
        char* _data; // Chunk data
    };

    // MIDI file header
    struct FILE_HEADER {
        unsigned short _format; // MIDI format
        unsigned short _tracks; // Number of tracks
        unsigned short _division; // Time division
    };

    // Data members -----------------------------------------------------------
protected:
    String _error; // Last error desription
    char* _data; // File data
    char* _dataOffset; // Offset to the current data
    int _dataSize; // Data size
    int _format; // MIDI format
    int _tracks; // Number of tracks in the file
    int _timeDivision; // Time division
    double _beatTime; // Note time
    NOTES _notes; // Notes of a track, which were started
    String _report; // Trace report
    Note _note; // Current note's info
    Chord::CHORD _chordType; // Chord type for the current note
    int _chordRoot; // Chor's root note for the current note
    static double _volumeRate; // Logarithmic convertion rate

    // Functions --------------------------------------------------------------
protected:
//    bool		GetChunks(Song* pSong);
//    bool		NextChunk(CHUNK_INFO& Chunk, bool& bDone);
//    bool		GetFileHeader(CHUNK_INFO& Chunk, Song* pSong);
//    bool		GetTrack(CHUNK_INFO& Chunk, Song* pSong, Track* track);
//    bool		GetMetaEvent(char* data, int dataSize, int& nProcessed, double dTrackTime, Song* pSong, Track* track);
//    bool		GetSystemEvent(char* data, int dataSize, int& nProcessed);
//    bool		GetMidiEvent(unsigned char nEvent, char* data, int dataSize, int& nProcessed, double dTrackTime, Track* track);
//    bool		GetNote(int nChannel, bool bNoteOn, char* data, int dataSize, int& nProcessed, double dTrackTime, Track* track);
//    bool		GetProgram(int nChannel, char* data, int dataSize, int& nProcessed, double dTrackTime, Track* track);
//    bool        AddNote(Note* pNote, double dTrackTime, Track* track);
//    bool		GetControl(int nChannel, char* data, int dataSize, int& nProcessed, double dTrackTime, Track* track);
    bool		PutVarLen(int nValue, char* data, int dataSize, int& nBytes);
//    bool		GetVarLen(char* data, int dataSize, int& nValue, int& nProcessed);
//    bool		ResetNotes();
//    bool		GetProgramInfo(int nProgram, String& sName, THING& nInstrument);
    bool		GetControlInfo(int nControl, String& sName);
//    bool        GetMarker(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        GetChord(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        GetSongPart(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        GetMeasure(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
    bool		SaveHeader(Song* pSong);
    bool		SaveSongInfo(Song* pSong);
    bool		SaveAllTracks(Song* pSong);
    //bool        SaveParts(Song* pSong, Buffer& sTrackData);
    bool		SaveTrack(const char* pTrackData, int nTrackLength);
    int GetTimeDiff(double dTimeDiff);
    double GetTimeDiff(int nTimeDiff);
    bool		SaveMetaEvent(int nTimeDiff, META_EVENT nMetaEvent, int nChannel, const char* pEventData, int nEventDataSize, Buffer& sTrackData);
    bool		SaveControlEvent(int nTimeDiff, int nChannel, unsigned char nType, unsigned char nValue, Buffer& sTrackData);
    bool SaveNote(int nTimeDiff, bool bEVent, int nChannel, bool bOn, int nNote, double fVolume, Buffer& sTrackData);
    bool        SaveTab(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    bool        SaveMarker(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    bool        SaveChord(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    //bool        SaveMix(int nTimeDiff, CMix* pMix, Buffer& sTrackData);
//    bool        GetMix(const char* pMarker, Song* pSong);
    //bool        SavePartInfo(int nTimeDiff, SongPart* pPart, Buffer& sTrackData);
//    bool        SaveNoteInfo(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
//    bool        GetPartInfo(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        GetNoteInfo(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        GetSongInfo(const char* pMarker, double dTrackTime, Song* pSong, Track* track);
//    bool        SaveSongInfo(Song* pSong, Buffer& sTrackData);
    bool SaveSongPart(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    bool SaveMeasure(int nTimeDiff, Note* pNote, Track* track, Buffer& sTrackData);
    bool SaveEvent(int nTimeDiff, MIDI_EVENT nEvent, int nChannel, const char* pEventData, int nEventDataSize, Buffer& sTrackData);
    bool SaveTrackInfo(Track* track, Buffer& sTrackData);
    bool SaveTrackNotes(Track* track, Buffer& sTrackData);
    unsigned char GetProgram(INSTRUMENT instrument);
    bool Log(const char* pFormat, const char* pMessage = NULL);
    double GetBeatTime(double dTime);
    bool AddData(void* data, int size);
    unsigned char GetMidiDrumNote(INSTRUMENT instrument);
    bool GetDrumInfo(int nNote, String& sName, INSTRUMENT& nInstrument);

public:
    const char*	GetError();
//    bool		ReadData(const char* pFile, Song* pSong);
    bool SaveData(const char* pFile, Song* pSong);
//    bool        ReadData(char* data, int dwDataSize, Song* pSong);
//    bool        SaveData(char*& data, int& dwDataSize, Song* pSong);
//    bool		Test();
    static unsigned char VolumeToMidi(double fVolume);
//    static double    MidiToVolume(unsigned char nVelocity);
};

} // namespace QuantoGraph
#endif // MUSIC_MIDI_FILE_H
