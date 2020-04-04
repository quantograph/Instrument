#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "Window.h"
#include "SetAudio.h"

//=================================================================================================
SetAudio::SetAudio() {

}

//=================================================================================================
bool SetAudio::init(Settings* settings, Window* parent) {
    uint16_t y = 0;
    uint16_t height;
    uint16_t width;

    Window::init(settings, parent);

    // Peak meter
    height = 10;
    _peakMeter = new PeakMeter(_settings, this, 0, y, _settings->_screen->_width, height);
    y += height + 30;

    // "Input" static text
    /*height = 30;
    _inputText = new TextBox(_settings, this, 10, y, _settings->_screen->_width, height);
    _inputText->update("Input:");
    y += height + 10;*/

    // "Mic" check box
    height = 30;
    width = (_settings->_screen->_width / 2) - 10;
    _micCheck = new CheckBox(_settings, this, 10, y, width, height, ControlId::chk_mic);
    _micCheck->_text = "Mic";

    // "Line" check box
    _lineCheck = new CheckBox(_settings, this, _settings->_screen->_width / 2, y, width, height, ControlId::chk_line);
    _lineCheck->_text = "Line";
    y += height + 30;

    // Mic/line level box
    height = 30;
    _level = new TextBox(_settings, this, 10, y, _settings->_screen->_width, height);
    y += height + 30;

    // Slider
    height = 40;
    _slider = new Slider(_settings, this, 30, y, _settings->_screen->_width - 60, height, ControlId::sld_velel);
    y += height + 30;

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetAudio::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void SetAudio::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _peakMeter->draw();
    _slider->draw();
    _backButton->draw();
    updateInput();
}

//=================================================================================================
void SetAudio::updateInput() {
    if(_settings->_input == Inputs::mic) { // Mic
        _settings->_audio->_audioControl.inputSelect(AUDIO_INPUT_MIC);
        _micCheck->update(true);
        _lineCheck->update(false);

        _slider->setValue(_settings->_micGain);
        //Serial.printf("SetAudio::updateInput: _micGain=%0.2f\n", _settings->_micGain);
    } else if(_settings->_input == Inputs::line) { // Line in
        _settings->_audio->_audioControl.inputSelect(AUDIO_INPUT_LINEIN);
        _micCheck->update(false);
        _lineCheck->update(true);

        _slider->setValue(_settings->_lineInLevel);
        //Serial.printf("SetAudio::updateInput: _lineInLevel=%0.2f\n", _settings->_lineInLevel);
    }

    updateLevel(false);
}

//=================================================================================================
void SetAudio::updateLevel(bool getSlider) {
    if(_settings->_input == Inputs::mic) { // Mic
        if(getSlider)
            _settings->_micGain = _slider->_value;

        _settings->_audio->setMicGain();
        sprintf(_string, "Mic gain: %d", (uint16_t)(_settings->_micGain * 100 + 0.5));
        //Serial.printf("SetAudio::updateLevel: MicGain=%0.2f (%d)\n", _settings->_micGain, value);
    } else if(_settings->_input == Inputs::line) { // Line in
        if(getSlider)
            _settings->_lineInLevel = _slider->_value;
    
        _settings->_audio->setLineInLevel();
        sprintf(_string, "Line in level: %d", (uint16_t)(_settings->_lineInLevel * 100 + 0.5));
        //Serial.printf("SetAudio::updateLevel: LineLevel=%0.2f (%d)\n", _settings->_lineInLevel, value);
    }

    _level->update(_string);
}

//=================================================================================================
void SetAudio::onPeakMeter(float left, float right) {
    //Serial.printf("SetAudio::onPeakMeter: left=%0.2f,  right=%0.2f\n", left, right);

    _peakMeter->update(left, right);
}

//=================================================================================================
bool SetAudio::onControl(Control* control) {
    //Serial.printf("SetAudio::: %s\n", button->_text.c_str());

    switch(control->_id) {
        case ControlId::chk_mic:
            _settings->_input = Inputs::mic;
            updateInput();
            break;

        case ControlId::chk_line:
            _settings->_input = Inputs::line;
            updateInput();
            break;

        case ControlId::sld_velel:
            updateLevel(true);
            break;

        case ControlId::btn_back:
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR SetAudio::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}
