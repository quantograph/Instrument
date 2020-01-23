#include "Misc.h"
#include "Note.h"
#include "Track.h"
#include "Song.h"
#include "MidiFile.h"

float MidiFile::_volumeRate = 0.3322;

//=================================================================================================
MidiFile::MidiFile() {
}

//=================================================================================================
MidiFile::~MidiFile() {
}

//=================================================================================================
// Rounds the beat time to the nearest time division
float MidiFile::GetBeatTime(float dTime) {
    float dBeatTime;

    if(_timeDivision == 0) {
        return false;
    }

    dBeatTime = (float)((int)(dTime * _timeDivision + 0.5)) / _timeDivision;

    return dBeatTime;
}

//=================================================================================================
uint16_t MidiFile::GetTimeDiff(float dTimeDiff) {
    if(dTimeDiff == 0.0)
        return 0;

    if(_beatTime == 0.0) {
        return 0;
    }

    return (int)((dTimeDiff / _beatTime) * _timeDivision + 0.5);
}

//=================================================================================================
float MidiFile::GetTimeDiff(int nTimeDiff) {
    if(nTimeDiff == 0)
        return 0.0;

    if(_beatTime == 0.0) {
        return 0;
    }

    return _beatTime * ((float)nTimeDiff / _timeDivision);
}

//=================================================================================================
// Reads the MIDI file into a Song object
bool MidiFile::Read(char* data, uint32_t size, Song* song) {
    Serial.printf("===== Reading MIDI file, %d bytes\n", size);
    _data = data;
    _dataSize = size;
    _dataRead = 0;

    _report = ""; // Reset the report
    _beatTime = 0.0;
    _timeDivision = 0;

    // Get all chunks
    if(!GetChunks(song))
        return false;

    Serial.printf("===== Done reading MIDI file\n");

    return true;
}

//=================================================================================================
// Gets all file chunks
bool MidiFile::GetChunks(Song* song) {
    ChunkInfo chunk;			// chunk info
    bool done = false;	// Whether done reading
    uint16_t fileTrack = 1;	// Track number in the file
    uint16_t saveTrack = 1;	// Track number to be saved
    Track* track;    // Current track info

    // Read all chunks
    _dataOffset = _data;
    do {
        // Read the next chunk
        if(!NextChunk(chunk, done)) {
            return false;
        }

        if(done) // No more data
            break;

        // Check for file header
        if(!strncmp(chunk._header._id, ID_FILE_HEADER, strlen(ID_FILE_HEADER))) { // File header
            if(!GetFileHeader(chunk, song))
                return false;
        } else if(!strncmp(chunk._header._id, ID_TRACK_HEADER, strlen(ID_TRACK_HEADER))) { // Track
            // Get track's data
            track = new Track();
            track->_trackNumber = fileTrack;
            if(!GetTrack(chunk, song, track))
                return false;

            // Save this track into the song
            if(_format == 1 && fileTrack == 1) {
                // Don't save first track if MIDI file format is 1. It only has song's data, no track notes.
            } else {
                song->_tracks.push_back(track);
                saveTrack++;
            }

            fileTrack++;
        } else { // Unknown chunk, ignore
        }

    } while(!done);

    return true;
}

//=================================================================================================
// Gets the next chunk from the file
bool MidiFile::NextChunk(ChunkInfo& chunk, bool& done) {
    Serial.printf(">>>>> NextChunk\n");
    done = false;	// Whether done processing

    // Read the chunk header
    if(_dataOffset + sizeof(ChunkHeader) > _data + _dataSize) { // Make sure we have enought data in the file
        // Check the size of data actually read
        uint32_t read = _dataOffset - _data;
        if(read == _dataSize)
            Serial.printf("Done reading: read %d bytes\n", read);
        else
            Serial.printf("ERROR: done reading, but read %d bytes out of %d\n", read, _dataSize);

        done = true; // Read all data
        return true;
    }

    memcpy(&chunk._header, _dataOffset, sizeof(ChunkHeader));
    _dataOffset += sizeof(ChunkHeader);
    Reverse4Bytes((char*)&chunk._header._length); // Length is stored in reversed byte order

    // Find the data
    if(_dataOffset + chunk._header._length > _data + _dataSize) { // Make sure we have enought data in the file
        done = true;
        Serial.printf("ERROR: Not enought data in the file for the chunk data: need %d, have only %d left\n", chunk._header._length, _data - _dataOffset);
        return false;
    }

    if(chunk._header._length > 0)
        chunk._data = _dataOffset;
    else
        chunk._data = NULL; // No data for this chunk

    _dataOffset += chunk._header._length;

    return true;
}

//=================================================================================================
// Get file's header
bool MidiFile::GetFileHeader(ChunkInfo& chunk, Song* song) {
    FileHeader*	header = NULL;	// File header

    // Check the header size
    if(chunk._header._length != 6) { // The header chunk must be 6 bytes long
        Serial.printf("ERROR: MIDI file header is not 6 bytes long: %d\n", chunk._header._length);
        return false;
    }

    header = (FileHeader*)chunk._data;

    // MIDI format.
    // 0 - one track for all notes
    // 1 - one MIDI track per song track. The first track contains all parameters for the song.
    // 2 - many tracks, but not played simultaneously. May be used to store patterns.
    Reverse2Bytes((char*)&header->_format);
    _format = header->_format;

    // Number of tracks
    Reverse2Bytes((char*)&header->_tracks);
    _tracks = header->_tracks;

    // Time division.
    // If positive, it's a number of ticks per quarter note (beat).
    Reverse2Bytes((char*)&header->_division);
    _timeDivision = header->_division;

    Serial.printf("File header: format=%d, tracks=%d, timeDivision=%d\n", _format, _tracks, _timeDivision);

    return true;
}

//=================================================================================================
// Reads one track
bool MidiFile::GetTrack(ChunkInfo& chunk, Song* song, Track* track) {
    int		dwProcessed = 0;	// Bytes processed
    int			size = 0;			// Field size
    int			nBytes = 0;			// Bytes in the data
    int		nTime = 0;			// Event's time (offset)
    int		nTrackTime = 0;     // Event's time (absolute)
    uint8_t		nEvent = 0;			// Event ID
    uint8_t		nPrevEvent = 0;		// Previous event
    float		dEventTime = 0.0;	// Event's relative time, in seconds
    float		dTrackTime = 0.0;	// Current track time, in seconds
    char*		pEventData = NULL;	// Event data
    int		dwEventLength = 0;	// Event's data length
    String	sString;			// General string
    int         nIndex = 0;         // List index
    Note*      note = NULL;       // Current note
    int	        i = 0;		        // Loop counter

    // Add to the report
    sString.Format("Track %d >>>>>>>>>>>>>>>>>>>>>>>>>>", track->_trackNumber);
    Log((const char*)sString);
    _report += sString;

    ResetNotes(); // Clear all started notes

    // Process all events in the track
    while(dwProcessed < chunk._header._length) {
        // Get the time
        size = Min(4, chunk._header._length - dwProcessed);
        GetVarLen(chunk._data + dwProcessed, size, nTime, nBytes);
        dwProcessed += nBytes;

        // Update track time.
        // Event time is relative to the time of the last event.
        nTrackTime += nTime;
        dEventTime = GetTimeDiff((int)nTime);
        dTrackTime = GetTimeDiff((int)nTrackTime);

        // Add event time to the report
        sString.Format("Event diff: %04d, %7.3f sec, beat=%7.3f, TrackTime=%7.3f", nTime, dEventTime, song->_beatTime, dTrackTime);
        //Log((const char*)sString);
        _report += sString;

        // If reading a repeated song, stop reading after the end of the real song
        if(song->_dSongTime != NO_DOUBLE) {
            if(dTrackTime > song->_dSongTime + 0.01)
                break;
        }

        // Get the event ID
        nEvent = *(uint8_t*)(chunk._data + dwProcessed);
        dwEventLength = chunk._header._length - dwProcessed;
        dwProcessed += 1;

        // Process the event
        if(nEvent == MIDI_META) { // Meta event
            pEventData = chunk._data + dwProcessed;
            if(!GetMetaEvent(pEventData, dwEventLength, nBytes, dTrackTime, song, track))
                return false;

            dwProcessed += nBytes;
        } else if(nEvent == MIDI_SYSTEM) { // System event
            pEventData = chunk._data + dwProcessed;
            if(!GetSystemEvent(pEventData, dwEventLength, nBytes))
                return false;

            dwProcessed += nBytes;
        } else if(nEvent >= 0x80 && nEvent <= 0xEF) { // MIDI event
            pEventData = chunk._data + dwProcessed;
            if(!GetMidiEvent(nEvent, pEventData, dwEventLength, nBytes, dTrackTime, track))
                return false;

            dwProcessed += nBytes;
            nPrevEvent = nEvent;
        } else if(nEvent < 0x80) { // Running status
            dwProcessed--; // Go back to the event data, there was no event ID
            pEventData = chunk._data + dwProcessed;
            if(!GetMidiEvent(nPrevEvent, pEventData, dwEventLength, nBytes, dTrackTime, track))
                return false;

            dwProcessed += nBytes;
        } else { // Unknown event
            // If we don't know the event, we don't know it's length and can't skip it.
            return false;
        }
    }

    if(track->_instrument == PERCUSSION) {
        // End all notes which were started, but not finished
        size = sizeof(_notes) / sizeof(Note);
        for(i = 0; i < size; i++) {
            note = &_notes[i];
            if(note->_nState == Note::STATE_START) // This note was started
                AddNote(note, song->_dSongTime, track);
        }
    }

    // If there is a silent note at the end of the track, remove it.
    // It was added in MidiFile::SaveTrackNotes to make MIDI players not to cut off the end of songs.
    if(track->_notes.GetLast(nIndex, note)) {
        if(note->_volume == 0.0f && !note->IsMarker())
            track->_notes.Delete(nIndex);
    }

    return true;
}

//=================================================================================================
// Get meta event
bool MidiFile::GetMetaEvent(char* data, int dataSize, int& nProcessed, float dTrackTime, Song* song, Track* track) {
    uint8_t		nType = 0;			// Event type
    int			nBytes = 0;			// Bytes in the variable lenght data
    int		length = 0;		// Data length
    String	sName;				// Event name
    char*		pMetaData = NULL;	// Data of the meta event
    int		dwData = 0;			// Data buffer for 4 bytes
    String	sString;			// General string
    String	sMarker;			// Marker string

    nProcessed = 0;

    // Get the meta event type
    nType = *(uint8_t*)data;
    nProcessed += 1;

    // Get data length
    GetVarLen(data + nProcessed, dataSize - nProcessed, length, nBytes);
    nProcessed += nBytes;

    // Get the data
    pMetaData = data + nProcessed;

    switch(nType) {
        case META_SEQUENCE: // Sequence number: 2 bytes (MSB, LSB)
            sName = "Sequence";
            break;

        case META_TEXT: // Text event: length, text
            sName = "Text";
            track->_sText.Copy(pMetaData, length);

            // For MIDI format 1, song's text is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_sText = track->_sText;

            // Add to the report
            sString.Format("Text '%s'", (const char*)track->_sText);
            //Log((const char*)sString);
            _report += sString;

            break;

        case META_COPYRIGHT: // Copyright notice: length, text
            sName = "Copyright";
            track->_sCopyright.Copy(pMetaData, length);

            // For MIDI format 1, song's text is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_sCopyright = track->_sCopyright;

            // Add to the report
            sString.Format("Copyright '%s'", (const char*)track->_sCopyright);
            //Log((const char*)sString);
            _report += sString;

            break;

        case META_TRACK_NAME: // Sequence or track name: length, text
            sName = "Track name";
            track->_name.Copy(pMetaData, length);

            // For MIDI format 1, song name is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_name = track->_name;

            // Add to the report
            sString.Format("Track '%s'", (const char*)track->_name);
            Log((const char*)sString);
            _report += sString;

            break;

        case META_INSTRUMENT: // Instrument name: length, text
            sName = "Instrument";
            track->_sInstrument.Copy(pMetaData, length);

            // Add to the report
            sString.Format("Instrument '%s'", (const char*)track->_sInstrument);
            Log((const char*)sString);
            _report += sString;

            break;

        case META_LYRICS: // Lyric text: length, text
            sName = "Lyric";
            break;

        case META_MARKER: // Marker text: length, text
            sName = "Marker";
            sMarker.Copy(pMetaData, length);
            GetMarker((const char*)sMarker, dTrackTime, song, track);

            // Add to the report
            sString.Format("Marker '%s'", (const char*)sMarker);
            //Log((const char*)sString);
            _report += sString;

            break;

        case META_CUE: // Cue point: length, text
            sName = "Cue";
            break;

        case META_CHANNEL: // MIDI channel prefix assignment: 1 byte, 0-15
            sName = "Channel";
            break;

        case META_END: // End of track, no data
            sName = "End of track";
            break;

        case META_TEMPO: // Tempo setting: 3 bytes, microseconds per quarter note
            sName = "Tempo";
            memcpy((char*)(&dwData) + 1, pMetaData, 3);
            Reverse4Bytes((char*)&dwData);
            song->_dTempo = (float)60 * 1000 * 1000 / dwData; // Tempo is microseconds per minute, divided by microseconds per beat (tempo)
            song->_beatTime = (float)dwData / (1000.0 * 1000.0); // Quarter note time in seconds
            _beatTime = GetBeatTime(song->_beatTime);
            break;

        case META_OFFSET: // SMPTE offset
            sName = "SMPTE offset";
            break;

        case META_TIME_SIGNATURE: // Time signature: 4 bytes
            sName = "Time signature";
            song->_measureBeats = (int)pMetaData[0]; // Numerator
            song->_beatNotes = (int)pow(2.0f, (int)pMetaData[1]); // Denominator: power of 2
            //pMetaData[2]); // Metronome
            //pMetaData[3]); // 32-nd's
            break;

        case META_KEY_SIGNATURE: // Key signature
            sName = "Key signature";
            song->_key = (KEY_SIGNATURE)pMetaData[0]; // Key (1 byte)
            song->_nScale = (SCALE)pMetaData[1]; // Scale (1 byte)
            break;

        case META_SPECIFIC: // Sequencer specific event
            sName = "Sequencer specific";
            break;

        default:
            sName = "Unknown";
            break;
    }

    nProcessed += length;

    // Add to the report
    sString.Format("Meta: %02X - %s, %d bytes", nType, (const char*)sName, length);
    //Log((const char*)sString);
    _report += sString;

    return true;
}

