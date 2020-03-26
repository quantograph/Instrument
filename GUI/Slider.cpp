#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "Slider.h"

//=================================================================================================
Slider::Slider(Settings::Data* settings, TouchScreen* screen, Window* parent, 
               uint16_t x, uint16_t y, uint16_t width, uint16_t height) : 
    Control(settings, screen, parent, x, y, width, height) {
}

//=================================================================================================
void Slider::init() {
    _screen->_screen.drawRect(_x, _y, _width, _height, ILI9341_WHITE);
}

//=================================================================================================
void Slider::drawBar(uint16_t x, uint16_t color) {
    for(uint16_t i = x - 2; i <= x + 2; ++i) {
	    _screen->_screen.drawFastVLine(i, _y + 1, _height - 2, color);
    }
}

//=================================================================================================
void Slider::draw(TS_Point point) {
    uint16_t left = _x + 3;
    uint16_t right = _x + _width - 4;
    uint16_t width = right - left;
    uint16_t x = point.x;

    // Limit within the control
    x = max(left, x);
    x = min(right, x);

    // Erase the old bar
    if(_lastX > 0)
        drawBar(_lastX, ILI9341_BLACK);

    drawBar(x, ILI9341_BLUE);
    _lastX = x;

    // Get the bar's value
    _value = (float)(x - left) / width;
    Serial.printf("value=%0.2f\n", _value);
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
