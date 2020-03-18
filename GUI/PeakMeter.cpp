#include <vector>
#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "PeakMeter.h"

//=================================================================================================
PeakMeter::PeakMeter(TouchScreen* screen, int x, int y, int height, int width) : 
    _screen(screen), _x(x), _y(y), _height(height), _width(width) {
    //Serial.printf("PeakMeter: %dx%d, %dx%d\n", _x, _y, _height, _width);
}

//=================================================================================================
void PeakMeter::draw(float left, float right) {
    int width = (_width / 2) - 1;

    drawMeterBar(_x, _y, width, left);
    drawMeterBar(_x + width + 1, _y, width, right);
}

//=================================================================================================
void PeakMeter::drawMeterBar(int x, int y, int width, float value) {
    int height = _height * value;
    uint16_t color = value > 0.99 ? ILI9341_RED : ILI9341_GREEN;
    
    //Serial.printf("%dx%d, %dx%d, %0.2f\n", x, y, width, height, value);
    _screen->vertRect(x, y, _height - height, width, ILI9341_NAVY);
    _screen->vertRect(x, y + (_height - height), height, width, color);
}
