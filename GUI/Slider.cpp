#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "Slider.h"

//=================================================================================================
Slider::Slider(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height) : 
    Control(settings, parent, x, y, width, height) {
    _left = _x + 3;
    _right = _x + _width - 4;
}

//=================================================================================================
void Slider::draw() {
    _settings->_screen->_screen.drawRect(_x, _y, _width, _height, ILI9341_WHITE);
}

//=================================================================================================
void Slider::drawBar(uint16_t x, uint16_t color) {
    for(uint16_t i = x - 2; i <= x + 2; ++i) {
	    _settings->_screen->_screen.drawFastVLine(i, _y + 1, _height - 2, color);
    }
}

//=================================================================================================
void Slider::update(TS_Point point) {
    uint16_t width = _right - _left;
    uint16_t x = point.x;

    // Limit within the control
    x = max(_left, x);
    x = min(_right, x);

    // Erase the old bar
    if(_lastX > 0)
        drawBar(_lastX, ILI9341_BLACK);

    drawBar(x, ILI9341_BLUE);
    _lastX = x;

    // Get the bar's value
    _value = (float)(x - _left) / width;
    //Serial.printf("value=%0.2f\n", _value);
}

//=================================================================================================
void Slider::setValue(float value) {
    TS_Point point;
    
    point.x = _left + (_right - _left) * value;
    point.y = 0;
    Serial.printf("Slider::setValue: value=%0.2f, x=%d\n", value, point.x);
    
    update(point);
}

//=================================================================================================
bool Slider::inside(const TS_Point& point) {
    return point.y >= _y && point.y <= _y + _height;
}

//=================================================================================================
void Slider::onTouch(const TS_Point& point) {
    if(!inside(point))
        return;

    //Serial.printf("Slider::onTouch: %dx%d\n", point.x, point.y);
    update(point);
}

//=================================================================================================
void Slider::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(!inside(toPoint))
        return;

    //Serial.printf("Slider::onRelease: from %dx%d to %dx%d\n\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    update(toPoint);
}

//=================================================================================================
void Slider::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(!inside(toPoint))
        return;

    //Serial.printf("Slider::onMove: from %dx%d to %dx%d\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    update(toPoint);
}
