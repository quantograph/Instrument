#include <vector>
#include "Settings.h"
#include "Control.h"
#include "Window.h"

//=================================================================================================
Window::Window(Settings::Data* settings) : _settings(settings) {

}

//=================================================================================================
Window::~Window() {
    for(auto control : _controls){
        delete control;
    }
}

//=================================================================================================
// Draw this window
void Window::draw() {
    for(auto control : _controls){
        control->draw();
    }
}
