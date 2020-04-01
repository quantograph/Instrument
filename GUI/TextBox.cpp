#include "../Devices/Devices.h"
#include "Control.h"
#include "TextBox.h"

//=================================================================================================
TextBox::TextBox(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id) : 
    Control(settings, parent, x, y, width, height, id) {
}

//=================================================================================================
void TextBox::update(String text) {
    _text = text;
    _settings->_screen->_screen.fillRect(_x, _y, _width, _height, ILI9341_BLACK);
    _settings->_screen->_screen.setCursor(_x, _y);
    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    _settings->_screen->_screen.print(_text);
}

//=================================================================================================
bool TextBox::onTouch(const TS_Point& point) {
    if(!inside(point))
        return false;

    return true;
}
