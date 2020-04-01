#include "../Devices/Devices.h"
#include "Control.h"
#include "Window.h"
#include "PeakMeter.h"
#include "Main.h"
#include "Gui.h"

//=================================================================================================
Gui::Gui() {
}

//=================================================================================================
bool Gui::init(Storage* storage, Settings* settings) {
    _storage = storage;
    _settings = settings;
    //Serial.printf("Gui::init\n");
    //_settings->show("GUI");

    _settings->_screen->light(1.0);
    
    _main = new Main();
    _main->init(_storage, _settings);
    _main->draw();
    _current = _main;

    return true;
}

//=================================================================================================
bool Gui::process() {
    bool touched{_settings->_screen->read()}; // Whether the screen is being touched
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
        _firstTouchPoint = _settings->_screen->_touchPoint;
    } else if((!touched && _touched) && // Was touched, now is not touched
              now > _lastTouch + delay) { // Not touched for some time
        edge = true;
        _touched = false;
        _firstTouch = 0;
    }

    if(edge) { // Pressed or released just now
        _lastTouchPoint = _settings->_screen->_touchPoint;
        if(_touched) {
            onTouch(_lastTouchPoint);
        } else {
            onRelease(_firstTouchPoint, _lastTouchPoint);
        }
    } else if(_touched && // Ignore very small movements of the finger
              std::max(abs(_lastTouchPoint.x - _settings->_screen->_touchPoint.x),
                       abs(_lastTouchPoint.y - _settings->_screen->_touchPoint.y)) > 3) {
        onMove(_lastTouchPoint, _settings->_screen->_touchPoint);
        _lastTouchPoint = _settings->_screen->_touchPoint;
    }

    return true;
}

//=================================================================================================
bool Gui::onTouch(const TS_Point& point) {
    //Serial.printf("GUI touched at %dx%d\n", point.x, point.y);
    return _current->onTouch(point);
}

//=================================================================================================
bool Gui::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    //Serial.printf("GUI released, moved from %dx%d to %dx%d\n\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    return _current->onRelease(fromPoint, toPoint);
}

//=================================================================================================
bool Gui::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    //Serial.printf("GUI moved from %dx%d to %dx%d\n", fromPoint.x, fromPoint.y, toPoint.x, toPoint.y);
    return _current->onMove(fromPoint, toPoint);
}

//=================================================================================================
void Gui::onPeakMeter(float left, float right) {
    //Serial.printf("Gui::onPeakMeter: left=%0.2f,  right=%0.2f\n", left, right);
    _current->onPeakMeter(left, right);
}
