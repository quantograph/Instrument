#ifndef AudioInput_h
#define AudioInput_h

#include "Effects.h"

class AudioBoard;

class AudioInput {
public:
    #define GUITAR_PLUG 3 // Teensy pin connected to guitar plug contact

    AudioInput(AudioBoard* audio);
    void Init();
    static void OnPlug();
    void Reset();
    void Passthrough();
    void Flange(double freq);
    void Chorus();

    AudioBoard* _audio{nullptr};
    AudioInputI2S _input; // audio shield: mic or line-in
    Effects* _effect1{nullptr};
    Effects* _effect2{nullptr};
    AudioConnection* _passthrough1{nullptr};
    AudioConnection* _passthrough2{nullptr};
};

#endif
