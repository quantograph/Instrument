// For MIDI specifications, see "midiformat.pdf" in Docs folder, 
// taken from here: http://www.music.mcgill.ca/~ich/classes/mumt306/midiformat.pdf
// Also here: http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
// Instruments: https://wiki.musink.net/doku.php/midi:instrument
// https://en.wikipedia.org/wiki/General_MIDI

#include "../Devices/Devices.h"
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
float MidiFile::getBeatTime(float time) {
    float beatTime;

    if(_timeDivision == 0) {
        return false;
    }

    beatTime = (float)((int32_t)(time * _timeDivision + 0.5)) / _timeDivision;

    return beatTime;
}

//=================================================================================================
int32_t MidiFile::getTimeDiff(float dTimeDiff) {
    if(dTimeDiff == 0.0)
        return 0;

    if(_beatTime == 0.0)
        return 0;

    return (int32_t)((dTimeDiff / _beatTime) * _timeDivision + 0.5);
}

//=================================================================================================
float MidiFile::getTimeDiff(int32_t timeDiff) {
    if(timeDiff == 0)
        return 0.0;

    if(_beatTime == 0.0)
        return 0;

    return _beatTime * ((float)timeDiff / _timeDivision);
}

//=================================================================================================
// Reads the MIDI file into a Song object
bool MidiFile::read(char* data, uint32_t size, Song* song) {
    Serial.printf("===== Reading MIDI file, %d bytes\n", size);
    _data = data;
    _dataSize = size;
    _dataRead = 0;

    _beatTime = 0.0;
    _timeDivision = 0;

    // Get all chunks
    if(!getChunks(song))
        return false;

    return true;
}

//=================================================================================================
// Gets all file chunks
bool MidiFile::getChunks(Song* song) {
    ChunkInfo chunk{}; // Current chunk info
    bool done{false}; // Whether done reading
    uint32_t fileTrack{1}; // Track number in the file
    uint32_t saveTrack{1}; // Track number to be saved
    Track* track{nullptr}; // Current track info

    // Read all chunks
    _dataOffset = _data;
    do {
        // Read the next chunk
        if(!nextChunk(chunk, done)) {
            return false;
        }

        if(done) // No more data
            break;

        // Check for file header - "MThd" chunk
        if(!strncmp(chunk._header._id, ID_FILE_HEADER, strlen(ID_FILE_HEADER))) { // File header
            if(!getFileHeader(chunk, song))
                return false;
        } else if(!strncmp(chunk._header._id, ID_TRACK_HEADER, strlen(ID_TRACK_HEADER))) { // Track
            // Get track's data
            track = new Track();
            track->_trackNumber = fileTrack;
            if(!getTrack(chunk, song, track))
                return false;

            // Save this track into the song
            if(_format == 1 && fileTrack == 1) {
                // Don't save first track if MIDI file format is 1. It only has song's data, no track notes.
            } else {
                sortNotes(&_notes);
                song->_tracks.push_back(track);
                saveTrack++;
            }

            fileTrack++;
        } else { // Unknown chunk, ignore
            char id[5];
            memcpy(id, chunk._header._id, 4);
            id[4] = 0;
            Serial.printf("Unknown chunk '%d' of %d bytes, ignoring\n", id, chunk._header._length);
        }

    } while(!done);

    return true;
}

//=================================================================================================
// Gets the next chunk from the file
bool MidiFile::nextChunk(ChunkInfo& chunk, bool& done) {
    done = false; // Whether done processing

    // Read this chunk's header
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
    reverse4Bytes((char*)&chunk._header._length); // Length is stored in reversed byte order

    // Find the data
    if(_dataOffset + chunk._header._length > _data + _dataSize) { // Make sure we have enought data in the file
        done = true;
        Serial.printf("ERROR: Not enought data in the file for the chunk data: need %d, have only %d left\n", 
                      chunk._header._length, _data - _dataOffset);
        return false;
    }

    if(chunk._header._length > 0)
        chunk._data = _dataOffset;
    else
        chunk._data = nullptr; // No data for this chunk

    _dataOffset += chunk._header._length;

    // Show chunk info
    char id[5];
    memcpy(id, chunk._header._id, 4);
    id[4] = 0;
    Serial.printf("Chunk '%s', size=%d\n", id, chunk._header._length);

    return true;
}

//=================================================================================================
// Get file's header
bool MidiFile::getFileHeader(ChunkInfo& chunk, Song* song) {
    FileHeader*	header{nullptr}; // File header

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
    reverse2Bytes((char*)&header->_format);
    _format = header->_format;

    // Number of tracks
    reverse2Bytes((char*)&header->_tracks);
    _tracks = header->_tracks;

    // Time division.
    // If positive, it's a number of ticks per quarter note (beat).
    reverse2Bytes((char*)&header->_division);
    _timeDivision = header->_division;

    Serial.printf("File header: format=%d, tracks=%d, timeDivision=%d\n", _format, _tracks, _timeDivision);

    return true;
}

//=================================================================================================
// Reads one track
bool MidiFile::getTrack(ChunkInfo& chunk, Song* song, Track* track) {
    uint32_t processed{0}; // Bytes processed
    uint32_t size{0}; // Field size
    uint32_t bytes{0}; // Bytes in the data
    int32_t time{0}; // Event's time (offset)
    int32_t trackTimeCount{0}; // Event's time (absolute)
    uint8_t eventId{0}; // Event ID
    uint8_t prevEvent{0}; // Previous eventId
    float trackTime{0.0}; // Current track time, in seconds
    char* eventData{nullptr}; // Event data
    uint32_t eventLength{0}; // Event's data length
    Note* note{nullptr};// Current note
    uint32_t i{0}; // Loop counter

    resetNotes(); // Clear all started notes

    // Process all events in the track
    while(processed < chunk._header._length) {
        // Get the time
        size = std::min((uint32_t)4, chunk._header._length - processed);
        getVarLen(chunk._data + processed, size, time, bytes);
        processed += bytes;

        // Update track time.
        // Event time is relative to the time of the last eventId.
        trackTimeCount += time;
        trackTime = getTimeDiff(trackTimeCount);

        // Get the eventId ID
        eventId = *(uint8_t*)(chunk._data + processed);
        eventLength = chunk._header._length - processed;
        processed += 1;

        // Process the eventId
        if(eventId == MIDI_META) { // Meta eventId
            eventData = chunk._data + processed;
            if(!getMetaEvent(eventData, eventLength, bytes, trackTime, song, track)) {
                Serial.printf("ERROR: getMetaEvent\n");
                return false;
            }

            processed += bytes;
        } else if(eventId == MIDI_SYSTEM) { // System eventId
            eventData = chunk._data + processed;
            if(!getSystemEvent(eventData, eventLength, bytes)) {
                Serial.printf("ERROR: getSystemEvent\n");
                return false;
            }

            processed += bytes;
        } else if(eventId >= 0x80 && eventId <= 0xEF) { // MIDI eventId
            eventData = chunk._data + processed;
            if(!getMidiEvent(eventId, eventData, eventLength, bytes, trackTime, track)) {
                Serial.printf("ERROR: getMidiEvent\n");
                return false;
            }

            processed += bytes;
            prevEvent = eventId;
        } else if(eventId < 0x80) { // Running status
            processed--; // Go back to the eventId data, there was no eventId ID
            eventData = chunk._data + processed;
            if(!getMidiEvent(prevEvent, eventData, eventLength, bytes, trackTime, track)) {
                Serial.printf("ERROR: getMidiEvent, running status\n");
                return false;
            }

            processed += bytes;
        } else { // Unknown eventId
            // If we don't know the eventId, we don't know it's length and can't skip it.
            Serial.printf("ERROR: Unknown eventId\n");
            return false;
        }
    }

    if(track->_instrument == NONE) {
        // End all notes which were started, but not finished
        size = sizeof(_notes) / sizeof(Note);
        for(i = 0; i < size; i++) {
            note = &_notes[i];
            if(note->_state == Note::STATE_START) // This note was started
                addNote(note, song->_songTime, track);
        }
    }

    // If there is a silent note at the end of the track, remove it.
    // It was added in MidiFile::SaveTrackNotes to make MIDI players not to cut off the end of songs.
    /*if(track->_notes.GetLast(index, note)) {
        if(note->_volume == 0.0f && !note->IsMarker())
            track->_notes.Delete(index);
    }*/

    // Make sure all data was processed
    if(processed != chunk._header._length)
        Serial.printf("ERROR: processed %d track bytes out of %d\n", processed, chunk._header._length);
    else
        Serial.printf("Processed %d track bytes\n", processed);

    return true;
}

