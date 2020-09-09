#pragma once

extern const AudioSynthWavetable::sample_data drumBlock_samples[1];
const uint8_t drumBlock_ranges[] = {DRUM_HI_WOOD_BLOCK};

const AudioSynthWavetable::instrument_data drumBlock = {1, drumBlock_ranges, drumBlock_samples };

extern const uint32_t sample_drumBlock[1664];
