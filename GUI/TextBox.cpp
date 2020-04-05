#include "../Devices/Devices.h"
#include "Window.h"
#include "Control.h"
#include "TextBox.h"

//=================================================================================================
TextBox::TextBox(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ControlId id) : 
    Control(settings, parent, x, y, width, height, id) {
}

//=================================================================================================
void TextBox::update(String text) {
    _text = text;
    draw();
}

//=================================================================================================
void TextBox::draw() {
    _settings->_screen->_screen.fillRect(_x, _y, _width, _height, ILI9341_BLACK);
    _settings->_screen->_screen.setCursor(_x, _y);
    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    _settings->_screen->_screen.print(_text);

    if(_frame)
        _settings->_screen->_screen.drawRect(_x, _y, _width, _height, ILI9341_WHITE);

    if(_dropDown) {
        _settings->_screen->_screen.drawRect(_x + _width, _y, _height, _height, ILI9341_WHITE);

        int16_t x = _x + _width + 2;
        int16_t y = _y + 2;
        int16_t width = _height - 4;
        _settings->_screen->_screen.fillTriangle(x, y, x + width, y, x + width * 0.5, y + width * 0.4, ILI9341_WHITE);
    }
}

//=================================================================================================
bool TextBox::onTouch(const TS_Point& point) {
    if(!inside(point))
        return false;

    _parent->onControl(this);

    return true;
}