//=================================================================================================
// Get meta eventId
bool MidiFile::getMetaEvent(char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Song* song, Track* track) {
    uint8_t type{0}; // Event type
    uint32_t bytes{0}; // Bytes in the variable lenght data
    int32_t length{0}; // Data length
    String name{}; // Event name
    char* metaData{nullptr}; // Data of the meta eventId
    int32_t dataBuffer{0}; // Data buffer for 4 bytes
    String string{}; // General string
    String marker{}; // Marker string

    processed = 0;

    // Get the meta eventId type
    type = *(uint8_t*)data;
    processed += 1;

    // Get data length
    getVarLen(data + processed, dataSize - processed, length, bytes);
    processed += bytes;

    // Get the data
    metaData = data + processed;

    switch(type) {
        case META_SEQUENCE: // Sequence number: 2 bytes (MSB, LSB)
            name = "Sequence";
            break;

        case META_TEXT: // Text eventId: length, text
            name = "Text";
            /*track->_sText.Copy(metaData, length);

            // For MIDI format 1, song's text is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_sText = track->_sText;

            Serial.printf("Text '%s'\n", (const char*)track->_sText);*/
            break;

        case META_COPYRIGHT: // Copyright notice: length, text
            name = "Copyright";
            /*track->_sCopyright.Copy(metaData, length);

            // For MIDI format 1, song's text is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_sCopyright = track->_sCopyright;

            Serial.printf("Copyright '%s'\n", (const char*)track->_sCopyright);*/
            break;

        case META_TRACK_NAME: // Sequence or track name: length, text
            name = "Track name";
            track->_name.copy(metaData, length);

            // For MIDI format 1, song name is stored in the first track
            if(_format == 1 && track->_trackNumber == 1)
                song->_name = track->_name;

            Serial.printf("Track '%s'\n", track->_name.c_str());
            break;

        case META_INSTRUMENT: // Instrument name: length, text
            name = "Instrument";
            track->_instrumentName.copy(metaData, length);
            Serial.printf("Instrument '%s'\n", track->_instrumentName.c_str());
            break;

        case META_LYRICS: // Lyric text: length, text
            name = "Lyric";
            break;

        case META_MARKER: // Marker text: length, text
            name = "Marker";
            marker.copy(metaData, length);
            //GetMarker(marker, trackTime, song, track);
            Serial.printf("Marker '%s'\n", marker.c_str());
            break;

        case META_CUE: // Cue point: length, text
            name = "Cue";
            break;

        case META_CHANNEL: // MIDI channel prefix assignment: 1 byte, 0-15
            name = "Channel";
            break;

        case META_END: // End of track, no data
            name = "End of track";
            break;

        case META_TEMPO: // Tempo setting: 3 bytes, microseconds per quarter note
            name = "Tempo";
            memcpy((char*)(&dataBuffer) + 1, metaData, 3);
            reverse4Bytes((char*)&dataBuffer);
            song->_tempo = (float)60 * 1000 * 1000 / dataBuffer; // Tempo is microseconds per minute, divided by microseconds per beat (tempo)
            song->_beatTime = (float)dataBuffer / (1000.0 * 1000.0); // Quarter note time in seconds
            _beatTime = getBeatTime(song->_beatTime);
            break;

        case META_OFFSET: // SMPTE offset
            name = "SMPTE offset";
            break;

        case META_TIME_SIGNATURE: // Time signature: 4 bytes
            name = "Time signature";
            song->_measureBeats = (int32_t)metaData[0]; // Numerator
            song->_beatNotes = (int32_t)pow(2.0f, (int32_t)metaData[1]); // Denominator: power of 2
            //metaData[2]); // Metronome
            //metaData[3]); // 32-nd's
            break;

        case META_KEY_SIGNATURE: // Key signature
            name = "Key signature";
            song->_key = (KEY_SIGNATURE)metaData[0]; // Key (1 byte)
            song->_scaleType = (SCALE)metaData[1]; // Scale (1 byte)
            break;

        case META_SPECIFIC: // Sequencer specific eventId
            name = "Sequencer specific";
            break;

        default:
            name = "Unknown";
            break;
    }

    processed += length;

    Serial.printf("Meta: %02X - %s, %d bytes\n", type, name.c_str(), length);

    return true;
}

//=================================================================================================
bool MidiFile::getSystemEvent(char* data, uint32_t dataSize, uint32_t& processed) {
    uint32_t bytes{0}; // Bytes in the data
    int32_t length{0}; // Data length
    String string{}; // General string

    processed = 0;

    // Get data length
    getVarLen(data + processed, dataSize - processed, length, bytes);
    processed += bytes;

    // Get the data

    processed += length;

    Serial.printf("System: \n");

    return true;
}

//=================================================================================================
bool MidiFile::getMidiEvent(uint8_t eventId, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track) {
    uint8_t param1{0}; // Event parameter 1
    uint8_t param2{0}; // Event parameter 2
    uint8_t type{0}; // Event type
    uint8_t channel{0}; // Event channel
    uint32_t bytes{0}; // Bytes processed in this call
    String string{}; // General string

    processed = 0;
    type = eventId & 0xF0;
    channel = eventId & 0x0F;

    // There is no instrument type for the drum set. MIDI channel 9 is reserved for drums.
    // So, if the channel is 9, set track's instrument to 'drum set'
    /*if(channel == DRUM_CHANNEL)
        track->_instrument = PERCUSSION;*/

    // 4 left bits specify
    switch(type) {
        case MIDI_NOTE_OFF: // Note Off
            if(!getNote(channel, false, data, dataSize, bytes, trackTime, track)) { // Note off
                Serial.printf("ERROR: MIDI_NOTE_OFF\n");
                return false;
            }

            processed += bytes;
            break;

        case MIDI_NOTE_ON: // Note on
            if(!getNote(channel, true, data, dataSize, bytes, trackTime, track)) { // Note on
                Serial.printf("ERROR: MIDI_NOTE_ON\n");
                return false;
            }

            processed += bytes;
            break;

        case MIDI_NOTE_AFTER: // Note aftertouch
            param1 = *(uint8_t*)(data + processed); // Note
            processed++;
            param2 = *(uint8_t*)(data + processed); // Value
            processed++;
            Serial.printf("Note %d aftertouch %d\n", param1, param2);
            break;

        case MIDI_CONTROL: // Control change
            if(!getControl(channel, data, dataSize, bytes, trackTime, track)) {
                Serial.printf("ERROR: MIDI_CONTROL\n");
                return false;
            }

            processed += bytes;
            break;

        case MIDI_PROGRAM: // Program change
            if(!getProgram(channel, data, dataSize, bytes, trackTime, track)) {
                Serial.printf("ERROR: MIDI_PROGRAM\n");
                return false;
            }

            processed += bytes;
            break;

        case MIDI_CHANNEL_AFTER: // Channel aftertouch
            param1 = *(uint8_t*)(data + processed); // Value
            processed++;
            Serial.printf("Channel aftertouch %d\n", param1);
            break;

        case MIDI_BEND: // Pitch bend
            param1 = *(uint8_t*)(data + processed); // Value (low byte)
            processed++;

            param2 = *(uint8_t*)(data + processed); // Value (high byte)
            processed++;

            Serial.printf("Pitch bend %d, %d\n", param1, param2);
            break;

        default:
            Serial.printf("ERROR: Unknown MIDI type %02X\n", type);
            return false;
    }

    return true;
}

//=================================================================================================
bool MidiFile::getNote(uint32_t channel, bool noteOn, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track) {
    Note note{}; // Note to save
    int32_t midiNote{0}; // MIDI note
    uint8_t velocity{0}; // Velocity
    String string{}; // General string

    // Check data size
    if(dataSize < 2) {
        Serial.printf("ERROR: Not enough data for a note: %d\n", dataSize);
        return false;
    }

    // MIDI note number
    midiNote = (int32_t)(uint8_t)data[0];
    if((midiNote < 0 || midiNote > 200) && midiNote != CHORD_MIDI_NOTE && midiNote != FINGER_MIDI_NOTE) {
        Serial.printf("ERROR: Bad MIDI note: %d\n", midiNote);
        return false;
    }

    // Velocity (0-127)
    velocity = (uint8_t)data[1];
    if(velocity > 127) {
        Serial.printf("ERROR: Bad note velocity: %d\n", velocity);
        return false;
    }

    // Check whether this note has been started before.
    // If yes, end it and save, even if it's started again.
    if(_notes[midiNote]._state == Note::STATE_START) { // This note was started
        note = _notes[midiNote];
        addNote(&note, trackTime, track);

        _notes[midiNote]._state = Note::STATE_NONE; // Done with this note
        _note.reset(); // No info for the next note unless it's read from the file again
        //_chordType = CHORD_NONE; // No chord type unless set
        //_chordRoot = NO_MIDI_NOTE; // No chord root unless set
    }

    // Start a new note, if the velocity is not zero.
    // If the velocity is zero, it's the end of note and the note has been already saved.
    if(velocity > 0) { // New note
        //_notes[midiNote]._volume = (float)velocity / 127; // Linear conversion
        _notes[midiNote]._volume = midiToVolume(velocity); // Logarithmic

        _notes[midiNote]._start = trackTime;
        _notes[midiNote]._midiNote = midiNote;
        _notes[midiNote]._channel = channel;
        _notes[midiNote]._state = Note::STATE_START;
        _notes[midiNote]._tab = _note._tab; // Save note's tab info
        /*_notes[midiNote]._nInterval = _note._nInterval;
        _notes[midiNote]._ChordInfo._nType = _chordType;
        _notes[midiNote]._ChordInfo._nRoot = _chordRoot;*/
    }

    Serial.printf("%7.3f sec: Note %s, note %3d, velocity %3d, channel=%d\n",
                  trackTime, noteOn ? "on" : "off", midiNote, velocity, channel);

    processed = 2;

    return true;
}

//=================================================================================================
bool MidiFile::addNote(Note* note, float trackTime, Track* track) {
    Instrument instrument{NONE}; // Instrument type
    String instrumentName{}; // Instrument name

    // Set note's instrument
    // If this is a drum set track, set the drum type for this note
    if(note->_channel == DRUM_CHANNEL) { // MIDI channel 9 (counting from 0) is reserved for drum sets
        if(getDrumInfo(note->_midiNote, instrumentName, instrument))
            note->_instrument = instrument;
        else
            note->_instrument = NONE;
    } else
        note->_instrument = track->_instrument;

    // Save the note
    note->_duration = trackTime - note->_start;
    //note->_nSource = Note::SOURCE_FILE;
    track->_notes.push_back(*note);

    return true;
}

