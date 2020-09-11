#ifndef QuantoGraph_h
#define QuantoGraph_h

#include <TimeLib.h>
#include <list>
#include <vector>

// Arduino libraries
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


#include "Devices/Devices.h"
#include "Devices/AudioBoard.h"
#include "Devices/Bluetooth.h"
#include "Devices/Led.h"
#include "Devices/MidiInput.h"
#include "Devices/SdCard.h"
#include "Devices/TouchScreen.h"
#include "Devices/Storage.h"

#include "GUI/Control.h"
#include "GUI/Window.h"
#include "GUI/Main.h"
#include "GUI/Gui.h"

#include "Sound/Sound.h"
#include "Music/Music.h"

#endif
