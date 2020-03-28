#include "../Devices/Devices.h"
#include "Settings.h"

// EEPROM API documentation: https://www.arduino.cc/en/Reference/EEPROM

//=================================================================================================
Settings::Settings() {
}

//=================================================================================================
// Reads all settings
bool Settings::readBuffer(int address, uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        buffer[i] = EEPROM.read(address + i);
    }

    return true;
}

    
//=================================================================================================
// Writes all settings
bool Settings::writeBuffer(int address, const uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        EEPROM.write(address + i, buffer[i]);
    }

    return true;
}

//=================================================================================================
// Read all settings
void Settings::putValue(String& string, const char* tag, int16_t value) {
    char temp[32];
    sprintf(temp, "%s=%d|", tag, value);
    string += temp;
}

//=================================================================================================
// Read all settings
bool Settings::read() {
    char marker[6];
    int offset = 0;
    char* buffer;

    // See if the marker is written
    readBuffer(offset, (uint8_t*)marker, 5);
    marker[5] = 0;
    if(strncmp(_marker, marker, 5)) {
        Serial.println("##### Settings start marker not found");
        return false;
    }
    
    // Read the data size
    _size = 0;
    offset += 5;
    readBuffer(offset, (uint8_t*)&_size, sizeof(_size)); // Read the buffer size first

    // Read the data
    offset += 2;
    buffer = (char*)malloc(_size + 1);
    readBuffer(offset, (uint8_t*)buffer, _size); // Read the buffer with settings now
    buffer[_size] = 0;

    // Parse the "tag=value|" pairs
    //Serial.printf("\n(%d) %s\n", _size, (const char*)buffer);
    offset = 0;
    char* start = buffer;
    char* end;
    do {
        end = strchr(start, '|');
        if(!end)
            break;

        *end = 0;
        getValue(start);
        start = end + 1;
    } while(end);

    show("Reading");

    free(buffer);

    return true;
}

//=================================================================================================
bool Settings::getValue(char* pair) {
    char* delim;
    char* tag;
    char* value;

    //Serial.printf("pair: %s, ", pair);

    // Get the tag and value strings
    delim = strchr(pair, '=');
    *delim = 0;
    tag = pair;
    value = delim + 1;
    //Serial.printf("%s=%s\n", tag, value);

    // Put the value into its member variable
    if(!strcmp(tag, inputTag))
        _data._input = (Inputs)atoi(value);
    else if(!strcmp(tag, micGainTag))
        _data._micGain = atoi(value);
    else if(!strcmp(tag, lineInLevelTag))
        _data._lineInLevel = atoi(value);
    else {
        Serial.printf("##### ERROR: Unknown tag %s=%s\n", tag, value);
    }

    return true;
}

//=================================================================================================
// Writes all settings
bool Settings::write() {
    int offset = 0;
    String string;

    writeBuffer(0, (const uint8_t*)_marker, 5); // Write the marker

    // Put all parameters
    putValue(string, inputTag, (int16_t)_data._input);
    putValue(string, micGainTag, _data._micGain);
    putValue(string, lineInLevelTag, _data._lineInLevel);
    string += endTag;

    // Write the data size
    offset += 5;
    _size = string.length();
    writeBuffer(offset, (const uint8_t*)&_size, sizeof(_size)); // Write the buffer size first

    // Write the data
    offset += 2;
    writeBuffer(offset, (const uint8_t*)string.c_str(), _size);

    show("Saving");
    Serial.printf("\n(%d) %s\n", _size, string.c_str());

    return true;
}

//=================================================================================================
void Settings::show(const char* title) {
    Serial.printf("===== %s settings\n", title);
    Serial.printf("   Marker=%s\n", _marker);
    Serial.printf("   Size=%d\n", _size);
    Serial.printf("   _input=%d\n", _data._input);
    Serial.printf("   _micGain=%d\n", _data._micGain);
    Serial.printf("   _lineInLevel=%d\n", _data._lineInLevel);

    //Serial.printf("      _screen=%p\n", _screen);

    //Serial.printf("   Instrument=%d\n", _data._midiInstrument);
    //Serial.printf("   Volume=%d\n", _data._volume);
}