//=================================================================================================
// Get MIDI program (instrument or patch)
bool MidiFile::getProgram(uint32_t channel, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track) {
    Instrument instrument{NONE}; // Instrument ID for this program
    uint32_t program{0}; // Program number. 0-127 in the file, but 1-128 in the specifications
    String programName{}; // Program name
    String string{}; // General string

    // Check data size
    if(dataSize < 1) {
        Serial.printf("ERROR: Not enough data for a program change: %d\n", dataSize);
        return false;
    }

    // MIDI program number
    program = (int32_t)(uint8_t)data[0];
    if(program < 0 || program > 127) {
        Serial.printf("ERROR: Bad program number: %d\n", program);
        return false;
    }

    // Get program's info
    if(channel == DRUM_CHANNEL) { // Check whether this is a Drum Channel: channel 9, with channel numbers starting with 0.
        instrument = PERCUSSION;
        programName = "Drum set";
        Serial.printf("########### Drum set channel\n", program, programName.c_str(), channel);
    } else {
        if(!getProgramInfo(program, programName, instrument)) {
            Serial.printf("ERROR: GetProgramInfo\n");
            return false;
        }
    }

    // Save the program
    track->_midiProgram = program;
    track->_instrument = instrument;
    track->_instrumentName = programName;

    Serial.printf("Program %3d, '%s', channel=%d, instrument=%d\n", program, programName.c_str(), channel, instrument);

    processed = 1;

    return true;
}

//=================================================================================================
// Get MIDI control code
bool MidiFile::getControl(uint32_t channel, char* data, uint32_t dataSize, uint32_t& processed, float trackTime, Track* track) {
    float half{127.0f / 2.0f}; // Half of the value range
    String controlName{}; // Control name
    int32_t control{0}; // Control type
    int32_t value{0}; // Control value, 0-127
    String string{}; // General string

    // Check data size
    if(dataSize < 2) {
        Serial.printf("ERROR: Not enough data for a control eventId: %d\n", dataSize);
        return false;
    }

    // Control type (0 - 127)
    control = (int32_t)(uint8_t)data[0];

    // Control value (0 - 127)
    value = (int32_t)(uint8_t)data[1];

    // Get control's info
    if(!getControlInfo(control, controlName)) {
        Serial.printf("ERROR: GetControlInfo\n");
        return false;
    }

    // Process the control
    switch(control) {
        case CONTROL_VOLUME: // Volume
            track->_volume = (float)value / 127;
            break;

        case CONTROL_PAN: // Pan
            track->_pan = ((float)value - half) / half; // Pan is from -1.0 to 1.0
            break;

        case CONTROL_TAB_STRING: // Guitar tab - string number
            _note._tab._string = value;
            break;

        case CONTROL_TAB_FRET: // Guitar tab - fret number
            _note._tab._fret = value;
            break;

        case CONTROL_TAB_FINGER: // Guitar tab - finger number
            _note._tab._finger = (uint8_t)value;
            break;

        case CONTROL_TAB_MODE: // Guitar tab - playing mode
            _note._tab._mode = (Note::TabInfo::MODE)value;
            break;

        case CONTROL_CHORD_TYPE: // Chord type
            //_chordType = (CHORD_TYPE)value;
            break;

        case CONTROL_CHORD_ROOT: // Chord's root note
            //_chordRoot = (int32_t)value;
            break;
    }

    Serial.printf("Control %3d, '%s' = %d, channel=%d\n", control, controlName.c_str(), value, channel);

    processed = 2;

    return true;
}

//=================================================================================================
// Resets all started notes
bool MidiFile::resetNotes() {
    int32_t size{0}; // Array size
    int32_t i{0}; // Loop counter

    size = sizeof(_notes) / sizeof(Note);
    for(i = 0; i < size; i++)
        _notes[i]._state = Note::STATE_NONE; // Note was not started

    return true;
}

//=================================================================================================
// Saves a variable length number
// dataSize - (in) size of data array
// bytes - (out) number of bytes saved into data
bool MidiFile::PutVarLen(int32_t value, char* data, uint32_t dataSize, uint32_t& bytes) {
    int32_t temp{0}; // Buffer for reversed data
    bool done = false; // Whether done processing

    bytes = 0;

    // Convert the number into a sequence of bits, using each 7-th bit as an indicator whether this is the last byte in the sequence.
    // This sequence will be reversed later to put the last byte to the end.
    temp = (char)(value & 0x7F); // There should be at least one byte. Use only 7 bits of each byte.
    while(value >>= 7) { // Check whether the 7-th bit is set. If it is 0, this is the last byte. If not, keep saving other bytes.
        temp <<= 8;
        temp |= 0x80; // Set the 7-th bit to 1 to indicate that this is not the last byte.
        temp += value & 0x7f;
    }

    // Reverse and save the bytes
    while(!done) {
        data[bytes] = (char)temp; // Save the current byte
        if(data[bytes] & 0x80) // This is not the last byte
            temp >>= 8; // Go to the next byte
        else
            done = true;

        bytes++;
    }

    // Maximum variable length must not be more than 4 bytes
    if(bytes > 4) {
        return false;
    }

    return true;
}

//=================================================================================================
// Gets a variable length number
bool MidiFile::getVarLen(char* data, uint32_t dataSize, int32_t& value, uint32_t& processed) {
    uint32_t i{0}; // Data index
    uint8_t nData{0}; // Current byte

    processed = 0;

    // Check the data size
    if(dataSize <= 0) {
        return false;
    }

    value = data[i]; // There is at least one byte
    i++;

    if(value & 0x80) { // 7-th bit of the last byte is set to 0. Process all bytes with 7-th bit set to 1.
        value &= 0x7F; // Use only 7 bits of the byte
        do {
            if(i >= dataSize) // Don't go out of the source data
                break;

            nData = data[i];
            value <<= 7; // Use only 7 bits of each byte
            value += nData & 0x7F; // Use only 7 bits of each byte
            i++;
        } while(nData & 0x80); // 7-th bit of the last byte is set to 0. Process all bytes with 7-th bit set to 1.
    }

    processed = i;

    return true;
}

//=================================================================================================
uint8_t MidiFile::VolumeToMidi(float volume) {
    uint8_t velocity{0};
    float log{0.0};

    if(volume < 0.01f)
        return 0;

    if(volume >= 1.0f)
        return 127;

    log = 1.0f + log10(volume) * _volumeRate;
    velocity = (uint8_t)(log * 127.0f + 0.5f);

    return velocity;
}

//=================================================================================================
float MidiFile::midiToVolume(uint8_t velocity) {
    float scaledVelocity{0.0};
    float volume{0.0};

    if(velocity == 0)
        return 0.0f;

    if(velocity >= 127)
        return 1.0f;

    scaledVelocity = (float)velocity / 127.0f;
    scaledVelocity = (scaledVelocity - 1.0f) / _volumeRate;
    volume = (float)(pow(10.0f, scaledVelocity));

    return volume;
}

