#pragma once

class Synth;

#define NO_INT INT32_MAX
#define NO_MIDI_NOTE 255
#define	NO_FLOAT FLT_MAX

// "No value" definitions
/*#define	NO_DWORD	ULONG_MAX
#define	NO_DOUBLE	DBL_MAX
#define	NO_WORD		USHRT_MAX
#define	NO_USHORT	USHRT_MAX
#define	NO_SHORT	SHRT_MAX
#define	NO_INT		INT_MAX
#define	NO_UINT		UINT_MAX
#define	NO_FLOAT	FLT_MAX
#define	NO_COLOR	Gdiplus::Color(0, 0, 0, 0);*/

#define	FINGER_MIDI_NOTE    253	    // MIDI note for 'finger' row
#define	CHORD_MIDI_NOTE     254	    // MIDI note for 'chord' row
#define	NO_MIDI_NOTE 	    255	    // MIDI note is not set. All good notes are from 0 to 127
#define	MAX_NOTES		    256	    // Maximum number of MIDI notes
#define	MAX_MIDI_NOTE       131     // Maximum MIDI note with a frequency
#define BASS_ROOT           36      // Root note for bass chords
#define BASS_MIN            28      // Lowest bass note
#define MIDDLE_C            60      // Middle C note
#define GUITAR_ROOT         48      // Root note for guitar chords
#define GUITAR_MIN          40      // Lowest guitar note
#define TENOR_SAX_START_NOTE (60 - 16) // MIDI note number for the lowest of tenor saxophone notes (fingering 17 is note 60)
#define ALTO_SAX_START_NOTE (TENOR_SAX_START_NOTE + 7) // MIDI note number for the lowest of alto saxophone notes

// Key signature
enum KEY_SIGNATURE {
    KEY_7FLATS	= -7,	// 7 flats
    KEY_6FLATS	= -6,	// 6 flats
    KEY_5FLATS	= -5,	// 5 flats
    KEY_4FLATS	= -4,	// 4 flats
    KEY_3FLATS	= -3,	// 3 flats
    KEY_2FLATS	= -2,	// 2 flats
    KEY_1FLAT	= -1,	// 1 flat
    KEY_NONE	= 0,	// Not set
    KEY_1SHARP	= 1,	// 1 sharps
    KEY_2SHARPS	= 2,	// 2 sharps
    KEY_3SHARPS	= 3,	// 3 sharps
    KEY_4SHARPS	= 4,	// 4 sharps
    KEY_5SHARPS	= 5,	// 5 sharps
    KEY_6SHARPS	= 6,	// 6 sharps
    KEY_7SHARPS	= 7,	// 7 sharps
};

