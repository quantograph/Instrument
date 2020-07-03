#include "../Devices/Devices.h"
#include "../Sound/Sound.h"
#include "Misc.h"
#include "Synth.h"

//=================================================================================================
String randomString() {
    String ret{};
    char buffer[8];

    sprintf(buffer, "%d", random(0, 100));
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
            info._name = "Acc. piano";
            info._sample = &epiano;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case Instrument::ELECTRIC_GRAND_PIANO:
        case Instrument::HONKY_TONK_PIANO:
        case Instrument::ELECTRIC_PIANO_1:
        case Instrument::ELECTRIC_PIANO_2:
            info._name = "El. piano";
            info._sample = &epiano;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
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
            info._name = "Vibraphone";
            info._sample = &epiano;
            //info._sample = &vibraphone;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case Instrument::DRAWBAR_ORGAN:
        case Instrument::PERCUSSIVE_ORGAN:
        case Instrument::ROCK_ORGAN:
        case Instrument::CHURCH_ORGAN:
        case Instrument::REED_ORGAN:
        case Instrument::ACCORDION:
        case Instrument::HARMONICA:
        case Instrument::TANGO_ACCORDION:
            info._name = "Accordion";
            info._sample = &epiano;
            //info._sample = &harmonica;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case Instrument::ACOUSTIC_GUITAR_NYLON:
            info._name = "Guitar (nylon)";
            info._sample = &steelstrgtr;
            //info._sample = &nylonstrgtr;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case Instrument::ACOUSTIC_GUITAR_STEEL:
        case Instrument::ELECTRIC_GUITAR_JAZZ:
        case Instrument::ELECTRIC_GUITAR_CLEAN:
            info._name = "Guitar (steel)";
            info._sample = &steelstrgtr;
            //info._voices = 6;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 1;
            break;
        case Instrument::ELECTRIC_GUITAR_MUTED:
            info._name = "Guitar (muted)";
            info._sample = &steelstrgtr;
            //info._sample = &mutedgtr;
            //info._voices = 6;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 1;
            break;
        case Instrument::OVERDRIVEN_GUITAR:
            info._name = "Guitar (overdr.)";
            info._sample = &distortiongt;
            //info._sample = &overdrivegt;
            //info._voices = 6;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 0;
            break;
        case Instrument::DISTORTION_GUITAR:
            info._name = "Guitar (dist.)";
            info._sample = &distortiongt;
            //info._voices = 6;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 0;
            break;
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
            info._sample = &nylonstrgtr;
            //info._voices = 4;
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
            info._name = "Violin";
            info._sample = &steelstrgtr;
            //info._sample = &harp;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::TIMPANI:
            info._name = "Timpani";
            info._sample = &steelstrgtr;
            //info._sample = &timpani;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::STRING_ENSEMBLE_1:
        case Instrument::STRING_ENSEMBLE_2:
        case Instrument::SYNTHSTRINGS_1:
        case Instrument::SYNTHSTRINGS_2:
            info._name = "Strings";
            info._sample = &strings;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case Instrument::CHOIR_AAHS:
        case Instrument::VOICE_OOHS:
        case Instrument::SYNTH_VOICE:
        case Instrument::ORCHESTRA_HIT:
        case Instrument::TRUMPET:
            info._name = "Trumpet";
            info._sample = &trumpet;
            //info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case Instrument::TROMBONE:
            info._name = "Trombone";
            info._sample = &trumpet;
            //info._sample = &trombone;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::TUBA:
            info._name = "Tuba";
            info._sample = &trumpet;
            //info._sample = &tuba;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::MUTED_TRUMPET:
        case Instrument::FRENCH_HORN:
            info._name = "French horn";
            info._sample = &trumpet;
            //info._sample = &frenchhorn;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::BRASS_SECTION:
        case Instrument::SYNTHBRASS_1:
        case Instrument::SYNTHBRASS_2:
        case Instrument::SOPRANO_SAX:
        case Instrument::ALTO_SAX:
        case Instrument::TENOR_SAX:
        case Instrument::BARITONE_SAX:
        case Instrument::OBOE:
            info._name = "Oboe";
            info._sample = &trumpet;
            //info._sample = &oboe;
            //info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case Instrument::ENGLISH_HORN:
        case Instrument::BASSOON:
            info._name = "Bassoon";
            info._sample = &trumpet;
            //info._sample = &bassoon;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::CLARINET:
            info._name = "Clarinet";
            info._sample = &trumpet;
            //info._sample = &clarinet;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::PICCOLO:
        case Instrument::FLUTE:
            info._name = "Flute";
            info._sample = &flute;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case Instrument::RECORDER:
        case Instrument::PAN_FLUTE:
        case Instrument::BLOWN_BOTTLE:
            info._name = "Recorder";
            info._sample = &flute;
            //info._sample = &recorder;
            //info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
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
            info._name = "Steel drums";
            info._sample = &trumpet;
            //info._sample = &vibraphone;
            //info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case Instrument::WOODBLOCK:
        case Instrument::TAIKO_DRUM:
        case Instrument::MELODIC_TOM:
        case Instrument::SYNTH_DRUM:
        case Instrument::REVERSE_CYMBAL:
        case Instrument::GUITAR_FRET_NOISE:
            info._name = "Melodic tom";
            info._sample = &trumpet;
            //info._sample = &gtfretnoise;
            //info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
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
