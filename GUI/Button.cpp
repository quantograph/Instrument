#include <vector>
#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Window.h"
#include "Control.h"
#include "Button.h"

//=================================================================================================
Button::Button(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id) : 
    Control(settings, parent, x, y, width, height, id) {
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

        case back:
            _text = "Back";
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", _id);
            break;
    }
}

//=================================================================================================
void Button::draw() {
    // Draw the button's frame
	_settings->_screen->_screen.fillRoundRect(_x, _y, _width, _height, _radius, _backColor);
	_settings->_screen->_screen.drawRoundRect(_x, _y, _width, _height, _radius, _borderColor);

    // Draw the text
    _settings->_screen->_screen.setCursor(_x + 5, _y + 20);
    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    _settings->_screen->_screen.print(_text);
}

//=================================================================================================
bool Button::onTouch(const TS_Point& point) {
    if(!inside(point))
        return false;

    //Serial.printf("Button::onTouch: %s\n", _text.c_str());
    _parent->onButton(this);

    return true;
}

//=================================================================================================
bool Button::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {

    return false;
}
