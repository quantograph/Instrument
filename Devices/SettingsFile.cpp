#include "Devices.h"

uint16_t SettingsFile::_count{0};

//=================================================================================================
SettingsFile::SettingsFile() {
}

//=================================================================================================
// Read all settings
bool SettingsFile::read(Settings& settings) {
    char* fileData;
    char* buffer;

    _readValues.clear();

    // Read the file data
    if(!_sdCard.readFile(_path.c_str(), fileData, _size))
        return false;

    // Make a string buffer
    buffer = (char*)malloc(_size + 1);
    memcpy(buffer, fileData, _size);
    free(fileData);
    buffer[_size] = 0;

    // Parse the "tag=value\n" pairs
    //Serial.printf("\n\nRead %d\n%s\n", _size, buffer);
    char* start = buffer;
    char* end;
    _count = 0;
    do {
        end = strchr(start, '\n');
        if(!end)
            break;

        *end = 0;
        //Serial.printf("Pair %3d: %s\n", _count++, start);
        getValue(settings, start);
        start = end + 1;

        if(end >= buffer + _size) {
            Serial.printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Processed all: %d (size=%d)", _size, end - buffer);
            break;
        }

    } while(end);

    //show("Reading");

    free(buffer);

    return true;
}

//=================================================================================================
// Writes all settings
bool SettingsFile::write(Settings& settings) {
    String string{};

    _writtenValues.clear();

    // Put all values into a string
    _count = 0;
    settings._audioSettings.putValues(string);
    settings._guiSettings.putValues(string);
    settings._guitarInput.putValues(string, in_guitarInputTag);
    settings._synthInput.putValues(string, in_synthInputTag);
    settings._synthSettings.putValues(string);

    // Save the string
    _sdCard.makeDir("/system");
    SD.remove(_path.c_str());
    if(!_sdCard.writeFile(_path.c_str(), string.c_str(), string.length()))
        return false;

    //show("Saving");
    //Serial.printf("\n\nWrite %d\n%s\n", string.length(), string.c_str());

    return true;
}

//=================================================================================================
void SettingsFile::putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, 
                       int16_t value) {
    char temp[64];
    sprintf(temp, "%04d_%04d-%04d=%d\n", parent, parent2, tag, value);
    //Serial.printf("Pair %3d: %s", _count++, temp);
    string += temp;
}

//=================================================================================================
void SettingsFile::putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, 
                       float value) {
    char temp[64];
    sprintf(temp, "%04d_%04d-%04d=%0.2f\n", parent, parent2, tag, value);
    //Serial.printf("Pair %3d: %s", _count++, temp);
    string += temp;
}

//=================================================================================================
void SettingsFile::putValue(String& string, uint16_t parent, uint16_t parent2, uint16_t tag, 
                       const char* value) {
    char temp[64];
    sprintf(temp, "%04d_%04d-%04d=%s\n", parent, parent2, tag, value);
    //Serial.printf("Pair %3d: %s", _count++, temp);
    string += temp;
}