//=================================================================================================
bool MidiFile::GetSystemEvent(char* data, int dataSize, int& nProcessed) {
    int			nBytes = 0;		// Bytes in the data
    int		length = 0;	// Data length
    String	sString;		// General string

    nProcessed = 0;

    // Get data length
    GetVarLen(data + nProcessed, dataSize - nProcessed, length, nBytes);
    nProcessed += nBytes;

    // Get the data

    nProcessed += length;

    // Add to the report
    sString.Format("System: ");
    //Log((const char*)sString);
    _report += sString;

    return true;
}

//=================================================================================================
bool MidiFile::GetMidiEvent(uint8_t nEvent, char* data, int dataSize, int& nProcessed, float dTrackTime, Track* track) {
    uint8_t		nParam1 = 0;	// Event parameter 1
    uint8_t		nParam2 = 0;	// Event parameter 2
    uint8_t		nType = 0;		// Event type
    uint8_t		nChannel = 0;	// Event channel
    int			nBytes = 0;		// Bytes processed in this call
    String	sString;		// General string

    nProcessed = 0;
    nType = nEvent & 0xF0;
    nChannel = nEvent & 0x0F;

    // There is no instrument type for the drum set. MIDI channel 9 is reserved for drums.
    // So, if the channel is 9, set track's instrument to 'drum set'
    if(nChannel == DRUM_CHANNEL)
        track->_instrument = PERCUSSION;

    // 4 left bits specify
    switch(nType) {
        case MIDI_NOTE_OFF: // Note Off
            if(!GetNote(nChannel, false, data, dataSize, nBytes, dTrackTime, track)) // Note off
                return false;

            nProcessed += nBytes;
            break;

        case MIDI_NOTE_ON: // Note on
            if(!GetNote(nChannel, true, data, dataSize, nBytes, dTrackTime, track)) // Note on
                return false;

            nProcessed += nBytes;
            break;

        case MIDI_NOTE_AFTER: // Note aftertouch
            nParam1 = *(uint8_t*)(data + nProcessed); // Note
            nProcessed++;

            nParam2 = *(uint8_t*)(data + nProcessed); // Value
            nProcessed++;

            // Add to the report
            sString.Format("Note %d aftertouch %d", nParam1, nParam2);
            //Log((const char*)sString);
            _report += sString;

            break;

        case MIDI_CONTROL: // Control change
            if(!GetControl(nChannel, data, dataSize, nBytes, dTrackTime, track))
                return false;

            nProcessed += nBytes;
            break;

        case MIDI_PROGRAM: // Program change
            if(!GetProgram(nChannel, data, dataSize, nBytes, dTrackTime, track))
                return false;

            nProcessed += nBytes;
            break;

        case MIDI_CHANNEL_AFTER: // Channel aftertouch
            nParam1 = *(uint8_t*)(data + nProcessed); // Value
            nProcessed++;

            // Add to the report
            sString.Format("Channel aftertouch %d", nParam1);
            //Log((const char*)sString);
            _report += sString;

            break;

        case MIDI_BEND: // Pitch bend
            nParam1 = *(uint8_t*)(data + nProcessed); // Value (low byte)
            nProcessed++;

            nParam2 = *(uint8_t*)(data + nProcessed); // Value (high byte)
            nProcessed++;

            // Add to the report
            sString.Format("Pitch bend %d, %d", nParam1, nParam2);
            //Log((const char*)sString);
            _report += sString;

            break;

        default:
            _error.Format("Unknown MIDI type %02X", nType);
            return false;
    }

    return true;
}

//=================================================================================================
bool MidiFile::GetNote(int nChannel, bool bNoteOn, char* data, int dataSize, int& nProcessed, float dTrackTime, Track* track) {
    Note		Note;			    // Note to save
    int			nNote = 0;		    // MIDI note
    uint8_t		nVelocity = 0;	    // Velocity
    String	sString;		    // General string

    // Check data size
    if(dataSize < 2) {
        _error.Format("Not enough data for a note: %d", dataSize);
        return false;
    }

    // MIDI note number
    nNote = (int)(uint8_t)data[0];
    if((nNote < 0 || nNote > 200) && nNote != CHORD_MIDI_NOTE && nNote != FINGER_MIDI_NOTE) {
        _error.Format("Bad MIDI note: %d", nNote);
        return false;
    }

    // Velocity (0-127)
    nVelocity = (uint8_t)data[1];
    if(nVelocity > 127) {
        _error.Format("Bad note velocity: %d", nVelocity);
        return false;
    }

    // Check whether this note has been started before.
    // If yes, end it and save, even if it's started again.
    if(_notes[nNote]._nState == Note::STATE_START) { // This note was started
        Note = _notes[nNote];
        AddNote(&Note, dTrackTime, track);

        _notes[nNote]._nState = Note::STATE_NONE; // Done with this note
        _note.Reset(); // No info for the next note unless it's read from the file again
        _chordType = CHORD_NONE; // No chord type unless set
        _chordRoot = NO_MIDI_NOTE; // No chord root unless set
    }

    // Start a new note, if the velocity is not zero.
    // If the velocity is zero, it's the end of note and the note has been already saved.
    if(nVelocity > 0) { // New note
        //_notes[nNote]._volume = (float)nVelocity / 127; // Linear conversion
        _notes[nNote]._volume = MidiToVolume(nVelocity); // Logarithmic

        _notes[nNote]._start = dTrackTime;
        _notes[nNote]._midiNote = nNote;
        _notes[nNote]._nChannel = nChannel;
        _notes[nNote]._nState = Note::STATE_START;
        _notes[nNote]._Tab = _note._Tab; // Save note's tab info
        _notes[nNote]._nInterval = _note._nInterval;
        _notes[nNote]._ChordInfo._nType = _chordType;
        _notes[nNote]._ChordInfo._nRoot = _chordRoot;
    }

    // Add to the report
    sString.Format("%7.3f sec: Note %s, note %3d, velocity %3d, channel=%d",
                   dTrackTime, bNoteOn ? "on" : "off", nNote, nVelocity, nChannel);
    Log((const char*)sString);
    _report += sString;

    nProcessed = 2;

    return true;
}

//=================================================================================================
bool MidiFile::AddNote(Note* note, float dTrackTime, Track* track) {
    THING       nInstrument = PERCUSSION;    // Instrument type
    String sInstrument;        // Instrument name

    // Set note's instrument
    // If this is a drum set track, set the drum type for this note
    if(note->_nChannel == DRUM_CHANNEL) { // MIDI channel 9 (counting from 0) is reserved for drum sets
        if(GetDrumInfo(note->_midiNote, sInstrument, nInstrument))
            note->_instrument = nInstrument;
        else
            note->_instrument = 0;
    } else
        note->_instrument = track->_instrument;

    // Save the note
    note->_duration = dTrackTime - note->_start;
    note->_nSource = Note::SOURCE_FILE;
    track->AddNote(*note);

    return true;
}

//=================================================================================================
// Get MIDI program (instrument or patch)
bool MidiFile::GetProgram(int nChannel, char* data, int dataSize, int& nProcessed, float dTrackTime, Track* track) {
    THING		nInstrument = PERCUSSION;	// Instrument ID for this program
    int			nProgram = 0;	// Program number. 0-127 in the file, but 1-128 in the specifications
    String	sProgramName;	// Program name
    String	sString;		// General string

    // Check data size
    if(dataSize < 1) {
        _error.Format("Not enough data for a program change: %d", dataSize);
        return false;
    }

    // MIDI program number
    nProgram = (int)(uint8_t)data[0];
    if(nProgram < 0 || nProgram > 127) {
        _error.Format("Bad program number: %d", nProgram);
        return false;
    }

    // Get program's info
    if(nChannel == DRUM_CHANNEL) { // Check whether this is a Drum Channel: channel 9, with channel numbers starting with 0.
        nInstrument = PERCUSSION;
        sProgramName = "Drum set";
    } else {
        if(!GetProgramInfo(nProgram, sProgramName, nInstrument))
            return false;
    }

    // Save the program
    track->_nMidiProgram = nProgram;
    track->_instrument = nInstrument;
    track->_sInstrument = sProgramName;

    // Add to the report
    sString.Format("Program %3d, '%s', channel=%d", nProgram, (const char*)sProgramName, nChannel);
    //Log((const char*)sString);
    _report += sString;

    nProcessed = 1;

    return true;
}

//=================================================================================================
// Get MIDI control code
bool MidiFile::GetControl(int nChannel, char* data, int dataSize, int& nProcessed, float dTrackTime, Track* track) {
    float		fHalf = 127.0f / 2.0f;	// Half of the value range
    String	sControlName;	// Control name
    int			nControl = 0;	// Control type
    int			nValue = 0;		// Control value, 0-127
    float		fValue = 0.0f;	// Control value, 0.0 - 1.0
    String	sString;		// General string

    // Check data size
    if(dataSize < 2) {
        _error.Format("Not enough data for a control event: %d", dataSize);
        return false;
    }

    // Control type (0 - 127)
    nControl = (int)(uint8_t)data[0];

    // Control value (0 - 127)
    nValue = (int)(uint8_t)data[1];

    // Get control's info
    if(!GetControlInfo(nControl, sControlName))
        return false;

    // Process the control
    fValue = (float)nValue / 127;
    switch(nControl) {
        case CONTROL_VOLUME: // Volume
            track->_volume = fValue;
            break;

        case CONTROL_PAN: // Pan
            track->_pan = ((float)nValue - fHalf) / fHalf; // Pan is from -1.0 to 1.0
            break;

        case CONTROL_TAB_STRING: // Guitar tab - string number
            _note._Tab._nString = nValue;
            break;

        case CONTROL_TAB_FRET: // Guitar tab - fret number
            _note._Tab._nFret = nValue;
            break;

        case CONTROL_TAB_FINGER: // Guitar tab - finger number
            _note._Tab._nFinger = (uint8_t)nValue;
            break;

        case CONTROL_TAB_MODE: // Guitar tab - playing mode
            _note._Tab._nMode = (Note::TAB_INFO::MODE)nValue;
            break;

        case CONTROL_CHORD_TYPE: // Chord type
            _chordType = (CHORD_TYPE)nValue;
            break;

        case CONTROL_CHORD_ROOT: // Chord's root note
            _chordRoot = (int)nValue;
            break;
    }

    // Add to the report
    sString.Format("Control %3d, '%s' = %d, channel=%d", nControl, (const char*)sControlName, nValue, nChannel);
    //Log((const char*)sString);
    _report += sString;

    nProcessed = 2;

    return true;
}

