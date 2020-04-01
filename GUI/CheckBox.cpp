#include "../Devices/Devices.h"
#include "Window.h"
#include "Control.h"
#include "CheckBox.h"

//=================================================================================================
CheckBox::CheckBox(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id) : 
    Control(settings, parent, x, y, width, height, id) {
}

//=================================================================================================
void CheckBox::update(bool check) {
    _settings->_screen->_screen.fillRect(_x, _y, _width, _height, ILI9341_BLACK);

    // Check box
    _checked = check;
    _settings->_screen->_screen.fillRect(_x, _y, _height, _height, _checked ? ILI9341_BLUE : ILI9341_BLACK);
    _settings->_screen->_screen.drawRect(_x, _y, _height, _height, ILI9341_WHITE);

    // Text
    _settings->_screen->_screen.setCursor(_x + _height + 3, _y);
    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    _settings->_screen->_screen.print(_text);
}

//=================================================================================================
bool CheckBox::onTouch(const TS_Point& point) {
    if(!inside(point))
        return false;

    //Serial.printf("CheckBox::onTouch: %s %dx%d\n", _text.c_str(), point.x, point.y);
    _parent->onCheckBox(this);

    return true;
}
