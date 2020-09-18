#include <QuantoGraph.h>

RealTime g_time = RealTime();

//=================================================================================================
// Startup procedure, called before entering the "loop()" function
void setup() {
    // Open the serial port
    Serial.begin(152000);
    while (!Serial && (millis() <= 1000));

    g_time.init();
    //g_time.sync();
}

//=================================================================================================
// The main loop. Called after "setup()" is done.
void loop() {
    g_time.show();
    delay(1000);
}
