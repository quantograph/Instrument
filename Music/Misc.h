#ifndef Misc_h
#define Misc_h

#include <Arduino.h>
#include <list>
#include <vector>
#include <float.h>

void Reverse2Bytes(char* data);
void Reverse4Bytes(char* data);
//void ShowBits(uint32_t value, String& s);
//void LogScreen(const char* line);
extern const char* _instrumentNames[];
extern const char* _drumNames[];

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
#define FINGER_TIP_RADIUS   0.6f    // Radius of the finger tip
#define MAX_STRINGS         7       // Maximum number of strings on a guitar
#define MAX_FRETS           22      // Maximum number of frets on a guitar
#define MAX_SOUND_FRET      18      // Maxumum fret with the loaded sound
#define MAX_FINGERS         3       // Maximum number fingers within the same fret on different strings (for 'A' chord)
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

// Scale type ----------
enum SCALE {
    SCALE_NONE, // Not set
    // ############ NOTE! Don't insert, only append ###########
    // ===== Major and minor scales (30)
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
    // ===== Modes
    SCALE_IONIAN, // Ionian mode of the major scale, starts at it's 1 step
    SCALE_DORIAN, // Dorian mode of the major scale, starts at it's 2 step
    SCALE_PHRYGIAN, // Phrygian mode of the major scale, starts at it's 3 step
    SCALE_LYDIAN, // Lydian mode of the major scale, starts at it's 4 step
    SCALE_MIXOLYDIAN, // Mixolydian mode of the major scale, starts at it's 5 step
    SCALE_AEOLIAN, // Aeolian mode of the major scale, starts at it's 6 step
    SCALE_LOCRIAN, // Locrian mode of the major scale, starts at it's 7 step
    // ===== Normal scales
    SCALE_MAJOR, // Diatonic major
    SCALE_NAT_MINOR, // Natural minor
    SCALE_HARM_MINOR, // Harmonic minor
    SCALE_MEL_MINOR, // Melodic minor (ascending)
    SCALE_MEL_MINOR_DESC, // Melodic minor (descending)
    // ===== Exotic scales
    SCALE_ENIGM, // Enigmatic
    SCALE_NEAP, // Neapolitanian
    SCALE_NEAP_MIN, // Neapolitanian minor
    SCALE_HUNG_MIN, // Hungarian minor
    // ===== Others
    SCALE_PENT_MAJOR, // Pentatonic major
    SCALE_PENT_MINOR, // Pentatonic minor
    SCALE_BEBOP, // Bebop
    SCALE_BLUES, // Blues
    SCALE_DIM, // Diminished
    SCALE_CHROMATIC // Chromatic
    // ############ NOTE! Don't insert, only append ###########
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
enum INSTRUMENT {
    PERCUSSION,
    BRIGHT_ACOUSTIC_PIANO,
    ELECTRIC_GRAND_PIANO,
    HONKY_TONK_PIANO,
    ELECTRIC_PIANO_1,
    ELECTRIC_PIANO_2,
    HARPSICHORD,CLAVI,
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
    ACCORDION,HARMONICA,
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
    ACOUSTIC_GRAND_PIANO
};

// MIDI notes for drums
#define DRUM_BASS_DRUM    35 // Acoustic Bass Drum
//#define DRUM_BASS_DRUM    36 // Bass Drum 1
//#define DRUM_SIDE_STICK     37 // Side Stick
#define DRUM_SNARE          38 // Acoustic Snare
// 39 Hand Clap
// 40 Electric Snare
#define DRUM_TOM_1          41 // Low Floor Tom
#define DRUM_HIHAT_CLOSED   42 // Closed Hi-Hat
#define DRUM_TOM_2          43 // High Floor Tom
#define DRUM_HIHAT_FOOT     44 // Pedal Hi-Hat
#define DRUM_TOM_3          45 // Low Tom
#define DRUM_HIHAT_OPEN     46 // Open Hi-Hat
#define DRUM_TOM_4          47 // Low-Mid Tom
#define DRUM_TOM_5          48 // Hi-Mid Tom
#define DRUM_CRASH_1        49 // Crash Cymbal 1
#define DRUM_TOM_6          50 // High Tom
#define DRUM_RIDE_CYMBAL    51 // Ride Cymbal 1
#define DRUM_CHINESE_1      52 // Chinese Cymbal
#define DRUM_RIDE_BELL      53 // Ride bell
// 54 Tambourine
#define DRUM_SPLASH_1       55 // Splash Cymbal
// 56 Cowbell
#define DRUM_CRASH_2        57 // Crash Cymbal 2
// 58 Vibraslap
// 59 Ride Cymbal 2
// 60 Hi Bongo
// 61 Low Bongo
// 62 Mute Hi Conga
// 63 Open Hi Conga
// 64 Low Conga
// 65 High Timbale
// 66 Low Timbale
// 67 High Agogo
// 68 Low Agogo
// 69 Cabasa
// 70 Maracas
// 71 Short Whistle
// 72 Long Whistle
// 73 Short Guiro
// 74 Long Guiro
// 75 Claves
// 76 Hi Wood Block
// 77 Low Wood Block
// 78 Mute Cuica
// 79 Open Cuica
// 80 Mute Triangle
// 81 Open Triangle
// 82 Shaker

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
    INSTRUMENT _instrument = INSTRUMENT::ACOUSTIC_GRAND_PIANO;
    String _name = "";
    //uint8_t _midiInstrument = 0;
    int _startNote = 0; // MIDI note number for the first fingering
    int _fingeringCount = 0; // Number of fingerings in _fingerings
    Fingering* _fingerings = NULL;
    int _buttonCount = 0; // Number of buttons mapped
    int* _buttonMap = NULL; // Touch sensors, array
    int* _keyMap = NULL; // Instrument keys, array
    uint8_t* _noteMap = NULL; // MIDI notes, array
    uint32_t _keyMask = 0; // Bitmask for the 'unwanted' keys
};

#endif
