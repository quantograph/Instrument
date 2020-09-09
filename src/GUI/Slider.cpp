#include "../Devices/Devices.h"
#include "Window.h"
#include "Control.h"
#include "Slider.h"

//=================================================================================================
Slider::Slider(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, 
               uint16_t height, ControlId id) : 
    Control(settings, parent, x, y, width, height, id) {
    _barTop = _y + _titleHeight;
    _barLeft = _x + 3;
    _barRight = _x + _width - 4;
    _barHeight = _height - _titleHeight;
}

//=================================================================================================
void Slider::activate() {
    //Serial.printf("List::activate\n");
    _ready = false;
    draw();
}

//=================================================================================================
void Slider::setTitle(const char* title) {
    _title = title;

    //Serial.printf("Slider::setTitle: %s\n", title);
    _settings->_screen->_screen.fillRect(_x, _y, _width, _titleHeight, ILI9341_BLACK);
    _settings->_screen->_screen.setCursor(_x, _y);
    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    _settings->_screen->_screen.print(_title);
}

//=================================================================================================
void Slider::draw() {
    _settings->_screen->_screen.drawRect(_x, _barTop, _width, _barHeight, ILI9341_WHITE);
}

//=================================================================================================
void Slider::drawBar(uint16_t x, uint16_t color) {
    for(uint16_t i = x - 2; i <= x + 2; ++i) {
	    _settings->_screen->_screen.drawFastVLine(i, _barTop + 1, _barHeight - 2, color);
    }
}

//=================================================================================================
void Slider::update(TS_Point point) {
    uint16_t width = _barRight - _barLeft;
    uint16_t x = point.x;

    // Limit within the control
    x = max(_barLeft, x);
    x = min(_barRight, x);

    // Erase the old bar
    if(_lastX > 0)
        drawBar(_lastX, ILI9341_BLACK);

    drawBar(x, ILI9341_BLUE);
    _lastX = x;

    // Get the bar's value
    _value = (float)(x - _barLeft) / width;
    //Serial.printf("value=%0.2f\n", _value);
}

//=================================================================================================
void Slider::setValue(float value, float min, float max) {
    TS_Point point;
    float scaled;
    
    scaled = scale(value, min, max, 0.0, 1.0);
    _value = scaled;
    point.x = _barLeft + (_barRight - _barLeft) * value;
    point.y = 0;
    //Serial.printf("Slider::setValue: value=%0.2f, x=%d\n", value, point.x);
    
    update(point);
}

//=================================================================================================
bool Slider::inside(const TS_Point& point) {
    return point.y >= _y && point.y <= _y + _height;
}

//=================================================================================================
bool Slider::onTouch(const TS_Point& point) {
    if(!_ready) {
        //Serial.printf("Slider::onTouch: NOT READY\n");
        return false;
    }

    if(!inside(point))
        return false;

    //Serial.printf("Slider::onTouch: %dx%d\n", point.x, point.y);
    update(point);
    _parent->onControl(this);

    return true;
}

//=================================================================================================
bool Slider::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    _ready = true;

    if(!inside(toPoint))
        return false;

    //Serial.printf("Slider::onRelease: from %dx%d to %dx%d\n\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    update(toPoint);
    _parent->onControl(this);

    return true;
}

//=================================================================================================
bool Slider::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(!_ready)
        return false;

    if(!inside(toPoint))
        return false;

    //Serial.printf("Slider::onMove: from %dx%d to %dx%d\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    update(toPoint);
    _parent->onControl(this);

    return true;
}
