#include "../Devices/Devices.h"
#include "Settings.h"

// EEPROM API documentation: https://www.arduino.cc/en/Reference/EEPROM

//=================================================================================================
Settings::Settings() {
}

//=================================================================================================
// Reads all settings
bool Settings::ReadBuffer(int address, uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        buffer[i] = EEPROM.read(address + i);
    }

    return true;
}

    
//=================================================================================================
// Writes all settings
bool Settings::WriteBuffer(int address, const uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        EEPROM.write(address + i, buffer[i]);
    }

    return true;
}

//=================================================================================================
// Read all settings
bool Settings::read() {
    char marker[5];
    int offset = 0;

    // See if the market is written
    ReadBuffer(offset, (uint8_t*)marker, 4);
    marker[4] = 0;
    if(strncmp(_marker, marker, 4)) {
        Serial.println("##### Settings marker not found");
        return false;
    }
    
    // Read the data size
    _size = 0;
    offset += 4;
    ReadBuffer(offset, (uint8_t*)&_size, sizeof(_size)); // Read the buffer size first

    // Read the data size
    offset += 2;
    ReadBuffer(offset, (uint8_t*)&_data, _size); // Read the buffer with settings now

    //Show("Reading");

    return true;
}

//=================================================================================================
// Writes all settings
bool Settings::Write() {
    int offset = 0;

    // Write the marker first
    WriteBuffer(0, (const uint8_t*)_marker, 4); // Write the buffer size first

    // Write the data size
    offset += 4;
    _size = sizeof(_data);
    WriteBuffer(offset, (const uint8_t*)&_size, sizeof(_size)); // Write the buffer size first

    // Write the data
    offset += 2;
    WriteBuffer(offset, (const uint8_t*)&_data, _size);

    //Show("Saving");

    return true;
}

//=================================================================================================
void Settings::Show(const char* title) {
    Serial.printf("===== %s settings\n", title);
    Serial.printf("   Marker=%s\n", _marker);
    Serial.printf("   Size=%d\n", _size);
    //Serial.printf("   Instrument=%d\n", _data._midiInstrument);
    //Serial.printf("   Volume=%d\n", _data._volume);
}
