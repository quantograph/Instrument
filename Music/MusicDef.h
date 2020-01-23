#ifndef MUSIC_MUSIC_DEF_H
#define MUSIC_MUSIC_DEF_H

#include <vector>
#include <list>
#include <Arduino.h>

// Global definitions ---------------------------------------------------------

// Colors ----------
struct Color {
    double _red; // Red (0.0 - 1.0)
    double _green; // Green (0.0 - 1.0)
    double _blue; // Blue (0.0 - 1.0)
    Color(double red, double green, double blue) : _red(red), _green(green), _blue(blue) {}
};

typedef std::vector<Color> COLORS;
typedef COLORS::iterator COLORS_ITER;
typedef COLORS::const_iterator COLORS_CITER;

// Values ----------
struct Value {
    enum SOURCE { // Source of the value
        SOURCE_NONE, // Not set
        SOURCE_KEYBOARD, // Computer keyboard
        SOURCE_ANALOG // Analog input
    };

    Value() : _source(SOURCE_NONE), _sequence(0), _int(0) {
    }

    SOURCE _source; // Source
    uint64_t _sequence; // Sequence number
    //Time _time; // Time of the value
    union { // The value
        double _double;
        int64_t _int;
    };
};

typedef std::list<Value> VALUES;
typedef VALUES::iterator VALUES_ITER;
typedef VALUES::const_iterator VALUES_CITER;

// List of integers
typedef std::vector<int> INT_LIST;
typedef INT_LIST::iterator INT_LIST_ITER;
typedef INT_LIST::const_iterator INT_LIST_CITER;

// Swaping two values
template<class T>
inline void SWAP(T& Value1, T& Value2) {
    T Temp = Value1;
    Value1 = Value2;
    Value2 = Temp;
}

#define NO_MIDI_NOTE 255

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
#define DRUM_BASS_DRUM_2    35 // Acoustic Bass Drum
#define DRUM_BASS_DRUM_1    36 // Bass Drum 1
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

// Settings for one instrument
struct InstrumentSettings {
    // Sound settings
    bool _play;
    double _volume;
    double _pan;
    INSTRUMENT _instrument;
    // Band settings
    int _lengthFrom; // Note length, from. Power of 2: 0 for whole note, 1 for 1/2, 2 for 1/4, ... , 6 for 1/32.
    int _lengthTo; // Note length, to. Power of 2.
    int _density; // Density of notes vs. pauses, %

    InstrumentSettings() {
        Reset();
    }

    void Reset() {
        _play = true;
        _volume = 1.0;
        _pan = 0.0;
        _instrument = (INSTRUMENT)-1;
        _lengthFrom = 8;
        _lengthTo = 4;
        _density = 70;
    }
};

// Settings for the Band
struct ComposerSettings {
    int _tempo; // Tempo, quarter notes per minute
    int _measureBeats; // Time signature numerator - number of beats in one measure (3 for 3/4 song)
    int _beatNote; // Time signature denominator - note duration for one beat, 4 is for a quarter note (4 for 3/4 song)
    SCALE _scaleType; // Scale type
    INT_LIST _verseChords; // Verse chord progression
    INT_LIST _chorusChords; // Chorus chord progression
    int _move; // Note to note move, in intervals
    int _moveRange; // Total note move range, in intervals
    int _repeatRhythm; // Repetition of rhythm, %
    int _repeatMelody; // Repetition of melody, %
    InstrumentSettings _lead; // Lead settings
    InstrumentSettings _rhythm; // Rhythm settings
    InstrumentSettings _bass; // Bass settings
    InstrumentSettings _drums; // Drums settings

    ComposerSettings() {
        Reset();
    }

    void Reset() {
        _tempo = 120;
        _measureBeats = 4;
        _beatNote = 4;
        _scaleType = SCALE_C_MAJOR;
        _verseChords.clear();
        _chorusChords.clear();
        _move = 3;
        _moveRange = 7;
        _repeatRhythm = 0;
        _repeatMelody = 0;
        // Volume
        _lead._volume = 0.8;
        _rhythm._volume = 0.5;
        _bass._volume = 0.9;
        _drums._volume = 1.0;
        // Pan
        _lead._pan = -0.5;
        _rhythm._pan = 0.5;
        _bass._pan = 0.0;
        _drums._pan = 0.0;
        // Instruments
        _lead._instrument = DISTORTION_GUITAR;
        _rhythm._instrument = ELECTRIC_GUITAR_CLEAN;
        _bass._instrument = ELECTRIC_BASS_FINGER;
        _drums._instrument = PERCUSSION;
    }
};

// Platforms
enum PLATFORM {
    PLATFORM_NONE, // Not set
    PLATFORM_EDISON, // Intel Edison
    PLATFORM_PI, // Raspberry Pi
    PLATFORM_WINDOWS // MS Windows
};

// Settings for all programs
/*struct Settings {
    PLATFORM _platform; // Platform - "None" for Linux or Windows, "Edison" for Intel Edison
    String _samplePath; // Path to instrument samples
    ComposerSettings _composer; // Composer settings

    Settings() {
        Reset();
    }

    void Reset() {
        _platform = PLATFORM_NONE;
        _composer.Reset();
    }
};*/

// Configuration parameter ----------
struct Parameter { // Configuration parameter, "tag=value"
    String _tag; // Tag name
    String _value; // Value string
};

typedef std::list<Parameter> PARAMS;
typedef PARAMS::iterator PARAMS_ITER;
typedef PARAMS::const_iterator PARAMS_CITER;

#endif // MUSIC_MUSIC_DEF_H
