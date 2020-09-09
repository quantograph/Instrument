#include "Devices.h"

// EEPROM API documentation: https://www.arduino.cc/en/Reference/EEPROM
// Max data size ia 1080 bytes.

//=================================================================================================
Storage::Storage() {
}

//=================================================================================================
// Reads all settings
bool Storage::readBuffer(int address, uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        buffer[i] = EEPROM.read(address + i);
    }

    return true;
}
    
//=================================================================================================
// Writes all settings
bool Storage::writeBuffer(int address, const uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        EEPROM.write(address + i, buffer[i]);
    }

    return true;
}

//=================================================================================================
bool Storage::test() {
    char c = 'a';
    char r;
    int i;
    for(i = 0; i < 1024*1024; ++i) {
        EEPROM.write(i, c);
        r = EEPROM.read(i);
        if(r != c) {
            Serial.printf("##### ERROR: %d, wrote %c, read %c\n", i, c, r);
            return false;
        }

        if(i % 1024 == 0)
            Serial.printf("%d: Wrote %c, read %c\n", i, c, r);

        c++;
        if(c == 'z')
            c = 'a';
    }

    Serial.printf("End: %d wrote %c, read %c\n", i, c, r);
    return true;
}