//=================================================================================================
// Get program info by it's number
bool MidiFile::getProgramInfo(uint32_t program, String& name, Instrument& instrument) {
    switch(program) {
        case 0:     name = "Acoustic Grand Piano";      instrument = ACOUSTIC_GRAND_PIANO; break; // Piano
        case 1:     name = "Bright Acoustic Piano";     instrument = BRIGHT_ACOUSTIC_PIANO; break;
        case 2:     name = "Electric Grand Piano";      instrument = ELECTRIC_GRAND_PIANO; break;
        case 3:     name = "Honky-tonk Piano";          instrument = HONKY_TONK_PIANO; break;
        case 4:     name = "Electric Piano 1";          instrument = ELECTRIC_PIANO_1; break;
        case 5:     name = "Electric Piano 2";          instrument = ELECTRIC_PIANO_2; break;
        case 6:     name = "Harpsichord";               instrument = HARPSICHORD; break;
        case 7:     name = "Clavi";                     instrument = CLAVI; break;
        case 8:     name = "Celesta";                   instrument = CELESTA; break; // Chromatic Percussion
        case 9:     name = "Glockenspiel";              instrument = GLOCKENSPIEL; break;
        case 10:    name = "Music Box";                 instrument = MUSIC_BOX; break;
        case 11:    name = "Vibraphone";                instrument = VIBRAPHONE; break;
        case 12:    name = "Marimba";                   instrument = MARIMBA; break;
        case 13:    name = "Xylophone";                 instrument = XYLOPHONE; break;
        case 14:    name = "Tubular Bells";             instrument = TUBULAR_BELLS; break;
        case 15:    name = "Dulcimer";                  instrument = DULCIMER; break;
        case 16:    name = "Drawbar Organ";             instrument = DRAWBAR_ORGAN; break; // Organ
        case 17:    name = "Percussive Organ";          instrument = PERCUSSIVE_ORGAN; break;
        case 18:    name = "Rock Organ";                instrument = ROCK_ORGAN; break;
        case 19:    name = "Church Organ";              instrument = CHURCH_ORGAN; break;
        case 20:    name = "Reed Organ";                instrument = REED_ORGAN; break;
        case 21:    name = "Accordion";                 instrument = ACCORDION; break;
        case 22:    name = "Harmonica";                 instrument = HARMONICA; break;
        case 23:    name = "Tango Accordion";           instrument = TANGO_ACCORDION; break;
        case 24:    name = "Acoustic Guitar (nylon)";   instrument = ACOUSTIC_GUITAR_NYLON; break; // Guitar
        case 25:    name = "Acoustic Guitar (steel)";   instrument = ACOUSTIC_GUITAR_STEEL; break;
        case 26:    name = "Electric Guitar (jazz)";    instrument = ELECTRIC_GUITAR_JAZZ; break;
        case 27:    name = "Electric Guitar (clean)";   instrument = ELECTRIC_GUITAR_CLEAN; break;
        case 28:    name = "Electric Guitar (muted)";   instrument = ELECTRIC_GUITAR_MUTED; break;
        case 29:    name = "Overdriven Guitar";         instrument = OVERDRIVEN_GUITAR; break;        
        case 30:    name = "Distortion Guitar";         instrument = DISTORTION_GUITAR; break;        
        case 31:    name = "Guitar harmonics";          instrument = GUITAR_HARMONICS; break;
        case 32:    name = "Acoustic Bass";             instrument = ACOUSTIC_BASS; break; // Bass
        case 33:    name = "Electric Bass (finger)";    instrument = ELECTRIC_BASS_FINGER; break;
        case 34:    name = "Electric Bass (pick)";      instrument = ELECTRIC_BASS_PICK; break;
        case 35:    name = "Fretless Bass";             instrument = FRETLESS_BASS; break;
        case 36:    name = "Slap Bass 1";               instrument = SLAP_BASS_1; break;
        case 37:    name = "Slap Bass 2";               instrument = SLAP_BASS_2; break;
        case 38:    name = "Synth Bass 1";              instrument = SYNTH_BASS_1; break;
        case 39:    name = "Synth Bass 2";              instrument = SYNTH_BASS_2; break;
        case 40:    name = "Violin";                    instrument = VIOLIN; break; // Strings
        case 41:    name = "Viola";                     instrument = VIOLA; break;
        case 42:    name = "Cello";                     instrument = CELLO; break;
        case 43:    name = "Contrabass";                instrument = CONTRABASS; break;
        case 44:    name = "Tremolo Strings";           instrument = TREMOLO_STRINGS; break;
        case 45:    name = "Pizzicato Strings";         instrument = PIZZICATO_STRINGS; break;
        case 46:    name = "Orchestral Harp";           instrument = ORCHESTRAL_HARP; break;
        case 47:    name = "Timpani";                   instrument = TIMPANI; break;
        case 48:    name = "String Ensemble 1";         instrument = STRING_ENSEMBLE_1; break; // Ensemble
        case 49:    name = "String Ensemble 2";         instrument = STRING_ENSEMBLE_2; break;
        case 50:    name = "SynthStrings 1";            instrument = SYNTHSTRINGS_1; break;
        case 51:    name = "SynthStrings 2";            instrument = SYNTHSTRINGS_2; break;
        case 52:    name = "Choir Aahs";                instrument = CHOIR_AAHS; break;
        case 53:    name = "Voice Oohs";                instrument = VOICE_OOHS; break;
        case 54:    name = "Synth Voice";               instrument = SYNTH_VOICE; break;
        case 55:    name = "Orchestra Hit";             instrument = ORCHESTRA_HIT; break;
        case 56:    name = "Trumpet";                   instrument = TRUMPET; break; // Brass
        case 57:    name = "Trombone";                  instrument = TROMBONE; break;
        case 58:    name = "Tuba";                      instrument = TUBA; break;
        case 59:    name = "Muted Trumpet";             instrument = MUTED_TRUMPET; break;
        case 60:    name = "French Horn";               instrument = FRENCH_HORN; break;
        case 61:    name = "Brass Section";             instrument = BRASS_SECTION; break;
        case 62:    name = "SynthBrass 1";              instrument = SYNTHBRASS_1; break;
        case 63:    name = "SynthBrass 2";              instrument = SYNTHBRASS_2; break;
        case 64:    name = "Soprano Sax";               instrument = SOPRANO_SAX; break; // Reed
        case 65:    name = "Alto Sax";                  instrument = ALTO_SAX; break;
        case 66:    name = "Tenor Sax";                 instrument = TENOR_SAX; break;
        case 67:    name = "Baritone Sax";              instrument = BARITONE_SAX; break;
        case 68:    name = "Oboe";                      instrument = OBOE; break;
        case 69:    name = "English Horn";              instrument = ENGLISH_HORN; break;
        case 70:    name = "Bassoon";                   instrument = BASSOON; break;
        case 71:    name = "Clarinet";                  instrument = CLARINET; break;
        case 72:    name = "Piccolo";                   instrument = PICCOLO; break; // Pipe
        case 73:    name = "Flute";                     instrument = FLUTE; break;
        case 74:    name = "Recorder";                  instrument = RECORDER; break;
        case 75:    name = "Pan Flute";                 instrument = PAN_FLUTE; break;
        case 76:    name = "Blown Bottle";              instrument = BLOWN_BOTTLE; break;
        case 77:    name = "Shakuhachi";                instrument = SHAKUHACHI; break;
        case 78:    name = "Whistle";                   instrument = WHISTLE; break;
        case 79:    name = "Ocarina";                   instrument = OCARINA; break;
        case 80:    name = "Lead 1 (square)";           instrument = LEAD_1_SQUARE; break; // Synth Lead
        case 81:    name = "Lead 2 (sawtooth)";         instrument = LEAD_2_SAWTOOTH; break;
        case 82:    name = "Lead 3 (calliope)";         instrument = LEAD_3_CALLIOPE; break;
        case 83:    name = "Lead 4 (chiff)";            instrument = LEAD_4_CHIFF; break;
        case 84:    name = "Lead 5 (charang)";          instrument = LEAD_5_CHARANG; break;
        case 85:    name = "Lead 6 (voice)";            instrument = LEAD_6_VOICE; break;
        case 86:    name = "Lead 7 (fifths)";           instrument = LEAD_7_FIFTHS; break;
        case 87:    name = "Lead 8 (bass + lead)";      instrument = LEAD_8_BASS_LEAD; break;
        case 88:    name = "Pad 1 (new age)";           instrument = PAD_1_NEW_AGE; break; // Synth Pad
        case 89:    name = "Pad 2 (warm)";              instrument = PAD_2_WARM; break;
        case 90:    name = "Pad 3 (polysynth)";         instrument = PAD_3_POLYSYNTH; break;
        case 91:    name = "Pad 4 (choir)";             instrument = PAD_4_CHOIR; break;
        case 92:    name = "Pad 5 (bowed)";             instrument = PAD_5_BOWED; break;
        case 93:    name = "Pad 6 (metallic)";          instrument = PAD_6_METALLIC; break;
        case 94:    name = "Pad 7 (halo)";              instrument = PAD_7_HALO; break;
        case 95:    name = "Pad 8 (sweep)";             instrument = PAD_8_SWEEP; break;
        case 96:    name = "FX 1 (rain)";               instrument = FX_1_RAIN; break; // Synth Effects
        case 97:    name = "FX 2 (soundtrack)";         instrument = FX_2_SOUNDTRACK; break;
        case 98:    name = "FX 3 (crystal)";            instrument = FX_3_CRYSTAL; break;
        case 99:    name = "FX 4 (atmosphere)";         instrument = FX_4_ATMOSPHERE; break;
        case 100:   name = "FX 5 (brightness)";         instrument = FX_5_BRIGHTNESS; break;
        case 101:   name = "FX 6 (goblins)";            instrument = FX_6_GOBLINS; break;
        case 102:   name = "FX 7 (echoes)";             instrument = FX_7_ECHOES; break;
        case 103:   name = "FX 8 (sci-fi)";             instrument = FX_8_SCIFI; break;
        case 104:   name = "Sitar";                     instrument = SITAR; break; // Ethnic
        case 105:   name = "Banjo";                     instrument = BANJO; break;
        case 106:   name = "Shamisen";                  instrument = SHAMISEN; break;
        case 107:   name = "Koto";                      instrument = KOTO; break;
        case 108:   name = "Kalimba";                   instrument = KALIMBA; break;
        case 109:   name = "Bag pipe";                  instrument = BAG_PIPE; break;
        case 110:   name = "Fiddle";                    instrument = FIDDLE; break;
        case 111:   name = "Shanai";                    instrument = SHANAI; break;
        case 112:   name = "Tinkle Bell";               instrument = TINKLE_BELL; break; // Percussive
        case 113:   name = "Agogo";                     instrument = AGOGO; break;
        case 114:   name = "Steel Drums";               instrument = STEEL_DRUMS; break;
        case 115:   name = "Woodblock";                 instrument = WOODBLOCK; break;
        case 116:   name = "Taiko Drum";                instrument = TAIKO_DRUM; break;
        case 117:   name = "Melodic Tom";               instrument = MELODIC_TOM; break;
        case 118:   name = "Synth Drum";                instrument = SYNTH_DRUM; break;
        case 119:   name = "Reverse Cymbal";            instrument = REVERSE_CYMBAL; break;
        case 120:   name = "Guitar Fret Noise";         instrument = GUITAR_FRET_NOISE; break; // Sound effects
        case 121:   name = "Breath Noise";              instrument = BREATH_NOISE; break;
        case 122:   name = "Seashore";                  instrument = SEASHORE; break;
        case 123:   name = "Bird Tweet";                instrument = BIRD_TWEET; break;
        case 124:   name = "Telephone Ring";            instrument = TELEPHONE_RING; break;
        case 125:   name = "Helicopter";                instrument = HELICOPTER; break;
        case 126:   name = "Applause";                  instrument = APPLAUSE; break;
        case 127:   name = "Gunshot";                   instrument = GUNSHOT; break;
        default:    name = "";                          instrument = NONE;
            Serial.printf("ERROR: Unknown program %d (0-127 range)\n", program);
            break;
    }

    return true;
}

