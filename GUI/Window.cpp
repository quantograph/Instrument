#include "../Devices/Devices.h"
#include "Control.h"
#include "Gui.h"
#include "Window.h"

//=================================================================================================
Window::Window() {
}

//=================================================================================================
Window::~Window() {
    for(auto control : _controls){
        delete control;
    }
}

//=================================================================================================
bool Window::init(Settings* settings, Window* parent, ControlId id /* = ControlId::ctl_none */) {
    _settings = settings;
    _parent = parent;
    _id = id;

    return true;
}

//=================================================================================================
void Window::activate() {
    _settings->_gui->_current = this;
    draw();
}

//=================================================================================================
void Window::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    for(auto control : _controls){
        control->draw();
    }
}

//=================================================================================================
bool Window::onTouch(const TS_Point& point) {
    for(auto control : _controls)
        control->onTouch(point);

    return false;
}

//=================================================================================================
bool Window::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    for(auto control : _controls)
        control->onRelease(fromPoint, toPoint);

    return false;
}

//=================================================================================================
bool Window::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    for(auto control : _controls)
        control->onMove(fromPoint, toPoint);

    return false;
}
