#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Button.h"
#include "Window.h"
#include "SetAudio.h"
#include "Main.h"

//=================================================================================================
Main::Main() {

}

//=================================================================================================
Main::~Main() {
    delete _setAudio;
    delete _peakMeter;
}

//=================================================================================================
bool Main::init(Settings* settings) {
    //Serial.printf("Main::init\n");
    Window::init(settings);

    _setAudio = new SetAudio();
    _setAudio->init(settings, this);

    _peakMeter = new PeakMeter(settings, this, 0, 0, 10, _settings->_screen->_height / 2);

    setupButtons();

    //Serial.printf("Main::init done, _setAudio=%p\n", _setAudio);

    return true;
}

//=================================================================================================
// Makes all menu buttons
void Main::setupButtons() {
    Button* button;
    Button::ButtonId id[] = {Button::ButtonId::guitar, Button::ButtonId::synth, Button::ButtonId::band, Button::ButtonId::settings};
    uint16_t width = _settings->_screen->_width / 4;
    uint16_t height = width;
    uint16_t x = 0;
    uint16_t y = _settings->_screen->_height - width;

    for(int i = 0; i < 4; ++i) {
        button = new Button(_settings, this, x, y, width, height, id[i]);
        button->init();
        _buttons.push_back(button);
        x += width;
    }
}

//=================================================================================================
void Main::draw() {
    //Serial.printf("Main::draw\n");
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _peakMeter->draw();

    for(auto button : _buttons) {
        button->draw();
    }
}

//=================================================================================================
void Main::onPeakMeter(float left, float right) {
    //Serial.printf("Main::onPeakMeter, _current=\n", _current);

    if(_current) {
        //Serial.printf("Main::onPeakMeter, current: left=%0.2f,  right=%0.2f\n", left, right);
        _current->onPeakMeter(left, right);
    } else {
        //Serial.printf("Main::onPeakMeter, NO current: left=%0.2f,  right=%0.2f\n", left, right);
    }
}

//=================================================================================================
void Main::onTouch(const TS_Point& point) {
    //Serial.printf("Main::onTouch: %dx%d\n", point.x, point.y);

    if(_current) {
        _current->onTouch(point);
    } else {
        for(auto button : _buttons) {
            button->onTouch(point);
        }
    }
}

//=================================================================================================
void Main::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(_current) {
        _current->onRelease(fromPoint, toPoint);
    }
}

//=================================================================================================
void Main::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(_current) {
        _current->onMove(fromPoint, toPoint);
    }
}

//=================================================================================================
void Main::onButton(Button* button) {
    Window* window{};
    //Serial.printf("Main::onButton: %s\n", button->_text.c_str());

    switch(button->_id) {
        case Button::ButtonId::guitar:
            break;

        case Button::ButtonId::synth:
            break;

        case Button::ButtonId::band:
            break;

        case Button::ButtonId::settings:
            window = _setAudio;
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", button->_id);
            return;
    }

    if(window) {
        window->draw();
        _current = window;
        Serial.printf("Main::onButton: _current=%p\n", _current);
    }
}

//=================================================================================================
void Main::onBack(Window* window) {
    _settings->write();
    _current = nullptr;
    draw();
}
