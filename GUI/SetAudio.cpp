#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "Window.h"
#include "SetAudio.h"

//=================================================================================================
SetAudio::SetAudio() {

}

//=================================================================================================
bool SetAudio::init(Settings* settings, Window* parent) {
    Window::init(settings, parent);

    _peakMeter = new PeakMeter(_settings, this, 0, 0, _settings->_screen->_width, 10);
    _slider = new Slider(_settings, this, 30, 150, _settings->_screen->_width - 60, _settings->_screen->_height * 0.15);

    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetAudio::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, _settings->_screen->_height - width, 
                             width, width, Button::ButtonId::back);
    _backButton->init();
}

//=================================================================================================
void SetAudio::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

    _slider->draw();
    _slider->setValue((float)_settings->_data._micGain / MIC_GAIN_MAX);

    _peakMeter->draw();
    _backButton->draw();
}

//=================================================================================================
// Saves all current values
void SetAudio::save() {
    // Mic gain
    _settings->_data._micGain = (uint16_t)(_slider->_value * MIC_GAIN_MAX + 0.5);
    _settings->_audio->_audioControl.micGain(_settings->_data._micGain);
    //Serial.printf("MicGain=%d\n", _settings->_data._micGain);
}

//=================================================================================================
void SetAudio::onPeakMeter(float left, float right) {
    //Serial.printf("SetAudio::onPeakMeter: left=%0.2f,  right=%0.2f\n", left, right);

    _peakMeter->update(left, right);
}

//=================================================================================================
void SetAudio::onTouch(const TS_Point& point) {
    _slider->onTouch(point);
    save();

    _backButton->onTouch(point);
}

//=================================================================================================
void SetAudio::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    _slider->onRelease(fromPoint, toPoint);
    save();
}

//=================================================================================================
void SetAudio::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    _slider->onMove(fromPoint, toPoint);
    save();
}

//=================================================================================================
void SetAudio::onButton(Button* button) {
    //Serial.printf("SetAudio::: %s\n", button->_text.c_str());

    switch(button->_id) {
        case Button::ButtonId::back:
            save();
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", button->_id);
            break;
    }
}
