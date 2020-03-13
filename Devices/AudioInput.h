#ifndef AudioInput_h
#define AudioInput_h

#include <list>
#include <Arduino.h>
#include <Audio.h>

class Effects;
class AudioBoard;

class AudioInput {
public:
    #define GUITAR_PLUG 3 // Teensy pin connected to guitar plug contact

    AudioInput(AudioBoard* audio);
    void init();
    void peakMeter();
    static void onPlug();
    void reset();
    void passthrough();
    void flange(double freq);
    void chorus();
    void reverb();
    void freeReverb();

    AudioBoard* _audio{nullptr};
    AudioInputI2S _input; // audio shield: mic or line-in
    std::list<AudioConnection*> _cords;
    Effects* _effect1{nullptr};
    Effects* _effect2{nullptr};
    AudioConnection* _passthrough1{nullptr};
    AudioConnection* _passthrough2{nullptr};
    AudioAnalyzePeak _peakLeft;
    AudioAnalyzePeak _peakRight;
};

#endif
