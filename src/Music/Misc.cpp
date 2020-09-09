#include "../Devices/Devices.h"
#include "../Sound/Sound.h"
#include "Misc.h"
#include "Synth.h"

const char* _instrumentNames[] = {
    "Acoustic Grand Piano", "Bright Acoustic Piano",
    "Electric Grand Piano", "Honky-tonk Piano", "Electric Piano 1", "Electric Piano 2", "Harpsichord", "Clavi",
    "Celesta", "Glockenspiel", "Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer",
    "Drawbar Organ", "Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica",
    "Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)", "Electric Guitar (jazz)",
    "Electric Guitar (clean)", "Electric Guitar (muted)", "Overdriven Guitar", "Distortion Guitar",
    "Guitar harmonics", "Acoustic Bass", "Electric Bass (finger)", "Electric Bass (pick)", "Fretless Bass",
    "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello", "Contrabass",
    "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp", "Timpani", "String Ensemble 1",
    "String Ensemble 2", "SynthStrings 1", "SynthStrings 2", "Choir Aahs", "Voice Oohs", "Synth Voice",
    "Orchestra Hit", "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section",
    "SynthBrass 1", "SynthBrass 2", "Soprano Sax", "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe",
    "English Horn", "Bassoon", "Clarinet", "Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle",
    "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)",
    "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)", "Lead 8 (bass + lead)",
    "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)",
    "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)",
    "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar",
    "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle", "Shanai", "Tinkle Bell", "Agogo",
    "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal",
    "Guitar Fret Noise", "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter", "Applause",
    "Gunshot", "Percussion"
};

const char* _drumNames[] = {
    "Acoustic Bass Drum",
    "Bass Drum 1",
    "Side Stick",
    "Acoustic Snare",
    "Hand Clap",
    "Electric Snare",
    "Low Floor Tom",
    "Closed Hi-Hat",
    "High Floor Tom",
    "Pedal Hi-Hat",
    "Low Tom",
    "Open Hi-Hat",
    "Low-Mid Tom",
    "Hi-Mid Tom",
    "Crash Cymbal 1",
    "High Tom",
    "Ride Cymbal 1",
    "Chinese Cymbal",
    "Ride Bell",
    "Tambourine",
    "Splash Cymbal",
    "Cowbell",
    "Crash Cymbal 2",
    "Vibraslap",
    "Ride Cymbal 2",
    "Hi Bongo",
    "Low Bongo",
    "Mute Hi Conga",
    "Open Hi Conga",
    "Low Conga",
    "High Timbale",
    "Low Timbale",
    "High Agogo",
    "Low Agogo",
    "Cabasa",
    "Maracas",
    "Short Whistle",
    "Long Whistle",
    "Short Guiro",
    "Long Guiro",
    "Claves",
    "Hi Wood Block",
    "Low Wood Block",
    "Mute Cuica",
    "Open Cuica",
    "Mute Triangle",
    "Open Triangle",
    "Shaker"
};


//=================================================================================================
String randomString() {
    String ret{};
    char buffer[8];

    sprintf(buffer, "%ld", random(0, 100));
    ret = buffer;
    return ret;
}

//=================================================================================================
float scale(float value, float fromMin, float fromMax, float toMin, float toMax) {
    float ret;
    float temp;

    if(fromMin == toMin && fromMax == toMax)
        return value;

    temp = (value - fromMin) / (fromMax - fromMin);
    //Serial.printf("temp=%0.2f\n", temp);
    ret = (toMax - toMin) * temp + toMin;

    return ret;
}

//=================================================================================================
uint8_t randomInt() {
    return (uint8_t)random(0, 100);
}

//=================================================================================================
float randomFloat() {
    return (float)random(0, 100);
}

//=================================================================================================
InstrumentInfo::~InstrumentInfo() {
    delete _synth;
    delete _fingerings;
}

//=================================================================================================
// Reverses 2 bytes
void reverse2Bytes(char* data) {
    char byte = 0; // Temporary byte

    byte = data[0];
    data[0] = data[1];
    data[1] = byte;
}

//=================================================================================================
// Reverses 4 bytes
void reverse4Bytes(char* data) {
    char byte = 0; // Temporary byte

    // Outer bytes
    byte = data[0];
    data[0] = data[3];
    data[3] = byte;

    // Inner bytes
    byte = data[1];
    data[1] = data[2];
    data[2] = byte;
}

