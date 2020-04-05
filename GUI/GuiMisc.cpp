#include "../Devices/Devices.h"
#include "GuiMisc.h"

String getEffectName(EffectType type) {
    switch(type) {
        case eff_clean: return "Clean";
        case eff_chorus: return "Chorus";
        case eff_flange: return "Flange";
        case eff_reverb: return "Reverb";
        case eff_freeverb: return "Freeverb";
        case eff_envelope: return "Envelope";
        case eff_delay: return "Delay";
        case eff_bitcrusher: return "Bitcrusher";
        case eff_waveshaper: return "Waveshaper";
        case eff_granular: return "Granular";
        default: return "Unknown";
    }
}

