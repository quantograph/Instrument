#pragma once

#include <Audio.h>
#include "../Music/Misc.h"

extern const AudioSynthWavetable::sample_data drumTomLow_samples[1];
const uint8_t drumTomLow_ranges[] = {DRUM_TOM_LOW};

const AudioSynthWavetable::instrument_data drumTomLow = {1, drumTomLow_ranges, drumTomLow_samples };

extern const uint32_t sample_drumTomLow[3712];
