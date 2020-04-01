#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "Window.h"
#include "SetSynth.h"

//=================================================================================================
SetSynth::SetSynth() {

}

//=================================================================================================
bool SetSynth::init(Settings* settings, Window* parent) {

    Window::init(settings, parent);


    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetSynth::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, Button::ButtonId::back);
    _backButton->init();
}

//=================================================================================================
void SetSynth::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _backButton->draw();
}

//=================================================================================================
bool SetSynth::onTouch(const TS_Point& point) {
    _backButton->onTouch(point);

    return true;
}

//=================================================================================================
bool SetSynth::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    /*if(_slider->onRelease(fromPoint, toPoint))
        updateLevel();*/

    return true;
}

//=================================================================================================
bool SetSynth::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {

    return true;
}

//=================================================================================================
bool SetSynth::onButton(Button* button) {
    //Serial.printf("SetSynth::: %s\n", button->_text.c_str());

    switch(button->_id) {
        case Button::ButtonId::back:
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", button->_id);
            return false;
    }

    return true;
}

//=================================================================================================
bool SetSynth::onCheckBox(CheckBox* checkBox) {
    //Serial.printf("SetSynth::onCheckBox %d\n", checkBox->_text.c_str());

    /*switch(checkBox->_id) {
        case CheckBox::CheckBoxId::mic:
            _settings->_input = Inputs::mic;
            updateInput();
            break;

        case CheckBox::CheckBoxId::line:
            _settings->_input = Inputs::line;
            updateInput();
            break;

        default:
            Serial.printf("##### ERROR: unknown check box ID: d\n", checkBox->_id);
            return false;
    }*/

    return true;
}