//=================================================================================================
bool SettingsFile::getValue(Settings& settings, char* pair) {
    String pairCopy = pair;
    uint16_t parent;
    uint16_t parent2;
    uint16_t tag;
    const char* value;

    _readValues.push_back(pair);
    split(pair, parent, parent2, tag, value);
    //Serial.printf("Pair: %s -> %d_%d-%d=%s\n", pairCopy.c_str(), parent, parent2, tag, value);

    switch(parent) {
        case au_audioTag: // Audio
            switch(parent2) {
                case no_tag:
                    switch(tag) {
                        case au_inputTag: settings._audioSettings._input = (Inputs)atoi(value); break;
                        case au_micGainTag: settings._audioSettings._micGain = atof(value); break;
                        case au_lineInLevelTag: settings._audioSettings._lineInLevel = atof(value); break;
                        default: Serial.printf("##### ERROR: Unknown Audio tag: %s\n", pair); break;
                    }
                    break;
            }
            break;

        case gu_guiTag: // GUI
            switch(parent2) {
                case no_tag:
                    switch(tag) {
                        case gu_windowColorTag: settings._guiSettings._windowColor = (uint16_t)atoi(value); break;
                        case gu_borderColorTag: settings._guiSettings._borderColor = (uint16_t)atoi(value); break;
                        case gu_textColorTag: settings._guiSettings._textColor = (uint16_t)atoi(value); break;
                        case gu_textSizeTag: settings._guiSettings._textSize = (int)atoi(value); break;
                        default: Serial.printf("##### ERROR: Unknown GUI tag: %s\n", pair); break;
                    }
                    break;
            }
            break;

        case in_guitarInputTag: // Guitar input
            getInputSettings(settings._guitarInput, parent, parent2, tag, value);
            break;

        case in_synthInputTag: // Synth input
            getInputSettings(settings._synthInput, parent, parent2, tag, value);
            break;

        case sn_synthTag: // Synth settings
            getSynthSettings(settings._synthSettings, parent, parent2, tag, value);
            break;

        default:
            Serial.printf("##### ERROR: Unknown parent tag: %s\n", pair);
            break;
    }

    return true;
}

//=================================================================================================
bool SettingsFile::getInputSettings(InputSettings& settings, uint16_t parent, uint16_t parent2, 
                                    uint16_t tag, const char* value) {
    //Serial.printf("   getInputSettings: %d_%d-%d=%s\n", parent, parent2, tag, value);
    switch(parent2) {
        case no_tag:
            switch(tag) {
                case in_effectsTag: 
                    settings._effects = (uint16_t)atoi(value); 
                    break;

                default:
                    Serial.printf("##### ERROR: Unknown InputSettings tag: %d\n", tag);
                    break;
            }
            break;

        case in_effect1Tag: 
            getEffectsSettings(settings._effect1, parent, parent2, tag, value);
            break;

        case in_effect2Tag:
            getEffectsSettings(settings._effect2, parent, parent2, tag, value);
            break;

        default:
            Serial.printf("##### ERROR: Unknown InputSettings parent2 tag: %d\n", parent2);
            break;
    }

    return true;
}

//=================================================================================================
bool SettingsFile::getEffectsSettings(EffectSettings& settings, uint16_t parent, uint16_t parent2, 
                                      uint16_t tag, const char* value) {
    //Serial.printf("      getEffectsSettings: %d_%d-%d=%s\n", parent, parent2, tag, value);
    switch(tag) {
        case ef_effectTypeTag: settings._effectType = (EffectType)atoi(value); break;
        case ef_effectNameTag: settings._effectName = value; break;
        // Chorus
        case ch_delayTag: settings._chorus._delay = (int)atoi(value); break;
        case ch_voicesTag: settings._chorus._voices = (int)atoi(value); break;
        // Flange
        case fl_delayTag: settings._flange._delay = (int)atoi(value); break;
        case fl_offsetTag: settings._flange._offset = (int)atoi(value); break;
        case fl_depthTag: settings._flange._depth = (int)atoi(value); break;
        case fl_rateTag: settings._flange._rate = (float)atof(value); break;
        // Reverb
        case rv_timeTag: settings._reverb._time = (float)atof(value); break;
        // Freeverb
        case fr_sizeTag: settings._freeverb._roomSize = (float)atof(value); break;
        case fr_dampingTag: settings._freeverb._damping = (float)atof(value); break;
        // Envelope
        case en_delayTag: settings._envelope._delay = (float)atof(value); break;
        case en_attackTag: settings._envelope._attack = (float)atof(value); break;
        case en_holdTag: settings._envelope._hold = (float)atof(value); break;
        case en_decayTag: settings._envelope._decay = (float)atof(value); break;
        case en_sustainTag: settings._envelope._sustain = (float)atof(value); break;
        case en_releaseTag: settings._envelope._release = (float)atof(value); break;
        case en_releaseNoteOnTag: settings._envelope._releaseNoteOn = (float)atof(value); break;
        // Delay
        case dl_1Tag: settings._delay._delays[0] = (float)atof(value); break;
        case dl_2Tag: settings._delay._delays[1] = (float)atof(value); break;
        case dl_3Tag: settings._delay._delays[2] = (float)atof(value); break;
        case dl_4Tag: settings._delay._delays[3] = (float)atof(value); break;
        case dl_5Tag: settings._delay._delays[4] = (float)atof(value); break;
        case dl_6Tag: settings._delay._delays[5] = (float)atof(value); break;
        case dl_7Tag: settings._delay._delays[6] = (float)atof(value); break;
        case dl_8Tag: settings._delay._delays[7] = (float)atof(value); break;
        // Bitcrusher
        case bc_bitsTag: settings._bitcrusher._bits = (uint8_t)atoi(value); break;
        case bc_rateTag: settings._bitcrusher._rate = (float)atof(value); break;
        // Waveshaper
        // Granular
        case gr_ratioTag: settings._granular._ratio = (float)atof(value); break;
        case gr_freezeTag: settings._granular._freeze = (float)atof(value); break;
        case gr_shiftTag: settings._granular._shift = (float)atof(value); break;
        default: Serial.printf("##### ERROR: Unknown Effects tag: %d\n", tag); break;
    }

    return true;
}