//=================================================================================================
// Get MIDI control(ler) info by it's number
bool MidiFile::getControlInfo(uint32_t control, String& name) {
    switch(control) {
        case 0:     name = "Bank Select"; break;
        case 1:     name = "Modulation"; break;
        case 2:     name = "Breath Controller"; break;
        case 4:     name = "Foot Controller"; break;
        case 5:     name = "Portamento Time"; break;
        case 6:     name = "Data Entry (MSB)"; break;
        case 7:     name = "Main Volume"; break;
        case 8:     name = "Balance"; break;
        case 9:     name = "Control 9 (0-127)"; break;
        case 10:    name = "Pan"; break;
        case 11:    name = "Expression Controller"; break;
        case 12:    name = "Effect Control 1"; break;
        case 13:    name = "Effect Control 2"; break;
        case 14:    name = "Control 14 (0-127)"; break;
        case 15:    name = "Control 15 (0-127)"; break;
        case 16:    name = "Guitar tab - string number"; break;
        case 17:    name = "Guitar tab - fret number"; break;
        case 18:    name = "Guitar tab - finger number"; break;
        case 19:    name = "Guitar tab - playing mode"; break;
        case 20:    name = "Control 20 (0-127)"; break;
        case 21:    name = "Control 21 (0-127)"; break;
        case 22:    name = "Control 22 (0-127)"; break;
        case 23:    name = "Control 23 (0-127)"; break;
        case 24:    name = "Control 24 (0-127)"; break;
        case 25:    name = "Control 25 (0-127)"; break;
        case 26:    name = "Control 26 (0-127)"; break;
        case 27:    name = "Control 27 (0-127)"; break;
        case 28:    name = "Control 28 (0-127)"; break;
        case 29:    name = "Control 29 (0-127)"; break;
        case 30:    name = "Control 30 (0-127)"; break;
        case 31:    name = "Control 31 (0-127)"; break;
        case 32:    name = "Control 32 (0-127)"; break;
        case 33:    name = "Control 33 (0-127)"; break;
        case 34:    name = "Control 34 (0-127)"; break;
        case 35:    name = "Control 35 (0-127)"; break;
        case 36:    name = "Control 36 (0-127)"; break;
        case 37:    name = "Control 37 (0-127)"; break;
        case 38:    name = "Control 38 (0-127)"; break;
        case 39:    name = "Control 39 (0-127)"; break;
        case 40:    name = "Control 40 (0-127)"; break;
        case 41:    name = "Control 41 (0-127)"; break;
        case 42:    name = "Control 42 (0-127)"; break;
        case 43:    name = "Control 43 (0-127)"; break;
        case 44:    name = "Control 44 (0-127)"; break;
        case 45:    name = "Control 45 (0-127)"; break;
        case 46:    name = "Control 46 (0-127)"; break;
        case 47:    name = "Control 47 (0-127)"; break;
        case 48:    name = "Control 48 (0-127)"; break;
        case 49:    name = "Control 49 (0-127)"; break;
        case 50:    name = "Control 50 (0-127)"; break;
        case 51:    name = "Control 51 (0-127)"; break;
        case 52:    name = "Control 52 (0-127)"; break;
        case 53:    name = "Control 53 (0-127)"; break;
        case 54:    name = "Control 54 (0-127)"; break;
        case 55:    name = "Control 55 (0-127)"; break;
        case 56:    name = "Control 56 (0-127)"; break;
        case 57:    name = "Control 57 (0-127)"; break;
        case 58:    name = "Control 58 (0-127)"; break;
        case 59:    name = "Control 59 (0-127)"; break;
        case 60:    name = "Control 60 (0-127)"; break;
        case 61:    name = "Control 61 (0-127)"; break;
        case 62:    name = "Control 62 (0-127)"; break;
        case 63:    name = "Control 63 (0-127)"; break;
        case 64:    name = "Damper pedal (sustain)"; break;
        case 65:    name = "Portamento"; break;
        case 66:    name = "Sostenuto"; break;
        case 67:    name = "Soft Pedal"; break;
        case 68:    name = "Legato Footswitch"; break;
        case 69:    name = "Hold 2"; break;
        case 70:    name = "Sound Controller 1 (Timber Variation)"; break;
        case 71:    name = "Sound Controller 2 (Timber/Harmonic Content)"; break;
        case 72:    name = "Sound Controller 3 (Release Time)"; break;
        case 73:    name = "Sound Controller 4 (Attack Time)"; break;
        case 74:    name = "Sound Controller 5"; break;
        case 75:    name = "Sound Controller 6"; break;
        case 76:    name = "Sound Controller 7"; break;
        case 77:    name = "Sound Controller 8"; break;
        case 78:    name = "Sound Controller 9"; break;
        case 79:    name = "Sound Controller 10"; break;
        case 80:    name = "Chord type"; break;
        case 81:    name = "Chord root"; break;
        case 82:    name = "General-Purpose Controller 7"; break;
        case 83:    name = "General-Purpose Controller 8"; break;
        case 84:    name = "Portamento Control"; break;
        case 85:    name = "Control 85 (0-127)"; break;
        case 86:    name = "Control 86 (0-127)"; break;
        case 87:    name = "Control 87 (0-127)"; break;
        case 88:    name = "Control 88 (0-127)"; break;
        case 89:    name = "Control 89 (0-127)"; break;
        case 90:    name = "Control 90 (0-127)"; break;
        case 91:    name = "Effects 1 Depth"; break;
        case 92:    name = "Effects 2 Depth"; break;
        case 93:    name = "Effects 3 Depth"; break;
        case 94:    name = "Effects 4 Depth"; break;
        case 95:    name = "Effects 5 Depth"; break;
        case 96:    name = "Data Increment"; break;
        case 97:    name = "Data Decrement"; break;
        case 98:    name = "Non-Registered Parameter Number (LSB)"; break;
        case 99:    name = "Non-Registered Parameter Number (MSB)"; break;
        case 100:   name = "Registered Parameter Number (LSB)"; break;
        case 101:   name = "Registered Parameter Number (MSB)"; break;
        case 102:   name = "Control 102 (0-127)"; break;
        case 103:   name = "Control 103 (0-127)"; break;
        case 104:   name = "Control 104 (0-127)"; break;
        case 105:   name = "Control 105 (0-127)"; break;
        case 106:   name = "Control 106 (0-127)"; break;
        case 107:   name = "Control 107 (0-127)"; break;
        case 108:   name = "Control 108 (0-127)"; break;
        case 109:   name = "Control 109 (0-127)"; break;
        case 110:   name = "Control 110 (0-127)"; break;
        case 111:   name = "Control 111 (0-127)"; break;
        case 112:   name = "Control 112 (0-127)"; break;
        case 113:   name = "Control 113 (0-127)"; break;
        case 114:   name = "Control 114 (0-127)"; break;
        case 115:   name = "Control 115 (0-127)"; break;
        case 116:   name = "Control 116 (0-127)"; break;
        case 117:   name = "Control 117 (0-127)"; break;
        case 118:   name = "Control 118 (0-127)"; break;
        case 119:   name = "Control 119 (0-127)"; break;
        case 120:   name = "Control 120 (0-127)"; break;
        case 121:   name = "Channel Mode Message: Reset All Controllers"; break;
        case 122:   name = "Channel Mode Message: Local Control"; break;
        case 123:   name = "Channel Mode Message: All Notes Off"; break;
        case 124:   name = "Channel Mode Message: Omni Off"; break;
        case 125:   name = "Channel Mode Message: Omni On"; break;
        case 126:   name = "Channel Mode Message: Mono On (Poly Off)"; break;
        case 127:   name = "Channel Mode Message: Poly On (Mono Off)"; break;
        default:
            Serial.printf("Unknown conrol %d\n", control);
            break;
    }

    return true;
}

