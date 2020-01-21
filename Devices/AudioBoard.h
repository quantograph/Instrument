#ifndef AudioBoard_h
#define AudioBoard_h

#include <Arduino.h>
#include <Audio.h>

class AudioBoard
{
public:
    enum Instrument {
        none,
        leadGuitar,
        rhythmGuitar,
        synth,
        bassGuitar,
        drums
    };

    const uint8_t _drumMidiNotes[48] = { 27, 28, 28, 29, 30, 31, 33, 34, 35, 36, 37, 39, 42, 45, 47, 48, 50, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 65, 67, 68, 69, 71, 72, 73, 74, 76, 78, 80, 81, 82, 83, 83, 84, 85, 85, 86, 87, 127, };
    enum DrumNotes {
        filtersnap, // 27
        whitenoisewave, // 28
        verbclickwave, // 28
        scratch, // 29
        guitarfret, // 30
        stix, // 31
        sinetick, // 33
        verbclickwave_2, // 34
        coldglass12wave, // 35
        bass, // 36 >>>
        cross_stick, // 37 ===
        snare, // 39 >>>
        snare_high, // 42 >>>
        tom_mid, // 45 >>>
        hat_open, // 47 ===
        tomHigh, // 48 >>>
        crash, // 50 >>>
        china, // 53 >>>
        hat, // 54
        splash, // 55 >>>
        cowbell, // 56 ===
        crash_high, // 57 >>>
        vibraloop, // 58
        ride, // 60 >>>
        bongo_tone, // 61 ===
        quinto_slap, // 62 ===
        quinto_tone,//  63 ===
        low_tumba, // 65 ===
        timpani, // 67 ===
        agogo_lotone, // 68 ===
        cabasa, // 69 ===
        maracas, // 71
        sambawhistle, // 72
        guirodown, // 73
        guiro2, // 74
        bockclave, // 76
        woodblock, // 78
        quica_down, // 80
        triangle, // 81
        cabasa_2, // 82
        tamborine_2, // 83
        chcrash_2, // 83 >>>
        belltree, // 84
        ebongostone, // 85
        sinetick_2, // 85
        bongo_tone_2, // 86
        tomLow, // 87 >>>
        timpani_2 // 127
    };

    int _noteNumber = 0;
    uint8_t cnt = 0;
    uint8_t _playingNote = 0;
    bool _noteOn = false;
    bool _noteDetected = false;
    int _sequence = 0;
    uint32_t _noteStartTime = 0;
    uint32_t _noteStopTime = 0;
    uint32_t _noteDetectTime = 0;

	void Init();
    void Process();
    void PeakMeter();
    void Test(int test);
    void TestDrums(int pause);
    void NoteFrequency();
    void NoteOn(Instrument instrument, uint8_t n, uint8_t vel, uint8_t string = 0);
    void NoteOn(Instrument instrument, int n, int vel) { NoteOn(instrument, (uint8_t)n, (uint8_t)vel); }
    void NoteOff(Instrument instrument, uint8_t n = 0, uint8_t string = 0);
    void NoteOff(Instrument instrument, int n, int string) { NoteOff(instrument, (uint8_t)n, (uint8_t)string); }
    AudioSynthWavetable* GetInstrument(Instrument instrument, uint8_t note, uint8_t string);
    AudioSynthWavetable* GetDrum(uint8_t note);
    uint8_t GetNote(Instrument instrument, uint8_t note);
    void NoteDetected(float frequency);



    AudioOutputI2S  _audioOutput;
    AudioControlSGTL5000 _audioControl;

    AudioAnalyzePeak peak_L;
    AudioAnalyzePeak peak_R;

    AudioAnalyzeNoteFrequency notefreq;

    AudioMixer4             _mixer1;
    AudioMixer4             _mixer2;
    AudioMixer4             _mixer3;
    AudioMixer4             _mixer4;
    AudioMixer4             _outMixer1;
    AudioMixer4             _outMixer2;
    AudioMixer4             _rhythmGuitarMixer1;
    AudioMixer4             _rhythmGuitarMixer2;
    AudioMixer4             _drumMixer1;
    AudioMixer4             _drumMixer2;

