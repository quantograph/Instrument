#include "TouchScreen.h"

//=================================================================================================
TouchScreen::TouchScreen() : _touch(T_CS, T_IRQ), _screen(TFT_CS, TFT_DC, TFT_RST,
    TFT_MOSI, TFT_SCK, TFT_SDO) {
}

//=================================================================================================
// Initializes the screent and the touch
void TouchScreen::Init() {
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
    analogWrite(TFT_LED, 255);

    //attachInterrupt(T_IRQ, OnTouched, FALLING);
}

//=================================================================================================
bool TouchScreen::Read() {
    _touched = false;
    if(_touch.tirqTouched()) {
        if(_touch.touched()) {
            _touched = true;

            // Map the touch point to the screen coordinates
            _touchPoint = _touch.getPoint();
            _touchPoint.x = map(_touchPoint.x, 0, 4095, 0, _width);
            _touchPoint.y = map(_touchPoint.y, 0, 4095, 0, _height);
            //Serial.printf("Touch: x=%5d y=%5d\n", _touchPoint.x, _touchPoint.y);

            //_screen.sleep(false);
        }
    }

    return _touched;
}

//=================================================================================================
void TouchScreen::Clear() {
    _screen.fillScreen(ILI9341_BLACK);
}

//=================================================================================================
void TouchScreen::OnTouched() {
    Serial.println(">>>> Screen touched");
    //_screen.Read();
}
