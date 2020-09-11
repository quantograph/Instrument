#include <QuantoGraph.h>

MidiInput g_midiInput = MidiInput();

void setup() {
    g_midiInput.init();
}

void loop() {
    g_midiInput.process();
  
    delay(50);
}
