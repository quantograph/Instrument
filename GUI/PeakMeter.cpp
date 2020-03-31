#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"

//=================================================================================================
PeakMeter::PeakMeter(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id) : 
    Control(settings, parent, x, y, width, height, id) {
    //Serial.printf("PeakMeter: %dx%d, %dx%d\n", _x, _y, _height, _width);
}

//=================================================================================================
void PeakMeter::draw() {

}

//=================================================================================================
void PeakMeter::update(float left, float right) {
    int height = (_height / 2) - 1;

    drawMeterBar(_y, height, left);
    drawMeterBar(_y + height + 1, height, right);
}

//=================================================================================================
void PeakMeter::drawMeterBar(uint16_t y, uint16_t height, float value) {
    int width = _width * value;
    uint16_t color = value > 0.99 ? ILI9341_RED : ILI9341_GREEN;
    
    //Serial.printf("%dx%d, %dx%d, %0.2f\n", x, y, width, height, value);

    int xMid = _x + width;
    _settings->_screen->horRect(_x, y, width, height, color); // Left
    _settings->_screen->horRect(xMid, y, _width - xMid, height, ILI9341_NAVY); // Right
}