// MIDI instruments
//  1-8    Piano                     65-72  Reed
//  9-16   Chromatic Percussion      73-80  Pipe
// 17-24   Organ                     81-88  Synth Lead
// 25-32   Guitar                    89-96  Synth Pad
// 33-40   Bass                      97-104 Synth Effects
// 41-48   Strings                  105-112 Ethnic
// 49-56   Ensemble                 113-120 Percussive
// 57-64   Brass                    121-128 Sound Effects
enum Instrument {
    ACOUSTIC_GRAND_PIANO,
    BRIGHT_ACOUSTIC_PIANO,
    ELECTRIC_GRAND_PIANO,
    HONKY_TONK_PIANO,
    ELECTRIC_PIANO_1,
    ELECTRIC_PIANO_2,
    HARPSICHORD,
    CLAVI,
    CELESTA,
    GLOCKENSPIEL,
    MUSIC_BOX,
    VIBRAPHONE,
    MARIMBA,
    XYLOPHONE,
    TUBULAR_BELLS,
    DULCIMER,
    DRAWBAR_ORGAN,
    PERCUSSIVE_ORGAN,
    ROCK_ORGAN,
    CHURCH_ORGAN,
    REED_ORGAN,
    ACCORDION,
    HARMONICA,
    TANGO_ACCORDION,
    ACOUSTIC_GUITAR_NYLON,
    ACOUSTIC_GUITAR_STEEL,
    ELECTRIC_GUITAR_JAZZ,
    ELECTRIC_GUITAR_CLEAN,
    ELECTRIC_GUITAR_MUTED,
    OVERDRIVEN_GUITAR,
    DISTORTION_GUITAR,
    GUITAR_HARMONICS,
    ACOUSTIC_BASS,
    ELECTRIC_BASS_FINGER,
    ELECTRIC_BASS_PICK,
    FRETLESS_BASS,
    SLAP_BASS_1,
    SLAP_BASS_2,
    SYNTH_BASS_1,
    SYNTH_BASS_2,
    VIOLIN,
    VIOLA,
    CELLO,
    CONTRABASS,
    TREMOLO_STRINGS,
    PIZZICATO_STRINGS,
    ORCHESTRAL_HARP,
    TIMPANI,
    STRING_ENSEMBLE_1,
    STRING_ENSEMBLE_2,
    SYNTHSTRINGS_1,
    SYNTHSTRINGS_2,
    CHOIR_AAHS,
    VOICE_OOHS,
    SYNTH_VOICE,
    ORCHESTRA_HIT,
    TRUMPET,
    TROMBONE,
    TUBA,
    MUTED_TRUMPET,
    FRENCH_HORN,
    BRASS_SECTION,
    SYNTHBRASS_1,
    SYNTHBRASS_2,
    SOPRANO_SAX,
    ALTO_SAX,
    TENOR_SAX,
    BARITONE_SAX,
    OBOE,
    ENGLISH_HORN,
    BASSOON,
    CLARINET,
    PICCOLO,
    FLUTE,
    RECORDER,
    PAN_FLUTE,
    BLOWN_BOTTLE,
    SHAKUHACHI,
    WHISTLE,
    OCARINA,
    LEAD_1_SQUARE,
    LEAD_2_SAWTOOTH,
    LEAD_3_CALLIOPE,
    LEAD_4_CHIFF,
    LEAD_5_CHARANG,
    LEAD_6_VOICE,
    LEAD_7_FIFTHS,
    LEAD_8_BASS_LEAD,
    PAD_1_NEW_AGE,
    PAD_2_WARM,
    PAD_3_POLYSYNTH,
    PAD_4_CHOIR,
    PAD_5_BOWED,
    PAD_6_METALLIC,
    PAD_7_HALO,
    PAD_8_SWEEP,
    FX_1_RAIN,
    FX_2_SOUNDTRACK,
    FX_3_CRYSTAL,
    FX_4_ATMOSPHERE,
    FX_5_BRIGHTNESS,
    FX_6_GOBLINS,
    FX_7_ECHOES,
    FX_8_SCIFI,
    SITAR,
    BANJO,
    SHAMISEN,
    KOTO,
    KALIMBA,
    BAG_PIPE,
    FIDDLE,
    SHANAI,
    TINKLE_BELL,
    AGOGO,
    STEEL_DRUMS,
    WOODBLOCK,
    TAIKO_DRUM,
    MELODIC_TOM,
    SYNTH_DRUM,
    REVERSE_CYMBAL,
    GUITAR_FRET_NOISE,
    BREATH_NOISE,
    SEASHORE,
    BIRD_TWEET,
    TELEPHONE_RING,
    HELICOPTER,
    APPLAUSE,
    GUNSHOT,
    PERCUSSION,
    NONE,
    MAX_INSTRUMENT
};

typedef std::list<Instrument> InstrumentList;

// MIDI notes for drums
enum DRUM_NOTES {
    DRUM_BASS1 = 35,       // 35 Acoustic Bass Drum
    DRUM_BASS2,            // 36 Bass Drum 1
    DRUM_SIDE_STICK,       // 37 Side Stick
    DRUM_SNARE,            // 38 Acoustic Snare
    DRUM_HAND_CLAP,        // 39 Hand Clap
    DRUM_ELECTRIC_SNARE,   // 40 Electric Snare
    DRUM_TOM_FLOOR_LOW,    // 41 Low Floor Tom
    DRUM_HIHAT_CLOSED,     // 42 Closed Hi-Hat
    DRUM_TOM_FLOOR_HIGH,   // 43 High Floor Tom
    DRUM_HIHAT_FOOT,       // 44 Pedal Hi-Hat
    DRUM_TOM_LOW,          // 45 Low Tom
    DRUM_HIHAT_OPEN,       // 46 Open Hi-Hat
    DRUM_TOM_LOW_MID,      // 47 Low-Mid Tom
    DRUM_TOM_HIGH_MID,     // 48 Hi-Mid Tom
    DRUM_CRASH1,           // 49 Crash Cymbal 1
    DRUM_TOM_HIGH,         // 50 High Tom
    DRUM_RIDE,             // 51 Ride Cymbal 1
    DRUM_CHINESE,          // 52 Chinese Cymbal
    DRUM_RIDE_BELL,        // 53 Ride bell
    DRUM_TAMBOURINE,       // 54 Tambourine
    DRUM_SPLASH1,          // 55 Splash Cymbal
    DRUM_COWBELL,          // 56 Cowbell
    DRUM_CRASH2,           // 57 Crash Cymbal 2
    DRUM_VIBRASLAP,        // 58 Vibraslap
    DRUM_RIDE2,            // 59 Ride Cymbal 2
    DRUM_HI_BONGO,         // 60 Hi Bongo
    DRUM_LOW_BONGO,        // 61 Low Bongo
    DRUM_MUTE_HI_CONGA,    // 62 Mute Hi Conga
    DRUM_OPEN_HI_CONGA,    // 63 Open Hi Conga
    DRUM_LOW_CONGA,        // 64 Low Conga
    DRUM_HIGH_TIMBALE,     // 65 High Timbale
    DRUM_LOW_TIMBALE,      // 66 Low Timbale
    DRUM_HIGH_AGOGO,       // 67 High Agogo
    DRUM_LOW_AGOGO,        // 68 Low Agogo
    DRUM_CABASA,           // 69 Cabasa
    DRUM_MARACAS,          // 70 Maracas
    DRUM_SHORT_WHISTLE,    // 71 Short Whistle
    DRUM_LONG_WHISTLE,     // 72 Long Whistle
    DRUM_SHORT_GUIRO,      // 73 Short Guiro
    DRUM_LONG_GUIRO,       // 74 Long Guiro
    DRUM_CLAVES,           // 75 Claves
    DRUM_HI_WOOD_BLOCK,    // 76 Hi Wood Block
    DRUM_LOW_WOOD_BLOCK,   // 77 Low Wood Block
    DRUM_MUTE_CUICA,       // 78 Mute Cuica
    DRUM_OPEN_CUICA,       // 79 Open Cuica
    DRUM_MUTE_TRIANGLE,    // 80 Mute Triangle
    DRUM_OPEN_TRIANGLE,    // 81 Open Triangle
    DRUM_SHAKER            // 82 Shaker
};

