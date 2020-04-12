#ifndef Devices_h
#define Devices_h

#include <limits>
#include <vector>
#include <list>
#include <float.h>

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <SerialFlash.h>
#include <Adafruit_MPR121.h>
#include <WireKinetis.h>
#include <CapacitiveSensor.h> // Must be the included the last to avoid conflicts with ILI9341_t3.h
#include <USBHost_t36.h>

#include "Definitions.h"
#include "AudioBoard.h"
#include "Bluetooth.h"
#include "MidiInput.h"
#include "SdCard.h"
#include "TouchScreen.h"
#include "Storage.h"

#endif