//=================================================================================================
// Resets all started notes
bool MidiFile::ResetNotes() {
    int	size = 0;	// Array size
    int	i = 0;		// Loop counter

    size = sizeof(_notes) / sizeof(Note);
    for(i = 0; i < size; i++)
        _notes[i]._nState = Note::STATE_NONE; // Note was not started

    return true;
}

//=================================================================================================
// Saves a variable length number
// dataSize - (in) size of data array
// nBytes - (out) number of bytes saved into data
bool MidiFile::PutVarLen(int nValue, char* data, int dataSize, int& nBytes) {
    int	Temp = 0;		// Buffer for reversed data
    bool	done = false;	// Whether done processing

    nBytes = 0;

    // Convert the number into a sequence of bits, using each 7-th bit as an indicator whether this is the last byte in the sequence.
    // This sequence will be reversed later to put the last byte to the end.
    Temp = (char)(nValue & 0x7F); // There should be at least one byte. Use only 7 bits of each byte.
    while(nValue >>= 7) { // Check whether the 7-th bit is set. If it is 0, this is the last byte. If not, keep saving other bytes.
        Temp <<= 8;
        Temp |= 0x80; // Set the 7-th bit to 1 to indicate that this is not the last byte.
        Temp += nValue & 0x7f;
    }

    // Reverse and save the bytes
    while(!done) {
        data[nBytes] = (char)Temp; // Save the current byte
        if(data[nBytes] & 0x80) // This is not the last byte
            Temp >>= 8; // Go to the next byte
        else
            done = true;

        nBytes++;
    }

    // Maximum variable length must not be more than 4 bytes
    if(nBytes > 4) {
        return false;
    }

    return true;
}

//=================================================================================================
// Gets a variable length number
bool MidiFile::GetVarLen(char* data, int dataSize, int& nValue, int& nProcessed) {
    int		i = 0;		// Data index
    uint8_t	nData = 0;	// Current byte

    nProcessed = 0;

    // Check the data size
    if(dataSize <= 0) {
        return false;
    }

    nValue = data[i]; // There is at least one byte
    i++;

    if(nValue & 0x80) { // 7-th bit of the last byte is set to 0. Process all bytes with 7-th bit set to 1.
        nValue &= 0x7F; // Use only 7 bits of the byte
        do {
            if(i >= dataSize) // Don't go out of the source data
                break;

            nData = data[i];
            nValue <<= 7; // Use only 7 bits of each byte
            nValue += nData & 0x7F; // Use only 7 bits of each byte
            i++;
        } while(nData & 0x80); // 7-th bit of the last byte is set to 0. Process all bytes with 7-th bit set to 1.
    }

    nProcessed = i;

    return true;
}

//=================================================================================================
uint8_t MidiFile::VolumeToMidi(float fVolume) {
    uint8_t    nVelocity;
    float   fLog;

    if(fVolume < 0.01f)
        return 0;

    if(fVolume >= 1.0f)
        return 127;

    fLog = 1.0f + log10(fVolume) * _volumeRate;
    nVelocity = (uint8_t)(fLog * 127.0f + 0.5f);

    return nVelocity;
}

//=================================================================================================
float MidiFile::MidiToVolume(uint8_t nVelocity) {
    float   fVelocity;
    float   fVolume;

    if(nVelocity == 0)
        return 0.0f;

    if(nVelocity >= 127)
        return 1.0f;

    fVelocity = (float)nVelocity / 127.0f;
    fVelocity = (fVelocity - 1.0f) / _volumeRate;
    fVolume = (float)(pow(10.0f, fVelocity));

    return fVolume;
}

//=================================================================================================
// Get program info by it's number
bool MidiFile::GetProgramInfo(int nProgram, String& sName, THING& nInstrument) {
    switch(nProgram) {
        case 0:
            sName = "Acoustic Grand Piano";
            nInstrument = PERCUSSION;
            break;
        case 1:
            sName = "Bright Acoustic Piano";
            nInstrument = PERCUSSION;
            break;
        case 2:
            sName = "Electric Grand Piano";
            nInstrument = PERCUSSION;
            break;
        case 3:
            sName = "Honky-tonk Piano";
            nInstrument = PERCUSSION;
            break;
        case 4:
            sName = "Electric Piano 1";
            nInstrument = PERCUSSION;
            break;
        case 5:
            sName = "Electric Piano 2";
            nInstrument = PERCUSSION;
            break;
        case 6:
            sName = "Harpsichord";
            nInstrument = PERCUSSION;
            break;
        case 7:
            sName = "Clavi";
            nInstrument = PERCUSSION;
            break;
        case 8:
            sName = "Celesta";
            nInstrument = PERCUSSION;
            break;
        case 9:
            sName = "Glockenspiel";
            nInstrument = PERCUSSION;
            break;
        case 10:
            sName = "Music Box";
            nInstrument = PERCUSSION;
            break;
        case 11:
            sName = "Vibraphone";
            nInstrument = PERCUSSION;
            break;
        case 12:
            sName = "Marimba";
            nInstrument = PERCUSSION;
            break;
        case 13:
            sName = "Xylophone";
            nInstrument = PERCUSSION;
            break;
        case 14:
            sName = "Tubular Bells";
            nInstrument = PERCUSSION;
            break;
        case 15:
            sName = "Dulcimer";
            nInstrument = PERCUSSION;
            break;
        case 16:
            sName = "Drawbar Organ";
            nInstrument = PERCUSSION;
            break;
        case 17:
            sName = "Percussive Organ";
            nInstrument = PERCUSSION;
            break;
        case 18:
            sName = "Rock Organ";
            nInstrument = PERCUSSION;
            break;
        case 19:
            sName = "Church Organ";
            nInstrument = PERCUSSION;
            break;
        case 20:
            sName = "Reed Organ";
            nInstrument = PERCUSSION;
            break;
        case 21:
            sName = "Accordion";
            nInstrument = PERCUSSION;
            break;
        case 22:
            sName = "Harmonica";
            nInstrument = PERCUSSION;
            break;
        case 23:
            sName = "Tango Accordion";
            nInstrument = PERCUSSION;
            break;

        case 24:
            sName = "Acoustic Guitar (nylon)";
            nInstrument = THING_RHYTHM_GUITAR;
            break;
        case 25:
            sName = "Acoustic Guitar (steel)";
            nInstrument = THING_RHYTHM_GUITAR;
            break;
        case 26:
            sName = "Electric Guitar (jazz)";
            nInstrument = THING_RHYTHM_GUITAR;
            break;
        case 27:
            sName = "Electric Guitar (clean)";
            nInstrument = THING_RHYTHM_GUITAR;
            break;
        case 28:
            sName = "Electric Guitar (muted)";
            nInstrument = THING_RHYTHM_GUITAR;
            break;

        case 29:
            sName = "Overdriven Guitar";
            nInstrument = THING_LEAD_GUITAR;
            break;
        case 30:
            sName = "Distortion Guitar";
            nInstrument = THING_SOLO_GUITAR;
            break;
        case 31:
            sName = "Guitar harmonics";
            nInstrument = THING_LEAD_GUITAR;
            break;

        case 32:
            sName = "Acoustic Bass";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 33:
            sName = "Electric Bass (finger)";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 34:
            sName = "Electric Bass (pick)";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 35:
            sName = "Fretless Bass";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 36:
            sName = "Slap Bass 1";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 37:
            sName = "Slap Bass 2";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 38:
            sName = "Synth Bass 1";
            nInstrument = THING_BASS_GUITAR;
            break;
        case 39:
            sName = "Synth Bass 2";
            nInstrument = THING_BASS_GUITAR;
            break;

        case 40:
            sName = "Violin";
            nInstrument = PERCUSSION;
            break;
        case 41:
            sName = "Viola";
            nInstrument = PERCUSSION;
            break;
        case 42:
            sName = "Cello";
            nInstrument = PERCUSSION;
            break;
        case 43:
            sName = "Contrabass";
            nInstrument = PERCUSSION;
            break;
        case 44:
            sName = "Tremolo Strings";
            nInstrument = PERCUSSION;
            break;
        case 45:
            sName = "Pizzicato Strings";
            nInstrument = PERCUSSION;
            break;
        case 46:
            sName = "Orchestral Harp";
            nInstrument = PERCUSSION;
            break;
        case 47:
            sName = "Timpani";
            nInstrument = PERCUSSION;
            break;
        case 48:
            sName = "String Ensemble 1";
            nInstrument = PERCUSSION;
            break;
        case 49:
            sName = "String Ensemble 2";
            nInstrument = PERCUSSION;
            break;
        case 50:
            sName = "SynthStrings 1";
            nInstrument = PERCUSSION;
            break;
        case 51:
            sName = "SynthStrings 2";
            nInstrument = PERCUSSION;
            break;
        case 52:
            sName = "Choir Aahs";
            nInstrument = PERCUSSION;
            break;
        case 53:
            sName = "Voice Oohs";
            nInstrument = PERCUSSION;
            break;
        case 54:
            sName = "Synth Voice";
            nInstrument = PERCUSSION;
            break;
        case 55:
            sName = "Orchestra Hit";
            nInstrument = PERCUSSION;
            break;
        case 56:
            sName = "Trumpet";
            nInstrument = PERCUSSION;
            break;
        case 57:
            sName = "Trombone";
            nInstrument = PERCUSSION;
            break;
        case 58:
            sName = "Tuba";
            nInstrument = PERCUSSION;
            break;
        case 59:
            sName = "Muted Trumpet";
            nInstrument = PERCUSSION;
            break;
        case 60:
            sName = "French Horn";
            nInstrument = PERCUSSION;
            break;
        case 61:
            sName = "Brass Section";
            nInstrument = PERCUSSION;
            break;
        case 62:
            sName = "SynthBrass 1";
            nInstrument = PERCUSSION;
            break;
        case 63:
            sName = "SynthBrass 2";
            nInstrument = PERCUSSION;
            break;
        case 64:
            sName = "Soprano Sax";
            nInstrument = PERCUSSION;
            break;
        case 65:
            sName = "Alto Sax";
            nInstrument = PERCUSSION;
            break;
        case 66:
            sName = "Tenor Sax";
            nInstrument = PERCUSSION;
            break;
        case 67:
            sName = "Baritone Sax";
            nInstrument = PERCUSSION;
            break;
        case 68:
            sName = "Oboe";
            nInstrument = PERCUSSION;
            break;
        case 69:
            sName = "English Horn";
            nInstrument = PERCUSSION;
            break;
        case 70:
            sName = "Bassoon";
            nInstrument = PERCUSSION;
            break;
        case 71:
            sName = "Clarinet";
            nInstrument = PERCUSSION;
            break;
        case 72:
            sName = "Piccolo";
            nInstrument = PERCUSSION;
            break;
        case 73:
            sName = "Flute";
            nInstrument = PERCUSSION;
            break;
        case 74:
            sName = "Recorder";
            nInstrument = PERCUSSION;
            break;
        case 75:
            sName = "Pan Flute";
            nInstrument = PERCUSSION;
            break;
        case 76:
            sName = "Blown Bottle";
            nInstrument = PERCUSSION;
            break;
        case 77:
            sName = "Shakuhachi";
            nInstrument = PERCUSSION;
            break;
        case 78:
            sName = "Whistle";
            nInstrument = PERCUSSION;
            break;
        case 79:
            sName = "Ocarina";
            nInstrument = PERCUSSION;
            break;
        case 80:
            sName = "Lead 1 (square)";
            nInstrument = PERCUSSION;
            break;
        case 81:
            sName = "Lead 2 (sawtooth)";
            nInstrument = PERCUSSION;
            break;
        case 82:
            sName = "Lead 3 (calliope)";
            nInstrument = PERCUSSION;
            break;
        case 83:
            sName = "Lead 4 (chiff)";
            nInstrument = PERCUSSION;
            break;
        case 84:
            sName = "Lead 5 (charang)";
            nInstrument = PERCUSSION;
            break;
        case 85:
            sName = "Lead 6 (voice)";
            nInstrument = PERCUSSION;
            break;
        case 86:
            sName = "Lead 7 (fifths)";
            nInstrument = PERCUSSION;
            break;
        case 87:
            sName = "Lead 8 (bass + lead)";
            nInstrument = PERCUSSION;
            break;
        case 88:
            sName = "Pad 1 (new age)";
            nInstrument = PERCUSSION;
            break;
        case 89:
            sName = "Pad 2 (warm)";
            nInstrument = PERCUSSION;
            break;
        case 90:
            sName = "Pad 3 (polysynth)";
            nInstrument = PERCUSSION;
            break;
        case 91:
            sName = "Pad 4 (choir)";
            nInstrument = PERCUSSION;
            break;
        case 92:
            sName = "Pad 5 (bowed)";
            nInstrument = PERCUSSION;
            break;
        case 93:
            sName = "Pad 6 (metallic)";
            nInstrument = PERCUSSION;
            break;
        case 94:
            sName = "Pad 7 (halo)";
            nInstrument = PERCUSSION;
            break;
        case 95:
            sName = "Pad 8 (sweep)";
            nInstrument = PERCUSSION;
            break;
        case 96:
            sName = "FX 1 (rain)";
            nInstrument = PERCUSSION;
            break;
        case 97:
            sName = "FX 2 (soundtrack)";
            nInstrument = PERCUSSION;
            break;
        case 98:
            sName = "FX 3 (crystal)";
            nInstrument = PERCUSSION;
            break;
        case 99:
            sName = "FX 4 (atmosphere)";
            nInstrument = PERCUSSION;
            break;
        case 100:
            sName = "FX 5 (brightness)";
            nInstrument = PERCUSSION;
            break;
        case 101:
            sName = "FX 6 (goblins)";
            nInstrument = PERCUSSION;
            break;
        case 102:
            sName = "FX 7 (echoes)";
            nInstrument = PERCUSSION;
            break;
        case 103:
            sName = "FX 8 (sci-fi)";
            nInstrument = PERCUSSION;
            break;
        case 104:
            sName = "Sitar";
            nInstrument = PERCUSSION;
            break;
        case 105:
            sName = "Banjo";
            nInstrument = PERCUSSION;
            break;
        case 106:
            sName = "Shamisen";
            nInstrument = PERCUSSION;
            break;
        case 107:
            sName = "Koto";
            nInstrument = PERCUSSION;
            break;
        case 108:
            sName = "Kalimba";
            nInstrument = PERCUSSION;
            break;
        case 109:
            sName = "Bag pipe";
            nInstrument = PERCUSSION;
            break;
        case 110:
            sName = "Fiddle";
            nInstrument = PERCUSSION;
            break;
        case 111:
            sName = "Shanai";
            nInstrument = PERCUSSION;
            break;
        case 112:
            sName = "Tinkle Bell";
            nInstrument = PERCUSSION;
            break;
        case 113:
            sName = "Agogo";
            nInstrument = PERCUSSION;
            break;
        case 114:
            sName = "Steel Drums";
            nInstrument = PERCUSSION;
            break;
        case 115:
            sName = "Woodblock";
            nInstrument = PERCUSSION;
            break;
        case 116:
            sName = "Taiko Drum";
            nInstrument = PERCUSSION;
            break;
        case 117:
            sName = "Melodic Tom";
            nInstrument = PERCUSSION;
            break;
        case 118:
            sName = "Synth Drum";
            nInstrument = PERCUSSION;
            break;
        case 119:
            sName = "Reverse Cymbal";
            nInstrument = PERCUSSION;
            break;
        case 120:
            sName = "Guitar Fret Noise";
            nInstrument = PERCUSSION;
            break;
        case 121:
            sName = "Breath Noise";
            nInstrument = PERCUSSION;
            break;
        case 122:
            sName = "Seashore";
            nInstrument = PERCUSSION;
            break;
        case 123:
            sName = "Bird Tweet";
            nInstrument = PERCUSSION;
            break;
        case 124:
            sName = "Telephone Ring";
            nInstrument = PERCUSSION;
            break;
        case 125:
            sName = "Helicopter";
            nInstrument = PERCUSSION;
            break;
        case 126:
            sName = "Applause";
            nInstrument = PERCUSSION;
            break;
        case 127:
            sName = "Gunshot";
            nInstrument = PERCUSSION;
            break;
        case 128:

        default:
            sName = "";
            nInstrument = PERCUSSION;
            _error.Format("Unknown program %d (1-128 range)", nProgram);
            break;
    }

    return true;
}

