#include "../Devices/Devices.h"
#include "Gui.h"
#include "Control.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Button.h"
#include "Window.h"
#include "SetGuitar.h"
#include "SetSynth.h"
#include "SetBand.h"
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
bool Main::init(Storage* storage, Settings* settings) {
    _storage = storage;
    //Serial.printf("Main::init: this=%p\n", this);
    Window::init(settings, nullptr);

    _setGuitar = new SetGuitar();
    _setGuitar->init(_settings, this);

    _setSynth = new SetSynth();
    _setSynth->init(_settings, this);

    _setBand = new SetBand();
    _setBand->init(_settings, this);

    _setAudio = new SetAudio();
    _setAudio->init(_settings, this);

    _peakMeter = new PeakMeter(settings, this, 0, 0, 10, _settings->_screen->_height / 2);

    setupButtons();

    //Serial.printf("Main::init done, _setAudio=%p\n", _setAudio);

    return true;
}

//=================================================================================================
// Makes all menu buttons
void Main::setupButtons() {
    Button* button;
    ControlId id[] = {ControlId::btn_guitar, ControlId::btn_synth, ControlId::btn_band, ControlId::btn_settings};
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
void Main::onPeakMeter(float left, float right) {
    //Serial.printf("Main::onPeakMeter, _current=\n", _current);
}

//=================================================================================================
bool Main::onControl(Control* control) {
    //Serial.printf("Main::onButton: %s\n", button->_text.c_str());

    switch(control->_id) {
        case ControlId::btn_guitar:
            _setGuitar->activate();
            break;

        case ControlId::btn_synth:
            _setSynth->activate();
            break;

        case ControlId::btn_band:
            _setBand->activate();
            break;

        case ControlId::btn_settings:
            _setAudio->activate();
            break;

        default:
            Serial.printf("##### ERROR Main::onControl: unknown control ID: d\n", control->_id);
            return false;
    }

    return true;
}

//=================================================================================================
void Main::onBack(Window* window) {
    //Serial.printf("Main::onBack: window=%p\n", window);

    _storage->write();
    _settings->_gui->_current = this;
    draw();
}
