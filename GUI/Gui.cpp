#include <vector>
#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "Window.h"
#include "PeakMeter.h"
#include "Main.h"
#include "Gui.h"

//=================================================================================================
Gui::Gui() {
}

//=================================================================================================
bool Gui::init(TouchScreen* screen, Settings::Data* settings) {
    _screen = screen;
    _settings = settings;

    _screen->light(1.0);
    
    _main = new Main();
    _main->init(_screen, _settings);

    return true;
}

//=================================================================================================
bool Gui::process() {
    bool touched{_screen->read()}; // Whether the screen is being touched
    bool edge{false}; // If we are on the "edge" of touching or releasing the screen
    uint32_t delay{50}; // Time to wait to see if the screen is no longer touched

    if(!touched && !_touched) // Was not touched and not touched now, nothing to do.
        return false;

    // Save times of the first and last touches
    uint32_t now = millis();
    if(touched) {
        _lastTouch = now;
        if(_firstTouch == 0)
            _firstTouch = now;
    }

    // While the screen is touched, we get a constant stream of touch signals.
    // Detect when the screen is touched and released.
    if(touched && !_touched) { // Was not touched, now is touched
        edge = true;
        _touched = true;
        _firstTouchPoint = _screen->_touchPoint;
    } else if((!touched && _touched) && // Was touched, now is not touched
              now > _lastTouch + delay) { // Not touched for some time
        edge = true;
        _touched = false;
        _firstTouch = 0;
    }

    if(edge) { // Pressed or released just now
        _lastTouchPoint = _screen->_touchPoint;
        if(_touched) {
            onTouch(_lastTouchPoint);
        } else {
            onRelease(_firstTouchPoint, _lastTouchPoint);
        }
    } else if(_touched && // Ignore very small movements of the finger
              std::max(abs(_lastTouchPoint.x - _screen->_touchPoint.x),
                       abs(_lastTouchPoint.y - _screen->_touchPoint.y)) > 3) {
        onMove(_lastTouchPoint, _screen->_touchPoint);
        _lastTouchPoint = _screen->_touchPoint;
    }

    return true;
}

//=================================================================================================
void Gui::onTouch(TS_Point point) {
    Serial.printf("GUI touched at %dx%d\n", point.x, point.y);
}

//=================================================================================================
void Gui::onRelease(TS_Point fromPoint, TS_Point toPoint) {
    Serial.printf("GUI released, moved from %dx%d to %dx%d\n\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
}

//=================================================================================================
void Gui::onMove(TS_Point fromPoint, TS_Point toPoint) {
    Serial.printf("GUI moved from %dx%d to %dx%d\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
}

//=================================================================================================
void Gui::onPeakMeter(float left, float right) {
    _main->onPeakMeter(left, right);
}
