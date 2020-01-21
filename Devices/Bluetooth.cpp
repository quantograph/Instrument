#include <Arduino.h>
#include "Misc.h"
#include "Bluetooth.h"

/*
Connections:
BT board -> Teensy
     TXD -> pin 0, RX1
     RXD -> pin 1, TX1

Protocol
--------
2 bytes - packet size
packet:
    2 bytes - tag number
    tag value


Tags:
1 - note info:
    2 - instrument, string: rhythmGuitar, leadGuitar, bassGuitar, drums
    3 - MIDI note number or drum name for "drums": 
            tamborine, bass, snare_low, snare_high, tom_low, tom_mid, tom_high, crash_low, crash_high, ride_shoulder
    4 - volume, 0 - 127
    5 - string number (for guitars)
*/

// Separators:
#define PACKET_START 1 // Start of packet
#define PACKET_END 2 // Start of packet
#define TAG_END 3 // End of tag number
#define VALUE_END 4 // End of tag value
#define GROUP_END 5 // End of group of tags

//=================================================================================================
Bluetooth::Bluetooth() {
}

//=================================================================================================
bool Bluetooth::Init() {
    Serial.printf("BT starting...\n");
    Serial1.begin(9600); // RXD on Bluetooth goes to pin 1 on Teensy, TXD -> pin 0
    Serial.printf("BT started...\n");

    return true;
}

//=================================================================================================
bool Bluetooth::Send(const char* buffer, int size) {
    Serial1.write(buffer, size);
    return true;
}

//=================================================================================================
bool Bluetooth::Receive() {
    while(Serial1.available() > 0 && _received < BUFFER_SIZE - 1) {
        _buffer[_received++] = (char)Serial1.read();
    }

    if(_received > 0)
        return true;
    else
        return false;
}

//=================================================================================================
bool Bluetooth::Process() {
    if(Serial1.available() == 0)
        return false;

    // Get the new packet size - first 2 bytes

    //g_audio.NoteOn(AudioBoard::Instrument::drums, AudioBoard::DrumNotes::snare_low, 90);

    sprintf(_string, "BT: %s", _buffer);
    //LogScreen(_string);

    _received = 0;

    return true;
}
