#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "Window.h"
#include "SetBand.h"

//=================================================================================================
SetBand::SetBand() {

}

//=================================================================================================
bool SetBand::init(Settings* settings, Window* parent) {

    Window::init(settings, parent);

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetBand::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void SetBand::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _backButton->draw();
}

//=================================================================================================
bool SetBand::onControl(Control* control) {
    //Serial.printf("SetBand::: %s\n", button->_text.c_str());

    switch(control->_id) {
        case ControlId::btn_back:
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR SetBand::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}
