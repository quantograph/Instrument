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

Storage g_storage = Storage();
TouchScreen g_screen = TouchScreen();
Gui g_gui = Gui();
AudioBoard g_audio = AudioBoard();
SdCard g_sdCard = SdCard();
MidiInput g_midiInput = MidiInput();
Bluetooth g_bluetooth = Bluetooth();
Player g_player = Player();
Synth g_synth = Synth();

//Menu g_menu = Menu();
//CapSensor g_cap = CapSensor();
//Midi g_midi = Midi();
//Led g_led = Led();
//Keys g_keys = Keys();

//=================================================================================================
// Startup procedure, called before entering the "loop()" function
void setup() {
    // Open the serial port
    Serial.begin(38400);
    while (!Serial && (millis() <= 1000));
    Serial.println("=====> setup");

    // Get the settings
    g_storage.write();
    g_storage.read();
    g_storage._settings._screen = &g_screen;
    g_storage._settings._gui = &g_gui;
    g_storage._settings._audio = &g_audio;
    g_storage._settings._sdCard = &g_sdCard;
    g_storage._settings._midiInput = &g_midiInput;
    g_storage._settings._bluetooth = &g_bluetooth;
    g_storage._settings._player = &g_player;
    g_storage._settings._synth = &g_synth;

    // Initialize everything
    g_screen.init();
    g_gui.init(&g_storage, &g_storage._settings);
    g_audio.init(&g_gui, &g_storage._settings);

    // MIDI input
    g_synth.init(ACOUSTIC_GRAND_PIANO, &g_storage._settings);
    g_midiInput.init(&g_synth);
    
    //g_player.init();
    //g_sdCard.init();
    //g_bluetooth.init();

    //g_cap.init();
    //g_keys.init();
    //g_menu.init();
    //g_led.init(0.04);
    //g_sdCard.test();
    //g_sdCard.printDirectory(SD.open("/"), 0);
    //g_midi.init();
    //g_midi.test(127);
    //g_player.test();
}

//=================================================================================================
// The main loop. Called after "setup()" is done.
void loop() {
    /*if(g_bluetooth.receive()) {
        Serial.printf("BT: %s\n", g_bluetooth._buffer);
        //g_bluetooth.send(g_bluetooth._buffer, g_bluetooth._received - 1);
        g_bluetooth._received = 0;
    }*/

    g_midiInput.process();
    g_audio.process();
    //g_player.process();
    g_gui.process();

    // Get the sensors touched
    /*uint32_t touched = 0;
    if(g_cap.GetTouched(touched, false, true) && g_menu._current == nullptr) { // Not in menu mode
        g_keys.Play(touched);
    }*/

    // All tests
    //g_audio.test(0);
    //g_audio.testDrums(500);
    //Serial.printf("Pressure: %5d\n", analogRead(A22));
    //g_bluetooth.test();
    //g_cap.test();
    //g_led.test();
    //Serial.println("=====> loop()");

    //delay(50);
}