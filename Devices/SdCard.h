#ifndef SdCard_h
#define SdCard_h

#include <SD.h>

class SdCard {
public:
    SdCard();
    void Init();
    void Test();
    bool ReadFile(const char* path, char*& data, uint32_t& dataSize);
    void PrintDirectory(File dir, int numTabs);
};

#endif
