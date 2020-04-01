#ifndef AudioBoard_h
#define AudioBoard_h

class Effects;
class Gui;
class Storage;

class AudioBoard
{
public:
    AudioBoard();
	void init(Gui* gui, Settings* settings);
    void setLineInLevel();
    void setMicGain();
    void setupMixers();
    void process();
    void noteFrequency();
    void noteDetected(float frequency);
    void peakMeter();
    static void onPlug();
    static void onVolume();
    void reset();
    bool effects(EffectType type1, EffectType type2);

    #define GUITAR_PLUG 3 // Teensy pin connected to guitar plug contact
    #define VOLUME_PIN 6 /*AVB - 22 in the last wiring */ // Teensy pin connected to guitar plug contact
    Gui* _gui{};
    Settings* _settings{};
    AudioInputI2S _input; // audio shield: mic or line-in
    std::list<AudioConnection*> _cords;
    int _noteNumber = 0;
    uint8_t cnt = 0;
    uint8_t _playingNote = 0;
    bool _noteOn = false;
    bool _noteDetected = false;
    int _sequence = 0;
    uint32_t _noteStartTime = 0;
    uint32_t _noteStopTime = 0;
    uint32_t _noteDetectTime = 0;
    AudioOutputI2S  _audioOutput;
    AudioControlSGTL5000 _audioControl;
    AudioAnalyzeNoteFrequency notefreq;
    // Input and output mixers
    AudioMixer4 _mixer1;
    AudioMixer4 _mixer2;
    AudioMixer4 _mixer3;
    AudioMixer4 _mixer4;
    AudioMixer4 _outMixer1;
    AudioMixer4 _outMixer2;
    char _string[64];
    Effects* _effect1{nullptr};
    Effects* _effect2{nullptr};
    AudioAnalyzePeak _peakLeft;
    AudioAnalyzePeak _peakRight;
};

#endif
