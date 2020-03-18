#ifndef SdCard_h
#define SdCard_h

class SdCard {
public:
    SdCard();
    void init();
    void test();
    bool readFile(const char* path, char*& data, uint32_t& dataSize);
    void printDirectory(File dir, int numTabs);
};

#endif
