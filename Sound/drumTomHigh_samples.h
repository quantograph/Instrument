#pragma once

#include <Audio.h>

extern const AudioSynthWavetable::sample_data drumTomHigh_samples[1];
const uint8_t drumTomHigh_ranges[] = {48};

const AudioSynthWavetable::instrument_data drumTomHigh = {1, drumTomHigh_ranges, drumTomHigh_samples };

extern const uint32_t sample_drumTomHigh[3712];
