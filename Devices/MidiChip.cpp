#include "MidiChip.h"

int Midi::_melodyChannel = 0;
int Midi::_bassChannel = 1;
int Midi::_drumChannel = 8;

//=================================================================================================
Midi::Midi() /*: _midi(0, VS1053_RX)*/ {
}

//=================================================================================================
// Initializes the MIDI connection
void Midi::Init() {
    //_midi.begin(31250); // MIDI uses a 'strange baud rate'
  
    pinMode(VS1053_RESET, OUTPUT);
    digitalWrite(VS1053_RESET, LOW);
    delay(10);
    digitalWrite(VS1053_RESET, HIGH);
    delay(10);
  
    // Set melody channel
    SetChannelBank(_melodyChannel, VS1053_BANK_MELODY);
    SetChannelPan(_melodyChannel, 64); // 0 - left, 64 - center, 127 - right
    SetInstrument(0, g_keys._currentInstrument->_midiInstrument);
    SetChannelVolume(_melodyChannel, 127);

    // Set bass channel
    SetChannelBank(_bassChannel, VS1053_BANK_MELODY);
    SetChannelPan(_bassChannel, 0);
    SetInstrument(_bassChannel, 70);
    SetChannelVolume(_bassChannel, 127);

    // Set drum channel
    SetChannelBank(_drumChannel, VS1053_BANK_DRUMS1);
    SetInstrument(_drumChannel, 30);
    SetChannelVolume(_drumChannel, 127);

    //Test();
}

//=================================================================================================
void Midi::SetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  //_midi.write(MIDI_CHAN_PROGRAM | chan);  
  //_midi.write(inst);
}

//=================================================================================================
void Midi::SetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  /*_midi.write(MIDI_CHAN_MSG | chan);
  _midi.write(MIDI_CHAN_VOLUME);
  _midi.write(vol);*/
}

//=================================================================================================
void Midi::SetChannelPan(uint8_t chan, uint8_t pan) {
  if (chan > 15) return;
  if (pan > 127) return;
  
  /*_midi.write(MIDI_CHAN_MSG | chan);
  _midi.write(MIDI_CHAN_PAN);
  _midi.write(pan);*/
}

//=================================================================================================
void Midi::SetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  /*_midi.write(MIDI_CHAN_MSG | chan);
  _midi.write((uint8_t)MIDI_CHAN_BANK);
  _midi.write(bank);*/
}

//=================================================================================================
void Midi::NoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) chan = 15;
  if (n > 127) n = 127;
  if (vel > 127) vel = 127;
  
  /*_midi.write(MIDI_NOTE_ON | chan);
  _midi.write(n);
  _midi.write(vel);*/
}

//=================================================================================================
void Midi::NoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  /*_midi.write(MIDI_NOTE_OFF | chan);
  _midi.write(n);
  _midi.write(vel);*/
}

//=================================================================================================
void Midi::Test(uint8_t volume) {  
    // Drums
    for (uint8_t i=27; i<=87; i++) {
        Serial.printf("Drum note on: note=%d, volume=%d, channel=%d\n", i, volume, _drumChannel);
        NoteOn(_drumChannel, i, volume);
        delay(200);
        Serial.printf("Drum note off: note=%d, channel=%d\n", i, _drumChannel);
        NoteOff(_drumChannel, i);
    }

    // Melodic
    for (uint8_t j=1; j<=128; j++) {
        SetInstrument(_melodyChannel, j);
        SetInstrument(_bassChannel, j);
        for (uint8_t i=64; i<67; i++) {
            Serial.printf("Melody note on: note=%d, volume=%d, channel=%d\n", i, volume, _melodyChannel);
            NoteOn(_melodyChannel, i, 70);
            delay(100);

            Serial.printf("Bass note on: note=%d, volume=%d, channel=%d\n", i, volume, _bassChannel);
            NoteOn(_bassChannel, i, 70);
            delay(100);

            Serial.printf("Melody note off: note=%d, channel=%d\n", i, _melodyChannel);
            NoteOff(_melodyChannel, i);
            Serial.printf("Bass note off: note=%d, channel=%d\n", i, _bassChannel);
            NoteOff(_bassChannel, i);
        }
    }
}
