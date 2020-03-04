#include "Misc.h"
#include "Synth.h"

InstrumentInfo::~InstrumentInfo() {
    delete _synth;
    delete _fingerings;
}

//-----------------------------------------------------------------------------
// Reverses 2 bytes
void reverse2Bytes(char* data) {
    char byte = 0; // Temporary byte

    byte = data[0];
    data[0] = data[1];
    data[1] = byte;
}

//-----------------------------------------------------------------------------
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
bool getInstrument(INSTRUMENT instrument, AudioBoard* audio, InstrumentInfo& info) {
    info._instrument = instrument;

    switch(instrument) {
        case INSTRUMENT::ACOUSTIC_GRAND_PIANO:
        case INSTRUMENT::BRIGHT_ACOUSTIC_PIANO:
            info._sample = &epiano;
            //info._sample = &piano;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            Serial.printf("case INSTRUMENT::BRIGHT_ACOUSTIC_PIANO\n");
            break;
        case INSTRUMENT::ELECTRIC_GRAND_PIANO:
        case INSTRUMENT::HONKY_TONK_PIANO:
        case INSTRUMENT::ELECTRIC_PIANO_1:
        case INSTRUMENT::ELECTRIC_PIANO_2:
            info._sample = &epiano;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::HARPSICHORD:
        case INSTRUMENT::CLAVI:
        case INSTRUMENT::CELESTA:
        case INSTRUMENT::GLOCKENSPIEL:
        case INSTRUMENT::MUSIC_BOX:
        case INSTRUMENT::VIBRAPHONE:
        case INSTRUMENT::MARIMBA:
        case INSTRUMENT::XYLOPHONE:
        case INSTRUMENT::TUBULAR_BELLS:
        case INSTRUMENT::DULCIMER:
            info._sample = &epiano;
            //info._sample = &vibraphone;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::DRAWBAR_ORGAN:
        case INSTRUMENT::PERCUSSIVE_ORGAN:
        case INSTRUMENT::ROCK_ORGAN:
        case INSTRUMENT::CHURCH_ORGAN:
        case INSTRUMENT::REED_ORGAN:
        case INSTRUMENT::ACCORDION:
        case INSTRUMENT::HARMONICA:
            info._sample = &epiano;
            //info._sample = &harmonica;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::TANGO_ACCORDION:
        case INSTRUMENT::ACOUSTIC_GUITAR_NYLON:
            info._sample = &steelstrgtr;
            //info._sample = &nylonstrgtr;
            info._voices = 6;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::ACOUSTIC_GUITAR_STEEL:
        case INSTRUMENT::ELECTRIC_GUITAR_JAZZ:
        case INSTRUMENT::ELECTRIC_GUITAR_CLEAN:
            info._sample = &steelstrgtr;
            info._voices = 6;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::ELECTRIC_GUITAR_MUTED:
            info._sample = &steelstrgtr;
            //info._sample = &mutedgtr;
            info._voices = 6;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::OVERDRIVEN_GUITAR:
            info._sample = &distortiongt;
            //info._sample = &overdrivegt;
            info._voices = 6;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 0;
            break;
        case INSTRUMENT::DISTORTION_GUITAR:
            info._sample = &distortiongt;
            info._voices = 6;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 0;
            break;
        case INSTRUMENT::GUITAR_HARMONICS:
        case INSTRUMENT::ACOUSTIC_BASS:
        case INSTRUMENT::ELECTRIC_BASS_FINGER:
        case INSTRUMENT::ELECTRIC_BASS_PICK:
        case INSTRUMENT::FRETLESS_BASS:
        case INSTRUMENT::SLAP_BASS_1:
        case INSTRUMENT::SLAP_BASS_2:
        case INSTRUMENT::SYNTH_BASS_1:
        case INSTRUMENT::SYNTH_BASS_2:
            info._sample = &nylonstrgtr;
            info._voices = 4;
            info._mixer = &audio->_mixer2;
            info._mixerChannel = 0;
            break;
        case INSTRUMENT::VIOLIN:
        case INSTRUMENT::VIOLA:
        case INSTRUMENT::CELLO:
        case INSTRUMENT::CONTRABASS:
        case INSTRUMENT::TREMOLO_STRINGS:
        case INSTRUMENT::PIZZICATO_STRINGS:
        case INSTRUMENT::ORCHESTRAL_HARP:
            info._sample = &steelstrgtr;
            //info._sample = &harp;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::TIMPANI:
            info._sample = &steelstrgtr;
            //info._sample = &timpani;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::STRING_ENSEMBLE_1:
        case INSTRUMENT::STRING_ENSEMBLE_2:
        case INSTRUMENT::SYNTHSTRINGS_1:
        case INSTRUMENT::SYNTHSTRINGS_2:
            info._sample = &strings;
            info._voices = 10;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 1;
            break;
        case INSTRUMENT::CHOIR_AAHS:
        case INSTRUMENT::VOICE_OOHS:
        case INSTRUMENT::SYNTH_VOICE:
        case INSTRUMENT::ORCHESTRA_HIT:
        case INSTRUMENT::TRUMPET:
            info._sample = &trumpet;
            info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::TROMBONE:
            info._sample = &trumpet;
            //info._sample = &trombone;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::TUBA:
            info._sample = &trumpet;
            //info._sample = &tuba;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::MUTED_TRUMPET:
        case INSTRUMENT::FRENCH_HORN:
            info._sample = &trumpet;
            //info._sample = &frenchhorn;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::BRASS_SECTION:
        case INSTRUMENT::SYNTHBRASS_1:
        case INSTRUMENT::SYNTHBRASS_2:
        case INSTRUMENT::SOPRANO_SAX:
        case INSTRUMENT::ALTO_SAX:
        case INSTRUMENT::TENOR_SAX:
        case INSTRUMENT::BARITONE_SAX:
        case INSTRUMENT::OBOE:
            info._sample = &trumpet;
            //info._sample = &oboe;
            info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::ENGLISH_HORN:
        case INSTRUMENT::BASSOON:
            info._sample = &trumpet;
            //info._sample = &bassoon;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::CLARINET:
            info._sample = &trumpet;
            //info._sample = &clarinet;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::PICCOLO:
        case INSTRUMENT::FLUTE:
            info._sample = &flute;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::RECORDER:
            info._sample = &flute;
            //info._sample = &recorder;
            info._voices = 1;
            info._mixer = &audio->_mixer3;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::PAN_FLUTE:
        case INSTRUMENT::BLOWN_BOTTLE:
        case INSTRUMENT::SHAKUHACHI:
        case INSTRUMENT::WHISTLE:
        case INSTRUMENT::OCARINA:
        case INSTRUMENT::LEAD_1_SQUARE:
        case INSTRUMENT::LEAD_2_SAWTOOTH:
        case INSTRUMENT::LEAD_3_CALLIOPE:
        case INSTRUMENT::LEAD_4_CHIFF:
        case INSTRUMENT::LEAD_5_CHARANG:
        case INSTRUMENT::LEAD_6_VOICE:
        case INSTRUMENT::LEAD_7_FIFTHS:
        case INSTRUMENT::LEAD_8_BASS_LEAD:
        case INSTRUMENT::PAD_1_NEW_AGE:
        case INSTRUMENT::PAD_2_WARM:
        case INSTRUMENT::PAD_3_POLYSYNTH:
        case INSTRUMENT::PAD_4_CHOIR:
        case INSTRUMENT::PAD_5_BOWED:
        case INSTRUMENT::PAD_6_METALLIC:
        case INSTRUMENT::PAD_7_HALO:
        case INSTRUMENT::PAD_8_SWEEP:
        case INSTRUMENT::FX_1_RAIN:
        case INSTRUMENT::FX_2_SOUNDTRACK:
        case INSTRUMENT::FX_3_CRYSTAL:
        case INSTRUMENT::FX_4_ATMOSPHERE:
        case INSTRUMENT::FX_5_BRIGHTNESS:
        case INSTRUMENT::FX_6_GOBLINS:
        case INSTRUMENT::FX_7_ECHOES:
        case INSTRUMENT::FX_8_SCIFI:
        case INSTRUMENT::SITAR:
        case INSTRUMENT::BANJO:
        case INSTRUMENT::SHAMISEN:
        case INSTRUMENT::KOTO:
        case INSTRUMENT::KALIMBA:
        case INSTRUMENT::BAG_PIPE:
        case INSTRUMENT::FIDDLE:
        case INSTRUMENT::SHANAI:
        case INSTRUMENT::TINKLE_BELL:
        case INSTRUMENT::AGOGO:
        case INSTRUMENT::STEEL_DRUMS:
            info._sample = &trumpet;
            //info._sample = &vibraphone;
            info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::WOODBLOCK:
        case INSTRUMENT::TAIKO_DRUM:
        case INSTRUMENT::MELODIC_TOM:
        case INSTRUMENT::SYNTH_DRUM:
        case INSTRUMENT::REVERSE_CYMBAL:
        case INSTRUMENT::GUITAR_FRET_NOISE:
            info._sample = &trumpet;
            //info._sample = &gtfretnoise;
            info._voices = 1;
            info._mixer = &audio->_mixer4;
            info._mixerChannel = 2;
            break;
        case INSTRUMENT::BREATH_NOISE:
        case INSTRUMENT::SEASHORE:
        case INSTRUMENT::BIRD_TWEET:
        case INSTRUMENT::TELEPHONE_RING:
        case INSTRUMENT::HELICOPTER:
        case INSTRUMENT::APPLAUSE:
        case INSTRUMENT::GUNSHOT:
        default:
            return false;
    }

    return true;
}
