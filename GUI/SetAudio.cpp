#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Window.h"
#include "SetAudio.h"

//=================================================================================================
SetAudio::SetAudio() {

}

//=================================================================================================
bool SetAudio::init(TouchScreen* screen, Settings::Data* settings) {
    _screen = screen;
    _settings = settings;
    Window::init(_screen, _settings);

    //_peakMeter = new PeakMeter(_settings, _screen, 0, 0, 10, _screen->_height / 2);

    _slider = new Slider(_settings, _screen, this, 30, 100, _screen->_width - 60, _screen->_height * 0.15);
    _slider->init();

    return true;
}

//=================================================================================================
void SetAudio::onPeakMeter(float left, float right) {
    //_peakMeter->draw(left, right);
}

//=================================================================================================
void SetAudio::onTouch(TS_Point point) {
    _slider->onTouch(point);
}

//=================================================================================================
void SetAudio::onRelease(TS_Point fromPoint, TS_Point toPoint) {
    //_slider->onRelease(fromPoint, toPoint);
}

//=================================================================================================
void SetAudio::onMove(TS_Point fromPoint, TS_Point toPoint) {
    //_slider->onMove(fromPoint, toPoint);
}