//=================================================================================================
// Get MIDI control(ler) info by it's number
bool MidiFile::GetControlInfo(int nControl, String& sName) {
    switch(nControl) {
        case 0:
            sName = "Bank Select";
            break;
        case 1:
            sName = "Modulation";
            break;
        case 2:
            sName = "Breath Controller";
            break;
        case 4:
            sName = "Foot Controller";
            break;
        case 5:
            sName = "Portamento Time";
            break;
        case 6:
            sName = "Data Entry (MSB)";
            break;
        case 7:
            sName = "Main Volume";
            break;
        case 8:
            sName = "Balance";
            break;
        case 9:
            sName = "Control 9 (0-127)";
            break;
        case 10:
            sName = "Pan";
            break;
        case 11:
            sName = "Expression Controller";
            break;
        case 12:
            sName = "Effect Control 1";
            break;
        case 13:
            sName = "Effect Control 2";
            break;
        case 14:
            sName = "Control 14 (0-127)";
            break;
        case 15:
            sName = "Control 15 (0-127)";
            break;
        case 16:
            sName = "Guitar tab - string number";
            break;
        case 17:
            sName = "Guitar tab - fret number";
            break;
        case 18:
            sName = "Guitar tab - finger number";
            break;
        case 19:
            sName = "Guitar tab - playing mode";
            break;
        case 20:
            sName = "Control 20 (0-127)";
            break;
        case 21:
            sName = "Control 21 (0-127)";
            break;
        case 22:
            sName = "Control 22 (0-127)";
            break;
        case 23:
            sName = "Control 23 (0-127)";
            break;
        case 24:
            sName = "Control 24 (0-127)";
            break;
        case 25:
            sName = "Control 25 (0-127)";
            break;
        case 26:
            sName = "Control 26 (0-127)";
            break;
        case 27:
            sName = "Control 27 (0-127)";
            break;
        case 28:
            sName = "Control 28 (0-127)";
            break;
        case 29:
            sName = "Control 29 (0-127)";
            break;
        case 30:
            sName = "Control 30 (0-127)";
            break;
        case 31:
            sName = "Control 31 (0-127)";
            break;
        case 32:
            sName = "Control 32 (0-127)";
            break;
        case 33:
            sName = "Control 33 (0-127)";
            break;
        case 34:
            sName = "Control 34 (0-127)";
            break;
        case 35:
            sName = "Control 35 (0-127)";
            break;
        case 36:
            sName = "Control 36 (0-127)";
            break;
        case 37:
            sName = "Control 37 (0-127)";
            break;
        case 38:
            sName = "Control 38 (0-127)";
            break;
        case 39:
            sName = "Control 39 (0-127)";
            break;
        case 40:
            sName = "Control 40 (0-127)";
            break;
        case 41:
            sName = "Control 41 (0-127)";
            break;
        case 42:
            sName = "Control 42 (0-127)";
            break;
        case 43:
            sName = "Control 43 (0-127)";
            break;
        case 44:
            sName = "Control 44 (0-127)";
            break;
        case 45:
            sName = "Control 45 (0-127)";
            break;
        case 46:
            sName = "Control 46 (0-127)";
            break;
        case 47:
            sName = "Control 47 (0-127)";
            break;
        case 48:
            sName = "Control 48 (0-127)";
            break;
        case 49:
            sName = "Control 49 (0-127)";
            break;
        case 50:
            sName = "Control 50 (0-127)";
            break;
        case 51:
            sName = "Control 51 (0-127)";
            break;
        case 52:
            sName = "Control 52 (0-127)";
            break;
        case 53:
            sName = "Control 53 (0-127)";
            break;
        case 54:
            sName = "Control 54 (0-127)";
            break;
        case 55:
            sName = "Control 55 (0-127)";
            break;
        case 56:
            sName = "Control 56 (0-127)";
            break;
        case 57:
            sName = "Control 57 (0-127)";
            break;
        case 58:
            sName = "Control 58 (0-127)";
            break;
        case 59:
            sName = "Control 59 (0-127)";
            break;
        case 60:
            sName = "Control 60 (0-127)";
            break;
        case 61:
            sName = "Control 61 (0-127)";
            break;
        case 62:
            sName = "Control 62 (0-127)";
            break;
        case 63:
            sName = "Control 63 (0-127)";
            break;
        case 64:
            sName = "Damper pedal (sustain)";
            break;
        case 65:
            sName = "Portamento";
            break;
        case 66:
            sName = "Sostenuto";
            break;
        case 67:
            sName = "Soft Pedal";
            break;
        case 68:
            sName = "Legato Footswitch";
            break;
        case 69:
            sName = "Hold 2";
            break;
        case 70:
            sName = "Sound Controller 1 (Timber Variation)";
            break;
        case 71:
            sName = "Sound Controller 2 (Timber/Harmonic Content)";
            break;
        case 72:
            sName = "Sound Controller 3 (Release Time)";
            break;
        case 73:
            sName = "Sound Controller 4 (Attack Time)";
            break;
        case 74:
            sName = "Sound Controller 5";
            break;
        case 75:
            sName = "Sound Controller 6";
            break;
        case 76:
            sName = "Sound Controller 7";
            break;
        case 77:
            sName = "Sound Controller 8";
            break;
        case 78:
            sName = "Sound Controller 9";
            break;
        case 79:
            sName = "Sound Controller 10";
            break;
        case 80:
            sName = "Chord type";
            break;
        case 81:
            sName = "Chord root";
            break;
        case 82:
            sName = "General-Purpose Controller 7";
            break;
        case 83:
            sName = "General-Purpose Controller 8";
            break;
        case 84:
            sName = "Portamento Control";
            break;
        case 85:
            sName = "Control 85 (0-127)";
            break;
        case 86:
            sName = "Control 86 (0-127)";
            break;
        case 87:
            sName = "Control 87 (0-127)";
            break;
        case 88:
            sName = "Control 88 (0-127)";
            break;
        case 89:
            sName = "Control 89 (0-127)";
            break;
        case 90:
            sName = "Control 90 (0-127)";
            break;
        case 91:
            sName = "Effects 1 Depth";
            break;
        case 92:
            sName = "Effects 2 Depth";
            break;
        case 93:
            sName = "Effects 3 Depth";
            break;
        case 94:
            sName = "Effects 4 Depth";
            break;
        case 95:
            sName = "Effects 5 Depth";
            break;
        case 96:
            sName = "Data Increment";
            break;
        case 97:
            sName = "Data Decrement";
            break;
        case 98:
            sName = "Non-Registered Parameter Number (LSB)";
            break;
        case 99:
            sName = "Non-Registered Parameter Number (MSB)";
            break;
        case 100:
            sName = "Registered Parameter Number (LSB)";
            break;
        case 101:
            sName = "Registered Parameter Number (MSB)";
            break;
        case 102:
            sName = "Control 102 (0-127)";
            break;
        case 103:
            sName = "Control 103 (0-127)";
            break;
        case 104:
            sName = "Control 104 (0-127)";
            break;
        case 105:
            sName = "Control 105 (0-127)";
            break;
        case 106:
            sName = "Control 106 (0-127)";
            break;
        case 107:
            sName = "Control 107 (0-127)";
            break;
        case 108:
            sName = "Control 108 (0-127)";
            break;
        case 109:
            sName = "Control 109 (0-127)";
            break;
        case 110:
            sName = "Control 110 (0-127)";
            break;
        case 111:
            sName = "Control 111 (0-127)";
            break;
        case 112:
            sName = "Control 112 (0-127)";
            break;
        case 113:
            sName = "Control 113 (0-127)";
            break;
        case 114:
            sName = "Control 114 (0-127)";
            break;
        case 115:
            sName = "Control 115 (0-127)";
            break;
        case 116:
            sName = "Control 116 (0-127)";
            break;
        case 117:
            sName = "Control 117 (0-127)";
            break;
        case 118:
            sName = "Control 118 (0-127)";
            break;
        case 119:
            sName = "Control 119 (0-127)";
            break;
        case 120:
            sName = "Control 120 (0-127)";
            break;
        case 121:
            sName = "Channel Mode Message: Reset All Controllers";
            break;
        case 122:
            sName = "Channel Mode Message: Local Control";
            break;
        case 123:
            sName = "Channel Mode Message: All Notes Off";
            break;
        case 124:
            sName = "Channel Mode Message: Omni Off";
            break;
        case 125:
            sName = "Channel Mode Message: Omni On";
            break;
        case 126:
            sName = "Channel Mode Message: Mono On (Poly Off)";
            break;
        case 127:
            sName = "Channel Mode Message: Poly On (Mono Off)";
            break;
        default:
            sName.Format("Unknown conrol %d", nControl);
            _error.Format("Unknown conrol %d", nControl);
            break;
    }

    return true;
}

