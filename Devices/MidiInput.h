#pragma once

#include <Arduino.h>
#include <USBHost_t36.h>

class Synth;

class MidiInput {
public:
    void init(Synth* synth);
    void process();
    static void myNoteOn(byte channel, byte note, byte velocity);
    static void myNoteOff(byte channel, byte note, byte velocity);
    static void myAfterTouchPoly(byte channel, byte note, byte velocity);
    static void myControlChange(byte channel, byte control, byte value);
    static void myProgramChange(byte channel, byte program);
    static void myAfterTouchChannel(byte channel, byte pressure);
    static void myPitchChange(byte channel, int pitch);
    static void mySystemExclusiveChunk(const byte *data, uint16_t length, bool last);
    static void mySystemExclusive(byte *data, unsigned int length);
    static void myTimeCodeQuarterFrame(byte data);
    static void mySongPosition(uint16_t beats);
    static void mySongSelect(byte songNumber);
    static void myTuneRequest();
    static void myClock();
    static void myStart();
    static void myContinue();
    static void myStop();
    static void myActiveSensing();
    static void mySystemReset();
    static void myRealTimeSystem(uint8_t realtimebyte);
    static void printBytes(const byte *data, unsigned int size);

    static Synth* _synth;
};
