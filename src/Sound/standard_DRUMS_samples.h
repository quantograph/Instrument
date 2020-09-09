#pragma once

extern const AudioSynthWavetable::sample_data standard_DRUMS_samples[48];
const uint8_t standard_DRUMS_ranges[] = {27, 28, 28, 29, 30, 31, 33, 34, 35, 36, 37, 39, 42, 45, 47, 48, 50, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 65, 67, 68, 69, 71, 72, 73, 74, 76, 78, 80, 81, 82, 83, 83, 84, 85, 85, 86, 87, 127, };

const AudioSynthWavetable::instrument_data standard_DRUMS = {48, standard_DRUMS_ranges, standard_DRUMS_samples };

extern const uint32_t sample_0_standard_DRUMS_filtersnap[256];      // filtersnap, // 27
extern const uint32_t sample_1_standard_DRUMS_whitenoisewave[4224]; // whitenoisewave, // 28
extern const uint32_t sample_2_standard_DRUMS_verbclickwave[640];   // verbclickwave, // 28
extern const uint32_t sample_3_standard_DRUMS_scratch[896];         // scratch, // 29
extern const uint32_t sample_4_standard_DRUMS_guitarfret[1792];     // guitarfret, // 30
extern const uint32_t sample_5_standard_DRUMS_stix[256];            // stix, // 31
extern const uint32_t sample_6_standard_DRUMS_sinetick[128];        // sinetick, // 33
extern const uint32_t sample_7_standard_DRUMS_verbclickwave[640];   // verbclickwave_2, // 34
extern const uint32_t sample_8_standard_DRUMS_coldglass12wave[128]; // coldglass12wave, // 35
extern const uint32_t sample_9_standard_DRUMS_bd15[896];            // bass, // 36 >>>
extern const uint32_t sample_10_standard_DRUMS_verbclickwave[640];  // cross_stick, // 37 ===
extern const uint32_t sample_11_standard_DRUMS_snare24[2048];       // snare, // 39 >>>
extern const uint32_t sample_12_standard_DRUMS_snare24[2048];       // snare_high, // 42 >>>
extern const uint32_t sample_13_standard_DRUMS_floortombrite[3712]; // tom_mid, // 45 >>>
extern const uint32_t sample_14_standard_DRUMS_hatopenms[5888];     // hat_open, // 47 ===
extern const uint32_t sample_15_standard_DRUMS_floortombrite[3712]; // tomHigh, // 48 >>>
extern const uint32_t sample_16_standard_DRUMS_crash5[6784];        // crash, // 50 >>>
extern const uint32_t sample_17_standard_DRUMS_chcrash[4864];       // china, // 53 >>>
extern const uint32_t sample_18_standard_DRUMS_tamborine[1920];     // hat, // 54
extern const uint32_t sample_19_standard_DRUMS_crash5[6784];        // splash, // 55 >>>
extern const uint32_t sample_20_standard_DRUMS_cowbell[896];        // cowbell, // 56 ===
extern const uint32_t sample_21_standard_DRUMS_crash5[6784];        // crash_high, // 57 >>>
extern const uint32_t sample_22_standard_DRUMS_vibraloop[512];      // vibraloop, // 58
extern const uint32_t sample_23_standard_DRUMS_paisteping[6656];    // ride, // 60 >>>
extern const uint32_t sample_24_standard_DRUMS_mbongotone[1408];    // bongo_tone, // 61 ===
extern const uint32_t sample_25_standard_DRUMS_quintoslap[1536];    // quinto_slap, // 62 ===
extern const uint32_t sample_26_standard_DRUMS_quintotone[1664];    // quinto_tone,//  63 ===
extern const uint32_t sample_27_standard_DRUMS_lowtumba[2048];      // low_tumba, // 65 ===
extern const uint32_t sample_28_standard_DRUMS_timpani[3968];       // timpani, // 67 ===
extern const uint32_t sample_29_standard_DRUMS_agogolotone[2304];   // agogo_lotone, // 68 ===
extern const uint32_t sample_30_standard_DRUMS_cabasastrk[1408];    // cabasa, // 69 ===
extern const uint32_t sample_31_standard_DRUMS_maracas[1664];       // maracas, // 71
extern const uint32_t sample_32_standard_DRUMS_sambawhistle[896];   // sambawhistle, // 72
extern const uint32_t sample_33_standard_DRUMS_guirodown[1408];     // guirodown, // 73
extern const uint32_t sample_34_standard_DRUMS_guiro2[1408];        // guiro2, // 74
extern const uint32_t sample_35_standard_DRUMS_bockclave[896];      // bockclave, // 76
extern const uint32_t sample_36_standard_DRUMS_woodblock[640];      // woodblock, // 78
extern const uint32_t sample_37_standard_DRUMS_quicadown[896];      // quica_down, // 80
extern const uint32_t sample_38_standard_DRUMS_triangle[1152];      // triangle, // 81
extern const uint32_t sample_39_standard_DRUMS_cabasastrk[1408];    // cabasa_2, // 82
extern const uint32_t sample_40_standard_DRUMS_tamborine[1920];     // tamborine_2, // 83
extern const uint32_t sample_41_standard_DRUMS_chcrash[4864];       // chcrash_2, // 83 >>>
extern const uint32_t sample_42_standard_DRUMS_belltree[3840];      // belltree, // 84
extern const uint32_t sample_43_standard_DRUMS_ebongostone[1664];   // ebongostone, // 85
extern const uint32_t sample_44_standard_DRUMS_sinetick[128];       // sinetick_2, // 85
extern const uint32_t sample_45_standard_DRUMS_mbongotone[1408];    // bongo_tone_2, // 86
extern const uint32_t sample_46_standard_DRUMS_floortombrite[3712]; // tomLow, // 87 >>>
extern const uint32_t sample_47_standard_DRUMS_timpani[3968];       // timpani_2 // 127
