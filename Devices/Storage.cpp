#include "Devices.h"

// EEPROM API documentation: https://www.arduino.cc/en/Reference/EEPROM

//=================================================================================================
Storage::Storage() {
}

//=================================================================================================
// Reads all settings
bool Storage::readBuffer(int address, uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        buffer[i] = EEPROM.read(address + i);
    }

    return true;
}
    
//=================================================================================================
// Writes all settings
bool Storage::writeBuffer(int address, const uint8_t* buffer, int size) {
    for(int i = 0; i < size; ++i) {
        EEPROM.write(address + i, buffer[i]);
    }

    return true;
}

//=================================================================================================
// Read all settings
bool Storage::read() {
    char marker[6];
    int offset = 0;
    char* buffer;

    // See if the marker is written
    readBuffer(offset, (uint8_t*)marker, 5);
    marker[5] = 0;
    if(strncmp(_marker, marker, 5)) {
        Serial.println("##### Storage start marker not found");
        return false;
    }
    
    // Read the data size
    _size = 0;
    offset += 5;
    readBuffer(offset, (uint8_t*)&_size, sizeof(_size)); // Read the buffer size first

    // Read the data
    offset += 2;
    buffer = (char*)malloc(_size + 1);
    readBuffer(offset, (uint8_t*)buffer, _size); // Read the buffer with settings now
    buffer[_size] = 0;

    // Parse the "tag=value\n" pairs
    //Serial.printf("\n(%d) %s\n", _size, (const char*)buffer);
    offset = 0;
    char* start = buffer;
    char* end;
    do {
        end = strchr(start, '\n');
        if(!end)
            break;

        *end = 0;
        getValue(start);
        start = end + 1;
    } while(end);

    show("Reading");

    free(buffer);

    return true;
}

//=================================================================================================
// Writes all settings
bool Storage::write() {
    int offset = 0;
    String string;

    writeBuffer(0, (const uint8_t*)_marker, 5); // Write the marker

    // Put all values
    _settings._audioSettings.putValues(string);
    _settings._guiSettings.putValues(string);
    _settings._guitarInput.putValues(string, in_guitarInputTag);
    _settings._synthInput.putValues(string, in_synthInputTag);
    _settings._synthSettings.putValues(string);

    string += endTag;

    // Write the data size
    offset += 5;
    _size = string.length();
    writeBuffer(offset, (const uint8_t*)&_size, sizeof(_size)); // Write the buffer size first

    // Write the data
    offset += 2;
    writeBuffer(offset, (const uint8_t*)string.c_str(), _size);

    show("Saving");
    Serial.printf("\n(%d) %s\n", _size, string.c_str());

    return true;
}

//=================================================================================================
void Storage::putValue(String& string, const char* parent, const char* parent2, const char* tag, 
                       int16_t value) {
    char temp[64];
    sprintf(temp, "%s_%s-%s=%d\n", parent, parent2, tag, value);
    string += temp;
}

//=================================================================================================
void Storage::putValue(String& string, const char* parent, const char* parent2, const char* tag, 
                       float value) {
    char temp[64];
    sprintf(temp, "%s_%s-%s=%0.3f\n", parent, parent2, tag, value);
    string += temp;
}

//=================================================================================================
void Storage::putValue(String& string, const char* parent, const char* parent2, const char* tag, 
                       const char* value) {
    char temp[64];
    sprintf(temp, "%s_%s-%s=%s\n", parent, parent2, tag, value);
    string += temp;
}

//=================================================================================================
bool Storage::getValue(char* pair) {
    String pairCopy = pair;
    const char* parent;
    const char* parent2;
    const char* tag;
    const char* value;

    split(pair, parent, parent2, tag, value);
    Serial.printf("Pair: %s -> %s_%s-%s=%s\n", pairCopy.c_str(), parent, parent2, tag, value);

    if(!strcmp(parent, au_audioTag)) { // Audio
        if(!strcmp(tag, au_inputTag))
            _settings._audioSettings._input = (Inputs)atoi(value);
        else if(!strcmp(tag, au_micGainTag))
            _settings._audioSettings._micGain = atof(value);
        else if(!strcmp(tag, au_lineInLevelTag))
            _settings._audioSettings._lineInLevel = atof(value);
        else {
            Serial.printf("##### ERROR: Unknown Audio tag: %s\n", pair);
        }
    } else if(!strcmp(parent, gu_guiTag)) { // GUI
        if(!strcmp(tag, gu_windowColorTag))
            _settings._guiSettings._windowColor = (uint16_t)atoi(value);
        else if(!strcmp(tag, gu_borderColorTag))
            _settings._guiSettings._borderColor = (uint16_t)atoi(value);
        else if(!strcmp(tag, gu_textColorTag))
            _settings._guiSettings._textColor = (uint16_t)atoi(value);
        else if(!strcmp(tag, gu_textSizeTag))
            _settings._guiSettings._textSize = (int)atoi(value);
        else {
            Serial.printf("##### ERROR: Unknown GUI tag: %s\n", pair);
        }
    } else if(!strcmp(parent, in_guitarInputTag)) { // Guitar input
    } else if(!strcmp(parent, in_synthInputTag)) { // Synth input
    } else if(!strcmp(parent, sn_synthTag)) { // Synth settings
    } else {
        Serial.printf("##### ERROR (Storage::getValue): Unknown parent tag: %s\n", parent);
    }

    return true;
}

