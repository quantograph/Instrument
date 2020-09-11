#include <QuantoGraph.h>

MidiInput g_midiInput = MidiInput();

void setup() {
    g_midiInput.init(nullptr, true);
}

void loop() {
    g_midiInput.process();
}
