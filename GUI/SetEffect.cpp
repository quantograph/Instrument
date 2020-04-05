#include "../Devices/Devices.h"
#include "Gui.h"
#include "GuiMisc.h"
#include "Window.h"
#include "Control.h"
#include "Button.h"
#include "PeakMeter.h"
#include "Slider.h"
#include "List.h"
#include "TextBox.h"
#include "CheckBox.h"
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
    _effect->_text = "Effect";
    y += height + 30;

    // Effects list
    _effectsList = new List();
    _effectsList->init(_settings, this, ControlId::wnd_effect_list);

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
bool SetEffect::onControl(Control* control) {
    switch(control->_id) {
        case ControlId::btn_back:
            //Serial.printf("SetEffect::onButton %s, parent=%p\n", control->_text.c_str(), _parent);
            _parent->onBack(this);
            break;

        case ControlId::txt_effect:
            showEffectList();
            break;

        default:
            Serial.printf("##### ERROR SetEffect::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    return true;
}

//=================================================================================================
void SetEffect::showEffectList() {
    Serial.printf("=============== SetEffect::showEffectList\n");
    for(int type = EffectType::eff_none + 1; type < EffectType::eff_last; ++type) {
        String name{getEffectName((EffectType)type)};
        _effectsList->_items.push_back(std::make_pair(name, (EffectType)type));
    }

    _effectsList->activate();
}

//=================================================================================================
void SetEffect::onBack(Window* window) {
    //Serial.printf("SetEffect::onBack: ID=%d\n", window->_id);

    if(window->_id == ControlId::wnd_effect_list) {
        Serial.printf("SetEffect::onBack, effect: %s (%d)\n", _effectsList->_selectedString.c_str(), _effectsList->_selectedId);
    }

    _settings->_gui->_current = this;
    draw();
}
