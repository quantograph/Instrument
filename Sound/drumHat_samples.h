#pragma once

#include <Audio.h>
#include "../Music/Misc.h"

extern const AudioSynthWavetable::sample_data drumHat_samples[1];
const uint8_t drumHat_ranges[] = {DRUM_HIHAT_CLOSED};

const AudioSynthWavetable::instrument_data drumHat = {1, drumHat_ranges, drumHat_samples };

extern const uint32_t sample_drumHat[1920];
