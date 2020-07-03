#pragma once

extern const char* _instrumentNames[];
extern const char* _drumNames[];

// List of integers
typedef std::vector<int> IntList;
typedef IntList::iterator IntListIter;

String randomString();
float scale(float value, float fromMin, float fromMax, float toMin, float toMax);
uint8_t randomInt();
float randomFloat();
void reverse2Bytes(char* data);
void reverse4Bytes(char* data);
//void ShowBits(uint32_t value, String& s);
//void LogScreen(const char* line);
bool getInstrument(Instrument instrument, AudioBoard* audio, InstrumentInfo& info);
int addIntList(IntList& list, int* values, int size);
double randomDouble(double from, double to);
bool isBass(Instrument instrument);
