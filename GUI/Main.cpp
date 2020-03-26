#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Button.h"
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

    _peakMeter = new PeakMeter(_settings, _screen, this, 0, 0, 10, _screen->_height / 2);

    /*_slider = new Slider(_settings, _screen, this, 30, 100, _screen->_width - 60, _screen->_height * 0.15);
    _slider->init();*/

    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void Main::setupButtons() {
    Button* button;
    Button::ButtonId id[] = {Button::ButtonId::guitar, Button::ButtonId::synth, Button::ButtonId::band, Button::ButtonId::settings};
    uint16_t width = _screen->_width / 4;
    uint16_t height = width;
    uint16_t x = 0;
    uint16_t y = _screen->_height - width;

    for(int i = 0; i < 4; ++i) {
        button = new Button(_settings, _screen, this, x, y, width, height, id[i]);
        button->init();
        _buttons.push_back(button);
        x += width;
    }
}

//=================================================================================================
void Main::onPeakMeter(float left, float right) {
    //_peakMeter->draw(left, right);
}

//=================================================================================================
void Main::onTouch(TS_Point point) {
    //Serial.printf("Main::onTouch: %dx%d\n", point.x, point.y);

    for(auto button : _buttons) {
        button->onTouch(point);
    }
}

//=================================================================================================
void Main::onRelease(TS_Point fromPoint, TS_Point toPoint) {
    //_slider->onRelease(fromPoint, toPoint);
}

//=================================================================================================
void Main::onMove(TS_Point fromPoint, TS_Point toPoint) {
    //_slider->onMove(fromPoint, toPoint);
}

//=================================================================================================
void Main::onButton(Button* button) {
    //Serial.printf("Main::onButton: %s\n", button->_text.c_str());

    switch(button->_id) {
        case Button::ButtonId::guitar:
            break;

        case Button::ButtonId::synth:
            break;

        case Button::ButtonId::band:
            break;

        case Button::ButtonId::settings:
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", button->_id);
            break;
    }
}
