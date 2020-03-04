#ifndef AudioBoard_h
#define AudioBoard_h

#include <list>
#include <Arduino.h>
#include <Audio.h>

class AudioBoard
{
public:
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

	void init();
    void process();
    void peakMeter();
    void noteFrequency();
    void noteDetected(float frequency);

    AudioOutputI2S  _audioOutput;
    AudioControlSGTL5000 _audioControl;

    AudioAnalyzePeak peak_L;
    AudioAnalyzePeak peak_R;

    AudioAnalyzeNoteFrequency notefreq;

    // Input and output mixers
    AudioMixer4 _mixer1;
    AudioMixer4 _mixer2;
    AudioMixer4 _mixer3;
    AudioMixer4 _mixer4;
    AudioMixer4 _outMixer1;
    AudioMixer4 _outMixer2;

    char _string[64];
};

#endif
