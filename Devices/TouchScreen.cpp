#include <XPT2046_Touchscreen.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>
#include "TouchScreen.h"

//=================================================================================================
TouchScreen::TouchScreen() : _touch(T_CS/*, T_IRQ*/), _screen(TFT_CS, TFT_DC, TFT_RST,
    TFT_MOSI, TFT_SCK, TFT_SDO) {
}

//=================================================================================================
// Initializes the screent and the touch
void TouchScreen::init() {
    // Initialize the screent
    _screen.begin();
    _screen.setRotation(0);
    _screen.fillScreen(ILI9341_BLACK);

    // Initialize the touchscreen
    _touch.begin();
    _touch.setRotation(2);

    _width = _screen.width(), 
    _height = _screen.height();

    pinMode(TFT_LED, OUTPUT);
    light(0.0);

    //attachInterrupt(T_IRQ, onTouched, FALLING);
}

//=================================================================================================
void TouchScreen::light(float value) {
    analogWrite(TFT_LED, 255 * value);
}

//=================================================================================================
bool TouchScreen::read() {
    _touched = false;
    if(/*_touch.tirqTouched() &&*/ _touch.touched()) {
        _touched = true;

        // Map the touch point to the screen coordinates
        _touchPoint = _touch.getPoint();
        _touchPoint.x = map(_touchPoint.x, 0, 4095, 0, _width);
        _touchPoint.y = map(_touchPoint.y, 0, 4095, 0, _height);
        //Serial.printf("Touch: x=%5d y=%5d\n", _touchPoint.x, _touchPoint.y);
    }

    return _touched;
}

//=================================================================================================
void TouchScreen::clear() {
    _screen.fillScreen(ILI9341_BLACK);
}

//=================================================================================================
void TouchScreen::onTouched() {
    //Serial.println(">>>> Screen touched");
    //_screen.read();
}

//=================================================================================================
// Draws a vertical rectangle
void TouchScreen::vertRect(int x, int y, int height, int width, uint16_t color) {
    for(int i = 0; i < 2; ++i) {
	    _screen.drawFastVLine(x + i, y, height, color);
    }
}