//=================================================================================================
bool SettingsFile::getSynthSettings(SynthSettings& settings, uint16_t& parent, uint16_t& parent2,
                                    uint16_t& tag, const char*& value) {
    switch(tag) {
        case sn_instrumentTag: settings._instrument = (uint16_t)atoi(value); break;
        case sn_instrumentNameTag: settings._instrumentName = value; break;
        default: Serial.printf("##### ERROR: Unknown Synth tag: %d\n", tag); break;
    }

    return true;
}

//=================================================================================================
bool SettingsFile::split(char* pair, uint16_t& parent, uint16_t& parent2, uint16_t& tag, 
                    const char*& value) {
    char* delim;
    char* left;
    char* parents;
    char* number;

    // Split "parent-tag" and "value"
    delim = strchr(pair, '=');
    *delim = 0;
    left = pair;
    value = delim + 1;

    // Split "parents" and "tag"
    delim = strchr(left, '-');
    *delim = 0;
    parents = left;

    number = delim + 1;
    tag = (uint16_t)atoi(number);

    // Split the parents
    delim = strchr(parents, '_');
    *delim = 0;

    number = parents;
    parent = (uint16_t)atoi(number);

    number = delim + 1;
    parent2 = (uint16_t)atoi(number);

    return true;
}

//=================================================================================================
void SettingsFile::show(const char* title) {
    Serial.printf("\n\n===== %s settings\n", title);
    Serial.printf("Size=%d\n", _size);
    _settings.show(title);
    Serial.printf("\n\n");
}

//=================================================================================================
void AudioSettings::putValues(String& string) {
    SettingsFile::putValue(string, au_audioTag, no_tag, au_inputTag, (int16_t)_input);
    SettingsFile::putValue(string, au_audioTag, no_tag, au_micGainTag, _micGain);
    SettingsFile::putValue(string, au_audioTag, no_tag, au_lineInLevelTag, _lineInLevel);
}

//=================================================================================================
void GuiSettings::putValues(String& string) {
    SettingsFile::putValue(string, gu_guiTag, no_tag, gu_windowColorTag, (int16_t)_windowColor);
    SettingsFile::putValue(string, gu_guiTag, no_tag, gu_borderColorTag, (int16_t)_borderColor);
    SettingsFile::putValue(string, gu_guiTag, no_tag, gu_textColorTag, (int16_t)_textColor);
    SettingsFile::putValue(string, gu_guiTag, no_tag, gu_textSizeTag, (int16_t)_textSize);
}

//=================================================================================================
void InputSettings::putValues(String& string, uint16_t parent) {
    SettingsFile::putValue(string, parent, no_tag, in_effectsTag, (int16_t)_effects);
    _effect1.putValues(string, parent, in_effect1Tag);
    _effect2.putValues(string, parent, in_effect2Tag);
}