//=================================================================================================
// Gets MIDI note number for a drum set instrument (thing) type
uint8_t MidiFile::getMidiDrumNote(Instrument instrument) {
    switch(instrument) {
        /*case DRUM_BASS_DRUM_2:
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
            return 55; // Splash Cymbal*/

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
bool MidiFile::getDrumInfo(uint32_t midiNote, String& name, Instrument& instrument) {
    switch(midiNote) {
        /*case 27: name = "High Q";         instrument = NONE; break;
        case 28: name = "Slap";           instrument = NONE; break;
        case 29: name = "Scratch1";       instrument = NONE; break;
        case 30: name = "Scratch2";       instrument = NONE; break;
        case 31: name = "Sticks";         instrument = NONE; break;
        case 32: name = "Square";         instrument = NONE; break;
        case 33: name = "Metronome1";     instrument = NONE; break;
        case 34: name = "Metronome2";     instrument = NONE; break;
        case 35: name = "Bass Drum 2";    instrument = (Instrument)DRUM_BASS_DRUM_2; break;
        case 36: name = "Bass Drum 1";    instrument = (Instrument)DRUM_BASS_DRUM_1; break;
        case 37: name = "Side Stick";     instrument = (Instrument)NONE; break;
        case 38: name = "Snare 1";        instrument = (Instrument)DRUM_SNARE; break;
        case 39: name = "Hand Clap";      instrument = NONE; break;
        case 40: name = "Snare 2";        instrument = (Instrument)DRUM_SNARE; break;
        case 41: name = "Low Tom 2";      instrument = (Instrument)DRUM_TOM_6; break;
        case 42: name = "Closed Hi-Hat";  instrument = (Instrument)DRUM_HIHAT_CLOSED; break;
        case 43: name = "Low Tom 1";      instrument = (Instrument)DRUM_TOM_5; break;
        case 44: name = "Pedal Hi-Hat";   instrument = (Instrument)DRUM_HIHAT_FOOT; break;
        case 45: name = "Mid Tom 2";      instrument = (Instrument)DRUM_TOM_4; break;
        case 46: name = "Open Hi-Hat";    instrument = (Instrument)DRUM_HIHAT_OPEN; break;
        case 47: name = "Mid Tom 1";      instrument = (Instrument)DRUM_TOM_3; break;
        case 48: name = "High Tom 2";     instrument = (Instrument)DRUM_TOM_2; break;
        case 49: name = "Crash Cymbal 1"; instrument = (Instrument)DRUM_CRASH_1; break;
        case 50: name = "High Tom 1";     instrument = (Instrument)DRUM_TOM_1; break;
        case 51: name = "Ride Cymbal 1";  instrument = (Instrument)DRUM_RIDE_CYMBAL; break;
        case 52: name = "Chinese Cymbal"; instrument = (Instrument)DRUM_CHINESE_1; break;
        case 53: name = "Ride Bell";      instrument = (Instrument)DRUM_RIDE_BELL; break;
        case 54: name = "Tambourine";     instrument = NONE; break;
        case 55: name = "Splash Cymbal";  instrument = (Instrument)DRUM_SPLASH_1; break;
        case 56: name = "Cowbell";        instrument = (Instrument)NONE; break;
        case 57: name = "Crash Cymbal 2"; instrument = (Instrument)DRUM_CRASH_2; break;
        case 58: name = "Vibra Slap";     instrument = NONE;  break;
        case 59: name = "Ride Cymbal 2";  instrument = NONE;  break;
        case 60: name = "Hi Bongo";       instrument = NONE;  break;
        case 61: name = "Low Bongo";      instrument = NONE;  break;
        case 62: name = "Mute Hi Conga";  instrument = NONE;  break;
        case 63: name = "Open Hi Conga";  instrument = NONE;  break;
        case 64: name = "Low Conga";      instrument = NONE;  break;
        case 65: name = "High Timbale";   instrument = NONE;  break;
        case 66: name = "Low Timbale";    instrument = NONE;  break;
        case 67: name = "High Agogo";     instrument = NONE;  break;
        case 68: name = "Low Agogo";      instrument = NONE;  break;
        case 69: name = "Cabasa";         instrument = NONE;  break;
        case 70: name = "Maracas";        instrument = NONE;  break;
        case 71: name = "Short Whistle";  instrument = NONE;  break;
        case 72: name = "Long Whistle";   instrument = NONE;  break;
        case 73: name = "Short Guiro";    instrument = NONE;  break;
        case 74: name = "Long Guiro";     instrument = NONE;  break;
        case 75: name = "Claves";         instrument = NONE;  break;
        case 76: name = "Hi Wood Block";  instrument = NONE;  break;
        case 77: name = "Low Wood Block"; instrument = NONE;  break;
        case 78: name = "Mute Cuica";     instrument = NONE;  break;
        case 79: name = "Open Cuica";     instrument = NONE;  break;
        case 80: name = "Mute Triangle";  instrument = NONE;  break;
        case 81: name = "Open Triangle";  instrument = NONE;  break;
        case 82: name = "Shaker";         instrument = NONE;  break;
        case 83: name = "Jingle Bell";    instrument = NONE;  break;
        case 84: name = "Belltree";       instrument = NONE;  break;
        case 85: name = "Castanets";      instrument = NONE;  break;
        case 86: name = "Mute Surdo";     instrument = NONE;  break;
        case 87: name = "Open Surdo";     instrument = NONE;  break;*/
        default: name = "";               instrument = NONE; return false;
    }

    if(instrument == 0)
        return false;

    return true;
}
/*
//=================================================================================================
// Saves a song into MIDI file
bool MidiFile::SaveData(char*& data, int32_t& dwDataSize, Song* song) {
    bool	bRet = true;	// Return value

    #ifndef _DEBUG
    __try {
    #endif

        Log("========== Lesson '%s': save ===============");
        //song->Show("MidiFile::SaveData");

        _data = nullptr;
        _dataSize = 0;

        // Set some parameters
        _format = 1;
        _tracks = song->GetNumberOfTracks();
        _timeDivision = 1000;//960;
        _beatTime = getBeatTime(song->_beatTime);

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
    int32_t     dataSize;
    int32_t ret;

    Log("========== MIDI file '%s': save ===============", pFile);
    //song->Show("MidiFile::SaveData");

    // Open the file for writing
    ret = Open(pFile, false, true); // A new file for writing
    if(ret)
        return false;

    _data = nullptr;
    _dataSize = 0;

    // Set some parameters
    _format = 1;
    _tracks = (int32_t)song->_tracks.size();
    _timeDivision = 1000;//960;
    _beatTime = getBeatTime(song->_beatTime);

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
    dataSize = (int32_t)(_dataOffset - _data);
    if(!Write(_data, dataSize)) {
        bRet = false;
        goto Exit;
    }

Exit:
    Close();

    free(_data);
    _data = nullptr;

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
    reverse4Bytes((char*)&ChunkHeader._length); // Length is stored in reversed byte order

    // Save the chunk header
    if(!AddData(&ChunkHeader, sizeof(ChunkHeader)))
        return false;

    // MIDI format
    FileHeader._format = _format;
    reverse2Bytes((char*)&FileHeader._format);

    // Number of tracks
    FileHeader._tracks = _tracks + 1; // The first track is for song info;
    reverse2Bytes((char*)&FileHeader._tracks);

    // Time division
    FileHeader._division = _timeDivision;
    reverse2Bytes((char*)&FileHeader._division);

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
    int32_t		data = 0;		// Converted data

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
    data = (int32_t)(song->_beatTime * (1000.0 * 1000.0) + 0.5);
    reverse4Bytes((char*)&data);
    if(!SaveMetaEvent(0, META_TEMPO, 0, (const char*)&data + 1, 3, sTrackData))
        return false;

    // Add 'end of track' eventId
    if(!SaveMetaEvent(0, META_END, 0, nullptr, 0, sTrackData))
        return false;

    // Save song info in a track
    if(!SaveTrack((const char*)sTrackData, sTrackData.Length()))
        return false;

    return true;
}

//=================================================================================================
// Saves info of all song parts
bool MidiFile::SaveParts(Song* song, Buffer& sTrackData) {
    int32_t         index;
    SongPart*  pPart;

    song->_SongParts.MoveToFirst(index);
    while(song->_SongParts.GetNext(index, pPart)) {
        if(!SavePartInfo(0, pPart, sTrackData))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves all song's tracks
bool MidiFile::SaveAllTracks(Song* song) {
    int32_t			channel = 0;		// Track counter
    Track*		track = nullptr;	// Current track
    Buffer		sTrackData;		// Track's data
    TRACKS_ITER iter;

    for(iter = song->_tracks.begin(); iter != song->_tracks.end(); iter++) {
        track = &(*iter);

        sTrackData.reset();

        // Set MIDI channel for drum set
        if(track->_instrument == NONE)
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
    float	    half = 127.0f / 2.0f;	// Half of the value range
    uint8_t	    cValue = 0;	    // Control value
    int32_t         value;
    String	string;		// General string

    // Add to the report
    Serial.printf("Save track %d '%s' (%s) -------------",
                   track->_trackNumber, (const char*)track->_trackName, (const char*)track->_instrumentName);
    Log((const char*)string);

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
    value = getProgram(track->_instrument);
    if(value != PROGRAM_NONE) {
        if(!SaveEvent(0, MIDI_PROGRAM, track->_channel, (const char*)&value, 1, sTrackData))
            return false;
    }

    // Volume
    value = (int32_t)(127.0f * track->_volume + 0.5f);
    if(value < 0)
        value = 0;

    if(value > 127)
        value = 127;

    cValue = (uint8_t)value;
    if(!SaveControlEvent(0, track->_channel, CONTROL_VOLUME, cValue, sTrackData))
        return false;

    // Pan
    value = (int32_t)(half + track->_pan * half + 0.5f);
    if(value < 0)
        value = 0;

    if(value > 127)
        value = 127;

    cValue = (uint8_t)value;
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
    Note*		note = nullptr;		// Current note
    Note		Note;				// Current note
    float		dPrevTime = 0.0;	// Last note's time
    int32_t		    timeDiff;	        // Time difference
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
        Note._start += Note._duration; // Time of the eventId - note's end
        events.push_back(Note); // Sorted by start time
    }

    std::sort(events.begin(), events.end(), SortNoteTime); // Sort events by start time

    // Save all note starts and stops
    for(noteIter = events.begin(); noteIter != events.end(); noteIter++) {
        note = &(*noteIter);
        timeDiff = getTimeDiff(note->_start - dPrevTime);

        // Save note
        if(!SaveNote(timeDiff, true, track->_channel, true, note->_midiNote, note->_volume, sTrackData))
            return false;

        dPrevTime = note->_start;
    }

    // Add a silent note to the end of track. This will prevent MIDI players from cutting the end of track.
    if(!SaveNote(0, true, track->_channel, true, 0, 0.0, sTrackData))
        return false;

    if(!SaveNote(getTimeDiff(0.5), true, track->_channel, true, 0, 0.0, sTrackData))
        return false;

    // Add 'end of track' eventId
    if(!SaveMetaEvent(0, META_END, 0, nullptr, 0, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves one track
bool MidiFile::SaveTrack(const char* pTrackData, int32_t nTrackLength) {
    ChunkHeader	ChunkHeader;	// chunk header

    // Set chunk ID
    memcpy(ChunkHeader._id, ID_TRACK_HEADER, sizeof(ID_TRACK_HEADER));

    // Set chunk length
    ChunkHeader._length = nTrackLength;
    reverse4Bytes((char*)&ChunkHeader._length); // Length is stored in reversed byte order

    // Save the chunk header
    if(!AddData(&ChunkHeader, sizeof(ChunkHeader)))
        return false;

    // Save track's data
    if(!AddData((void*)pTrackData, nTrackLength))
        return false;

    return true;
}

//=================================================================================================
// Saves meta eventId
bool MidiFile::SaveMetaEvent(int32_t timeDiff, META_EVENT nMetaEvent, int32_t channel, const char* eventData, int32_t nEventDataSize,
                              Buffer& sTrackData) {
    uint8_t		szBuffer[64];	// Conversion buffer
    int32_t			bytes = 0;		// Bytes saved in the conversion
    Buffer		sEventData;		// Converted eventId data
    String	string;		// General string
    String	sData;

    // Meta eventId type
    sEventData.Add((const char*)(uint8_t*)&nMetaEvent, 1);

    // Data length
    if(PutVarLen(nEventDataSize, (char*)szBuffer, sizeof(szBuffer), bytes))
        sEventData.Add((const char*)szBuffer, bytes);
    else
        return false;

    // Data
    if(eventData && nEventDataSize)
        sEventData.Add(eventData, nEventDataSize);

    // Save as eventId
    if(!SaveEvent(timeDiff, MIDI_META, channel, (const char*)sEventData, sEventData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves control eventId
bool MidiFile::SaveControlEvent(int32_t timeDiff, int32_t channel, uint8_t type, uint8_t value, Buffer& sTrackData) {
    String	controlName;	// Control name
    String	string;		// General string
    uint8_t	    szBuffer[8];	// Event data

    // Get control's info
    if(!getControlInfo(type, controlName))
        return false;

    // Add to the report
    Serial.printf("Control %3d, '%s' = %d, channel=%d", type, (const char*)controlName, value, channel);
    //Log((const char*)string);

    // Control data
    szBuffer[0] = type;
    szBuffer[1] = value;

    // Save the eventId
    if(!SaveEvent(timeDiff, MIDI_CONTROL, channel, (const char*)szBuffer, 2, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves a marker note
bool MidiFile::SaveMarker(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    switch(note->_state) {
        case Note::STATE_CHORD_MARK:
            if(!SaveChord(timeDiff, note, track, sTrackData))
                return false;

            break;

        case Note::STATE_PART_MARK:
            if(!SaveSongPart(timeDiff, note, track, sTrackData))
                return false;

            break;

        case Note::STATE_MEASURE_MARK:
            if(!SaveMeasure(timeDiff, note, track, sTrackData))
                return false;

            break;

        default:
            return false;
    }

    return true;
}

//=================================================================================================
// Saves note's chord info using MIDI control events
bool MidiFile::SaveChord(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

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
    Serial.printf("Chord: %s", (const char*)sData);
    //Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets chord data
bool MidiFile::GetChord(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    CChord      Chord;
    CChord*     pChord;
    Note       Note;
    String string;

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

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = trackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._state = Note::STATE_CHORD_MARK;
    Note._ChordInfo = Chord._RootNote._ChordInfo;
    Note._nSource = Note::SOURCE_FILE;

    track->addNote(Note);

    // Add to the report
    Serial.printf("%6.2f  Chord: %d", trackTime, Chord._RootNote._ChordInfo._nRoot);
    //Log((const char*)string);

    return true;
}

//=================================================================================================
// Saves mix data
bool MidiFile::SaveMix(int32_t timeDiff, CMix* pMix, Buffer& sTrackData) {
    MIX_MAP_ITER    Iter;	    // Map iterator
    MIX_INFO*       pMixInfo;   // Mix info
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

    sData = MARKER_MIX;

    for(Iter = pMix->_Mixes.begin(); Iter != pMix->_Mixes.end(); Iter++) {
        pMixInfo = &Iter->second;

        // Instrument
        if(ThingToString(pMixInfo->_instrument, string)) {
            sPair.Format("%s%s|", MARKER_FIELD_INSTRUMENT, (const char*)string);
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
    Serial.printf("Mix: %s", (const char*)sData);
    //Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets mix data
bool MidiFile::GetMix(const char* pMarker, Song* song) {
    MIX_MAP_INS_PAIR	Pair;	    // Insert pair
    MIX_INFO    MixInfo;    // Mix info
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
    String	sCopy;	    // String copy
    const char* pValue;
    THING       instrument;        // Source instrument
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
                    Serial.printf("Can't insert into mix map");
                    return false;
                }
            }

            pValue = pField + strlen(MARKER_FIELD_INSTRUMENT);
            if(!StringToThing(pValue, instrument)) {
                return false;
            }

            MapThings(instrument, nToInstrument);
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

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    // Save the last intrument
    if(MixInfo._instrument != 0) {
        // Insert the info
        Pair = song->_Mix._Mixes.insert(MIX_MAP_PAIR(MixInfo._instrument, MixInfo));
        if(!Pair.second) { // Can't insert
            Serial.printf("Can't insert into mix map");
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
    String string;

    sData = MARKER_SONG_INFO;

    // Description
    sPair.Format("%s%s|", MARKER_FIELD_DESCR, (const char*)song->_sDescription);
    sData += sPair;

    // Repeat
    sPair.Format("%s%d|", MARKER_FIELD_REPEAT, song->_nRepeat);
    sData += sPair;

    // Song length (without repetition)
    if(song->_songTime != NO_DOUBLE) {
        sPair.Format("%s%0.3f|", MARKER_FIELD_SONG_LENGTH, song->_songTime);
        sData += sPair;
    }

    // Add to the report
    Serial.printf("Song info: %s", (const char*)sData);
    Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(0, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song info
bool MidiFile::GetSongInfo(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
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
            song->_songTime = atof(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    return true;
}

//=================================================================================================
// Saves full info of one song part
bool MidiFile::SavePartInfo(int32_t timeDiff, SongPart* pPart, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

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
    sPair.Format("%s%0.3f|", MARKER_FIELD_PART_LENGTH, pPart->_Part._songTime);
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
    sPair.Format("%s%0.2f|", MARKER_FIELD_TEMPO, pPart->_Part._tempo);
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
    Serial.printf("Song part info: %s", (const char*)sData);
    Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, 0, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song part info
bool MidiFile::GetPartInfo(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
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
            Part._Part._songTime = atof(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_SCALE, strlen(MARKER_FIELD_SCALE))) { // Scale
            pValue = pField + strlen(MARKER_FIELD_SCALE);
            if(!NoteManager::GetScaleID(pValue, Part._Part._nScale))
                return false;
        } else if(!strncmp(pField, MARKER_FIELD_SCALE_ROOT, strlen(MARKER_FIELD_SCALE_ROOT))) { // Scale root note
            pValue = pField + strlen(MARKER_FIELD_SCALE_ROOT);
            Part._Part._nScaleRoot = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_TEMPO, strlen(MARKER_FIELD_TEMPO))) { // Tempo
            pValue = pField + strlen(MARKER_FIELD_TEMPO);
            Part._Part._tempo = atof(pValue);
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

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    // Save the part
    if(!song->AddSongPart(&Part))
        return false;

    return true;
}

//=================================================================================================
// Saves note info
bool MidiFile::SaveNoteInfo(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

    sData = MARKER_NOTE_INFO;

    // Interval
    if(note->_nInterval != 0) {
        sPair.Format("%s%d|", MARKER_FIELD_INTERVAL, note->_nInterval);
        sData += sPair;
    }

    // String
    if(note->_tab._string != NO_INT && note->_tab._string != 0) {
        sPair.Format("%s%d|", MARKER_FIELD_STRING, note->_tab._string);
        sData += sPair;
    }

    // Fret
    if(note->_tab._fret != NO_INT) {
        sPair.Format("%s%d|", MARKER_FIELD_FRET, note->_tab._fret);
        sData += sPair;
    }

    // Finger
    if(note->_tab._finger != NO_INT) {
        sPair.Format("%s%d|", MARKER_FIELD_FINGER, note->_tab._finger);
        sData += sPair;
    }

    // Mode
    if(note->_tab._mode != Note::TAB_INFO::MODE_NONE) {
        sPair.Format("%s%d|", MARKER_FIELD_MODE, note->_tab._mode);
        sData += sPair;
    }

    // Add to the report
    Serial.printf("Note info: %s", (const char*)sData);
    //Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets note info
bool MidiFile::GetNoteInfo(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
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
            _note._tab._string = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_FRET, strlen(MARKER_FIELD_FRET))) { // Tab - fret
            pValue = pField + strlen(MARKER_FIELD_FRET);
            _note._tab._fret = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_FINGER, strlen(MARKER_FIELD_FINGER))) { // Tab - finger
            pValue = pField + strlen(MARKER_FIELD_FINGER);
            _note._tab._finger = atoi(pValue);
        } else if(!strncmp(pField, MARKER_FIELD_MODE, strlen(MARKER_FIELD_MODE))) { // Tab - mode
            pValue = pField + strlen(MARKER_FIELD_MODE);
            _note._tab._mode = atoi(pValue);
        } else { // Unknown
            return false;
        }

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    return true;
}

//=================================================================================================
// Saves song part's name to mark the start of new part
bool MidiFile::SaveSongPart(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

    sData = MARKER_SONG_PART;

    // Part's name
    sPair.Format("%s%s|", MARKER_FIELD_NAME, (const char*)note->_sSongPart);
    sData += sPair;

    // Add to the report
    Serial.printf("Song part: %s", (const char*)sData);
    Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets song part name
bool MidiFile::GetSongPart(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    Note       Note;
    String string;

    sCopy = pMarker;
    pField = strtok_s((char*)(const char*)sCopy, "|", &pNextToken);
    while(pField) {
        if(!strncmp(pField, MARKER_FIELD_NAME, strlen(MARKER_FIELD_NAME))) { // Part's name
            pValue = pField + strlen(MARKER_FIELD_NAME);
            Note._sSongPart = pValue;
        } else { // Unknown
            return false;
        }

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = trackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._state = Note::STATE_PART_MARK;
    Note._nSource = Note::SOURCE_FILE;

    track->addNote(Note);

    // Add to the report
    Serial.printf("%6.2f  Song part: %s", trackTime, (const char*)Note._sSongPart);
    Log((const char*)string);

    return true;
}

//=================================================================================================
// Saves measure numbers using MIDI control events
bool MidiFile::SaveMeasure(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    String sData;  // Data to be saved
    String sValue;
    String sPair;
    String string;

    sData = MARKER_MEASURE;

    // Part measure
    sPair.Format("%s%d|", MARKER_FIELD_PART_MEASURE, note->_nPartMeasure);
    sData += sPair;

    // Song measure
    sPair.Format("%s%d|", MARKER_FIELD_SONG_MEASURE, note->_nSongMeasure);
    sData += sPair;

    // Add to the report
    Serial.printf("Measure: %s", (const char*)sData);
    //Log((const char*)string);

    // Save the data
    if(!SaveMetaEvent(timeDiff, META_MARKER, track->_channel, (const char*)sData, sData.Length(), sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Gets measure numbers
bool MidiFile::GetMeasure(const char* pMarker, float trackTime, Song* song, Track* track) {
    char*		pField = nullptr;	// Current date field
    char*		pNextToken = nullptr;	// Next token
    String	sCopy;	// String copy
    const char* pValue;
    Note       Note;
    String string;

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

        pField = strtok_s(nullptr, "|", &pNextToken);
    }

    // Make the mark note
    Note._start = trackTime;
    Note._volume = 0.0f;
    Note._duration = 0.0f;
    Note._state = Note::STATE_MEASURE_MARK;
    Note._nSource = Note::SOURCE_FILE;

    track->addNote(Note);

    // Add to the report
    Serial.printf("%6.2f  Measure: %d/%d", trackTime, Note._nPartMeasure, Note._nSongMeasure);
    //Log((const char*)string);

    return true;
}

//=================================================================================================
// Gets marker data: chord info, measure mark, song part's name, etc.
// Format: <MarkerType>Field=Value|Field=Value
bool MidiFile::GetMarker(const char* pMarker, float trackTime, Song* song, Track* track) {
    if(!strncmp(pMarker, MARKER_CHORD, strlen(MARKER_CHORD))) { // Chord
        if(!GetChord(pMarker + strlen(MARKER_CHORD), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_PART, strlen(MARKER_SONG_PART))) { // Song part
        if(!GetSongPart(pMarker + strlen(MARKER_SONG_PART), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_PART_INFO, strlen(MARKER_SONG_PART_INFO))) { // Song part info
        if(!GetPartInfo(pMarker + strlen(MARKER_SONG_PART_INFO), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_NOTE_INFO, strlen(MARKER_NOTE_INFO))) { // Note info
        if(!GetNoteInfo(pMarker + strlen(MARKER_NOTE_INFO), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_SONG_INFO, strlen(MARKER_SONG_INFO))) { // Song part info
        if(!GetSongInfo(pMarker + strlen(MARKER_SONG_INFO), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_MEASURE, strlen(MARKER_MEASURE))) { // Measure number
        if(!GetMeasure(pMarker + strlen(MARKER_MEASURE), trackTime, song, track))
            return false;
    } else if(!strncmp(pMarker, MARKER_MIX, strlen(MARKER_MIX))) { // Mix
        if(!GetMix(pMarker + strlen(MARKER_MIX), song))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves note's tab info using MIDI control events
bool MidiFile::SaveTab(int32_t timeDiff, Note* note, Track* track, Buffer& sTrackData) {
    // String
    if(note->_tab._string != NO_INT) {
        if(!SaveControlEvent(timeDiff, track->_channel, CONTROL_TAB_STRING, note->_tab._string, sTrackData))
            return false;
    }

    // Fret
    if(note->_tab._fret != NO_INT) {
        if(!SaveControlEvent(timeDiff, track->_channel, CONTROL_TAB_FRET, note->_tab._fret, sTrackData))
            return false;
    }

    // Finger
    if(note->_tab._finger != NO_INT) {
        if(!SaveControlEvent(timeDiff, track->_channel, CONTROL_TAB_FINGER, note->_tab._finger, sTrackData))
            return false;
    }

    // Mode
    if(note->_tab._mode != Note::TAB_INFO::MODE_NONE) {
        if(!SaveControlEvent(timeDiff, track->_channel, CONTROL_TAB_MODE, note->_tab._mode, sTrackData))
            return false;
    }

    return true;
}

//=================================================================================================
// Saves control eventId
// bEvent - whether to save eventId ID or keep the 'running status'
bool MidiFile::SaveNote(int32_t timeDiff, bool bEvent, int32_t channel, bool bOn, int32_t midiNote, float volume, Buffer& sTrackData) {
    uint8_t	    szBuffer[8];	// Event data
    String	string;		// General string

    // Note data
    szBuffer[0] = midiNote;

    // Velocity (volume). Looks like it's common not to use 'note off' eventId, but use 'note on' with velocity set to 0.
    if(bOn) { // Start of the note
        //szBuffer[1] = (uint8_t)(127.0f * volume + 0.5f); // Linear conversion
        szBuffer[1] = VolumeToMidi(volume); // Logarithmic
    } else // End of the note
        szBuffer[1] = 0;

    Serial.printf("%7.3f sec: Save note %d, volume %0.2f, velocity %d", getTimeDiff(timeDiff), midiNote, volume, szBuffer[1]);
    Log((const char*)string);

    // Save the eventId
    if(!SaveEvent(timeDiff, bEvent ? MIDI_NOTE_ON : MIDI_NONE, channel, (const char*)szBuffer, 2, sTrackData))
        return false;

    return true;
}

//=================================================================================================
// Saves a MIDI (note, system or meta) eventId
bool MidiFile::SaveEvent(int32_t timeDiff, MIDI_EVENT eventId, int32_t channel, const char* eventData, int32_t nEventDataSize, Buffer& sTrackData) {
    uint8_t	    szBuffer[64];	// Conversion buffer
    float      dTimeDiff;		// Event time
    int32_t		    bytes = 0;		// Bytes saved in the conversion
    uint8_t	    nEventID = 0;	// Event ID and channel (4 bits and 4 bits)
    String	string;		// General string

    // Event time (incremental) ----------
    if(PutVarLen(timeDiff, (char*)szBuffer, sizeof(szBuffer), bytes))
        sTrackData.Add((const char*)szBuffer, bytes);
    else
        return false;

    // Add eventId time to the report
    dTimeDiff = getTimeDiff((int32_t)timeDiff);
    Serial.printf("Event diff: %04d, %7.3f sec, beat=%7.3f", timeDiff, dTimeDiff, _beatTime);
    //Log((const char*)string);

    // Event ID and channel ----------
    if(eventId != MIDI_NONE) { // Note a MIDI note eventId, no channel
        if(eventId >= MIDI_SYSTEM) { // Note a MIDI note eventId, no channel
            nEventID = (uint8_t)eventId;
        } else { // Set the channel
            // 4 bits for eventId type, 4 bits for channel number
            nEventID = ((uint8_t)eventId) & 0xF0; // Event type
            nEventID += ((uint8_t)channel) & 0x0F; // Channel number;
        }

        sTrackData.Add((const char*)(uint8_t*)&nEventID, 1);
    }

    // Event data ----------
    sTrackData.Add(eventData, nEventDataSize);

    return true;
}

//=================================================================================================
// Gets MIDI program number for an instrument (thing) type
uint8_t MidiFile::getProgram(Instrument instrument) {
    switch(instrument) {
        case ELECTRIC_BASS_FINGER:
            return PROGRAM_FINGER_BASS;
        case ELECTRIC_GUITAR_CLEAN:
            return PROGRAM_CLEAN_GUITAR;
        case OVERDRIVEN_GUITAR:
            return PROGRAM_OVERDRIVEN_GUITAR;
        case DISTORTION_GUITAR:
            return PROGRAM_DISTORTION_GUITAR;
        case NONE:
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
bool MidiFile::AddData(void* data, int32_t size) {
    int32_t increment = 1024 * 100;
    int32_t dataSize;

    // Check the buffer size
    dataSize = (int32_t)(_dataOffset - _data);
    if(dataSize + size > (int32_t)_dataSize) {
        _dataSize += Max(increment, size);
        _data = (char*)realloc(_data, _dataSize); // +1 is for zero-terminator
        if(!_data) {
            Serial.printf("Can't allocate %d bytes", _dataSize);
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
