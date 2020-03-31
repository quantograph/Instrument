#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"

//=================================================================================================
Control::Control(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id) : 
    _settings(settings), _parent(parent), _x(x), _y(y), _width(width), _height(height), _id(id) {
}

//=================================================================================================
// Draw this control
void Control::draw() {
    switch(_type) {
        case button:
            break;

        case edit:
            break;

        case list:
            break;

        default:
            Serial.printf("Unknown control type: %d\n", _type);
    }
}

//=================================================================================================
// Whether the point is inside the control
bool Control::inside(const TS_Point& point) {
    if(point.x >= _x && point.x <= _x + _width &&
       point.y >= _y && point.y <= _y + _height)
        return true;
    else
        return false;
}