//=================================================================================================
bool Storage::split(char* pair, const char*& parent, const char*& parent2, const char*& tag, 
                    const char*& value) {
    char* delim;
    char* left;
    char* parents;

    // Split "parent-tag" and "value"
    delim = strchr(pair, '=');
    *delim = 0;
    left = pair;
    value = delim + 1;

    // Split "parents" and "tag"
    delim = strchr(left, '-');
    *delim = 0;
    parents = left;
    tag = delim + 1;

    // Split the parents
    delim = strchr(parents, '_');
    *delim = 0;
    parent = parents;
    parent2 = delim + 1;

    return true;
}

//=================================================================================================
void Storage::show(const char* title) {
    Serial.printf("===== %s settings\n", title);
    Serial.printf("Marker=%s\n", _marker);
    Serial.printf("Size=%d\n", _size);
    _settings.show();
}

//=================================================================================================
void Chorus::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, ch_delayTag, (int16_t)_delay);
    Storage::putValue(string, parent, parent2, ch_voicesTag, (int16_t)_voices);
}

//=================================================================================================
void Flange::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, fl_delayTag, (int16_t)_delay);
    Storage::putValue(string, parent, parent2, fl_offsetTag, (int16_t)_offset);
    Storage::putValue(string, parent, parent2, fl_depthTag, (int16_t)_depth);
    Storage::putValue(string, parent, parent2, fl_rateTag, _rate);
}

//=================================================================================================
void Reverb::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, rv_timeTag, _time);
}

//=================================================================================================
void Freeverb::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, fr_sizeTag, _roomSize);
    Storage::putValue(string, parent, parent2, fr_dampingTag, _damping);
}

//=================================================================================================
void Envelope::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, en_delayTag, _delay);
    Storage::putValue(string, parent, parent2, en_attackTag, _attack);
    Storage::putValue(string, parent, parent2, en_holdTag, _hold);
    Storage::putValue(string, parent, parent2, en_decayTag, _decay);
    Storage::putValue(string, parent, parent2, en_sustainTag, _sustain);
    Storage::putValue(string, parent, parent2, en_releaseTag, _release);
    Storage::putValue(string, parent, parent2, en_releaseNoteOnTag, _releaseNoteOn);
}

//=================================================================================================
void Delay::putValues(String& string, const char* parent, const char* parent2) {
    char tag[32];
    for(int i = 0; i < 8; ++i) {
        sprintf(tag, "dl%d", i);
        Storage::putValue(string, parent, parent2, tag, _delays[i]);
    }
}

//=================================================================================================
void Bitcrusher::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, bc_bitsTag, (int16_t)_bits);
    Storage::putValue(string, parent, parent2, bc_rateTag, _rate);
}

//=================================================================================================
void Waveshaper::putValues(String& string, const char* parent, const char* parent2) {
}

//=================================================================================================
void Granular::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, gr_ratioTag, _ratio);
    Storage::putValue(string, parent, parent2, gr_freezeTag, _freeze);
    Storage::putValue(string, parent, parent2, gr_shiftTag, _shift);
}

//=================================================================================================
void EffectSettings::putValues(String& string, const char* parent, const char* parent2) {
    Storage::putValue(string, parent, parent2, ef_effectTypeTag, (int16_t)_effectType);
    Storage::putValue(string, parent, parent2, ef_effectNameTag, _effectName.c_str());
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
void InputSettings::putValues(String& string, const char* parent) {
    Storage::putValue(string, parent, "", in_effectsTag, (int16_t)_effects);
    _effect1.putValues(string, parent, in_effect1Tag);
    _effect2.putValues(string, parent, in_effect2Tag);
}

//=================================================================================================
void AudioSettings::putValues(String& string) {
    Storage::putValue(string, au_audioTag, "", au_inputTag, (int16_t)_input);
    Storage::putValue(string, au_audioTag, "", au_micGainTag, _micGain);
    Storage::putValue(string, au_audioTag, "", au_lineInLevelTag, _lineInLevel);
}

//=================================================================================================
void GuiSettings::putValues(String& string) {
    Storage::putValue(string, gu_guiTag, "", gu_windowColorTag, (int16_t)_windowColor);
    Storage::putValue(string, gu_guiTag, "", gu_borderColorTag, (int16_t)_borderColor);
    Storage::putValue(string, gu_guiTag, "", gu_textColorTag, (int16_t)_textColor);
    Storage::putValue(string, gu_guiTag, "", gu_textSizeTag, (int16_t)_textSize);
}

void SynthSettings::putValues(String& string) {
    Storage::putValue(string, sn_synthTag, "", sn_instrumentTag, (int16_t)_instrument);
    Storage::putValue(string, sn_synthTag, "", sn_instrumentNameTag, _instrumentName.c_str());
}
