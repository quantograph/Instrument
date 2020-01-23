/*
const int Keys::_cNumber = 60;

//=================================================================================================
Keys::Keys() {

}

//=================================================================================================
bool Keys::Init() {
    _sopranoSaxInfo._instrument = INSTRUMENT::SOPRANO_SAX;
    _sopranoSaxInfo._name = "Soprano saxophone";
    //_sopranoSaxInfo._midiInstrument = MIDI_SOPRANO_SAXOPHONE;
    _sopranoSaxInfo._startNote = Keys::_cNumber + 9;
    _sopranoSaxInfo._fingeringCount = SAX_FINGERINGS;
    _sopranoSaxInfo._fingerings = _saxFingerings;
    _sopranoSaxInfo._buttonCount = SAX_BUTTONS;
    _sopranoSaxInfo._buttonMap = (int*)&_saxButtonMap;
    _sopranoSaxInfo._keyMap = (int*)&_saxKeyMap;
    //_sopranoSaxInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_sopranoSaxInfo))
        return false;

    _altoSaxInfo._instrument = INSTRUMENT::ALTO_SAX;
    _altoSaxInfo._name = "Alto saxophone";
    //_altoSaxInfo._midiInstrument = MIDI_ALTO_SAXOPHONE;
    _altoSaxInfo._startNote = _cNumber + 9;
    _altoSaxInfo._fingeringCount = SAX_FINGERINGS;
    _altoSaxInfo._fingerings = _saxFingerings;
    _altoSaxInfo._buttonCount = SAX_BUTTONS;
    _altoSaxInfo._buttonMap = (int*)&_saxButtonMap;
    _altoSaxInfo._keyMap = (int*)&_saxKeyMap;
    //_altoSaxInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_altoSaxInfo))
        return false;

    _tenorSaxInfo._instrument = INSTRUMENT::TENOR_SAX;
    _tenorSaxInfo._name = "Tenor saxophone";
    //_tenorSaxInfo._midiInstrument = MIDI_TENOR_SAXOPHONE;
    _tenorSaxInfo._startNote = _cNumber - 16;
    _tenorSaxInfo._fingeringCount = SAX_FINGERINGS;
    _tenorSaxInfo._fingerings = _saxFingerings;
    _tenorSaxInfo._buttonCount = SAX_BUTTONS;
    _tenorSaxInfo._buttonMap = (int*)&_saxButtonMap;
    _tenorSaxInfo._keyMap = (int*)&_saxKeyMap;
    //_tenorSaxInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_tenorSaxInfo))
        return false;

    _baritoneSaxInfo._instrument = INSTRUMENT::BARITONE_SAX;
    _baritoneSaxInfo._name = "Baritone saxophone";
    //_baritoneSaxInfo._midiInstrument = MIDI_BARITONE_SAXOPHONE;
    _baritoneSaxInfo._startNote = _cNumber - 16;
    _baritoneSaxInfo._fingeringCount = SAX_FINGERINGS;
    _baritoneSaxInfo._fingerings = _saxFingerings;
    _baritoneSaxInfo._buttonCount = SAX_BUTTONS;
    _baritoneSaxInfo._buttonMap = (int*)&_saxButtonMap;
    _baritoneSaxInfo._keyMap = (int*)&_saxKeyMap;
    //_baritoneSaxInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_baritoneSaxInfo))
        return false;

    _fluteInfo._instrument = INSTRUMENT::FLUTE;
    _fluteInfo._name = "Flute";
    //_fluteInfo._midiInstrument = MIDI_FLUTE;
    _fluteInfo._startNote = 62;
    _fluteInfo._fingeringCount = FLUTE_FINGERINGS;
    _fluteInfo._fingerings = _fluteFingerings;
    _fluteInfo._buttonCount = FLUTE_BUTTONS;
    _fluteInfo._buttonMap = (int*)&_fluteButtonMap;
    _fluteInfo._keyMap = (int*)&_fluteKeyMap;
    //_fluteInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_fluteInfo))
        return false;

    _trumpetInfo._instrument = INSTRUMENT::TRUMPET;
    _trumpetInfo._name = "Trumpet";
    //_trumpetInfo._midiInstrument = MIDI_TRUMPET;
    _trumpetInfo._startNote = 52;
    _trumpetInfo._fingeringCount = TRUMPET_FINGERINGS;
    _trumpetInfo._fingerings = _trumpetFingerings;
    _trumpetInfo._buttonCount = TRUMPET_BUTTONS;
    _trumpetInfo._buttonMap = (int*)&_trumpetButtonMap;
    _trumpetInfo._keyMap = (int*)&_trumpetKeyMap;
    //_trumpetInfo._channel = Midi::_melodyChannel;
    if(!MapButtons(&_trumpetInfo))
        return false;

    _drumsInfo._instrument = INSTRUMENT::PERCUSSION;
    _drumsInfo._name = "Drums";
    //_drumsInfo._midiInstrument = MIDI_DRUMS;
    _drumsInfo._startNote = 0;
    _drumsInfo._buttonCount = DRUMS_BUTTONS;
    _drumsInfo._buttonMap = (int*)&_drumsButtonMap;
    _drumsInfo._noteMap = (uint8_t*)&_drumsNoteMap;
    //_drumsInfo._channel = Midi::_drumChannel;

    _steelDrumInfo._instrument = INSTRUMENT_STEEL_DRUM;
    _steelDrumInfo._name = "Steel drum";
    //_steelDrumInfo._midiInstrument = MIDI_STEEL_DRUM;
    _steelDrumInfo._startNote = 0;
    _steelDrumInfo._buttonCount = STEEL_DRUM_BUTTONS;
    _steelDrumInfo._buttonMap = (int*)&_steelDrumButtonMap;
    _steelDrumInfo._noteMap = (uint8_t*)&_steelDrumNoteMap;
    //_steelDrumInfo._channel = Midi::_melodyChannel;

    SetInstrument();

    return true;
}

//=================================================================================================
void Keys::SetInstrument() {
    switch(g_settings._data._midiInstrument) {
        case MIDI_SOPRANO_SAXOPHONE:
            _currentInstrument = &_sopranoSaxInfo;
            ShowIntrument("Soprano saxophone");
            break;

        case MIDI_ALTO_SAXOPHONE:
            _currentInstrument = &_altoSaxInfo;
            ShowIntrument("Alto saxophone");
            break;

        case MIDI_TENOR_SAXOPHONE:
            _currentInstrument = &_tenorSaxInfo;
            ShowIntrument("Tenor saxophone");
            break;

        case MIDI_BARITONE_SAXOPHONE:
            _currentInstrument = &_baritoneSaxInfo;
            ShowIntrument("Baritone saxophone");
            break;

        case MIDI_FLUTE:
            _currentInstrument = &_fluteInfo;
            ShowIntrument("Flute");
            break;

        case MIDI_TRUMPET:
            _currentInstrument = &_trumpetInfo;
            ShowIntrument("Trumpet");
            break;

        case MIDI_DRUMS:
            _currentInstrument = &_drumsInfo;
            ShowIntrument("Drums");
            break;

        case MIDI_STEEL_DRUM:
            _currentInstrument = &_steelDrumInfo;
            ShowIntrument("Steel drums");
            break;
    }

    _fingers.Init(_currentInstrument, 3, 80);
    _staff.Init(45, 80);
    //Serial.printf("Keys::SetInstrument: %s\n", _currentInstrument->_name.c_str());
}

//=================================================================================================
void Keys::ShowIntrument(const char* name) {
    int x = 5;
    int y = 5;

    g_screen._screen.setCursor(x, y);
    g_screen._screen.setTextColor(ILI9341_CYAN);
    g_screen._screen.setTextSize(2);
    g_screen._screen.println(name);
}

//=================================================================================================
bool Keys::MapButtons(InstrumentInfo* info) {
    int bitToSet;
    char copy[64];
    char* key;

    //Serial.printf("\n%s map buttons --------\n", info->_name.c_str());
    for(int f = 0; f < info->_fingeringCount; ++f) { // All fingerings
        Fingering* data = &(info->_fingerings[f]);
  
        strcpy(copy, data->_buttonString.c_str());
        key = strtok(copy, ",");
        while(key) {
            //Serial.printf("Buttons %d: ", f);
            bitSet(data->_keys, atoi(key) - 1);
            key = strtok(NULL, ",");
        }

        //Serial.printf("Buttons %d: ", f);
        data->_mappedKeys = 0;
        data->_mapped = true;
        data->_note = f + 1;
        for(int bit = 0; bit < 32; ++bit) { 
            if(bitRead(data->_keys, bit) == 0) // Map only the bits set to 1
                continue;

            bool mapped = false;
            for(int button = 0; button < info->_buttonCount; ++button) {
                if(info->_keyMap[button] == bit + 1) {
                    bitToSet = info->_buttonMap[button] - 1;
                    bitSet(data->_mappedKeys, bitToSet);
                    //Serial.printf("%d->%d  ", info->_keyMap[button], info->_buttonMap[button]);
                    mapped = true;
                    break;
                }
            }

            // If there is a non-mapped button, don't use this key combination
            if(!mapped) {
                data->_mapped = false;
                break;
            }
        }
        //Serial.printf(". ");

        // Show the keys for this fingering
        String k = " ";
        String m = " ";
        //ShowBits(data->_keys, k);
        //ShowBits(data->_mappedKeys, m);
        //Serial.printf("Note %2d: %s -> %s\n", data->_note, k.c_str(), m.c_str());
    }

    // Make a bitmask for blocking the unwanted buttons
    info->_keyMask = 0;
    for(int button = 0; button < info->_buttonCount; ++button) {
        bitSet(info->_keyMask, info->_buttonMap[button] - 1);
    }

    if(!CheckButtons(info))
        return false;

    return true;
}

//=================================================================================================
bool Keys::CheckButtons(InstrumentInfo* info) {
    int note = NO_NOTE;

    // Check for duplicate key strings
    for(int f1 = 0; f1 < info->_fingeringCount; ++f1) {
        const Fingering* data1 = &(info->_fingerings[f1]);
        for(int f2 = f1; f2 < info->_fingeringCount; ++f2) {
            const Fingering* data2 = &(info->_fingerings[f2]);
            if(f1 == f2) // Don't compare with itself
                continue;

            if(strcmp(data1->_buttonString.c_str(), data2->_buttonString.c_str()) == 0) {
                Serial.printf("########## ERROR: duplicate key strings '%s'\n", 
                    data1->_buttonString.c_str());
            }
        }
    }

    //Serial.printf("\n%s check buttons ----------\n", info->_name.c_str());
    for(int f = 0; f < info->_fingeringCount; ++f) {
        const Fingering* data = &(info->_fingerings[f]);

        if(!data->_mapped) {
            Serial.printf("########## ERROR: key string '%s' is not mapped\n", 
                data->_buttonString.c_str());
            return false;
        }

        // Show the key bitmask taken from the key string
        String b;
        //ShowBits(data->_keys, b);
        //Serial.printf("Key string='%s', bits='%s'\n", data->_buttonString.c_str(), b.c_str());
        if(strcmp(data->_buttonString.c_str(), b.c_str())) {
            Serial.printf("########## ERROR: key string '%s' doesn't match bits '%s'\n", 
                data->_buttonString.c_str(), b.c_str());
            return false;
        }

        // Keys numbers must be unique and incrementing
        if(note == NO_NOTE) {
            note = data->_note;
        } else {
            if(data->_note != ++note) {
                Serial.printf("########## ERROR: expected %s note %d, got %d\n", 
                    info->_name.c_str(), note, data->_note);
                return false;
            }
        }

        String k = " ";
        String m = " ";
        //ShowBits(data->_keys, k);
        //ShowBits(data->_mappedKeys, m);
        //Serial.printf("Note %2d (%08X): %s ==> %s\n", 
        //    data->_note, data->_keys, k.c_str(), m.c_str());
    }

    String m = " ";
    //ShowBits(info->_keyMask, m);
    //Serial.printf("%s key mask: %s\n", info->_name.c_str(), m.c_str());

    return true;
}

//=================================================================================================
void Keys::Play(uint32_t touched) {
    if(_currentInstrument->_midiInstrument == MIDI_DRUMS ||
       _currentInstrument->_midiInstrument == MIDI_STEEL_DRUM) {
        PlayDrums(touched);
    } else
        PlaySaxophone(touched);
}

//=================================================================================================
void Keys::PlayDrums(uint32_t touched) {
    uint8_t note = NO_NOTE;
    bool play = false;
    bool on = false;

    for(int bit = 0; bit < 32; ++bit) {
        play = false;
        on = false;

        if(bitRead(touched, bit) == 1 && bitRead(_lastTouched, bit) == 0) { // Note on
            play = true;
            on = true;
        } else if(bitRead(touched, bit) == 0 && bitRead(_lastTouched, bit) == 1) { // Note off
            play = true;
            on = false;
        }

        if(play) {
            for(int n = 0; n < _currentInstrument->_buttonCount; ++n) {
                //Serial.printf("Button %d -> note %d\n", _currentInstrument->_buttonMap[n], _currentInstrument->_noteMap[n]);
                if(_currentInstrument->_buttonMap[n] == bit + 1) {
                    note = _currentInstrument->_noteMap[n];
                    if(on) {
                        g_audio.NoteOn(AudioBoard::Instrument::drums, note, g_settings._data._volume);
                        ShowNote(note, true);
                        //sprintf(g_string, "Drum note: %d", note);
                        //LogScreen(g_string);
                    } else {
                        g_audio.NoteOff(AudioBoard::Instrument::drums, note);
                        ShowNote(note, false);
                    }

                    break;
                }
            }
        }
    }


    _lastTouched = touched;
}

//=================================================================================================
// Check the touch buttons and play a note, if found
void Keys::PlaySaxophone(uint32_t touched) {
    Fingering* data = NULL;
    uint32_t buttons = 0;
    uint32_t keys = 0;
    int note = NO_NOTE;
    bool play = false;

    // Find the fingering for the button combination
    buttons = touched & _currentInstrument->_keyMask;
    for(int f = 0; f < _currentInstrument->_fingeringCount; ++f) { // Fingerings
        data = &(_currentInstrument->_fingerings[f]);
        if(data->_mapped && buttons == data->_mappedKeys) { // Skip combinations with non-mapped keys
            note = _currentInstrument->_startNote + data->_note - 1;
            keys = data->_keys;
            break;
        }
    }

    // Buttons 13 and 14 are used a 'blow sensor'
    play = bitRead(touched, 12) || bitRead(touched, 13);

    // Stop the last note played
    if(_prevNoteNumber != NO_NOTE) {
        ShowNote(_prevNoteNumber, false);
        //g_audio.NoteOff(AudioBoard::Instrument::drums, _prevNoteNumber);
    }

    // Play the new note
    if(note != NO_NOTE && play) {
        //Serial.printf("Play note");
        //g_audio.NoteOn(AudioBoard::Instrument::drums, note, g_settings._data._volume);
        _prevNoteNumber = note;
        ShowNote(note, true);
    } else {
        //Serial.printf("Stop note");
        _prevNoteNumber = NO_NOTE;
    }

    if(_currentInstrument == &_trumpetInfo)
        ShowTrumpet(g_screen._width / 3, g_screen._height / 2 + 10, keys, note == NO_NOTE);
    else
        _fingers.ShowSaxophone(keys, note == NO_NOTE);

    // Debugging the key combinations
    String b = " ";
    String t = " ";
    String k = " ";
    //ShowBits(buttons, b);
    //ShowBits(touched, t);
    //ShowBits(keys, k);
    if(play && note != NO_NOTE) {
        Serial.printf("Saxophone note %d (%d): keys=%s buttons=%s touched=%s\n", note, data->_note, k.c_str(), b.c_str(), t.c_str());
    } else {
        Serial.printf("Saxophone stop: keys=%s buttons=%s touched=%s\n", k.c_str(), b.c_str(), t.c_str());
    }
}

////=================================================================================================
//// Sends a MIDI note to the synthesizer and computer
//bool Note(byte channel, byte note, byte velocity) {
//    size_t sent; // Bytes sent
//
//    // Send the note to the computer
//    ^^^_noteData._note = note;
//    _noteData._channel = channel;
//    _noteData._velocity = velocity;
//    _noteData._sequence = _sequence++;
//    sent = Serial.write((const uint8_t*)&_noteData, _noteDataSize);
//    if(sent != _noteDataSize) {
//        sprintf(_string, "Returned %d", sent);
//        ShowError("Serial write", _string);
//    }
//
//    // Send to the MIDI chip
//    SendMidi((0x90 | channel), note, velocity);
//
//    return true;
//}

//=================================================================================================
void Keys::ShowNote(int noteNumber, bool show) {
    const char* noteName = NULL; // Curent note's name
    int x = 70;
    int y = 55;
    //g_screen.Clear();

    // Show the current note
    if(noteNumber <= 84)
        _staff.DrawNote(noteNumber, show);

    // Show note's name
    g_screen._screen.setCursor(x, y);
    g_screen._screen.setTextColor(ILI9341_WHITE);
    g_screen._screen.setTextSize(3);
    if(show && noteNumber != NO_NOTE) {
        noteName = _noteNames[noteNumber % 12];
        sprintf(_string, "%5s %d   ", noteName, noteNumber);
        g_screen._screen.fillRect(x, y, 145, 25, ILI9341_BLACK);
        g_screen._screen.println(_string);
        //Serial.printf(">>> %s\n", _string);
    } else {
        //Serial.printf("<<<\n");
    }
}

//=================================================================================================
// Shows trumpet keys pressed
void Keys::ShowTrumpet(int x, int y, uint32_t keys, bool error) {
    int keyStep = 17;
    int keyX = x;
    int keyY = y;
    int radius = keyStep / 2 - 1;
    uint16_t pressedColor = ILI9341_YELLOW;
    uint16_t freeColor = ILI9341_BLACK;
    uint16_t errorColor = ILI9341_RED;
    
    if(error)
        return;

    String k = " ";
    //ShowBits(keys, k);
    //Serial.printf("\n ShowTrumpet ==> (%0x) %s\n", keys, k.c_str());

    // Trumpet keys
    keyX = x + 40;
    for(int i = 0; i < 3; ++i) {
        keyY += keyStep;

        if(bitRead(keys, _currentInstrument->_keyMap[i] - 1)) {
            g_screen._screen.fillCircle(keyX, keyY, radius, error ? errorColor : pressedColor);
        } else {
            g_screen._screen.fillCircle(keyX, keyY, radius, freeColor);
        }

        g_screen._screen.drawCircle(keyX, keyY, radius, error ? errorColor : pressedColor);
    }

    // 'Lip' keys
    keyY = y;
    for(int i = 3; i < 8; ++i) {
        if(i == 3)
            keyX = x + keyStep / 2;
        else
            keyX = x;
        
        keyY += keyStep;

        if(bitRead(keys, _currentInstrument->_keyMap[i] - 1)) {
            g_screen._screen.fillCircle(keyX, keyY, radius, error ? errorColor : pressedColor);
        } else {
            g_screen._screen.fillCircle(keyX, keyY, radius, freeColor);
        }

        g_screen._screen.drawCircle(keyX, keyY, radius, error ? errorColor : pressedColor);
    }
}
*/