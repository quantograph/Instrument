#pragma once

extern const AudioSynthWavetable::sample_data drumCrash_samples[1];
const uint8_t drumCrash_ranges[] = {DRUM_CRASH1};

const AudioSynthWavetable::instrument_data drumCrash = {1, drumCrash_ranges, drumCrash_samples };

extern const uint32_t sample_drumCrash[6784];
