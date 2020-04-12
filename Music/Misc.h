#pragma once

extern const char* _instrumentNames[];
extern const char* _drumNames[];

void reverse2Bytes(char* data);
void reverse4Bytes(char* data);
//void ShowBits(uint32_t value, String& s);
//void LogScreen(const char* line);
bool getInstrument(Instrument instrument, AudioBoard* audio, InstrumentInfo& info);
