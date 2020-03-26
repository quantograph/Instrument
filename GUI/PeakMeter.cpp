#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"

//=================================================================================================
PeakMeter::PeakMeter(Settings::Data* settings, TouchScreen* screen, Window* parent, 
                     uint16_t x, uint16_t y, uint16_t width, uint16_t height) : 
    Control(settings, screen, parent, x, y, width, height) {
    //Serial.printf("PeakMeter: %dx%d, %dx%d\n", _x, _y, _height, _width);
}

//=================================================================================================
void PeakMeter::draw(float left, float right) {
    int width = (_width / 2) - 1;

    drawMeterBar(_x, _y, width, left);
    drawMeterBar(_x + width + 1, _y, width, right);
}

//=================================================================================================
void PeakMeter::drawMeterBar(uint16_t x, uint16_t y, uint16_t width, float value) {
    int height = _height * value;
    uint16_t color = value > 0.99 ? ILI9341_RED : ILI9341_GREEN;
    
    //Serial.printf("%dx%d, %dx%d, %0.2f\n", x, y, width, height, value);

    int yMid = y + (_height - height);
    _screen->vertRect(x, y, width, _height - height, ILI9341_NAVY); // Top
    _screen->vertRect(x, yMid, width, height, color); // Bottom
}
