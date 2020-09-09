#pragma once

class SdCard {
public:
    SdCard();
    void init();
    void test();
    bool writeFile(const char* path, const char* data, uint32_t dataSize);
    bool readFile(const char* path, char*& data, uint32_t& dataSize);
    void printDirectory(File dir, int numTabs);
    bool makeDir(const char* path);
};
