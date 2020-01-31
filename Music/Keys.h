#ifndef Keys_h
#define Keys_h
/*
class Keys {
public:

    #define NO_NOTE 255

InstrumentInfo _sopranoSaxInfo;
InstrumentInfo _altoSaxInfo;
InstrumentInfo _tenorSaxInfo;
InstrumentInfo _baritoneSaxInfo;
#define SAX_BUTTONS 9
int _saxButtonMap[SAX_BUTTONS] = { 2, 4, 6, 8, 10, 16, 18, 20, 22 }; // Buttons
int _saxKeyMap[SAX_BUTTONS] =    { 1, 2, 4, 5, 10, 14, 15, 16, 17 }; // Saxophone keys
uint32_t _saxKeyMask = 0; // Bitmask for the 'unwanted' keys
//   Key numbers    
//   \1\ (6)        
//      ( 2 ) |7|   
//        (3)  |8|  
//      ( 4 ) |9|   
//  |19|( 5 )       
//  |20|        10  
//  |21|( 14 ) 11X12
//      ( 15 )  13  
// |22| ( 16 )      
// (23)  17         
//      18          
#define SAX_FINGERINGS 24
Fingering _saxFingerings[SAX_FINGERINGS] = { // All saxophone fingerings
Fingering("2,4,5,14,15,16,"),         
Fingering("2,4,5,14,15,16,17,"),      
Fingering("2,4,5,14,15,"),            
Fingering("2,4,5,14,"),               
Fingering("2,4,5,15,"),               
Fingering("2,4,5,"),                  
Fingering("2,4,5,10,"),               
Fingering("2,4,"),                    
Fingering("2,14,"),                   
Fingering("2,"),                                        
Fingering("4,"),
Fingering(","),
Fingering("1,2,4,5,14,15,16,"),         
Fingering("1,2,4,5,14,15,16,17,"),      
Fingering("1,2,4,5,14,15,"),            
Fingering("1,2,4,5,14,"),               
Fingering("1,2,4,5,15,"),               
Fingering("1,2,4,5,"),                  
Fingering("1,2,4,5,10,"),               
Fingering("1,2,4,"),                    
Fingering("1,2,14,"),                   
Fingering("1,2,"),                                        
Fingering("1,4,"),
Fingering("1,")
};                                                                                      

// Flute ------------------------------
//          10          18 - octave shift
//      2 4 5    14 15 16
//    1                   17
InstrumentInfo _fluteInfo;
#define FLUTE_BUTTONS 11
int _fluteButtonMap[FLUTE_BUTTONS] = { 2, 4, 6, 8, 10, 16, 18, 20, 22, 13 }; // Buttons
int _fluteKeyMap[FLUTE_BUTTONS] =    { 1, 2, 4, 5, 10, 14, 15, 16, 17, 18 }; // Flute keys
uint32_t _fluteKeyMask = 0; // Bitmask for the 'unwanted' keys
#define FLUTE_FINGERINGS 32
Fingering _fluteFingerings[FLUTE_FINGERINGS] = { // All flute fingerings
Fingering("1,2,4,5,14,15,16,"),         // "D"    
Fingering("1,2,4,5,14,15,16,17,"),      // "D#/Eb"
Fingering("1,2,4,5,14,15,17,"),         // "E"    
Fingering("1,2,4,5,14,17,"),            // "F"    
Fingering("1,2,4,5,16,17,"),            // "F#/Gb"
Fingering("1,2,4,5,17,"),               // "G"    
Fingering("1,2,4,5,10,17,"),            // "G#/Ab"
Fingering("1,2,4,17,"),                 // "A"    
Fingering("1,2,14,17,"),                // "A#/Bb"
Fingering("1,2,17,"),                   // "B"    
Fingering("2,17,"),                     // "C"    
Fingering("17,"),                       // "C#/Db"
Fingering("1,4,5,14,15,16,"),           // "D"    
Fingering("1,4,5,14,15,16,17,"),        // "D#/Eb"
Fingering("1,2,4,5,14,15,17,18,"),      // "E"    
Fingering("1,2,4,5,14,17,18,"),         // "F"    
Fingering("1,2,4,5,16,17,18,"),         // "F#/Gb"
Fingering("1,2,4,5,17,18,"),            // "G"    
Fingering("1,2,4,5,10,17,18,"),         // "G#/Ab"
Fingering("1,2,4,17,18,"),              // "A"    
Fingering("1,2,14,17,18,"),             // "A#/Bb"
Fingering("1,2,17,18,"),                // "B"    
Fingering("2,17,18,"),                  // "C"    
Fingering("17,18,"),                    // "C#/Db"
Fingering("1,4,5,17,18,"),              // "D"    
Fingering("1,2,4,5,10,14,15,16,17,18,"),// "D#/Eb"
Fingering("1,2,4,14,15,17,18,"),        // "E"    
Fingering("1,2,5,14,17,18,"),           // "F"    
Fingering("1,2,5,16,17,18,"),           // "F#/Gb"
Fingering("2,4,5,17,18,"),              // "G"    
Fingering("4,5,10,17,18,"),             // "G#/Ab"
Fingering("1,4,14,17,18,")              // "A"    
};                                                                                      

// Trumpet ------------------------------
//  4
//   5   1
//   6   2
//   7   3
//   8
InstrumentInfo _trumpetInfo; 
#define TRUMPET_BUTTONS 8
int _trumpetButtonMap[TRUMPET_BUTTONS] = { 16, 18, 20, 2, 4, 6, 8, 10 }; // Buttons
int _trumpetKeyMap[TRUMPET_BUTTONS] =    {  1,  2,  3, 4, 5, 6, 7,  8 }; // Trumpet keys
uint32_t _trumpetKeyMask = 0; // Bitmask for the 'unwanted' keys
#define TRUMPET_FINGERINGS 31
Fingering _trumpetFingerings[TRUMPET_FINGERINGS] = {
Fingering("1,2,3,4,"),  // E in concert scale, F#/Gb in Bb scale  
Fingering("1,3,4,"),    // F
Fingering("2,3,4,"),    // F#
Fingering("1,2,4,"),    // G
Fingering("1,4,"),      // G#
Fingering("2,4,"),      // A
Fingering("4,"),        // A#
Fingering("1,2,3,5,"),  // B
Fingering("1,3,5,"),    // Middle C
Fingering("2,3,5,"),    // C#
Fingering("1,2,5,"),    // D
Fingering("1,5,"),      // D#
Fingering("2,5,"),      // E
Fingering("5,"),        // F
Fingering("2,3,6,"),    // F#
Fingering("1,2,6,"),    // G
Fingering("1,6,"),      // G#
Fingering("2,6,"),      // A
Fingering("6,"),        // A#
Fingering("1,2,7,"),    // B
Fingering("1,7,"),      // C
Fingering("2,7,"),      // C#
Fingering("7,"),        // D
Fingering("1,8,"),      // D#
Fingering("2,8,"),      // E
Fingering("8,"),        // F
Fingering("2,3,"),      // F#
Fingering("1,2,"),      // G
Fingering("1,"),        // G#
Fingering("2,"),        // A
Fingering(",")          // A#
};                                                                                      

// Drums ------------------------------
InstrumentInfo _drumsInfo;
#define DRUMS_BUTTONS 24
int _drumsButtonMap[DRUMS_BUTTONS] =   {  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 }; // Buttons
uint8_t _drumsNoteMap[DRUMS_BUTTONS] = { 
0, // 1
AudioBoard::DrumNotes::hat_open, // 2
0, // 3
AudioBoard::DrumNotes::snare, // 4
0, // 5
AudioBoard::DrumNotes::crash, // 6
0, // 7
AudioBoard::DrumNotes::chcrash_2, // 8
0, // 9
AudioBoard::DrumNotes::crash, // 10
0, // 11
0, // 12
0, // 13
AudioBoard::DrumNotes::bass, // 14
0, // 15
AudioBoard::DrumNotes::snare_high, // 16
0, // 17
AudioBoard::DrumNotes::tomHigh, // 18
0, // 19
AudioBoard::DrumNotes::tom_mid, // 20
0, // 21
AudioBoard::DrumNotes::tomLow, // 22
0, // 23
0  // 24
}; // MIDI notes
uint32_t _drumsKeyMask = 0; // Bitmask for the 'unwanted' keys

// Steel drum ------------------------------
InstrumentInfo _steelDrumInfo;
#define STEEL_DRUM_BUTTONS 10
int _steelDrumButtonMap[STEEL_DRUM_BUTTONS] =   {  2,  4,  6,  8, 10, 14, 16, 18, 20, 22 }; // Buttons
uint8_t _steelDrumNoteMap[STEEL_DRUM_BUTTONS] = { 60, 62, 64, 65, 67, 69, 71, 72, 74, 76 }; // MIDI notes
uint32_t _steelDrumKeyMask = 0; // Bitmask for the 'unwanted' keys

    // Variables --------------------------------------------------------------------------------------         
    uint32_t _lastTouched = 0;
    const char* _noteNames[12] = { "C", "C#/Db", "D" , "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    int _prevNoteNumber = NO_NOTE; // Current note's number
    char _string[64];
    InstrumentInfo* _currentInstrument;
    Fingers _fingers;
    Staff _staff;
    static const int _cNumber; // MIDI note number for note 'middle C'

    Keys();
    bool init();
    void SetInstrument();
    void ShowIntrument(const char* name);
    void Play(uint32_t touched);
    void PlayDrums(uint32_t touched);
    void PlaySaxophone(uint32_t touched);
    bool MapButtons(InstrumentInfo* info);
    bool CheckButtons(InstrumentInfo* info);
    void ShowNote(int noteNumber, bool show);
    void ShowTrumpet(int x, int y, uint32_t keys, bool error);
};
*/
#endif // #ifndef Keys_h
