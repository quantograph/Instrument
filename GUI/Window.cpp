#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
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
bool Window::init(Settings* settings, Window* parent /* = nullptr */) {
    _settings = settings;
    _parent = parent;

    return true;
}

//=================================================================================================
// Draw this window
void Window::draw() {
    for(auto control : _controls){
        control->draw();
    }
}
