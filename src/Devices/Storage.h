#pragma once

class Storage {
public:
    Storage();
    bool readBuffer(int address, uint8_t* buffer, int size);
    bool writeBuffer(int address, const uint8_t* buffer, int size);
    bool test();
};
