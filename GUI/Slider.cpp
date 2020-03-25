#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "Slider.h"

//=================================================================================================
Slider::Slider(Settings::Data* settings, TouchScreen* screen, uint16_t x, uint16_t y, uint16_t width, uint16_t height) : 
    Control(settings, screen, x, y, width, height) {
}

//=================================================================================================
void Slider::init() {
    _screen->_screen.drawRect(_x, _y, _width, _height, ILI9341_WHITE);
}

//=================================================================================================
void Slider::drawBar(uint16_t y, uint16_t color) {
    for(uint16_t i = y - 2; i <= y + 2; ++i) {
	    _screen->_screen.drawFastHLine(_x + 1, i, _width - 2, color);
    }
}

//=================================================================================================
void Slider::draw(TS_Point point) {
    uint16_t top = _y + 3;
    uint16_t bottom = _y + _height - 4;
    uint16_t height = bottom - top;
    uint16_t y = point.y;

    // Limit within the control
    y = max(top, y);
    y = min(bottom, y);

    // Erase the old bar
    if(_lastY > 0)
        drawBar(_lastY, ILI9341_BLACK);

    drawBar(y, ILI9341_BLUE);
    _lastY = y;

    // Get the bar's value
    _value = (float)(bottom - y) / height;
    //Serial.printf("value=%0.2f\n", _value);
}

//=================================================================================================
float Slider::getValue() {
    return _value;
}

//=================================================================================================
void Slider::onTouch(TS_Point point) {
    /*if(!inside(point))
        return;*/

    //Serial.printf("Slider::onTouch: %dx%d\n", point.x, point.y);
    draw(point);
}

//=================================================================================================
void Slider::onRelease(TS_Point fromPoint, TS_Point toPoint) {
    /*if(!inside(toPoint))
        return;*/

    //Serial.printf("Slider::onRelease: from %dx%d to %dx%d\n\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    draw(toPoint);
}

//=================================================================================================
void Slider::onMove(TS_Point fromPoint, TS_Point toPoint) {
    /*if(!inside(toPoint))
        return;*/

    //Serial.printf("Slider::onMove: from %dx%d to %dx%d\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    draw(toPoint);
}
