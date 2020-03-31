#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
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
    height = 20;
    width = (_settings->_screen->_width / 2) - 10;
    _micCheck = new CheckBox(_settings, this, 10, y, width, height, CheckBox::CheckBoxId::mic);
    _micCheck->_text = "Mic";

    // "Line" check box
    _lineCheck = new CheckBox(_settings, this, _settings->_screen->_width / 2, y, width, height, CheckBox::CheckBoxId::line);
    _lineCheck->_text = "Line";
    y += height + 30;

    // Mic/line level box
    height = 30;
    _level = new TextBox(_settings, this, 10, y, _settings->_screen->_width, height);
    y += height + 30;

    // Slider
    height = 40;
    _slider = new Slider(_settings, this, 30, y, _settings->_screen->_width - 60, height);
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
                             _settings->_screen->_height - width, width, width, Button::ButtonId::back);
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
    if(_settings->_data._input == Inputs::mic) { // Mic
        _settings->_audio->_audioControl.inputSelect(AUDIO_INPUT_MIC);
        _micCheck->update(true);
        _lineCheck->update(false);

        _slider->setValue(_settings->_data._micGain);
        //Serial.printf("SetAudio::updateInput: _micGain=%0.2f\n", _settings->_data._micGain);
    } else if(_settings->_data._input == Inputs::line) { // Line in
        _settings->_audio->_audioControl.inputSelect(AUDIO_INPUT_LINEIN);
        _micCheck->update(false);
        _lineCheck->update(true);

        _slider->setValue(_settings->_data._lineInLevel);
        //Serial.printf("SetAudio::updateInput: _lineInLevel=%0.2f\n", _settings->_data._lineInLevel);
    }

    updateLevel(false);
}

//=================================================================================================
void SetAudio::updateLevel(bool getSlider) {
    uint16_t value;

    if(_settings->_data._input == Inputs::mic) { // Mic
        if(getSlider)
            _settings->_data._micGain = _slider->_value;

        _settings->_audio->setMicGain();
        sprintf(_string, "Mic gain: %d", (uint16_t)(_settings->_data._micGain * 100 + 0.5));
        //Serial.printf("SetAudio::updateLevel: MicGain=%0.2f (%d)\n", _settings->_data._micGain, value);
    } else if(_settings->_data._input == Inputs::line) { // Line in
        if(getSlider)
            _settings->_data._lineInLevel = _slider->_value;
    
        _settings->_audio->setLineInLevel();
        sprintf(_string, "Line in level: %d", (uint16_t)(_settings->_data._lineInLevel * 100 + 0.5));
        //Serial.printf("SetAudio::updateLevel: LineLevel=%0.2f (%d)\n", _settings->_data._lineInLevel, value);
    }

    _level->update(_string);
}

//=================================================================================================
void SetAudio::onPeakMeter(float left, float right) {
    //Serial.printf("SetAudio::onPeakMeter: left=%0.2f,  right=%0.2f\n", left, right);

    _peakMeter->update(left, right);
}

//=================================================================================================
bool SetAudio::onTouch(const TS_Point& point) {
    if(_micCheck->onTouch(point) || _lineCheck->onTouch(point)) {
        //Serial.printf("===== SetAudio::onTouch check boxes: %dx%d\n", point.x, point.y);
        updateInput();
    } else if (_slider->onTouch(point)) {
        //Serial.printf("===== SetAudio::onTouch slider: %dx%d\n", point.x, point.y);
        updateLevel(true);
    } else {
        //Serial.printf("===== SetAudio::onTouch button: %dx%d\n", point.x, point.y);
        _backButton->onTouch(point);
    }

    return true;
}

//=================================================================================================
bool SetAudio::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    /*if(_slider->onRelease(fromPoint, toPoint))
        updateLevel();*/

    return true;
}

//=================================================================================================
bool SetAudio::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    if(_slider->onMove(fromPoint, toPoint))
        updateLevel(true);

    return true;
}

//=================================================================================================
bool SetAudio::onButton(Button* button) {
    //Serial.printf("SetAudio::: %s\n", button->_text.c_str());

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
bool SetAudio::onCheckBox(CheckBox* checkBox) {
    //Serial.printf("SetAudio::onCheckBox %d\n", checkBox->_text.c_str());

    switch(checkBox->_id) {
        case CheckBox::CheckBoxId::mic:
            _settings->_data._input = Inputs::mic;
            updateInput();
            break;

        case CheckBox::CheckBoxId::line:
            _settings->_data._input = Inputs::line;
            updateInput();
            break;

        default:
            Serial.printf("##### ERROR: unknown check box ID: d\n", checkBox->_id);
            return false;
    }

    return true;
}
