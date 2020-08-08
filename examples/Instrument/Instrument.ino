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
#include <QuantoGraph.h>

SettingsFile g_settingsFile = SettingsFile();
TouchScreen g_screen = TouchScreen();
Gui g_gui = Gui();
AudioBoard g_audio = AudioBoard();
SdCard g_sdCard = SdCard();
MidiInput g_midiInput = MidiInput();
Bluetooth g_bluetooth = Bluetooth();
Player g_player = Player();
RealTime g_time = RealTime();
//Synth g_synth = Synth();

//Menu g_menu = Menu();
//CapSensor g_cap = CapSensor();
//Midi g_midi = Midi();
//Led g_led = Led();
//Keys g_keys = Keys();

//=================================================================================================
// Startup procedure, called before entering the "loop()" function
void setup() {
    // Open the serial port
    Serial.begin(152000);
    while (!Serial && (millis() <= 1000));
    Serial.println("=====> setup");

    g_sdCard.init();
    g_time.init();

    // Get the settings
    g_sdCard.makeDir("system");
    g_settingsFile.read(g_settingsFile._settings);
    g_settingsFile._settings._screen = &g_screen;
    g_settingsFile._settings._gui = &g_gui;
    g_settingsFile._settings._audioBoard = &g_audio;
    g_settingsFile._settings._sdCard = &g_sdCard;
    g_settingsFile._settings._midiInput = &g_midiInput;
    g_settingsFile._settings._bluetooth = &g_bluetooth;
    g_settingsFile._settings._player = &g_player;
    //g_settingsFile._settings._synth = &g_synth;

    // Initialize everything
    g_screen.init();
    //g_gui.init(&g_settingsFile, &g_settingsFile._settings);
    g_audio.init(&g_settingsFile._settings);

    // MIDI input
    //g_synth.init((Instrument)g_settingsFile._settings._synthSettings._instrument, &g_settingsFile._settings);
    //g_midiInput.init(&g_synth);
    
    g_player.init(&g_settingsFile._settings);
    //g_bluetooth.init();

    // Band
    Serial.printf("\n\n\n");
    Band band(&g_settingsFile._settings);
    band.test();
    g_player.play(&band._outSong);
    Serial.printf("\n\n\n");

    //Serial.println("=====> setup end");
}

//=================================================================================================
// The main loop. Called after "setup()" is done.
void loop() {
    delay(50);

    g_midiInput.process();
    g_audio.process();
    //g_gui.process();
    //g_player.process();
    //g_time.show();

    //Serial.println("=====> loop()");
}
