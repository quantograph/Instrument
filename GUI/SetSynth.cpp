#include "../Devices/Devices.h"
#include "../Music/MusicDef.h"
#include "../Music/Synth.h"
#include "Gui.h"
#include "Window.h"
#include "Control.h"
#include "Button.h"
#include "List.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "SetEffect.h"
#include "SetSynth.h"

//=================================================================================================
SetSynth::SetSynth() {

}

//=================================================================================================
bool SetSynth::init(Settings* settings, Window* parent) {
    //Serial.printf("SetSynth::init: this=%p, parent=%p\n", this, parent);
    uint16_t y = 20;
    uint16_t height;
    uint16_t width;

    Window::init(settings, parent);
    _inputSettings = &_settings->_synthInput;

    // "Instrument" text box
    height = 30;
    _instrument = new TextBox(_settings, this, 10, y, _settings->_screen->_width * 0.8, height, 
                              ControlId::txt_instrument);
    _instrument->_frame = true;
    _instrument->_dropDown = true;
    _instrument->_text = _settings->_synthSettings._instrumentName;
    y += height + 30;

    // Instrument list
    _instrumentList = new List();
    _instrumentList->init(_settings, this, ControlId::wnd_instrument_list);
    _instrumentList->_items.push_back(std::make_pair("Piano", Instrument::ACOUSTIC_GRAND_PIANO));
    _instrumentList->_items.push_back(std::make_pair("Guitar (steel)", Instrument::ACOUSTIC_GUITAR_STEEL));
    _instrumentList->_items.push_back(std::make_pair("Guitar (distortion)", Instrument::DISTORTION_GUITAR));
    _instrumentList->_items.push_back(std::make_pair("Guitar (nylon)", Instrument::ACOUSTIC_GUITAR_NYLON));
    _instrumentList->_items.push_back(std::make_pair("Strings", Instrument::STRING_ENSEMBLE_1));
    _instrumentList->_items.push_back(std::make_pair("Trumpet", Instrument::TRUMPET));
    _instrumentList->_items.push_back(std::make_pair("Flute", Instrument::FLUTE));

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

    // "SetEffect" window
    _setEffect = new SetEffect();
    _setEffect->init(_settings, this);

    return true;
}

//=================================================================================================
// Makes all menu buttons
void SetSynth::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void SetSynth::draw() {
    updateNumber();
    _effect1->_text = _inputSettings->_effect1._effectName;
    _effect2->_text = _inputSettings->_effect2._effectName;
    Window::draw();
}

//=================================================================================================
void SetSynth::updateNumber() {
    //Serial.printf("SetSynth::updateNumber: %d\n", _settings->_guitarEffects);
    if(_inputSettings->_effects == 1) { // Single
        _singleCheck->update(true);
        _doubleCheck->update(false);
    } else if(_inputSettings->_effects == 2) { // Double
        _singleCheck->update(false);
        _doubleCheck->update(true);
    }
}

//=================================================================================================
bool SetSynth::onControl(Control* control) {
    switch(control->_id) {
        case ControlId::txt_instrument:
            _instrumentList->activate();
            break;

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
            //Serial.printf("SetSynth::onControl %s, parent=%p\n", control->_text.c_str(), _parent);
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR SetSynth::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}

//=================================================================================================
void SetSynth::onBack(Window* window) {
    //Serial.printf("SetSynth::onBack: ID=%d\n", window->_id);

    _settings->_gui->_current = this;

    // Instrument selected
    if(window->_id == ControlId::wnd_instrument_list) {
        _settings->_synthSettings._instrumentName = _instrumentList->_selectedString;
        _instrument->_text = _instrumentList->_selectedString;
        _settings->_synthSettings._instrument = (Instrument)_instrumentList->_selectedId;
        //Serial.printf("SetSynth::onBack, instrument: %s (%d)\n", _settings->_synthSettings._instrumentName.c_str(), _settings->_synthSettings._instrument);

        _settings->_synth->setInstrument((Instrument)_settings->_synthSettings._instrument);
    }

    draw();
}
