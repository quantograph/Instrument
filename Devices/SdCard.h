#ifndef SdCard_h
#define SdCard_h

#include <SD.h>

class SdCard {
public:
    SdCard();
    void Init();
    void Test();
    void PrintDirectory(File dir, int numTabs);
};

#endif
