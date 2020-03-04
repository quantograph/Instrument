#pragma once

#include <Audio.h>
#include "../Music/Misc.h"

extern const AudioSynthWavetable::sample_data drumSnare_samples[1];
const uint8_t drumSnare_ranges[] = {DRUM_SNARE};

const AudioSynthWavetable::instrument_data drumSnare = {1, drumSnare_ranges, drumSnare_samples };

extern const uint32_t sample_drumSnare[2048];