// Scale type ----------
enum SCALE {
    SCALE_NONE, // Not set
    // ############ NOTE! Don't insert, only append ###########
    // Major and minor scales (30) =====
    // No flats, no sharps
    SCALE_C_MAJOR,
    SCALE_A_MINOR,
    // Sharps
    SCALE_G_MAJOR, // 1#
    SCALE_E_MINOR, // 1#
    SCALE_D_MAJOR, // 2#
    SCALE_B_MINOR, // 2#
    SCALE_A_MAJOR, // 3#
    SCALE_Fs_MINOR, // 3#
    SCALE_E_MAJOR, // 4#
    SCALE_Cs_MINOR, // 4#
    SCALE_B_MAJOR, // 5#
    SCALE_Gs_MINOR, // 5#
    SCALE_Fs_MAJOR, // 6#
    SCALE_Ds_MINOR, // 6#
    SCALE_Cs_MAJOR, // 7#
    SCALE_As_MINOR, // 7#
    // Flats
    SCALE_F_MAJOR, // 1b
    SCALE_D_MINOR, // 1b
    SCALE_Bb_MAJOR, // 2b
    SCALE_G_MINOR, // 2b
    SCALE_Eb_MAJOR, // 3b
    SCALE_C_MINOR, // 3b
    SCALE_Ab_MAJOR, // 4b
    SCALE_F_MINOR, // 4b
    SCALE_Db_MAJOR, // 5b
    SCALE_Bb_MINOR, // 5b
    SCALE_Gb_MAJOR, // 6b
    SCALE_Eb_MINOR, // 6b
    SCALE_Cb_MAJOR, // 7b
    SCALE_Ab_MINOR, // 7b
    // ===== End of major and minor scales
    SCALE_CHROMATIC, // Chromatic
    SCALE_MAJOR, // Diatonic major
    SCALE_IONIAN, // Ionian mode of the major scale, starts at it's 1 step
    SCALE_DORIAN, // Dorian mode of the major scale, starts at it's 2 step
    SCALE_PHRYGIAN, // Phrygian mode of the major scale, starts at it's 3 step
    SCALE_LYDIAN, // Lydian mode of the major scale, starts at it's 4 step
    SCALE_MIXOLYDIAN, // Mixolydian mode of the major scale, starts at it's 5 step
    SCALE_AEOLIAN, // Aeolian mode of the major scale, starts at it's 6 step
    SCALE_LOCRIAN, // Locrian mode of the major scale, starts at it's 7 step
    SCALE_NAT_MINOR, // Natural minor
    SCALE_HARM_MINOR, // Harmonic minor
    SCALE_MEL_MINOR, // Melodic minor (ascending)
    SCALE_MEL_MINOR_DESC, // Melodic minor (descending)
    SCALE_PENT_MAJOR, // Pentatonic major
    SCALE_PENT_MINOR, // Pentatonic minor
    SCALE_BLUES, // Blues
    SCALE_DIM, // Diminished
    SCALE_BEBOP, // Bebop
    SCALE_ENIGM, // Enigmatic
    SCALE_NEAP, // Neapolitanian
    SCALE_NEAP_MIN, // Neapolitanian minor
    SCALE_HUNG_MIN // Hungarian minor
    // ############ NOTE! Don't insert, only append ###########
};

