#ifndef Midi_h
#define Midi_h

class Midi {
public:
    struct Instrument {
        Instrument(uint8_t note, const char* name) : _note(note), _name(name) {}
        uint8_t _note;
        const char* _name;
    };

/*
Adafruit product page: https://www.adafruit.com/product/1381
Library: https://learn.adafruit.com/adafruit-vs1053-mp3-aac-ogg-midi-wav-play-and-record-codec-tutorial/software

Signal	Function	    Teensy
------------------------------
SS	    Select Device	10
SCK	    Clock	        13
MOSI	Data Output	    11
MISO	Data Input	    12 
CS                       5
*/

// Connections:
// GPIO-0 -> GND
// GPIO-1 -> 3.3v
// VCC -> 3.3v
#define VS1053_RESET 6 // * This is the pin that connects to the RESET pin on VS1053
#define VS1053_RX 2 // * This is the pin that connects to the RX pin on VS1053

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_PAN 0x0A
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

// MIDI instruments
#define MIDI_SOPRANO_SAXOPHONE 65
#define MIDI_ALTO_SAXOPHONE 66
#define MIDI_TENOR_SAXOPHONE 67
#define MIDI_BARITONE_SAXOPHONE 68
#define MIDI_FLUTE 74
#define MIDI_TRUMPET 57
#define MIDI_DRUMS 255
#define MIDI_STEEL_DRUM 14

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
// VS1053b Melodic Instruments (GM1)
#define MELODIC_INSTRUMENTS 128
Instrument _melodic[MELODIC_INSTRUMENTS] = {
    Instrument(1, "Acoustic Grand Piano"),     
    Instrument(2, "Bright Acoustic Piano"),    
    Instrument(3, "Electric Grand Piano"),     
    Instrument(4, "Honky-tonk Piano"),         
    Instrument(5, "Electric Piano 1"),         
    Instrument(6, "Electric Piano 2"),         
    Instrument(7, "Harpsichord"),              
    Instrument(8, "Clavi"),                    
    Instrument(9, "Celesta"),                  
    Instrument(10, "Glockenspiel"),             
    Instrument(11, "Music Box"),                
    Instrument(12, "Vibraphone"),               
    Instrument(13, "Marimba"),                  
    Instrument(14, "Xylophone"),                
    Instrument(15, "Tubular Bells"),            
    Instrument(16, "Dulcimer"),                 
    Instrument(17, "Drawbar Organ"),            
    Instrument(18, "Percussive Organ"),         
    Instrument(19, "Rock Organ"),               
    Instrument(20, "Church Organ"),             
    Instrument(21, "Reed Organ"),               
    Instrument(22, "Accordion"),                
    Instrument(23, "Harmonica"),                
    Instrument(24, "Tango Accordion"),          
    Instrument(25, "Acoustic Guitar (nylon)"),  
    Instrument(26, "Acoustic Guitar (steel)"),  
    Instrument(27, "Electric Guitar (jazz)"),   
    Instrument(28, "Electric Guitar (clean)"),  
    Instrument(29, "Electric Guitar (muted)"),  
    Instrument(30, "Overdriven Guitar"),        
    Instrument(31, "Distortion Guitar"),        
    Instrument(32, "Guitar Harmonics"),         
    Instrument(33, "Acoustic Bass"),            
    Instrument(34, "Electric Bass (finger)"),   
    Instrument(35, "Electric Bass (pick)"),     
    Instrument(36, "Fretless Bass"),            
    Instrument(37, "Slap Bass 1"),              
    Instrument(38, "Slap Bass 2"),              
    Instrument(39, "Synth Bass 1"),             
    Instrument(40, "Synth Bass 2"),             
    Instrument(41, "Violin"),                   
    Instrument(42, "Viola"),                    
    Instrument(43, "Cello"),                    
    Instrument(44, "Contrabass"),               
    Instrument(45, "Tremolo Strings"),          
    Instrument(46, "Pizzicato Strings"),        
    Instrument(47, "Orchestral Harp"),          
    Instrument(48, "Timpani"),                  
    Instrument(49, "String Ensembles 1"),       
    Instrument(50, "String Ensembles 2"),       
    Instrument(51, "Synth Strings 1"),          
    Instrument(52, "Synth Strings 2"),          
    Instrument(53, "Choir Aahs"),               
    Instrument(54, "Voice Oohs"),               
    Instrument(55, "Synth Voice"),              
    Instrument(56, "Orchestra Hit"),            
    Instrument(57, "Trumpet"),                  
    Instrument(58, "Trombone"),                 
    Instrument(59, "Tuba"),                     
    Instrument(60, "Muted Trumpet"),            
    Instrument(61, "French Horn"),              
    Instrument(62, "Brass Section"),            
    Instrument(63, "Synth Brass 1"),            
    Instrument(64, "Synth Brass 2"),            
    Instrument(65, "Soprano Sax"),              
    Instrument(66, "Alto Sax"),                 
    Instrument(67, "Tenor Sax"),                
    Instrument(68, "Baritone Sax"),             
    Instrument(69, "Oboe"),                     
    Instrument(70, "English Horn"),             
    Instrument(71, "Bassoon"),                  
    Instrument(72, "Clarinet"),                 
    Instrument(73, "Piccolo"),                  
    Instrument(74, "Flute"),                    
    Instrument(75, "Recorder"),                 
    Instrument(76, "Pan Flute"),                
    Instrument(77, "Blown Bottle"),             
    Instrument(78, "Shakuhachi"),               
    Instrument(79, "Whistle"),                  
    Instrument(80, "Ocarina"),                  
    Instrument(81, "Square Lead (Lead 1)"),     
    Instrument(82, "Saw Lead (Lead)"),          
    Instrument(83, "Calliope Lead (Lead 3)"),   
    Instrument(84, "Chiff Lead (Lead 4)"),      
    Instrument(85, "Charang Lead (Lead 5)"),    
    Instrument(86, "Voice Lead (Lead 6)"),      
    Instrument(87, "Fifths Lead (Lead 7)"),     
    Instrument(88, "Bass + Lead (Lead 8)"),     
    Instrument(89, "New Age (Pad 1)"),          
    Instrument(90, "Warm Pad (Pad 2)"),         
    Instrument(91, "Polysynth (Pad 3)"),        
    Instrument(92, "Choir (Pad 4)"),            
    Instrument(93, "Bowed (Pad 5)"),            
    Instrument(94, "Metallic (Pad 6)"),         
    Instrument(95, "Halo (Pad 7)"),             
    Instrument(96, "Sweep (Pad 8)"),            
    Instrument(97, "Rain (FX 1)"),
    Instrument(98, "Sound Track (FX 2)"),
    Instrument(99, "Crystal (FX 3)"),
    Instrument(100, "Atmosphere (FX 4)"),
    Instrument(101, "Brightness (FX 5)"),
    Instrument(102, "Goblins (FX 6)"),
    Instrument(103, "Echoes (FX 7)"),
    Instrument(104, "Sci-fi (FX 8)"),
    Instrument(105, "Sitar"),
    Instrument(106, "Banjo"),
    Instrument(107, "Shamisen"),
    Instrument(108, "Koto"),
    Instrument(109, "Kalimba"),
    Instrument(110, "Bag Pipe"),
    Instrument(111, "Fiddle"),
    Instrument(112, "Shanai"),
    Instrument(113, "Tinkle Bell"),
    Instrument(114, "Agogo"),
    Instrument(115, "Pitched Percussion"),
    Instrument(116, "Woodblock"),
    Instrument(117, "Taiko Drum"),
    Instrument(118, "Melodic Tom"),
    Instrument(119, "Synth Drum"),
    Instrument(120, "Reverse Cymbal"),
    Instrument(121, "Guitar Fret Noise"),
    Instrument(122, "Breath Noise"),
    Instrument(123, "Seashore"),
    Instrument(124, "Bird Tweet"),
    Instrument(125, "Telephone Ring"),
    Instrument(126, "Helicopter"),
    Instrument(127, "Applause"),
    Instrument(128, "Gunshot")
};

// VS1053b Percussion Instruments (GM1+GM2)
#define PERCUSSION_INSTRUMENTS 61
Instrument _percussion[PERCUSSION_INSTRUMENTS] = {
    Instrument(27, "High Q"),                   
    Instrument(28, "Slap"),                     
    Instrument(29, "Scratch Push [EXC 7]"),     
    Instrument(30, "Scratch Pull [EXC 7]"),     
    Instrument(31, "Sticks"),                   
    Instrument(32, "Square Click"),             
    Instrument(33, "Metronome Click"),          
    Instrument(34, "Metronome Bell"),           
    Instrument(35, "Acoustic Bass Drum"),       
    Instrument(36, "Bass Drum 1"),              
    Instrument(37, "Side Stick"),               
    Instrument(38, "Acoustic Snare"),           
    Instrument(39, "Hand Clap"),                
    Instrument(40, "Electric Snare"),           
    Instrument(41, "Low Floor Tom"),            
    Instrument(42, "Closed Hi-hat [EXC 1]"),    
    Instrument(43, "High Floor Tom"),           
    Instrument(44, "Pedal Hi-hat [EXC 1]"),     
    Instrument(45, "Low Tom"),                  
    Instrument(46, "Open Hi-hat [EXC 1]"),      
    Instrument(47, "Low-Mid Tom"),              
    Instrument(48, "High Mid Tom"),             
    Instrument(49, "Crash Cymbal 1"),           
    Instrument(50, "High Tom"),                 
    Instrument(51, "Ride Cymbal 1"),            
    Instrument(52, "Chinese Cymbal"),           
    Instrument(53, "Ride Bell"),                
    Instrument(54, "Tambourine"),               
    Instrument(55, "Splash Cymbal"),            
    Instrument(56, "Cowbell"),                  
    Instrument(57, "Crash Cymbal 2"),           
    Instrument(58, "Vibra-slap"),               
    Instrument(59, "Ride Cymbal 2"),            
    Instrument(60, "High Bongo"),               
    Instrument(61, "Low Bongo"),                
    Instrument(62, "Mute Hi Conga"),            
    Instrument(63, "Open Hi Conga"),            
    Instrument(64, "Low Conga"),                
    Instrument(65, "High Timbale"),             
    Instrument(66, "Low Timbale"),              
    Instrument(67, "High Agogo"),               
    Instrument(68, "Low Agogo"),                
    Instrument(69, "Cabasa"),                   
    Instrument(70, "Maracas"),                  
    Instrument(71, "Short Whistle [EXC 2]"),    
    Instrument(72, "Long Whistle [EXC 2]"),
    Instrument(73, "Short Guiro [EXC 3]"),
    Instrument(74, "Long Guiro [EXC 3]"),
    Instrument(75, "Claves"),
    Instrument(76, "Hi Wood Block"),
    Instrument(77, "Low Wood Block"),
    Instrument(78, "Mute Cuica [EXC 4]"),
    Instrument(79, "Open Cuica [EXC 4]"),
    Instrument(80, "Mute Triangle [EXC 5]"),
    Instrument(81, "Open Triangle [EXC 5]"),
    Instrument(82, "Shaker"),
    Instrument(83, "Jingle bell"),
    Instrument(84, "Bell tree"),
    Instrument(85, "Castanets"),
    Instrument(86, "Mute Surdo [EXC 6]"),
    Instrument(87, "Open Surdo [EXC 6]")
};

public:
    Midi();
    void Init();
    void SetInstrument(uint8_t chan, uint8_t inst);
    void SetChannelVolume(uint8_t chan, uint8_t vol);
    void SetChannelPan(uint8_t chan, uint8_t pan);
    void SetChannelBank(uint8_t chan, uint8_t bank);
    void NoteOn(uint8_t chan, uint8_t n, uint8_t vel);
    void NoteOff(uint8_t chan, uint8_t n, uint8_t vel = 0);
    void Test(uint8_t volume);

    //SoftwareSerial _midi; // TX only, do not use the 'rx' side
    static int _melodyChannel;
    static int _bassChannel;
    static int _drumChannel;
};

#endif // #ifndef Midi_h
