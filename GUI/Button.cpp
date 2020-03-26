#include <vector>
#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Window.h"
#include "Control.h"
#include "Button.h"

//=================================================================================================
Button::Button(Settings::Data* settings, TouchScreen* screen, Window* parent, 
               uint16_t x, uint16_t y, uint16_t width, uint16_t height, ButtonId id) : 
    Control(settings, screen, parent, x, y, width, height), _id(id) {
}

//=================================================================================================
void Button::init() {
    switch(_id) {
        case guitar:
            _text = "Gtr";
            break;

        case synth:
            _text = "Synt";
            break;

        case band:
            _text = "Drum";
            break;

        case settings:
            _text = "Setp";
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", _id);
            break;
    }

    // Draw the button's frame
	_screen->_screen.fillRoundRect(_x, _y, _width, _height, _radius, _backColor);
	_screen->_screen.drawRoundRect(_x, _y, _width, _height, _radius, _borderColor);

    // Draw the text
    _screen->_screen.setCursor(_x + 5, _y + 20);
    _screen->_screen.setTextColor(_textColor);
    _screen->_screen.setTextSize(_textSize);
    _screen->_screen.print(_text);
}

//=================================================================================================
void Button::onTouch(TS_Point point) {
    if(!inside(point))
        return;

    //Serial.printf("Button::onTouch: %s\n", _text.c_str());
    _parent->onButton(this);
}

//=================================================================================================
void Button::onRelease(TS_Point fromPoint, TS_Point toPoint) {
}