//=================================================================================================
// Gets MIDI note number for a drum set instrument (thing) type
uint8_t MidiFile::GetMidiDrumNote(INSTRUMENT instrument) {
    switch(instrument) {
        case DRUM_BASS_DRUM_2:
            return 35; // Bass Drum 2
        case DRUM_BASS_DRUM_1:
            return 36; // Bass Drum 1

        case DRUM_SNARE:
            return 38; // Snare 1

        case DRUM_HIHAT_CLOSED:
            return 42; // Closed Hi-Hat
        case DRUM_HIHAT_OPEN:
            return 46; // Open Hi-Hat
        case DRUM_HIHAT_FOOT:
            return 44; // Pedal Hi-Hat

        case DRUM_TOM_1:
            return 41; // Low Tom 2
        case DRUM_TOM_2:
            return 43; // Low Tom 1
        case DRUM_TOM_3:
            return 45; // Mid Tom 2
        case DRUM_TOM_4:
            return 47; // Mid Tom 1
        case DRUM_TOM_5:
            return 48; // High Tom 2
        case DRUM_TOM_6:
            return 50; // High Tom 1

        case DRUM_CRASH_1:
            return 49; // Crash Cymbal 1
        case DRUM_CRASH_2:
            return 57; // Crash Cymbal 2

        case DRUM_RIDE_CYMBAL:
            return 51; // Ride Cymbal 1
        case DRUM_RIDE_BELL:
            return 53; // Ride Cymbal 1 (bell)
        case DRUM_CHINESE_1:
            return 52; // Chinese Cymbal
        case DRUM_SPLASH_1:
            return 55; // Splash Cymbal

        default:
            return NO_MIDI_NOTE;
    }
}

