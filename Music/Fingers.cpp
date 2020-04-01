#include "../Devices/Devices.h"
#include "Misc.h"
#include "Fingers.h"

//=================================================================================================
Fingers::Fingers() {

}

//=================================================================================================
bool Fingers::init(InstrumentInfo* instrument, int x, int y) {
    _instrument = instrument;
    _x = x;
    _y = y;

    switch(instrument->_instrument) {
        case INSTRUMENT::SOPRANO_SAX:
        case INSTRUMENT::ALTO_SAX:
        case INSTRUMENT::TENOR_SAX:
        case INSTRUMENT::BARITONE_SAX:
        case INSTRUMENT::FLUTE:
            initSaxophone();
            break;

        case INSTRUMENT::TRUMPET:
            break;

        case INSTRUMENT::NONE:
            break;

        /*case INSTRUMENT::STEEL_DRUM:
            break;*/

        default:
            return false;
    }

    return true;
}

//=================================================================================================
void Fingers::initSaxophone() {
    _keyStep = 30;
    _shift = _keyStep * 0.7;
    _radius = _keyStep / 2 - 2;
    _smallRadius = _radius * 0.85;
    _keyCount = 9;
    int x0 = _x + _keyStep * 1.2;
    int y0 = _y + _radius * 1.3;
    int x = x0;
    int y = y0;
    
    for(int i = 0; i < _keyCount; ++i) {
        Key* key = &_keys[i];
        if(i == 0) {
            x = x0 - _shift;
        } else if(i == 1) {
            x = x0;
            y += _shift;
        } else if(i == 4) {
            x = x0 + _shift;
            y += _shift;
        } else if(i == 5) {
            x = x0;
            y += _shift;
        } else if(i == 8) {
            x = x0 - _shift;
            y += _shift;
        } else if(i == 9) {
            x = x0;
            y += _shift;
        } else {
            x = x0;
            y += _keyStep;
        }

        key->_x = x;
        key->_y = y;
        //g_screen._screen.drawCircle(key->_x, key->_y, _radius, _backColor);
    }

    showSaxophone(0, false);
}

//=================================================================================================
// Shows saxophone keys pressed
void Fingers::showSaxophone(uint32_t keys, bool error) {
    /*for(int i = 0; i < _keyCount; ++i) {
        Key* key = &_keys[i];
        if(bitRead(keys, _instrument->_keyMap[i] - 1)) {
            g_screen._screen.fillCircle(key->_x, key->_y, _smallRadius, error ? _errorColor : _pressedColor);
        } else {
            g_screen._screen.fillCircle(key->_x, key->_y, _smallRadius, _freeColor);
        }
    }*/
}
