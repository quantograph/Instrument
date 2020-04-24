#include "../Devices/Devices.h"
#include "Gui.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "Window.h"
#include "SetEffect.h"
#include "SetGuitar.h"

//=================================================================================================
SetGuitar::SetGuitar() {

}

//=================================================================================================
bool SetGuitar::init(SettingsFile* settingsFile, Settings* settings, Window* parent, ControlId id) {
    _settingsFile = settingsFile;
    //Serial.printf("SetGuitar::init: this=%p, parent=%p\n", this, parent);
    uint16_t y = 20;
    uint16_t height;
    uint16_t width;

    Window::init(settings, parent, id);
    _inputSettings = &_settings->_guitarInput;

    // "SetEffect" window
    _setEffect = new SetEffect();
    _setEffect->init(_settings, this);

    // "Single" check box
    height = 30;
    width = (_settings->_screen->_width / 2) - 10;
    _singleCheck = new CheckBox(_settings, this, 10, y, width, height, ControlId::chk_singleEffect);
    _singleCheck->_text = "Single";

    // "Double" check box
    _doubleCheck = new CheckBox(_settings, this, _settings->_screen->_width / 2, y, width, height, 
                                ControlId::chk_doubleEffect);
    _doubleCheck->_text = "Double";
    y += height + 30;

    // "Effect 1" box
    height = 30;
    _effect1 = new TextBox(_settings, this, 10, y, _settings->_screen->_width * 0.8, height, 
                           ControlId::txt_effect1);
    _effect1->_frame = true;
    _effect1->_text = _inputSettings->_effect1._effectName;
    y += height + 30;

    // "Effect 2" box
    height = 30;
    _effect2 = new TextBox(_settings, this, 10, y, _settings->_screen->_width * 0.8, height, 
                           ControlId::txt_effect2);
    _effect2->_frame = true;
    _effect1->_text = _inputSettings->_effect2._effectName;
    y += height + 30;

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetGuitar::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void SetGuitar::draw() {
    updateNumber();
    _effect1->_text = _inputSettings->_effect1._effectName;
    _effect2->_text = _inputSettings->_effect2._effectName;
    Window::draw();
}

//=================================================================================================
void SetGuitar::updateNumber() {
    //Serial.printf("SetGuitar::updateNumber: %d\n", _inputSettings);
    if(_inputSettings->_effects == 1) { // Single
        _singleCheck->update(true);
        _doubleCheck->update(false);
    } else if(_inputSettings->_effects == 2) { // Double
        _singleCheck->update(false);
        _doubleCheck->update(true);
    }
}

//=================================================================================================
bool SetGuitar::onControl(Control* control) {
    switch(control->_id) {
        case ControlId::chk_singleEffect:
            _inputSettings->_effects = 1;
            updateNumber();
            break;

        case ControlId::chk_doubleEffect:
            _inputSettings->_effects = 2;
            updateNumber();
            break;

        case ControlId::txt_effect1:
            _setEffect->activate(&_inputSettings->_effect1);
            break;

        case ControlId::txt_effect2:
            _setEffect->activate(&_inputSettings->_effect2);
            break;

        case ControlId::btn_back:
            //Serial.printf("SetGuitar::onControl %s, parent=%p\n", control->_text.c_str(), _parent);
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR SetGuitar::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}

//=================================================================================================
void SetGuitar::onBack(Window* window) {
    //Serial.printf("SetGuitar::onBack: window=%p\n", window);
    _settingsFile->write(_settingsFile->_settings);

    _settings->_audio->setEffects();

    _settings->_gui->_current = this;

    draw();
}