//-----------------------------------------------------------------------------
// Adds an array of integers to integer list
// Returns: error number
int addIntList(IntList& list, int* values, int size) {
    int i;

    for(i = 0; i < size; i++) {
        list.push_back(values[i]);
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Random double
double randomDouble(double from, double to) {
    int rand;
    double fraction = 1000.0;

    rand = random((int)(from * fraction), (int)(to * fraction));
    return (double)rand / fraction;
}

//-----------------------------------------------------------------------------
// 33-40   Bass
bool isBass(Instrument instrument) {
    if((instrument >= ACOUSTIC_BASS && instrument <= SYNTH_BASS_2) ||
       instrument == CONTRABASS) // Contrabass
        return true;
    else
        return false;
}

// Shows a number as bits
/*void ShowBits(uint32_t value, String& s) {
    char buffer[8];

    if(value == 0) {
        s = ",";
        return;
    }

    for(int i = 0; i < 32; ++i) {
        if(bitRead(value, i)) {
            sprintf(buffer, "%d,", i + 1);
            s = s + buffer;
        }
    }
}*/

// Writes to the top of the screen
/*void LogScreen(const char* line) {
    g_screen._screen.setTextColor(ILI9341_WHITE);
    g_screen._screen.setTextSize(2);
    g_screen._screen.setCursor(0, 0);
    g_screen._screen.fillRect(0, 0, 300, 40, ILI9341_BLACK);
    g_screen._screen.print(line);
}*/

//=================================================================================================
// MIDI instruments
//  1-8    Piano                     65-72  Reed
//  9-16   Chromatic Percussion      73-80  Pipe
// 17-24   Organ                     81-88  Synth Lead
// 25-32   Guitar                    89-96  Synth Pad
// 33-40   Bass                      97-104 Synth Effects
// 41-48   Strings                  105-112 Ethnic
// 49-56   Ensemble                 113-120 Percussive
// 57-64   Brass                    121-128 Sound Effects
bool getInstrument(Instrument instrument, AudioBoard* audio, InstrumentInfo& info) {
    info._instrument = instrument;
    info._voices = 10;

    switch(instrument) {
        case Instrument::ACOUSTIC_GRAND_PIANO:
        case Instrument::BRIGHT_ACOUSTIC_PIANO:
        case Instrument::ELECTRIC_GRAND_PIANO:
        case Instrument::HONKY_TONK_PIANO:
        case Instrument::ELECTRIC_PIANO_1:
        case Instrument::ELECTRIC_PIANO_2:
        case Instrument::HARPSICHORD:
        case Instrument::CLAVI:
        case Instrument::CELESTA:
        case Instrument::GLOCKENSPIEL:
        case Instrument::MUSIC_BOX:
        case Instrument::VIBRAPHONE:
        case Instrument::MARIMBA:
        case Instrument::XYLOPHONE:
        case Instrument::TUBULAR_BELLS:
        case Instrument::DULCIMER:
        case Instrument::DRAWBAR_ORGAN:
        case Instrument::PERCUSSIVE_ORGAN:
        case Instrument::ROCK_ORGAN:
        case Instrument::CHURCH_ORGAN:
        case Instrument::REED_ORGAN:
        case Instrument::ACCORDION:
        case Instrument::HARMONICA:
        case Instrument::TANGO_ACCORDION:
            /*info._name = "Piano";
            info._sample = &epiano;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;*/
        case Instrument::ACOUSTIC_GUITAR_NYLON:
        case Instrument::ACOUSTIC_GUITAR_STEEL:
        case Instrument::ELECTRIC_GUITAR_JAZZ:
        case Instrument::ELECTRIC_GUITAR_CLEAN:
            info._name = "Guitar (steel)";
            info._sample = &steelstrgtr;
            info._voices = 6;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 1;
            break;
        case Instrument::ELECTRIC_GUITAR_MUTED:
        case Instrument::OVERDRIVEN_GUITAR:
        case Instrument::DISTORTION_GUITAR:
            /*info._name = "Guitar (dist.)";
            info._sample = &distortiongt;
            info._voices = 6;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 0;
            break;*/
        case Instrument::GUITAR_HARMONICS:
        case Instrument::ACOUSTIC_BASS:
        case Instrument::ELECTRIC_BASS_FINGER:
        case Instrument::ELECTRIC_BASS_PICK:
        case Instrument::FRETLESS_BASS:
        case Instrument::SLAP_BASS_1:
        case Instrument::SLAP_BASS_2:
        case Instrument::SYNTH_BASS_1:
        case Instrument::SYNTH_BASS_2:
            info._name = "Bass";
            info._sample = &steelstrgtr;
            info._voices = 4;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 0;
            break;
        case Instrument::VIOLIN:
        case Instrument::VIOLA:
        case Instrument::CELLO:
        case Instrument::CONTRABASS:
        case Instrument::TREMOLO_STRINGS:
        case Instrument::PIZZICATO_STRINGS:
        case Instrument::ORCHESTRAL_HARP:
        case Instrument::TIMPANI:
        case Instrument::STRING_ENSEMBLE_1:
        case Instrument::STRING_ENSEMBLE_2:
        case Instrument::SYNTHSTRINGS_1:
        case Instrument::SYNTHSTRINGS_2:
        case Instrument::CHOIR_AAHS:
        case Instrument::VOICE_OOHS:
        case Instrument::SYNTH_VOICE:
        case Instrument::ORCHESTRA_HIT:
        case Instrument::TRUMPET:
        case Instrument::TROMBONE:
        case Instrument::TUBA:
        case Instrument::MUTED_TRUMPET:
        case Instrument::BRASS_SECTION:
        case Instrument::SYNTHBRASS_1:
        case Instrument::SYNTHBRASS_2:
        case Instrument::SOPRANO_SAX:
        case Instrument::ALTO_SAX:
        case Instrument::TENOR_SAX:
        case Instrument::BARITONE_SAX:
        case Instrument::OBOE:
        case Instrument::ENGLISH_HORN:
        case Instrument::BASSOON:
        case Instrument::CLARINET:
        case Instrument::PICCOLO:
        case Instrument::FLUTE:
            info._name = "Flute";
            info._sample = &flute;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::RECORDER:
        case Instrument::PAN_FLUTE:
        case Instrument::BLOWN_BOTTLE:
        case Instrument::SHAKUHACHI:
        case Instrument::WHISTLE:
        case Instrument::OCARINA:
        case Instrument::LEAD_1_SQUARE:
        case Instrument::LEAD_2_SAWTOOTH:
        case Instrument::LEAD_3_CALLIOPE:
        case Instrument::LEAD_4_CHIFF:
        case Instrument::LEAD_5_CHARANG:
        case Instrument::LEAD_6_VOICE:
        case Instrument::LEAD_7_FIFTHS:
        case Instrument::LEAD_8_BASS_LEAD:
        case Instrument::PAD_1_NEW_AGE:
        case Instrument::PAD_2_WARM:
        case Instrument::PAD_3_POLYSYNTH:
        case Instrument::PAD_4_CHOIR:
        case Instrument::PAD_5_BOWED:
        case Instrument::PAD_6_METALLIC:
        case Instrument::PAD_7_HALO:
        case Instrument::PAD_8_SWEEP:
        case Instrument::FX_1_RAIN:
        case Instrument::FX_2_SOUNDTRACK:
        case Instrument::FX_3_CRYSTAL:
        case Instrument::FX_4_ATMOSPHERE:
        case Instrument::FX_5_BRIGHTNESS:
        case Instrument::FX_6_GOBLINS:
        case Instrument::FX_7_ECHOES:
        case Instrument::FX_8_SCIFI:
        case Instrument::SITAR:
        case Instrument::BANJO:
        case Instrument::SHAMISEN:
        case Instrument::KOTO:
        case Instrument::KALIMBA:
        case Instrument::BAG_PIPE:
        case Instrument::FIDDLE:
        case Instrument::SHANAI:
        case Instrument::TINKLE_BELL:
        case Instrument::AGOGO:
        case Instrument::STEEL_DRUMS:
        case Instrument::WOODBLOCK:
        case Instrument::TAIKO_DRUM:
        case Instrument::MELODIC_TOM:
        case Instrument::SYNTH_DRUM:
        case Instrument::REVERSE_CYMBAL:
        case Instrument::GUITAR_FRET_NOISE:
        case Instrument::BREATH_NOISE:
        case Instrument::SEASHORE:
        case Instrument::BIRD_TWEET:
        case Instrument::TELEPHONE_RING:
        case Instrument::HELICOPTER:
        case Instrument::APPLAUSE:
        case Instrument::GUNSHOT:
            info._name = "None";
        default:
            return false;
    }

    return true;
}
