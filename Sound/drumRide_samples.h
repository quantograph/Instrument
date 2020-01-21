#pragma once

#include <Audio.h>

extern const AudioSynthWavetable::sample_data drumRide_samples[1];
const uint8_t drumRide_ranges[] = {60};

const AudioSynthWavetable::instrument_data drumRide = {1, drumRide_ranges, drumRide_samples };

extern const uint32_t sample_drumRide[6656];
