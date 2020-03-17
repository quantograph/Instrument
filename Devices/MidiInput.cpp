#include <list>
#include "AudioBoard.h"
#include "../Music/Synth.h"
#include "MidiInput.h"

Synth* MidiInput::_synth{nullptr};
USBHost _usbHost;
MIDIDevice _midiDevice(_usbHost);

void MidiInput::init(Synth* synth) {
    Serial.begin(115200);
    
    _synth = synth;

  // Wait 1.5 seconds before turning on USB Host.  If connected USB devices
  // use too much power, Teensy at least completes USB enumeration, which
  // makes isolating the power issue easier.
  delay(1500);
  _usbHost.begin();

  _midiDevice.setHandleNoteOn(myNoteOn);
  _midiDevice.setHandleNoteOff(myNoteOff);
  _midiDevice.setHandleAfterTouchPoly(myAfterTouchPoly);
  _midiDevice.setHandleControlChange(myControlChange);
  _midiDevice.setHandleProgramChange(myProgramChange);
  _midiDevice.setHandleAfterTouchChannel(myAfterTouchChannel);
  _midiDevice.setHandlePitchChange(myPitchChange);
  // Only one of these System Exclusive handlers will actually be
  // used.  See the comments below for the difference between them.
  _midiDevice.setHandleSystemExclusive(mySystemExclusiveChunk);
  _midiDevice.setHandleSystemExclusive(mySystemExclusive); 
  _midiDevice.setHandleTimeCodeQuarterFrame(myTimeCodeQuarterFrame);
  _midiDevice.setHandleSongPosition(mySongPosition);
  _midiDevice.setHandleSongSelect(mySongSelect);
  _midiDevice.setHandleTuneRequest(myTuneRequest);
  _midiDevice.setHandleClock(myClock);
  _midiDevice.setHandleStart(myStart);
  _midiDevice.setHandleContinue(myContinue);
  _midiDevice.setHandleStop(myStop);
  _midiDevice.setHandleActiveSensing(myActiveSensing);
  _midiDevice.setHandleSystemReset(mySystemReset);
  // This generic System Real Time handler is only used if the
  // more specific ones are not set.
  _midiDevice.setHandleRealTimeSystem(myRealTimeSystem);

  Serial.println("MidiInput::Init");
}

void MidiInput::process() {
  // The handler functions are called when _midiDevice reads data.  They
  // will not be called automatically.  You must call _midiDevice.read()
  // regularly from loop() for _midiDevice to actually read incoming
  // data and run the handler functions as messages arrive.
  _usbHost.Task();
  _midiDevice.read();
}

void MidiInput::myNoteOn(byte channel, byte note, byte velocity) {
  // When a USB device with multiple virtual cables is used,
  // _midiDevice.getCable() can be used to read which of the virtual
  // MIDI cables received this message.
  /*Serial.print("Note On, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);*/

  _synth->noteOn(note, velocity);
}

void MidiInput::myNoteOff(byte channel, byte note, byte velocity) {
  /*Serial.print("Note Off, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);*/

  _synth->noteOff(note);
}

void MidiInput::myAfterTouchPoly(byte channel, byte note, byte velocity) {
  Serial.print("AfterTouch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
}

void MidiInput::myControlChange(byte channel, byte control, byte value) {
  Serial.print("Control Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", control=");
  Serial.print(control, DEC);
  Serial.print(", value=");
  Serial.println(value, DEC);
}

void MidiInput::myProgramChange(byte channel, byte program) {
  Serial.print("Program Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", program=");
  Serial.println(program, DEC);
}

void MidiInput::myAfterTouchChannel(byte channel, byte pressure) {
  Serial.print("After Touch, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pressure=");
  Serial.println(pressure, DEC);
}

void MidiInput::myPitchChange(byte channel, int pitch) {
  Serial.print("Pitch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pitch=");
  Serial.println(pitch, DEC);
}

// This 3-input System Exclusive function is more complex, but allows you to
// process very large messages which do not fully fit within the _midiDevice's
// internal buffer.  Large messages are given to you in chunks, with the
// 3rd parameter to tell you which is the last chunk.  This function is
// a Teensy extension, not available in the Arduino MIDI library.
void MidiInput::mySystemExclusiveChunk(const byte *data, uint16_t length, bool last) {
  Serial.print("SysEx Message: ");
  printBytes(data, length);
  if (last) {
    Serial.println(" (end)");
  } else {
    Serial.println(" (to be continued)");
  }
}

// This simpler 2-input System Exclusive function can only receive messages
// up to the size of the internal buffer.  Larger messages are truncated, with
// no way to receive the data which did not fit in the buffer.  If both types
// of SysEx functions are set, the 3-input version will be called by _midiDevice.
void MidiInput::mySystemExclusive(byte *data, unsigned int length) {
  Serial.print("SysEx Message: ");
  printBytes(data, length);
  Serial.println();
}

void MidiInput::myTimeCodeQuarterFrame(byte data) {
  static char SMPTE[8]={'0','0','0','0','0','0','0','0'};
  static byte fps=0;
  byte index = data >> 4;
  byte number = data & 15;
  if (index == 7) {
    fps = (number >> 1) & 3;
    number = number & 1;
  }
  if (index < 8 || number < 10) {
    SMPTE[index] = number + '0';
    Serial.print("TimeCode: ");  // perhaps only print when index == 7
    Serial.print(SMPTE[7]);
    Serial.print(SMPTE[6]);
    Serial.print(':');
    Serial.print(SMPTE[5]);
    Serial.print(SMPTE[4]);
    Serial.print(':');
    Serial.print(SMPTE[3]);
    Serial.print(SMPTE[2]);
    Serial.print('.');
    Serial.print(SMPTE[1]);  // perhaps add 2 to compensate for MIDI latency?
    Serial.print(SMPTE[0]);
    switch (fps) {
      case 0: Serial.println(" 24 fps"); break;
      case 1: Serial.println(" 25 fps"); break;
      case 2: Serial.println(" 29.97 fps"); break;
      case 3: Serial.println(" 30 fps"); break;
    }
  } else {
    Serial.print("TimeCode: invalid data = ");
    Serial.println(data, HEX);
  }
}

void MidiInput::mySongPosition(uint16_t beats) {
  Serial.print("Song Position, beat=");
  Serial.println(beats);
}

void MidiInput::mySongSelect(byte songNumber) {
  Serial.print("Song Select, song=");
  Serial.println(songNumber, DEC);
}

void MidiInput::myTuneRequest() {
  Serial.println("Tune Request");
}

void MidiInput::myClock() {
  Serial.println("Clock");
}

void MidiInput::myStart() {
  Serial.println("Start");
}

void MidiInput::myContinue() {
  Serial.println("Continue");
}

void MidiInput::myStop() {
  Serial.println("Stop");
}

void MidiInput::myActiveSensing() {
  Serial.println("Actvice Sensing");
}

void MidiInput::mySystemReset() {
  Serial.println("System Reset");
}

void MidiInput::myRealTimeSystem(uint8_t realtimebyte) {
  Serial.print("Real Time Message, code=");
  Serial.println(realtimebyte, HEX);
}

void MidiInput::printBytes(const byte *data, unsigned int size) {
  while (size > 0) {
    byte b = *data++;
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    if (size > 1) Serial.print(' ');
    size = size - 1;
  }
}
