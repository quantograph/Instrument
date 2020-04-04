#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "Window.h"
#include "SetEffect.h"

//=================================================================================================
SetEffect::SetEffect() {

}

//=================================================================================================
bool SetEffect::init(Settings* settings, Window* parent) {
    //Serial.printf("SetEffect::init: this=%p, parent=%p\n", this, parent);
    uint16_t y = 20;
    uint16_t height;

    Window::init(settings, parent);

    // "Effect" box
    height = 30;
    _effect = new TextBox(_settings, this, 10, y, _settings->_screen->_width * 0.8, height, ControlId::txt_effect);
    _effect->_frame = true;
    _effect->_dropDown = true;
    y += height + 30;

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
void SetEffect::activate(EffectSettings* effectSettings) {
    _effectSettings = effectSettings;
    Window::activate();
}

//=================================================================================================
// Makes all menu buttons
void SetEffect::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void SetEffect::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _effect->update("Effect");

    _backButton->draw();
}

//=================================================================================================
bool SetEffect::onControl(Control* control) {
    switch(control->_id) {
        case ControlId::btn_back:
            //Serial.printf("SetEffect::onButton %s, parent=%p\n", control->_text.c_str(), _parent);
            _parent->onBack(this);
            break;

        case ControlId::txt_effect:
            break;

        default:
            Serial.printf("##### ERROR SetEffect::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}
