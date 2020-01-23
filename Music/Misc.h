#ifndef Misc_h
#define Misc_h

#include <Arduino.h>
#include <list>
#include <vector>

void Reverse2Bytes(char* data);
void Reverse4Bytes(char* data);
//void ShowBits(uint32_t value, String& s);
//void LogScreen(const char* line);
extern const char* _instrumentNames[];
extern const char* _drumNames[];

#define NO_INT INT32_MAX
#define NO_MIDI_NOTE 255

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
