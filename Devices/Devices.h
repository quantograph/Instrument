#ifndef Devices_h
#define Devices_h

#include <Arduino.h>
#include <Audio.h>
#include <vector>
#include <list>
#include <XPT2046_Touchscreen.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <USBHost_t36.h>
#include <SD.h>
#include <EEPROM.h>

// Audio inputs
enum Inputs {
    none,
    mic,
    line
};

#endif
