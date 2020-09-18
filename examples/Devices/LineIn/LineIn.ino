#include <QuantoGraph.h>

Settings g_settings{};
AudioBoard g_audio = AudioBoard();

//=================================================================================================
// Startup procedure, called before entering the "loop()" function
void setup() {
    // Open the serial port
    Serial.begin(152000);
    while (!Serial && (millis() <= 1000));

    g_settings._audioSettings._input = Inputs::line;
    g_audio.init(&g_settings);
}

//=================================================================================================
// The main loop. Called after "setup()" is done.
void loop() {
    g_audio.process();
    delay(50);
}