//=================================================================================================
void EffectSettings::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, ef_effectTypeTag, (int16_t)_effectType);
    SettingsFile::putValue(string, parent, parent2, ef_effectNameTag, _effectName.c_str());
    _chorus.putValues(string, parent, parent2);
    _flange.putValues(string, parent, parent2);
    _reverb.putValues(string, parent, parent2);
    _freeverb.putValues(string, parent, parent2);
    _envelope.putValues(string, parent, parent2);
    _delay.putValues(string, parent, parent2);
    _bitcrusher.putValues(string, parent, parent2);
    _waveshaper.putValues(string, parent, parent2);
    _granular.putValues(string, parent, parent2);
}

//=================================================================================================
void SynthSettings::putValues(String& string) {
    SettingsFile::putValue(string, sn_synthTag, no_tag, sn_instrumentTag, (int16_t)_instrument);
    SettingsFile::putValue(string, sn_synthTag, no_tag, sn_instrumentNameTag, _instrumentName.c_str());
}

//=================================================================================================
void Chorus::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, ch_delayTag, (int16_t)_delay);
    SettingsFile::putValue(string, parent, parent2, ch_voicesTag, (int16_t)_voices);
}

//=================================================================================================
void Flange::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, fl_delayTag, (int16_t)_delay);
    SettingsFile::putValue(string, parent, parent2, fl_offsetTag, (int16_t)_offset);
    SettingsFile::putValue(string, parent, parent2, fl_depthTag, (int16_t)_depth);
    SettingsFile::putValue(string, parent, parent2, fl_rateTag, _rate);
}

//=================================================================================================
void Reverb::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, rv_timeTag, _time);
}

//=================================================================================================
void Freeverb::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, fr_sizeTag, _roomSize);
    SettingsFile::putValue(string, parent, parent2, fr_dampingTag, _damping);
}

//=================================================================================================
void Envelope::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, en_delayTag, _delay);
    SettingsFile::putValue(string, parent, parent2, en_attackTag, _attack);
    SettingsFile::putValue(string, parent, parent2, en_holdTag, _hold);
    SettingsFile::putValue(string, parent, parent2, en_decayTag, _decay);
    SettingsFile::putValue(string, parent, parent2, en_sustainTag, _sustain);
    SettingsFile::putValue(string, parent, parent2, en_releaseTag, _release);
    SettingsFile::putValue(string, parent, parent2, en_releaseNoteOnTag, _releaseNoteOn);
}

//=================================================================================================
void Delay::putValues(String& string, uint16_t parent, uint16_t parent2) {
    int tag = (int)dl_1Tag;
    for(int i = 0; i < 8; ++i) {
        SettingsFile::putValue(string, parent, parent2, tag++, _delays[i]);
    }
}

//=================================================================================================
void Bitcrusher::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, bc_bitsTag, (int16_t)_bits);
    SettingsFile::putValue(string, parent, parent2, bc_rateTag, _rate);
}

//=================================================================================================
void Waveshaper::putValues(String& string, uint16_t parent, uint16_t parent2) {
}

//=================================================================================================
void Granular::putValues(String& string, uint16_t parent, uint16_t parent2) {
    SettingsFile::putValue(string, parent, parent2, gr_ratioTag, _ratio);
    SettingsFile::putValue(string, parent, parent2, gr_freezeTag, _freeze);
    SettingsFile::putValue(string, parent, parent2, gr_shiftTag, _shift);
}

//=================================================================================================
bool SettingsFile::test() {
    Settings settings1;
    Settings settings2;

    //Serial.printf(">>> Random\n");
    randomSeed(analogRead(0));
    settings1.random();
    settings2.random();

    Serial.printf("\n\n\n>>> Write\n");
    write(settings1);
    settings1.show("");
    Serial.printf("\n\n\n>>> Read\n");
    read(settings2);
    settings2.show("");

    Serial.printf("\n\n\n>>> Compare\n");
    if(settings1.compare(settings2)) {
        Serial.printf("Settings match\n");
        return true;
    } else {
        Serial.printf("##### ERROR: Settings DON'T match\n");
        return false;
    }

    return true;
}
