#include <QuantoGraph.h>

SdCard g_sdCard = SdCard();

//=================================================================================================
// Startup procedure, called before entering the "loop()" function
void setup() {
    // Open the serial port
    Serial.begin(152000);
    while (!Serial && (millis() <= 1000));

    g_sdCard.init();
    g_sdCard.test();
}

//=================================================================================================
// The main loop. Called after "setup()" is done.
void loop() {
    delay(50);
}
