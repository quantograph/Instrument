#pragma once

#include <Audio.h>

extern const AudioSynthWavetable::sample_data drumBass_samples[1];
const uint8_t drumBass_ranges[] = {36};

const AudioSynthWavetable::instrument_data drumBass = {1, drumBass_ranges, drumBass_samples };

extern const uint32_t sample_drumBass[896];
