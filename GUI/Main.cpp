#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Window.h"
#include "Main.h"

//=================================================================================================
Main::Main() {

}

//=================================================================================================
bool Main::init(TouchScreen* screen, Settings::Data* settings) {
    _screen = screen;
    _settings = settings;
    Window::init(_screen, _settings);

    //_peakMeter = new PeakMeter(_settings, _screen, 0, 0, 10, _screen->_height / 2);

    _slider = new Slider(_settings, _screen, 100, 50, 100, _screen->_height * 0.6);
    _slider->init();

    return true;
}

//=================================================================================================
void Main::onPeakMeter(float left, float right) {
    //_peakMeter->draw(left, right);
}

//=================================================================================================
void Main::onTouch(TS_Point point) {
    _slider->onTouch(point);
}

//=================================================================================================
void Main::onRelease(TS_Point fromPoint, TS_Point toPoint) {
    _slider->onRelease(fromPoint, toPoint);
}

//=================================================================================================
void Main::onMove(TS_Point fromPoint, TS_Point toPoint) {
    _slider->onMove(fromPoint, toPoint);
}
