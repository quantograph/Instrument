#include "../Devices/Devices.h"
#include "frenchhorn_samples.h"

const AudioSynthWavetable::sample_data frenchhorn_samples[1] = {
	{
		(int16_t*)sample_0_frenchhorn_frenchhorng4, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * WAVETABLE_CENTS_SHIFT(15) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(90) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1484 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1480 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1480 - 1) << (32 - 11)) - (((uint32_t)1420 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(7.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(11993.38 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(986.23 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-14.9)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.4 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

const uint32_t sample_0_frenchhorn_frenchhorng4[768] = {
0x00000000,0xffd3ffde,0xffc9ffca,0xffd8ffcc,0x0002ffe9,0x00450024,0x007a0063,0x00a50099,
0x00b700b9,0x00a000b2,0x0072008f,0x00330054,0x00010012,0xffcdffe1,0xffb4ffbc,0xffadffaa,
0xffb5ffaf,0xffb4ffbb,0xffc7ffc2,0x0002ffe1,0x00510024,0x008a006e,0x00c300ad,0x00eb00e0,
0x00cc00eb,0x007a00af,0x002c004e,0xfff3000c,0xffccffd7,0xffa8ffb9,0xff9cff9e,0xffbcff97,
0xfbd2fe8c,0xf822f988,0xfc05f8d2,0x03a6fff5,0x0801066f,0x0779081e,0x04f90665,0x034303e4,
0x050203d2,0x07e80732,0x03ed0737,0xfc090034,0xf2e8f76a,0xec45ef0d,0xeca9eb38,0xf7fbf0ae,
0x093b00bf,0x12750f79,0x100b12b4,0x07bd0c44,0x006403a1,0xfedfff28,0x0054ff6b,0x035601c4,
0x06bc055a,0x0452063b,0xfb07000a,0xef36f4e5,0xe484e9ce,0xe2b4e069,0xff0dee61,0x1b750f8d,
0x1fd72171,0x112b19ae,0x01fd091a,0xfa10fcdd,0xf94af875,0xfd54fb0e,0x0020feb5,0x045d0203,
0x069d0656,0xff830461,0xf2e8f970,0xe616ec90,0xdf7fe046,0xfbc6e9bf,0x1cbc0e98,0x231523be,
0x13aa1cd9,0x02500a61,0xf9c5fd34,0xf7dff835,0xfa78f97f,0xfa0ffadd,0xffb9fb7d,0x093504ce,
0x08480a35,0xfc2a028f,0xecfbf45e,0xdf98e526,0xe916de3c,0x0f29fbf4,0x25241e10,0x1d03243b,
0x08d512f2,0xfae300a3,0xf76ef868,0xfa01f8ae,0xfb9efb26,0xfef0fc88,0x06680294,0x09900962,
0x00f5066a,0xf37afa34,0xe959ee1d,0xe006e49c,0xee2ce0dd,0x18330328,0x2ac32686,0x1b9c25e4,
0x03830efd,0xf5a6faab,0xf6a8f48f,0xfbadf957,0xfe30fcf5,0x021fffc7,0x083e0573,0x06850928,
0xfadd0143,0xef6bf445,0xe8cfebf5,0xdd40e396,0xf0f9e018,0x224d0ac9,0x2ff22f28,0x19c9278a,
0xfed00b93,0xf453f717,0xf860f64b,0xfc1bfa87,0xfd75fc74,0x03280053,0x09290715,0x04b908f0,
0xf679fdb7,0xeb24f01a,0xd9b5e5c9,0xc8dec5c1,0x188eef84,0x418936d7,0x2d993d39,0x09461aea,
0xf454fb2b,0xf6edf406,0xfa7ff852,0xfe3dfcff,0xfd4ffdab,0xff78fdf4,0x033902a5,0xf9d7fed7,
0xeeccf330,0xe1d8e8fb,0xcc6dd49b,0x0297df8d,0x3ae02606,0x38574046,0x15a82891,0xf87404ae,
0xf37af33a,0xf629f4b5,0xfdb5f981,0xfcc9ff03,0xfd80fc70,0x04d70176,0xffba0402,0xf177f89f,
0xe605eb7c,0xd08fdeb9,0xe5a6cb4c,0x2e270b5a,0x421c3f9d,0x25b0370d,0x00901195,0xefa6f4da,
0xf02eef14,0xf961f3e1,0x0152ff71,0x022401cb,0x04ce02d3,0x0296052d,0xf497fc90,0xe54aec74,
0xd511de88,0xd5d0c95f,0x1e9af97c,0x438d39f9,0x2f283e53,0x09321c87,0xf0c1f986,0xef58ef12,
0xf735f20d,0x0219fe00,0x04a90369,0x05a1060e,0x013e047f,0xf50dfaef,0xe68fed9b,0xd639df25,
0xd289cd19,0x12f8f125,0x3eae3060,0x34614027,0x0f7422f2,0xf327fe4e,0xf11bef75,0xf848f396,
0x0102fd36,0x003400bf,0x035c01c6,0x01860364,0xf77dfc87,0xed24f279,0xdcbee659,0xcc60d068,
0x08c1e494,0x3d822a0f,0x3a884295,0x15702937,0xf3cb0246,0xeafbeb6a,0xf2c0ee22,0xfdfbf81e,
0x03fa0248,0x079b05a2,0x06ed0873,0xfbba0280,0xee0af4f0,0xdc81e714,0xc016cac0,0x02c7d800,
0x3f702887,0x3d204533,0x17c52b73,0xf6ac0554,0xeb5eed26,0xf127ed48,0xfea4f781,0x0353025e,
0x078304b7,0x08e90978,0xfd2b034f,0xeffdf590,0xdbe8e868,0xb87ec392,0x025ed4f5,0x419f2ae4,
0x3f4f4819,0x19b82dbc,0xf8c107af,0xed7def9c,0xefb2edf6,0xfbbaf56e,0xff44feed,0x06c3021f,
0x097a0a5a,0xfdf1051e,0xeed0f663,0xdd48e86f,0xafe4c143,0x0564d431,0x48643151,0x43094e01,
0x175a2e1e,0xf6160426,0xeccaee1b,0xec97eb7e,0xfa75f1fb,0x00e0ffbc,0x07f503c8,0x0a370b4c,
0xfd510533,0xed3bf55e,0xde0ae7dd,0xa520bf9c,0x06b0cf23,0x50073748,0x48c35512,0x1870319b,
0xf22f01fd,0xea72ea72,0xecbceaab,0xfb5bf26c,0x00e50093,0x051f01a2,0x0a9609c4,0xff800685,
0xee01f72c,0xdde9e6c1,0xa55bc51b,0x0167c6bf,0x4f5d32f6,0x4ced5621,0x1dd336dc,0xf1f50497,
0xe848e807,0xed67e9ee,0xfc16f3f4,0xfcdefeee,0x032cfe91,0x0aac0946,0x011707fb,0xefd6f949,
0xe1b8e841,0xaab7d1b9,0xf1e7b14d,0x4e602781,0x559b589a,0x288d4143,0xf69e0cb8,0xe414e742,
0xe961e4fd,0xfc6ef1e3,0xfd780103,0xfd3efbac,0x0932048c,0x058b0ab9,0xf475fdd5,0xe4d6ec48,
0xb2ead726,0xdf0fa5e2,0x48221b89,0x574b5830,0x2f5a46ba,0xfd67154b,0xe322eadc,0xe493e287,
0xf8b8ec6d,0x01f7028c,0xfd3efd5b,0x06e20113,0x06fa089c,0xf8c1ffd2,0xea12f0ba,0xc055de99,
0xc5f89f01,0x39ed05e4,0x58c154b8,0x378c4d8a,0x07d71fa0,0xe98df539,0xe41de5ea,0xf366e896,
0x01beff1b,0xfc3dfd98,0x0594ff58,0x06b20780,0xf9f40001,0xeb8ef28b,0xba4fdd6f,0xc7499acd,
0x39ed06c0,0x581d530a,0x381b4dc1,0x0ae720fd,0xef2df9f4,0xe528ea8a,0xef59e60a,0x01f0fb94,
0xfba5fe6a,0x016ffd80,0x035f04c4,0xfa460051,0xed18f4d7,0xb9cbde5c,0xcb27a1e6,0x3874066f,
0x56a750be,0x369d4c33,0x07dd1d8d,0xf0d7f8b3,0xe9a7edf6,0xee75e80c,0xff3bf93f,0xfe0efdf9,
0x05e901e7,0x021805a7,0xf80cfdd4,0xeb5af1ff,0xb023dacd,0xd444a042,0x3f3b0eea,0x56ef5323,
0x33f34997,0x03c319ba,0xf1bbf5b0,0xec82f09d,0xefc8eaac,0xfcdff919,0xfd58fc3a,0x06b702c4,
0x00490592,0xf65afb42,0xeb47f094,0xa7dbda68,0xdb779c75,0x479115c4,0x5ab657dd,0x3287499f,
0xfe3015b9,0xeca1ef40,0xec6eed17,0xf515ee2c,0xfea2fd3e,0xfd7afc23,0x085803da,0xff120685,
0xf3fcf9bd,0xead6ef5b,0xa2fbdb38,0xe01a97a5,0x4f591c59,0x5dad5bec,0x31b84a05,0xfb241323,
0xe8f2ebce,0xe9e1e99d,0xf5d0ed9a,0xfea5ff0d,0xfaf8fa90,0x08880202,0x00dc078a,0xf783fca3,
0xed30f2da,0xa5b2dee1,0xdb37926b,0x4df1184a,0x5e955b0c,0x351f4ca0,0xfc381623,0xe572e967,
0xe6f2e6d7,0xf5bcebc9,0xfe83ffdc,0xfb35f983,0x088801d3,0x030407f1,0xfa2bfe27,0xf194f651,
0xb66ee63d,0xc7cb8340,0x49020a29,0x61335d51,0x3a1652f1,0x008f1cff,0xe304ec2a,0xe492e5d7,
0xf3b3e9ce,0x015100eb,0xfa42fb52,0x052efed1,0x03230650,0xf9c7fe31,0xf3f4f77d,0xc07cea2e,
0xbc2587cc,0x3f320003,0x60ee5bb1,0x3f075694,0x075224b8,0xe48df127,0xe199e472,0xf007e5d3,
0x015bff31,0xf9d0fa8f,0x056efdcc,0x04070637,0xfa47fee9,0xf4c3f72f,0xc3abeb7c,0xb68b88e3,
0x3a84fb6d,0x62035bba,0x40c859a0,0x071225a7,0xe612f212,0xe39ae704,0xee53e5cd,0x01a7fd71,
0xfa80fc4e,0x05e7001b,0xff7a056d,0xf5f9fa8e,0xf394f49d,0xbf9aeb50,0xc3af8f89,0x3ba10077,
0x5e6157a4,0x3d8d5432,0x07782386,0xe9e5f425,0xe76aead9,0xed9ae664,0x005bfc14,0xf9effbdc,
0x0512fff3,0xfc9e030d,0xf535f8c6,0xf221f26a,0xb24fe6b2,0xd4a699b5,0x3ffc0c54,0x5a1e53de,
0x39314dcf,0x06041eef,0xec96f3c7,0xeb7becca,0xef6ee947,0x014dfd58,0xfaa6fb71,0x069f012d,
0xf919022c,0xf16af4f3,0xee53eebd,0xa62edeb9,0xe810aad8,0x45641dae,0x54bc5219,0x310b45bf,
0x02a31848,0xeb1cf099,0xede3eba4,0xf46eeda2,0x0008febe,0xfa68f979,0x087f0204,0xf8d202a3,
0xf0f9f4a8,0xed01ee4d,0x9bc2d9e7,0xf249b54a,0x48c12910,0x506d5034,0x2bd240e2,0xfe551217,
0xea48ed79,0xef8feb5a,0xf7ddf094,0xfe93ff71,0xfc40f8fc,0x0b67050c,0xf95d0376,0xeef7f332,
0xebc0ec4c,0x9a7dda15,0xf4b1b3eb,0x4f102e45,0x516e520f,0x2ac740fa,0xfa290ec8,0xe54fe861,
0xed74e7a0,0xfb87f243,0x011003df,0xfd0ffafe,0x0aac05bd,0xf7960253,0xecfff0cd,0xed87ec6a,
0xa270e243,0xf243aab5,0x51f92aeb,0x534d5347,0x2c3841a9,0xf9370f3f,0xe169e593,0xeacde45d,
0xfcd5f217,0x022c05f1,0xfcfafc26,0x0ad80527,0xf8650277,0xed52f048,0xeee6ecc2,0xa7a6e5fe,
0xee1da3ac,0x53b4273e,0x55af56ab,0x2d2b4374,0xf8560f70,0xdefce431,0xe7b7e236,0xfb00ef53,
0x04ed0702,0xfdd9fd67,0x0c22064b,0xf8170349,0xed57efd4,0xefd3edc2,0xaccce79f,0xe90fa023,
0x520f22c2,0x58ee5992,0x2ddf45ad,0xf78c0fc2,0xde2de3a2,0xe5c9e1ee,0xfa01ecf6,0x065607fc,
0xfe01fd9a,0x0c630637,0xf7620301,0xee58ef81,0xf02bee99,0xaeaee6eb,0xe8c7a3c6,0x4ea42157,
0x5992591d,0x2e5646e6,0xf7780ff5,0xe0e2e585,0xe669e403,0xf678eb2d,0x04f904d1,0xfd61fccf,
0x0eaf0743,0xf7e404db,0xedf6efb1,0xedb0eda6,0xa8bede92,0xf01ab065,0x4bf62721,0x56bb5667,
0x2d444593,0xf67b0ef8,0xe2abe55e,0xe807e44a,0xf6daeb1b,0x04a703ed,0xfe54fd0e,0x0fac0834,
0xf6e3038d,0xedfdee8d,0xec6becb3,0xa1aad597,0xf9d2bf1f,0x4a722fcc,0x518652e8,0x299141b7,
0xf4fa0c2d,0xe4bae578,0xeab5e633,0xf717ece2,0x03100242,0x004cfdd4,0x10c00a76,0xf50c0374,
0xebc9ed39,0xe890ea98,0xa020cb69,0x0533cdb3,0x4c3d3a22,0x4c7d5056,0x257c3d52,0xf22508db,
0xe400e428,0xebfee63c,0xf98def9e,0x017902b7,0x0194fe53,0x109a0bcf,0xf32201fd,0xea3beba7,
0xe796ea39,0x9f46c565,0x0d3bd6fe,0x4f844208,0x496f507f,0x219439cc,0xef93063e,0xe1fee2ef,
0xea2de3ce,0xfb12f1a8,0xfefc01a9,0x036cfeaf,0x11790e2b,0xf3280344,0xe90aea51,0xe68fe9be,
0x9d75c149,0x11a9da95,0x5375480a,0x49f252f0,0x204f39a4,0xecdb04df,0xde0ae007,0xe6fbe0a2,
0xfdbdf226,0xff580389,0x0331fec7,0x111f0e65,0xf3fc0275,0xe966ea2a,0xe72fea12,0x9db9c4af,
0x1173d879,0x54f04751,0x4b9454b4,0x1fb3392b,0xec360445,0xdd86deaf,0xe57be046,0xfdbcf13f,
0xfc6901b3,0x0393fda6,0x12ec105b,0xf4f804ac,0xea75eb4a,0xe6b5eb09,0x9d5dc59c,0x0f2cd55a,
0x54d74498,0x4f5a576e,0x21963bb2,0xec6104eb,0xdd66dda4,0xe48edfef,0xfe2bf055,0xfd430367,
0x025ffcdd,0x12a40f84,0xf4950405,0xea37eafa,0xe71ceab9,0x9d49c55b,0x0fb9d52a,0x556d4536,
0x4e1c5741,0x203d3a69,0xec2b0427,0xdddcde66,0xe4b4e03e,0xfda5f062,0xfc78020f,0x037ffd73,
0x1326108a,0xf4ea04ac,0xea71eb39,0xe6b5eb05,0x9d54c56f,0x0000d552,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