// Scale names
#define SCALE_NAME_CHROMATIC "Chromatic"
#define SCALE_NAME_MAJOR "Major"
#define SCALE_NAME_IONIAN "Ionian"
#define SCALE_NAME_DORIAN "Dorian"
#define SCALE_NAME_PHRYGIAN "Phrygian"
#define SCALE_NAME_LYDIAN "Lydian"
#define SCALE_NAME_MIXOLYDIAN "Mixolydian"
#define SCALE_NAME_AEOLIAN "Aeolian"
#define SCALE_NAME_LOCRIAN "Locrian"
#define SCALE_NAME_NATURAL_MINOR "Natural minor"
#define SCALE_NAME_HARMONIC_MINOR "Harmonic minor"
#define SCALE_NAME_MELODIC_MINOR "Melodic minor"
#define SCALE_NAME_MELODIC_MINOR_DESC "Melodic minor (descending)"
#define SCALE_NAME_PENTATONIC_MAJOR "Pentatonic major"
#define SCALE_NAME_PENTATONIC_MINOR "Pentatonic minor"
#define SCALE_NAME_BLUES "Blues"
#define SCALE_NAME_BEBOP "Bebop"
#define SCALE_NAME_DIMINISHED "Diminished"
#define SCALE_NAME_ENIGMATIC "Enigmatic"
#define SCALE_NAME_NEAPOLITANIAN "Neapolitanian"
#define SCALE_NAME_NEAPOLITANIAN_MINOR "Neapolitanian minor"
#define SCALE_NAME_HUNGARIAN_MINOR "Hungarian minor"
#define SCALE_NAME_C_MAJOR "C maj"
#define SCALE_NAME_A_MINOR "A min"
#define SCALE_NAME_G_MAJOR "G maj"
#define SCALE_NAME_E_MINOR "E min"
#define SCALE_NAME_D_MAJOR "D maj"
#define SCALE_NAME_B_MINOR "B min"
#define SCALE_NAME_A_MAJOR "A maj"
#define SCALE_NAME_Fs_MINOR "F# min"
#define SCALE_NAME_E_MAJOR "E maj"
#define SCALE_NAME_Cs_MINOR "C# min"
#define SCALE_NAME_B_MAJOR "B maj"
#define SCALE_NAME_Gs_MINOR "G# min"
#define SCALE_NAME_Fs_MAJOR "F# maj"
#define SCALE_NAME_Ds_MINOR "D# min"
#define SCALE_NAME_Cs_MAJOR "C# maj"
#define SCALE_NAME_As_MINOR "A# min"
#define SCALE_NAME_F_MAJOR "F maj"
#define SCALE_NAME_D_MINOR "D min"
#define SCALE_NAME_Bb_MAJOR "Bb maj"
#define SCALE_NAME_G_MINOR "G min"
#define SCALE_NAME_Eb_MAJOR "Eb maj"
#define SCALE_NAME_C_MINOR "C min"
#define SCALE_NAME_Ab_MAJOR "Ab maj"
#define SCALE_NAME_F_MINOR "F min"
#define SCALE_NAME_Db_MAJOR "Db maj"
#define SCALE_NAME_Bb_MINOR "Bb min"
#define SCALE_NAME_Gb_MAJOR "Gb maj"
#define SCALE_NAME_Eb_MINOR "Eb min"
#define SCALE_NAME_Cb_MAJOR "Cb maj"
#define SCALE_NAME_Ab_MINOR "Ab min"

// Saxophone fingering
struct Fingering {
    Fingering(const char* buttons) : _buttonString(buttons) {} // List of button numbers
    String _buttonString;
    uint32_t _keys; // Bitmask with key numbers
    uint32_t _mappedKeys; // Keys mapped to sensors
    bool _mapped;
    uint16_t _note; // Note number, starting with 1 for the lowest
    int _buttons[24]; // Button numbers pressed
    int _buttonCount; // Number of buttons in the list
};

struct InstrumentInfo {
    ~InstrumentInfo();
    const AudioSynthWavetable::instrument_data* _sample{nullptr};
    Instrument _instrument = Instrument::NONE;
    String _name = "";
    //uint8_t _midiInstrument = 0;
    int _startNote = 0; // MIDI note number for the first fingering
    int _fingeringCount = 0; // Number of fingerings in _fingerings
    Fingering* _fingerings = nullptr;
    int _buttonCount = 0; // Number of buttons mapped
    int* _buttonMap = nullptr; // Touch sensors, array
    int* _keyMap = nullptr; // Instrument keys, array
    uint8_t* _noteMap = nullptr; // MIDI notes, array
    uint32_t _keyMask = 0; // Bitmask for the 'unwanted' keys
    AudioMixer4* _mixer{nullptr};
    uint8_t _mixerChannel{0};
    Synth* _synth{nullptr}; // Synthesizer for this instrument
    int _voices = 0; // Number of simultaneous voices for the synthesizer
};

typedef std::list<InstrumentInfo> InstrumentInfoList;