//=================================================================================================
// Get drum info by it's note number in the drum kit
// Drum bank is accessed by setting cc#00 (Bank Select MSB) to 120 and cc#32 (Bank Select LSB) to 0.
// MIDI drum kits:
// 1 Standard Kit - the only kit specified by General MIDI Level 1
// 9 Room Kit - more ambient percussive sounds
// 17 Power Kit - more powerful kick and snare sounds
// 25 Electronic Kit - emulations of various electronic drum machines
// 26 TR-808 Kit - analog drum kit similar to Roland TR-808
// 33 Jazz Kit - nearly identical to the Standard kit
// 41 Brush Kit - many brush sounds added
// 49 Orchestra Kit - a collection of concert drums and timpani
// 57 Sound FX Kit - a collection of sound effects
// 128 CM-64/CM-32L - the standard MT-32 Drum Kit
bool MidiFile::GetDrumInfo(int nNote, String& sName, INSTRUMENT& nInstrument) {
    switch(nNote) {
        case 27:
            sName = "High Q";
            nInstrument = PERCUSSION;
            break;
        case 28:
            sName = "Slap";
            nInstrument = PERCUSSION;
            break;
        case 29:
            sName = "Scratch1";
            nInstrument = PERCUSSION;
            break;
        case 30:
            sName = "Scratch2";
            nInstrument = PERCUSSION;
            break;
        case 31:
            sName = "Sticks";
            nInstrument = PERCUSSION;
            break;
        case 32:
            sName = "Square";
            nInstrument = PERCUSSION;
            break;
        case 33:
            sName = "Metronome1";
            nInstrument = PERCUSSION;
            break;
        case 34:
            sName = "Metronome2";
            nInstrument = PERCUSSION;
            break;
        case 35:
            sName = "Bass Drum 2";
            nInstrument = (INSTRUMENT)DRUM_BASS_DRUM_2;
            break;
        case 36:
            sName = "Bass Drum 1";
            nInstrument = (INSTRUMENT)DRUM_BASS_DRUM_1;
            break;
        case 37:
            sName = "Side Stick";
            nInstrument = (INSTRUMENT)PERCUSSION;
            break;
        case 38:
            sName = "Snare 1";
            nInstrument = (INSTRUMENT)DRUM_SNARE;
            break;
        case 39:
            sName = "Hand Clap";
            nInstrument = PERCUSSION;
            break;
        case 40:
            sName = "Snare 2";
            nInstrument = (INSTRUMENT)DRUM_SNARE;
            break;
        case 41:
            sName = "Low Tom 2";
            nInstrument = (INSTRUMENT)DRUM_TOM_6;
            break;
        case 42:
            sName = "Closed Hi-Hat";
            nInstrument = (INSTRUMENT)DRUM_HIHAT_CLOSED;
            break;
        case 43:
            sName = "Low Tom 1";
            nInstrument = (INSTRUMENT)DRUM_TOM_5;
            break;
        case 44:
            sName = "Pedal Hi-Hat";
            nInstrument = (INSTRUMENT)DRUM_HIHAT_FOOT;
            break;
        case 45:
            sName = "Mid Tom 2";
            nInstrument = (INSTRUMENT)DRUM_TOM_4;
            break;
        case 46:
            sName = "Open Hi-Hat";
            nInstrument = (INSTRUMENT)DRUM_HIHAT_OPEN;
            break;
        case 47:
            sName = "Mid Tom 1";
            nInstrument = (INSTRUMENT)DRUM_TOM_3;
            break;
        case 48:
            sName = "High Tom 2";
            nInstrument = (INSTRUMENT)DRUM_TOM_2;
            break;
        case 49:
            sName = "Crash Cymbal 1";
            nInstrument = (INSTRUMENT)DRUM_CRASH_1;
            break;
        case 50:
            sName = "High Tom 1";
            nInstrument = (INSTRUMENT)DRUM_TOM_1;
            break;
        case 51:
            sName = "Ride Cymbal 1";
            nInstrument = (INSTRUMENT)DRUM_RIDE_CYMBAL;
            break;
        case 52:
            sName = "Chinese Cymbal";
            nInstrument = (INSTRUMENT)DRUM_CHINESE_1;
            break;
        case 53:
            sName = "Ride Bell";
            nInstrument = (INSTRUMENT)DRUM_RIDE_BELL;
            break;
        case 54:
            sName = "Tambourine";
            nInstrument = PERCUSSION;
            break;
        case 55:
            sName = "Splash Cymbal";
            nInstrument = (INSTRUMENT)DRUM_SPLASH_1;
            break;
        case 56:
            sName = "Cowbell";
            nInstrument = (INSTRUMENT)PERCUSSION;
            break;
        case 57:
            sName = "Crash Cymbal 2";
            nInstrument = (INSTRUMENT)DRUM_CRASH_2;
            break;
        case 58:
            sName = "Vibra Slap";
            nInstrument = PERCUSSION;
            break;
        case 59:
            sName = "Ride Cymbal 2";
            nInstrument = PERCUSSION;
            break;
        case 60:
            sName = "Hi Bongo";
            nInstrument = PERCUSSION;
            break;
        case 61:
            sName = "Low Bongo";
            nInstrument = PERCUSSION;
            break;
        case 62:
            sName = "Mute Hi Conga";
            nInstrument = PERCUSSION;
            break;
        case 63:
            sName = "Open Hi Conga";
            nInstrument = PERCUSSION;
            break;
        case 64:
            sName = "Low Conga";
            nInstrument = PERCUSSION;
            break;
        case 65:
            sName = "High Timbale";
            nInstrument = PERCUSSION;
            break;
        case 66:
            sName = "Low Timbale";
            nInstrument = PERCUSSION;
            break;
        case 67:
            sName = "High Agogo";
            nInstrument = PERCUSSION;
            break;
        case 68:
            sName = "Low Agogo";
            nInstrument = PERCUSSION;
            break;
        case 69:
            sName = "Cabasa";
            nInstrument = PERCUSSION;
            break;
        case 70:
            sName = "Maracas";
            nInstrument = PERCUSSION;
            break;
        case 71:
            sName = "Short Whistle";
            nInstrument = PERCUSSION;
            break;
        case 72:
            sName = "Long Whistle";
            nInstrument = PERCUSSION;
            break;
        case 73:
            sName = "Short Guiro";
            nInstrument = PERCUSSION;
            break;
        case 74:
            sName = "Long Guiro";
            nInstrument = PERCUSSION;
            break;
        case 75:
            sName = "Claves";
            nInstrument = PERCUSSION;
            break;
        case 76:
            sName = "Hi Wood Block";
            nInstrument = PERCUSSION;
            break;
        case 77:
            sName = "Low Wood Block";
            nInstrument = PERCUSSION;
            break;
        case 78:
            sName = "Mute Cuica";
            nInstrument = PERCUSSION;
            break;
        case 79:
            sName = "Open Cuica";
            nInstrument = PERCUSSION;
            break;
        case 80:
            sName = "Mute Triangle";
            nInstrument = PERCUSSION;
            break;
        case 81:
            sName = "Open Triangle";
            nInstrument = PERCUSSION;
            break;
        case 82:
            sName = "Shaker";
            nInstrument = PERCUSSION;
            break;
        case 83:
            sName = "Jingle Bell";
            nInstrument = PERCUSSION;
            break;
        case 84:
            sName = "Belltree";
            nInstrument = PERCUSSION;
            break;
        case 85:
            sName = "Castanets";
            nInstrument = PERCUSSION;
            break;
        case 86:
            sName = "Mute Surdo";
            nInstrument = PERCUSSION;
            break;
        case 87:
            sName = "Open Surdo";
            nInstrument = PERCUSSION;
            break;
        default:
            sName = "";
            nInstrument = PERCUSSION;
            //_ASSERTE(false);
            return false;
    }

    if(nInstrument == 0)
        return false;

    return true;
}
/*
//=================================================================================================
// Saves a song into MIDI file
bool MidiFile::SaveData(char*& data, int& dwDataSize, Song* song) {
    bool	bRet = true;	// Return value

    #ifndef _DEBUG
    __try {
    #endif

        Log("========== Lesson '%s': save ===============");
        //song->Show("MidiFile::SaveData");

        _data = NULL;
        _dataSize = 0;

        // Set some parameters
        _format = 1;
        _tracks = song->GetNumberOfTracks();
        _timeDivision = 1000;//960;
        _beatTime = GetBeatTime(song->_beatTime);

        // Save the header
        if(!SaveHeader(song)) {
            bRet = false;
            goto Exit;
        }

        // Save song's info in the first track
        if(!SaveSongInfo(song)) {
            bRet = false;
            goto Exit;
        }

        // Save all tracks
        if(!SaveAllTracks(song)) {
            bRet = false;
            goto Exit;
        }

    #ifndef _DEBUG
    } __except(::ExceptionFilter(GetExceptionInformation())) {
        return false;
    }
    #endif

Exit:
    data = _data;
    dwDataSize = _dataOffset - _data;

    Log("========== Lesson: end of save ===============");
    return bRet;
}

//=================================================================================================
// Saves a song into MIDI file
bool MidiFile::SaveData(const char* pFile, Song* song) {
    bool	bRet = true;	// Return value
    int     dataSize;
    int ret;

    Log("========== MIDI file '%s': save ===============", pFile);
    //song->Show("MidiFile::SaveData");

    // Open the file for writing
    ret = Open(pFile, false, true); // A new file for writing
    if(ret)
        return false;

    _data = NULL;
    _dataSize = 0;

    // Set some parameters
    _format = 1;
    _tracks = (int)song->_tracks.size();
    _timeDivision = 1000;//960;
    _beatTime = GetBeatTime(song->_beatTime);

    // Save the header
    if(!SaveHeader(song)) {
        bRet = false;
        goto Exit;
    }

    // Save song's info in the first track
    if(!SaveSongInfo(song)) {
        bRet = false;
        goto Exit;
    }

    // Save all tracks
    if(!SaveAllTracks(song)) {
        bRet = false;
        goto Exit;
    }

    // Write the file
    dataSize = (int)(_dataOffset - _data);
    if(!Write(_data, dataSize)) {
        bRet = false;
        goto Exit;
    }

Exit:
    Close();

    free(_data);
    _data = NULL;

    Log("========== MIDI file: end of save ===============");
    return bRet;
}

//=================================================================================================
// Saves MIDI file header
bool MidiFile::SaveHeader(Song* song) {
    ChunkHeader	ChunkHeader;	// chunk header
    FileHeader		FileHeader;		// File header

    // Set chunk ID
    memcpy(ChunkHeader._id, ID_FILE_HEADER, sizeof(ID_FILE_HEADER));

    // Set chunk length
    ChunkHeader._length = sizeof(FileHeader);
    Reverse4Bytes((char*)&ChunkHeader._length); // Length is stored in reversed byte order

    // Save the chunk header
    if(!AddData(&ChunkHeader, sizeof(ChunkHeader)))
        return false;

    // MIDI format
    FileHeader._format = _format;
    Reverse2Bytes((char*)&FileHeader._format);

    // Number of tracks
    FileHeader._tracks = _tracks + 1; // The first track is for song info;
    Reverse2Bytes((char*)&FileHeader._tracks);

    // Time division
    FileHeader._division = _timeDivision;
    Reverse2Bytes((char*)&FileHeader._division);

    // Save the file header
    if(!AddData(&FileHeader, sizeof(FileHeader)))
        return false;

    return true;
}

//=================================================================================================
// Saves song info in the track
bool MidiFile::SaveSongInfo(Song* song) {
    uint8_t		szBuffer[64];	// Conversion buffer
    Buffer		sTrackData;		// Track's data
    int		dwData = 0;		// Converted data

    // Name
    if(song->_name.Length()) {
        if(!SaveMetaEvent(0, META_TRACK_NAME, 0, (const char*)song->_name, song->_name.Length(), sTrackData))
            return false;
    }

    // Copyright
    if(song->_copyright.Length()) {
        if(!SaveMetaEvent(0, META_COPYRIGHT, 0, (const char*)song->_copyright, song->_copyright.Length(),
                          sTrackData))
            return false;
    }

    // Text
    if(song->_text.Length()) {
        if(!SaveMetaEvent(0, META_TEXT, 0, (const char*)song->_text, song->_text.Length(), sTrackData))
            return false;
    }

    // Time signature: 4 bytes
    szBuffer[0] = (uint8_t)song->_measureBeats; // Numerator
    szBuffer[1] = (uint8_t)(log((float)song->_beatNote) / log(2.0f)); // Denominator: power of 2
    szBuffer[2] = 0; // Metronome
    szBuffer[3] = 0; // 32-nd's
    if(!SaveMetaEvent(0, META_TIME_SIGNATURE, 0, (const char*)szBuffer, 4, sTrackData))
        return false;

    // Key signature
    szBuffer[0] = (uint8_t)song->_key; // Key (1 byte)
    szBuffer[1] = (uint8_t)song->_scaleType; // Scale (1 byte)
    if(!SaveMetaEvent(0, META_KEY_SIGNATURE, 0, (const char*)szBuffer, 2, sTrackData))
        return false;

    // Tempo: 3 bytes, microseconds per quarter note
    dwData = (int)(song->_beatTime * (1000.0 * 1000.0) + 0.5);
    Reverse4Bytes((char*)&dwData);
    if(!SaveMetaEvent(0, META_TEMPO, 0, (const char*)&dwData + 1, 3, sTrackData))
        return false;

    // Add 'end of track' event
    if(!SaveMetaEvent(0, META_END, 0, NULL, 0, sTrackData))
        return false;

    // Save song info in a track
    if(!SaveTrack((const char*)sTrackData, sTrackData.Length()))
        return false;

    return true;
}

//=================================================================================================
// Saves info of all song parts
bool MidiFile::SaveParts(Song* song, Buffer& sTrackData) {
    int         nIndex;
    SongPart*  pPart;

    song->_SongParts.MoveToFirst(nIndex);
    while(song->_SongParts.GetNext(nIndex, pPart)) {
        if(!SavePartInfo(0, pPart, sTrackData))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves all song's tracks
bool MidiFile::SaveAllTracks(Song* song) {
    int			channel = 0;		// Track counter
    Track*		track = NULL;	// Current track
    Buffer		sTrackData;		// Track's data
    TRACKS_ITER iter;

    for(iter = song->_tracks.begin(); iter != song->_tracks.end(); iter++) {
        track = &(*iter);

        sTrackData.Reset();

        // Set MIDI channel for drum set
        if(track->_instrument == PERCUSSION)
            track->_channel = DRUM_CHANNEL;
        else
            track->_channel = channel;

        // Put track's info
        track->_trackNumber = channel;
        if(!SaveTrackInfo(track, sTrackData))
            return false;

        // Put track's notes
        if(!SaveTrackNotes(track, sTrackData))
            return false;

        // Save the track
        if(!SaveTrack((const char*)sTrackData, sTrackData.Length()))
            return false;

        channel++;
    }

    return true;
}

//=================================================================================================
// Saves track's info
bool MidiFile::SaveTrackInfo(Track* track, Buffer& sTrackData) {
    float	    fHalf = 127.0f / 2.0f;	// Half of the value range
    uint8_t	    cValue = 0;	    // Control value
    int         nValue;
    String	sString;		// General string

    // Add to the report
    sString.Format("Save track %d '%s' (%s) -------------",
                   track->_trackNumber, (const char*)track->_trackName, (const char*)track->_instrumentName);
    Log((const char*)sString);
    _report += sString;

    // Name
    if(track->_trackName.Length()) {
        if(!SaveMetaEvent(0, META_TRACK_NAME, track->_channel, (const char*)track->_trackName, track->_trackName.Length(), sTrackData))
            return false;
    }

    // Instrument name
    if(track->_instrumentName.Length()) {
        if(!SaveMetaEvent(0, META_INSTRUMENT, track->_channel, (const char*)track->_instrumentName, track->_instrumentName.Length(), sTrackData))
            return false;
    }

    // Instrument type
    nValue = GetProgram(track->_instrument);
    if(nValue != PROGRAM_NONE) {
        if(!SaveEvent(0, MIDI_PROGRAM, track->_channel, (const char*)&nValue, 1, sTrackData))
            return false;
    }

    // Volume
    nValue = (int)(127.0f * track->_volume + 0.5f);
    if(nValue < 0)
        nValue = 0;

    if(nValue > 127)
        nValue = 127;

    cValue = (uint8_t)nValue;
    if(!SaveControlEvent(0, track->_channel, CONTROL_VOLUME, cValue, sTrackData))
        return false;

    // Pan
    nValue = (int)(fHalf + track->_pan * fHalf + 0.5f);
    if(nValue < 0)
        nValue = 0;

    if(nValue > 127)
        nValue = 127;

    cValue = (uint8_t)nValue;
    if(!SaveControlEvent(0, track->_channel, CONTROL_PAN, cValue, sTrackData))
        return false;

    // Reset all controllers
    cValue = 0;
    if(!SaveControlEvent(0, track->_channel, CONTROL_RESET, cValue, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves track notes
bool MidiFile::SaveTrackNotes(Track* track, Buffer& sTrackData) {
    Note*		note = NULL;		// Current note
    Note		Note;				// Current note
    float		dPrevTime = 0.0;	// Last note's time
    int		    nTimeDiff;	        // Time difference
    NOTES	events;				// List of notes as on/off events
    NOTES_ITER noteIter;

    //track->Show();

    // Make a list of all note starts and stops
    for(noteIter = track->_notes.begin(); noteIter != track->_notes.end(); noteIter++) {
        note = &(*noteIter);

        // Save note's start
        events.push_back(*note); // Sorted by start time

        // Save note's end
        Note = *note;
        Note._volume = 0.0; // 'Note off' signal
        Note._start += Note._duration; // Time of the event - note's end
        events.push_back(Note); // Sorted by start time
    }

    std::sort(events.begin(), events.end(), SortNoteTime); // Sort events by start time

    // Save all note starts and stops
    for(noteIter = events.begin(); noteIter != events.end(); noteIter++) {
        note = &(*noteIter);
        nTimeDiff = GetTimeDiff(note->_start - dPrevTime);

        // Save note
        if(!SaveNote(nTimeDiff, true, track->_channel, true, note->_midiNote, note->_volume, sTrackData))
            return false;

        dPrevTime = note->_start;
    }

    // Add a silent note to the end of track. This will prevent MIDI players from cutting the end of track.
    if(!SaveNote(0, true, track->_channel, true, 0, 0.0, sTrackData))
        return false;

    if(!SaveNote(GetTimeDiff(0.5), true, track->_channel, true, 0, 0.0, sTrackData))
        return false;

    // Add 'end of track' event
    if(!SaveMetaEvent(0, META_END, 0, NULL, 0, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves one track
bool MidiFile::SaveTrack(const char* pTrackData, int nTrackLength) {
    ChunkHeader	ChunkHeader;	// chunk header

    // Set chunk ID
    memcpy(ChunkHeader._id, ID_TRACK_HEADER, sizeof(ID_TRACK_HEADER));

    // Set chunk length
    ChunkHeader._length = nTrackLength;
    Reverse4Bytes((char*)&ChunkHeader._length); // Length is stored in reversed byte order

    // Save the chunk header
    if(!AddData(&ChunkHeader, sizeof(ChunkHeader)))
        return false;

    // Save track's data
    if(!AddData((void*)pTrackData, nTrackLength))
        return false;

    return true;
}

//=================================================================================================
// Saves meta event
bool MidiFile::SaveMetaEvent(int nTimeDiff, META_EVENT nMetaEvent, int nChannel, const char* pEventData, int nEventDataSize,
                              Buffer& sTrackData) {
    uint8_t		szBuffer[64];	// Conversion buffer
    int			nBytes = 0;		// Bytes saved in the conversion
    Buffer		sEventData;		// Converted event data
    String	sString;		// General string
    String	sData;

    // Meta event type
    sEventData.Add((const char*)(uint8_t*)&nMetaEvent, 1);

    // Data length
    if(PutVarLen(nEventDataSize, (char*)szBuffer, sizeof(szBuffer), nBytes))
        sEventData.Add((const char*)szBuffer, nBytes);
    else
        return false;

    // Data
    if(pEventData && nEventDataSize)
        sEventData.Add(pEventData, nEventDataSize);

    // Save as event
    if(!SaveEvent(nTimeDiff, MIDI_META, nChannel, (const char*)sEventData, sEventData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves control event
bool MidiFile::SaveControlEvent(int nTimeDiff, int nChannel, uint8_t nType, uint8_t nValue, Buffer& sTrackData) {
    String	sControlName;	// Control name
    String	sString;		// General string
    uint8_t	    szBuffer[8];	// Event data

    // Get control's info
    if(!GetControlInfo(nType, sControlName))
        return false;

    // Add to the report
    sString.Format("Control %3d, '%s' = %d, channel=%d", nType, (const char*)sControlName, nValue, nChannel);
    //Log((const char*)sString);
    _report += sString;

    // Control data
    szBuffer[0] = nType;
    szBuffer[1] = nValue;

    // Save the event
    if(!SaveEvent(nTimeDiff, MIDI_CONTROL, nChannel, (const char*)szBuffer, 2, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves a marker note
bool MidiFile::SaveMarker(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    switch(note->_nState) {
        case Note::STATE_CHORD_MARK:
            if(!SaveChord(nTimeDiff, note, track, sTrackData))
                return false;

            break;

        case Note::STATE_PART_MARK:
            if(!SaveSongPart(nTimeDiff, note, track, sTrackData))
                return false;

            break;

        case Note::STATE_MEASURE_MARK:
            if(!SaveMeasure(nTimeDiff, note, track, sTrackData))
                return false;

            break;

        default:
            return false;
    }

    return true;
}

//=================================================================================================
// Saves note's chord info using MIDI control events
bool MidiFile::SaveChord(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_CHORD;

    // Root MIDI note
    sPair.Format("%s%d|", MARKER_FIELD_MIDI_NOTE, note->_ChordInfo._nRoot);
    sData += sPair;

    // Root # or b
    if(!NoteManager::NoteTypeToString(note->_ChordInfo._Modifiers._Pitch, sValue))
        return false;

    sPair.Format("%s%s|", MARKER_FIELD_PITCH, (const char*)sValue);
    sData += sPair;

    // Chord type
    if(!NoteManager::ChordTypeToString(note->_ChordInfo._nType, sValue))
        return false;

    sPair.Format("%s%s|", MARKER_FIELD_CHORD_TYPE, (const char*)sValue);
    sData += sPair;

    // Chord group
    if(!NoteManager::GetChordGroupName(note->_ChordInfo._nGroup, sValue))
        return false;

    sPair.Format("%s%s|", MARKER_FIELD_CHORD_GROUP, (const char*)sValue);
    sData += sPair;

    // Chord number in the scale
    sPair.Format("%s%d|", MARKER_FIELD_NUMBER, note->_ChordInfo._nNumber);
    sData += sPair;

    // Scale
    if(!NoteManager::GetScaleName(note->_ChordInfo._nScale, sValue))
        return false;

    sPair.Format("%s%s|", MARKER_FIELD_SCALE, (const char*)sValue);
    sData += sPair;

    // Scale root note
    sPair.Format("%s%d|", MARKER_FIELD_SCALE_ROOT, note->_ChordInfo._nScaleRoot);
    sData += sPair;

    // Add to the report
    sString.Format("Chord: %s", (const char*)sData);
    //Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets chord data
bool MidiFile::GetChord(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    CChord      Chord;
    CChord*     pChord;
    Note       Note;
    String sString;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_MIDI_NOTE, strlen(MARKER_FIELD_MIDI_NOTE))) { // Root MIDI note
            pValue = pField + strlen(MARKER_FIELD_MIDI_NOTE);
            Chord._RootNote._ChordInfo._nRoot = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_PITCH, strlen(MARKER_FIELD_PITCH))) { // Root # or b
            pValue = pField + strlen(MARKER_FIELD_PITCH);
            if(!NoteManager::StringToNoteType(pValue, Chord._RootNote._Modifiers._Pitch))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_CHORD_TYPE, strlen(MARKER_FIELD_CHORD_TYPE))) { // Chord type
            pValue = pField + strlen(MARKER_FIELD_CHORD_TYPE);
            if(!NoteManager::StringToChordType(pValue, Chord._RootNote._ChordInfo._nType))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_REPEAT, strlen(MARKER_FIELD_REPEAT))) { // Number of times to repeat
            pValue = pField + strlen(MARKER_FIELD_REPEAT);
            Chord._nRepeat = 1; //atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_CHORD_GROUP, strlen(MARKER_FIELD_CHORD_GROUP))) { // Chord group
            pValue = pField + strlen(MARKER_FIELD_CHORD_GROUP);
            if(!NoteManager::GetChordGroup(pValue, Chord._RootNote._ChordInfo._nGroup))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_NUMBER, strlen(MARKER_FIELD_NUMBER))) { // Chord number in the scale
            pValue = pField + strlen(MARKER_FIELD_NUMBER);
            Chord._RootNote._ChordInfo._nNumber = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_SCALE, strlen(MARKER_FIELD_SCALE))) { // Scale
            pValue = pField + strlen(MARKER_FIELD_SCALE);
            if(!NoteManager::GetScaleID(pValue, Chord._RootNote._ChordInfo._nScale))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_SCALE_ROOT, strlen(MARKER_FIELD_SCALE_ROOT))) { // Scale root note
            pValue = pField + strlen(MARKER_FIELD_SCALE_ROOT);
            Chord._RootNote._ChordInfo._nScaleRoot = atoi(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = dTrackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._nState = Note::STATE_CHORD_MARK;
    Note._ChordInfo = Chord._RootNote._ChordInfo;
    Note._nSource = Note::SOURCE_FILE;

    track->AddNote(Note);

    // Add to the report
    sString.Format("%6.2f  Chord: %d", dTrackTime, Chord._RootNote._ChordInfo._nRoot);
    //Log((const char*)sString);
    _report += sString;

    return true;
}

//=================================================================================================
// Saves mix data
bool MidiFile::SaveMix(int nTimeDiff, CMix* pMix, Buffer& sTrackData) {
    MIX_MAP_ITER    Iter;	    // Map iterator
    MIX_INFO*       pMixInfo;   // Mix info
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_MIX;

    for(Iter = pMix->_Mixes.begin(); Iter != pMix->_Mixes.end(); Iter++) {
        pMixInfo = &Iter->second;

        // Instrument
        if(ThingToString(pMixInfo->_instrument, sString)) {
            sPair.Format("%s%s|", MARKER_FIELD_INSTRUMENT, (const char*)sString);
            sData += sPair;
        } else {
            return false;
        }

        // Volume
        sPair.Format("%s%0.2f|", MARKER_FIELD_VOLUME, pMixInfo->_volume);
        sData += sPair;

        // Pan
        sPair.Format("%s%0.2f|", MARKER_FIELD_PAN, pMixInfo->_pan);
        sData += sPair;
    }

    // Add to the report
    sString.Format("Mix: %s", (const char*)sData);
    //Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets mix data
bool MidiFile::GetMix(const char* pMarker, Song* song) {
    MIX_MAP_INS_PAIR	Pair;	    // Insert pair
    MIX_INFO    MixInfo;    // Mix info
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	    // String copy
    const char* pValue;
    THING       nInstrument;        // Source instrument
    THING       nToInstrument;      // Mapped instrument

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_INSTRUMENT, strlen(MARKER_FIELD_INSTRUMENT))) { // Instrument
            // Save the previous intrument
            if(MixInfo._instrument != 0) {
                // Insert the info
                Pair = song->_Mix._Mixes.insert(MIX_MAP_PAIR(MixInfo._instrument, MixInfo));
                if(!Pair.second) { // Can't insert
                    _error.Format("Can't insert into mix map");
                    return false;
                }
            }

            pValue = pField + strlen(MARKER_FIELD_INSTRUMENT);
            if(!StringToThing(pValue, nInstrument)) {
                return false;
            }

            MapThings(nInstrument, nToInstrument);
            MixInfo._instrument = nToInstrument;
        } else if(!strncmp(pField, MARKER_FIELD_VOLUME, strlen(MARKER_FIELD_VOLUME))) { // Volume
            pValue = pField + strlen(MARKER_FIELD_VOLUME);
            MixInfo._volume = (float)atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_PAN, strlen(MARKER_FIELD_PAN))) { // Pan
            pValue = pField + strlen(MARKER_FIELD_PAN);
            MixInfo._pan = (float)atof(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    // Save the last intrument
    if(MixInfo._instrument != 0) {
        // Insert the info
        Pair = song->_Mix._Mixes.insert(MIX_MAP_PAIR(MixInfo._instrument, MixInfo));
        if(!Pair.second) { // Can't insert
            _error.Format("Can't insert into mix map");
            return false;
        }
    }

    return true;
}

//=================================================================================================
// Saves song info
bool MidiFile::SaveSongInfo(Song* song, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_SONG_INFO;

    // Description
    sPair.Format("%s%s|", MARKER_FIELD_DESCR, (const char*)song->_sDescription);
    sData += sPair;

    // Repeat
    sPair.Format("%s%d|", MARKER_FIELD_REPEAT, song->_nRepeat);
    sData += sPair;

    // Song length (without repetition)
    if(song->_dSongTime != NO_DOUBLE) {
        sPair.Format("%s%0.3f|", MARKER_FIELD_SONG_LENGTH, song->_dSongTime);
        sData += sPair;
    }

    // Add to the report
    sString.Format("Song info: %s", (const char*)sData);
    Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(0, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song info
bool MidiFile::GetSongInfo(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_DESCR, strlen(MARKER_FIELD_DESCR))) { // Description
            pValue = pField + strlen(MARKER_FIELD_DESCR);
            song->_sDescription = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_REPEAT, strlen(MARKER_FIELD_REPEAT))) { // Repeat
            pValue = pField + strlen(MARKER_FIELD_REPEAT);
            song->_nRepeat = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_SONG_LENGTH, strlen(MARKER_FIELD_SONG_LENGTH))) { // Song length
            pValue = pField + strlen(MARKER_FIELD_SONG_LENGTH);
            song->_dSongTime = atof(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    return true;
}

//=================================================================================================
// Saves full info of one song part
bool MidiFile::SavePartInfo(int nTimeDiff, SongPart* pPart, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_SONG_PART_INFO;

    // Name
    sPair.Format("%s%s|", MARKER_FIELD_NAME, (const char*)pPart->_name);
    sData += sPair;

    // Description
    sPair.Format("%s%s|", MARKER_FIELD_DESCR, (const char*)pPart->_sDescription);
    sData += sPair;

    // Repeat
    sPair.Format("%s%d|", MARKER_FIELD_REPEAT, pPart->_nRepeat);
    sData += sPair;

    // Groove sytle
    sPair.Format("%s%s|", MARKER_FIELD_STYLE, (const char*)pPart->_sGrooveStyle);
    sData += sPair;

    // Groove name
    sPair.Format("%s%s|", MARKER_FIELD_GROOVE_NAME, (const char*)pPart->_sGrooveName);
    sData += sPair;

    // Groove file
    sPair.Format("%s%s|", MARKER_FIELD_GROOVE_FILE, (const char*)pPart->_sGrooveFile);
    sData += sPair;

    // Groove length
    sPair.Format("%s%0.3f|", MARKER_FIELD_GROOVE_LENGTH, pPart->_dGrooveLength);
    sData += sPair;

    // Part length
    sPair.Format("%s%0.3f|", MARKER_FIELD_PART_LENGTH, pPart->_Part._dSongTime);
    sData += sPair;

    // Scale
    if(!NoteManager::GetScaleName(pPart->_Part._nScale, sValue))
        return false;

    sPair.Format("%s%s|", MARKER_FIELD_SCALE, (const char*)sValue);
    sData += sPair;

    // Scale root note
    sPair.Format("%s%d|", MARKER_FIELD_SCALE_ROOT, pPart->_Part._nScaleRoot);
    sData += sPair;

    // Tempo
    sPair.Format("%s%0.2f|", MARKER_FIELD_TEMPO, pPart->_Part._dTempo);
    sData += sPair;

    // Beat time
    sPair.Format("%s%0.3f|", MARKER_FIELD_BEAT_TIME, pPart->_Part._beatTime);
    sData += sPair;

    // Measures
    sPair.Format("%s%d|", MARKER_FIELD_MEASURES, pPart->_Part._measures);
    sData += sPair;

    // Beat notes
    sPair.Format("%s%d|", MARKER_FIELD_BEAT_NOTES, pPart->_Part._beatNotes);
    sData += sPair;

    // Measure beats
    sPair.Format("%s%d|", MARKER_FIELD_MEASURE_BEATS, pPart->_Part._measureBeats);
    sData += sPair;

    // Add to the report
    sString.Format("Song part info: %s", (const char*)sData);
    Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song part info
bool MidiFile::GetPartInfo(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    SongPart   Part;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_NAME, strlen(MARKER_FIELD_NAME))) { // Part's name
            pValue = pField + strlen(MARKER_FIELD_NAME);
            Part._name = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_DESCR, strlen(MARKER_FIELD_DESCR))) { // Description
            pValue = pField + strlen(MARKER_FIELD_DESCR);
            Part._sDescription = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_REPEAT, strlen(MARKER_FIELD_REPEAT))) { // Repeat
            pValue = pField + strlen(MARKER_FIELD_REPEAT);
            Part._nRepeat = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_STYLE, strlen(MARKER_FIELD_STYLE))) { // Groove sytle
            pValue = pField + strlen(MARKER_FIELD_STYLE);
            Part._sGrooveStyle = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_GROOVE_NAME, strlen(MARKER_FIELD_GROOVE_NAME))) { // Groove name
            pValue = pField + strlen(MARKER_FIELD_GROOVE_NAME);
            Part._sGrooveName = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_GROOVE_FILE, strlen(MARKER_FIELD_GROOVE_FILE))) { // Groove file
            pValue = pField + strlen(MARKER_FIELD_GROOVE_FILE);
            Part._sGrooveFile = pValue;
        } else if(!strncmp(pField, MARKER_FIELD_GROOVE_LENGTH, strlen(MARKER_FIELD_GROOVE_LENGTH))) { // Groove length
            pValue = pField + strlen(MARKER_FIELD_GROOVE_LENGTH);
            Part._dGrooveLength = atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_PART_LENGTH, strlen(MARKER_FIELD_PART_LENGTH))) { // Part length
            pValue = pField + strlen(MARKER_FIELD_PART_LENGTH);
            Part._Part._dSongTime = atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_SCALE, strlen(MARKER_FIELD_SCALE))) { // Scale
            pValue = pField + strlen(MARKER_FIELD_SCALE);
            if(!NoteManager::GetScaleID(pValue, Part._Part._nScale))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_SCALE_ROOT, strlen(MARKER_FIELD_SCALE_ROOT))) { // Scale root note
            pValue = pField + strlen(MARKER_FIELD_SCALE_ROOT);
            Part._Part._nScaleRoot = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_TEMPO, strlen(MARKER_FIELD_TEMPO))) { // Tempo
            pValue = pField + strlen(MARKER_FIELD_TEMPO);
            Part._Part._dTempo = atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_BEAT_TIME, strlen(MARKER_FIELD_BEAT_TIME))) { // Beat time
            pValue = pField + strlen(MARKER_FIELD_BEAT_TIME);
            Part._Part._beatTime = atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_MEASURES, strlen(MARKER_FIELD_MEASURES))) { // Measures
            pValue = pField + strlen(MARKER_FIELD_MEASURES);
            Part._Part._measures = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_BEAT_NOTES, strlen(MARKER_FIELD_BEAT_NOTES))) { // Beat notes
            pValue = pField + strlen(MARKER_FIELD_BEAT_NOTES);
            Part._Part._beatNotes = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_MEASURE_BEATS, strlen(MARKER_FIELD_MEASURE_BEATS))) { // Measure beats
            pValue = pField + strlen(MARKER_FIELD_MEASURE_BEATS);
            Part._Part._measureBeats = atoi(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    // Save the part
    if(!song->AddSongPart(&Part))
        return false;

    return true;
}

//=================================================================================================
// Saves note info
bool MidiFile::SaveNoteInfo(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_NOTE_INFO;

    // Interval
    if(note->_nInterval != 0) {
        sPair.Format("%s%d|", MARKER_FIELD_INTERVAL, note->_nInterval);
        sData += sPair;
    }

    // String
    if(note->_Tab._nString != NO_INT && note->_Tab._nString != 0) {
        sPair.Format("%s%d|", MARKER_FIELD_STRING, note->_Tab._nString);
        sData += sPair;
    }

    // Fret
    if(note->_Tab._nFret != NO_INT) {
        sPair.Format("%s%d|", MARKER_FIELD_FRET, note->_Tab._nFret);
        sData += sPair;
    }

    // Finger
    if(note->_Tab._nFinger != NO_INT) {
        sPair.Format("%s%d|", MARKER_FIELD_FINGER, note->_Tab._nFinger);
        sData += sPair;
    }

    // Mode
    if(note->_Tab._nMode != Note::TAB_INFO::MODE_NONE) {
        sPair.Format("%s%d|", MARKER_FIELD_MODE, note->_Tab._nMode);
        sData += sPair;
    }

    // Add to the report
    sString.Format("Note info: %s", (const char*)sData);
    //Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets note info
bool MidiFile::GetNoteInfo(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_INTERVAL, strlen(MARKER_FIELD_INTERVAL))) { // Interval
            pValue = pField + strlen(MARKER_FIELD_INTERVAL);
            _note._nInterval = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_STRING, strlen(MARKER_FIELD_STRING))) { // Tab - string
            pValue = pField + strlen(MARKER_FIELD_STRING);
            _note._Tab._nString = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_FRET, strlen(MARKER_FIELD_FRET))) { // Tab - fret
            pValue = pField + strlen(MARKER_FIELD_FRET);
            _note._Tab._nFret = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_FINGER, strlen(MARKER_FIELD_FINGER))) { // Tab - finger
            pValue = pField + strlen(MARKER_FIELD_FINGER);
            _note._Tab._nFinger = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_MODE, strlen(MARKER_FIELD_MODE))) { // Tab - mode
            pValue = pField + strlen(MARKER_FIELD_MODE);
            _note._Tab._nMode = atoi(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    return true;
}

//=================================================================================================
// Saves song part's name to mark the start of new part
bool MidiFile::SaveSongPart(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_SONG_PART;

    // Part's name
    sPair.Format("%s%s|", MARKER_FIELD_NAME, (const char*)note->_sSongPart);
    sData += sPair;

    // Add to the report
    sString.Format("Song part: %s", (const char*)sData);
    Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song part name
bool MidiFile::GetSongPart(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    Note       Note;
    String sString;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_NAME, strlen(MARKER_FIELD_NAME))) { // Part's name
            pValue = pField + strlen(MARKER_FIELD_NAME);
            Note._sSongPart = pValue;
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = dTrackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._nState = Note::STATE_PART_MARK;
    Note._nSource = Note::SOURCE_FILE;

    track->AddNote(Note);

    // Add to the report
    sString.Format("%6.2f  Song part: %s", dTrackTime, (const char*)Note._sSongPart);
    Log((const char*)sString);
    _report += sString;

    return true;
}

//=================================================================================================
// Saves measure numbers using MIDI control events
bool MidiFile::SaveMeasure(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String sString;

    sData = MARKER_MEASURE;

    // Part measure
    sPair.Format("%s%d|", MARKER_FIELD_PART_MEASURE, note->_nPartMeasure);
    sData += sPair;

    // Song measure
    sPair.Format("%s%d|", MARKER_FIELD_SONG_MEASURE, note->_nSongMeasure);
    sData += sPair;

    // Add to the report
    sString.Format("Measure: %s", (const char*)sData);
    //Log((const char*)sString);
    _report += sString;

    // Save the data
    if(!SaveMetaEvent(nTimeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets measure numbers
bool MidiFile::GetMeasure(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    char*		pField = NULL;	// Current date field
    char*		pNextToken = NULL;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    Note       Note;
    String sString;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_PART_MEASURE, strlen(MARKER_FIELD_PART_MEASURE))) { // Part measure
            pValue = pField + strlen(MARKER_FIELD_PART_MEASURE);
            Note._nPartMeasure = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_SONG_MEASURE, strlen(MARKER_FIELD_SONG_MEASURE))) { // Song measure
            pValue = pField + strlen(MARKER_FIELD_SONG_MEASURE);
            Note._nSongMeasure = atoi(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(NULL, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = dTrackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._nState = Note::STATE_MEASURE_MARK;
    Note._nSource = Note::SOURCE_FILE;

    track->AddNote(Note);

    // Add to the report
    sString.Format("%6.2f  Measure: %d/%d", dTrackTime, Note._nPartMeasure, Note._nSongMeasure);
    //Log((const char*)sString);
    _report += sString;

    return true;
}

//=================================================================================================
// Gets marker data: chord info, measure mark, song part's name, etc.
// Format: <MarkerType>Field=Value|Field=Value
bool MidiFile::GetMarker(const char* pMarker, float dTrackTime, Song* song, Track* track) {
    if(!strncmp(pMarker, MARKER_CHORD, strlen(MARKER_CHORD))) { // Chord
        if(!GetChord(pMarker + strlen(MARKER_CHORD), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_PART, strlen(MARKER_SONG_PART))) { // Song part
        if(!GetSongPart(pMarker + strlen(MARKER_SONG_PART), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_PART_INFO, strlen(MARKER_SONG_PART_INFO))) { // Song part info
        if(!GetPartInfo(pMarker + strlen(MARKER_SONG_PART_INFO), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_NOTE_INFO, strlen(MARKER_NOTE_INFO))) { // Note info
        if(!GetNoteInfo(pMarker + strlen(MARKER_NOTE_INFO), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_INFO, strlen(MARKER_SONG_INFO))) { // Song part info
        if(!GetSongInfo(pMarker + strlen(MARKER_SONG_INFO), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_MEASURE, strlen(MARKER_MEASURE))) { // Measure number
        if(!GetMeasure(pMarker + strlen(MARKER_MEASURE), dTrackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_MIX, strlen(MARKER_MIX))) { // Mix
        if(!GetMix(pMarker + strlen(MARKER_MIX), song))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves note's tab info using MIDI control events
bool MidiFile::SaveTab(int nTimeDiff, Note* note, Track* track, Buffer& sTrackData) {
    // String
    if(note->_Tab._nString != NO_INT) {
        if(!SaveControlEvent(nTimeDiff, track->_channel, CONTROL_TAB_STRING, note->_Tab._nString, sTrackData))
            return false;
    }

    // Fret
    if(note->_Tab._nFret != NO_INT) {
        if(!SaveControlEvent(nTimeDiff, track->_channel, CONTROL_TAB_FRET, note->_Tab._nFret, sTrackData))
            return false;
    }

    // Finger
    if(note->_Tab._nFinger != NO_INT) {
        if(!SaveControlEvent(nTimeDiff, track->_channel, CONTROL_TAB_FINGER, note->_Tab._nFinger, sTrackData))
            return false;
    }

    // Mode
    if(note->_Tab._nMode != Note::TAB_INFO::MODE_NONE) {
        if(!SaveControlEvent(nTimeDiff, track->_channel, CONTROL_TAB_MODE, note->_Tab._nMode, sTrackData))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves control event
// bEvent - whether to save event ID or keep the 'running status'
bool MidiFile::SaveNote(int nTimeDiff, bool bEvent, int nChannel, bool bOn, int nNote, float fVolume, Buffer& sTrackData) {
    uint8_t	    szBuffer[8];	// Event data
    String	sString;		// General string

    // Note data
    szBuffer[0] = nNote;

    // Velocity (volume). Looks like it's common not to use 'note off' event, but use 'note on' with velocity set to 0.
    if(bOn) { // Start of the note
        //szBuffer[1] = (uint8_t)(127.0f * fVolume + 0.5f); // Linear conversion
        szBuffer[1] = VolumeToMidi(fVolume); // Logarithmic
    } else // End of the note
        szBuffer[1] = 0;

    sString.Format("%7.3f sec: Save note %d, volume %0.2f, velocity %d", GetTimeDiff(nTimeDiff), nNote, fVolume, szBuffer[1]);
    Log((const char*)sString);

    // Save the event
    if(!SaveEvent(nTimeDiff, bEvent ? MIDI_NOTE_ON : MIDI_NONE, nChannel, (const char*)szBuffer, 2, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves a MIDI (note, system or meta) event
bool MidiFile::SaveEvent(int nTimeDiff, MIDI_EVENT nEvent, int nChannel, const char* pEventData, int nEventDataSize, Buffer& sTrackData) {
    uint8_t	    szBuffer[64];	// Conversion buffer
    float      dTimeDiff;		// Event time
    int		    nBytes = 0;		// Bytes saved in the conversion
    uint8_t	    nEventID = 0;	// Event ID and channel (4 bits and 4 bits)
    String	sString;		// General string

    // Event time (incremental) ----------
    if(PutVarLen(nTimeDiff, (char*)szBuffer, sizeof(szBuffer), nBytes))
        sTrackData.Add((const char*)szBuffer, nBytes);
    else
        return false;

    // Add event time to the report
    dTimeDiff = GetTimeDiff((int)nTimeDiff);
    sString.Format("Event diff: %04d, %7.3f sec, beat=%7.3f", nTimeDiff, dTimeDiff, _beatTime);
    //Log((const char*)sString);
    _report += sString;

    // Event ID and channel ----------
    if(nEvent != MIDI_NONE) { // Note a MIDI note event, no channel
        if(nEvent >= MIDI_SYSTEM) { // Note a MIDI note event, no channel
            nEventID = (uint8_t)nEvent;
        } else { // Set the channel
            // 4 bits for event type, 4 bits for channel number
            nEventID = ((uint8_t)nEvent) & 0xF0; // Event type
            nEventID += ((uint8_t)nChannel) & 0x0F; // Channel number;
        }

        sTrackData.Add((const char*)(uint8_t*)&nEventID, 1);
    }

    // Event data ----------
    sTrackData.Add(pEventData, nEventDataSize);

    return true;
}

//=================================================================================================
// Gets MIDI program number for an instrument (thing) type
uint8_t MidiFile::GetProgram(INSTRUMENT instrument) {
    switch(instrument) {
        case ELECTRIC_BASS_FINGER:
            return PROGRAM_FINGER_BASS;
        case ELECTRIC_GUITAR_CLEAN:
            return PROGRAM_CLEAN_GUITAR;
        case OVERDRIVEN_GUITAR:
            return PROGRAM_OVERDRIVEN_GUITAR;
        case DISTORTION_GUITAR:
            return PROGRAM_DISTORTION_GUITAR;
        case PERCUSSION:
            return PROGRAM_DRUM_SET;
        default:
            return PROGRAM_NONE;
    }
}

//=================================================================================================
// Logs a message
bool MidiFile::Log(const char* pFormat, const char* pMessage) {

    return true;
}

//=================================================================================================
// Adds data to the buffer
bool MidiFile::AddData(void* data, int size) {
    int increment = 1024 * 100;
    int dataSize;

    // Check the buffer size
    dataSize = (int)(_dataOffset - _data);
    if(dataSize + size > (int)_dataSize) {
        _dataSize += Max(increment, size);
        _data = (char*)realloc(_data, _dataSize); // +1 is for zero-terminator
        if(!_data) {
            _error.Format("Can't allocate %d bytes", _dataSize);
            return false;
        }

        _dataOffset = _data + dataSize;
    }

    // Add the data
    memcpy(_dataOffset, data, size);
    _dataOffset += size;

    return true;
}
*/