    // Guitars
    AudioSynthWavetable     _leadGuitar;
    AudioSynthWavetable     _bassGuitar;
    AudioSynthWavetable     _rhythmGuitar1;
    AudioSynthWavetable     _rhythmGuitar2;
    AudioSynthWavetable     _rhythmGuitar3;
    AudioSynthWavetable     _rhythmGuitar4;
    AudioSynthWavetable     _rhythmGuitar5;
    AudioSynthWavetable     _rhythmGuitar6;
    // Drums
    AudioSynthWavetable     _drumHat;
    AudioSynthWavetable     _drumBass;
    AudioSynthWavetable     _drumSnare;
    AudioSynthWavetable     _drumTomHigh;
    AudioSynthWavetable     _drumTomLow;
    AudioSynthWavetable     _drumCrash;
    AudioSynthWavetable     _drumRide;
    AudioSynthWavetable     _drumBlock;

    /*AudioConnection         _patchCord21(_leadGuitar, 0, _mixer1, 0);
    AudioConnection         _patchCord22(_bassGuitar, 0, _mixer4, 0);
    AudioConnection         _patchCord23(_rhythmGuitar1, 0, _rhythmGuitarMixer1, 0);
    AudioConnection         _patchCord24(_rhythmGuitar2, 0, _rhythmGuitarMixer1, 1);
    AudioConnection         _patchCord25(_rhythmGuitar3, 0, _rhythmGuitarMixer1, 2);
    AudioConnection         _patchCord26(_rhythmGuitar4, 0, _rhythmGuitarMixer1, 3);
    AudioConnection         _patchCord27(_rhythmGuitar5, 0, _rhythmGuitarMixer2, 0);
    AudioConnection         _patchCord28(_rhythmGuitar6, 0, _rhythmGuitarMixer2, 1);
    AudioConnection         _patchCord29(_rhythmGuitarMixer1, 0, _mixer2, 0);
    AudioConnection         _patchCord30(_rhythmGuitarMixer2, 0, _mixer2, 1);*/
    // Mixer1-0 audioInput1
    // Mixer1-1 synth1
    // Mixer1-2 drumHat
    // Mixer1-3 drumCrash
    // --------
    // Mixer2-0 guitarBass
    // Mixer2-1 guitarRhythm
    // Mixer2-2 drumSnare
    // Mixer2-3 drumTomHigh
    // --------
    // Mixer3-0 guitarLead
    // Mixer3-1 drumBass
    // Mixer3-2 drumTomMid
    // Mixer3-3 drumRide
    // --------
    // Mixer4-0 audioInput2
    // Mixer4-1 synth2
    // Mixer4-2 drumTomLow
    // Mixer4-3 drumBlock
    // Drums
    AudioConnection*         _patchCord12{nullptr};
    AudioConnection*         _patchCord13{nullptr};
    AudioConnection*         _patchCord14{nullptr};
    AudioConnection*         _patchCord15{nullptr};
    AudioConnection*         _patchCord16{nullptr};
    AudioConnection*         _patchCord17{nullptr};
    AudioConnection*         _patchCord18{nullptr};
    AudioConnection*         _patchCord19{nullptr};
    // 2 output mixers
    AudioConnection*         _patchCord01{nullptr};
    AudioConnection*         _patchCord02{nullptr};
    AudioConnection*         _patchCord03{nullptr};
    AudioConnection*         _patchCord04{nullptr};
    AudioConnection*         _patchCord05{nullptr};
    AudioConnection*         _patchCord06{nullptr};
    AudioConnection*         _patchCord07{nullptr};
    AudioConnection*         _patchCord08{nullptr};
    AudioConnection*         _patchCord09{nullptr};
    AudioConnection*         _patchCord10{nullptr};
    char _string[64];
};

#endif
