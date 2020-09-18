#include <QuantoGraph.h>

Led g_led = Led();

void setup() {
    g_led.Init(14, 24, 15, 1.0);

}

void loop() {
    g_led.Test();
    delay(3);
}
