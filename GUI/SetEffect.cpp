#include "../Devices/Devices.h"
#include "../Music/Misc.h"
#include "../Music/Synth.h"
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

// Windows for selecting one effect type and/or changing its parameters

//=================================================================================================
SetEffect::SetEffect() {

}

//=================================================================================================
SetEffect::~SetEffect() {
    reset();
}

//=================================================================================================
bool SetEffect::init(Settings* settings, Window* parent) {
    Serial.printf("SetEffect::init: parentID=%d\n", parent->_id);
    uint16_t y = 20;
    uint16_t height;

    Window::init(settings, parent);

    // "Effect" box
    height = 30;
    _effect = new TextBox(_settings, this, 10, y, _settings->_screen->_width * 0.8, height, 
                          ControlId::txt_effect);
    _effect->_title = "Effect";
    _effect->_frame = true;
    _effect->_dropDown = true;
    y += height + 30;

    // Effects list
    _effectsList = new List();
    _effectsList->init(_settings, this, ControlId::wnd_effect_list);
    for(int type = EffectType::eff_none + 1; type < EffectType::eff_last; ++type) {
        String name{getEffectName((EffectType)type)};
        _effectsList->_items.push_back(std::make_pair(name, (EffectType)type));
    }

    // Sliders
    /*int id{sld_1};
    height = 50;
    for(int i = 0; i < 4; ++i) {
        Slider* slider = new Slider(_settings, this, 30, y, _settings->_screen->_width - 60, height, (ControlId)id++);
        slider->_hidden = true;
        _sliders.push_back(slider);
        y += height + 20;
    }*/

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
void SetEffect::activate(EffectSettings* effectSettings) {
    Serial.printf("SetEffect::activate\n");
    _effectSettings = effectSettings;
    showEffect();
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
void SetEffect::reset() {
    for(auto control : _sliders) {
        delete control;
    }

    _sliders.clear();
}

//=================================================================================================
void SetEffect::showEffect() {
    Slider* slider;
    float value;
    Serial.printf("SetEffect::showEffect (%p), effect: %s (%d)\n", 
                  _effectSettings, _effectSettings->_effectName.c_str(), _effectSettings->_effectType);

    _effect->_text = _effectSettings->_effectName;

    switch(_effectSettings->_effectType) {
        case EffectType::eff_clean:
            break;

        case EffectType::eff_chorus:
            /*showSliders(2);
            // Delay
            slider = _sliders[0];
            value = scale(_effectSettings->_chorus._delay, ChorusDelayMin, ChorusDelayMax, 0.0, 1.0);
            slider->setValue(value);
            // Voices
            slider = _sliders[1];
            value = scale(_effectSettings->_chorus._voices, ChorusVoicesMin, ChorusVoicesMax, 0.0, 1.0);
            slider->setValue(value);*/
            break;

        case EffectType::eff_flange:
            break;

        case EffectType::eff_reverb:
            break;

        case EffectType::eff_freeverb:
            break;

        case EffectType::eff_envelope:
            break;

        case EffectType::eff_delay:
            break;

        case EffectType::eff_bitcrusher:
            break;

        case EffectType::eff_waveshaper:
            break;

        case EffectType::eff_granular:
            break;

        default:
            Serial.printf("##### ERROR, SetEffect::showEffect: unknown effect type %d\n", 
                          _effectSettings->_effectType);
    }

    Window::draw();
    showTitles();
}

//=================================================================================================
void SetEffect::showTitles() {
    char buffer[32];
    Slider* slider;

    switch(_effectSettings->_effectType) {
        case EffectType::eff_clean:
            break;

        case EffectType::eff_chorus:
            /*// Delay
            slider = _sliders[0];
            sprintf(buffer, "Delay: %d", _effectSettings->_chorus._delay);
            slider->setTitle(buffer);
            // Voices
            slider = _sliders[1];
            sprintf(buffer, "Voices: %d", _effectSettings->_chorus._voices);
            slider->setTitle(buffer);*/
            break;

        case EffectType::eff_flange:
            break;

        case EffectType::eff_reverb:
            break;

        case EffectType::eff_freeverb:
            break;

        case EffectType::eff_envelope:
            break;

        case EffectType::eff_delay:
            break;

        case EffectType::eff_bitcrusher:
            break;

        case EffectType::eff_waveshaper:
            break;

        case EffectType::eff_granular:
            break;

        default:
            Serial.printf("##### ERROR, SetEffect::showValue: unknown effect type %d\n", 
                          _effectSettings->_effectType);
    }
}

//=================================================================================================
void SetEffect::showSliders(int number) {
    /*Slider* slider;
    for(int i = 0; i < 4; ++i) {
        slider = (Slider*)_sliders[i];
        if(i < number)
            slider->_hidden = false;
        else
            slider->_hidden = true;
    }*/
}

//=================================================================================================
bool SetEffect::onControl(Control* control) {
    Slider* slider;
    bool update{true};

    switch(control->_id) {
        // Going back to the parent
        case ControlId::btn_back:
            //Serial.printf("SetEffect::onButton %s, parent=%p\n", control->_text.c_str(), _parent);
            update = false;
            _parent->onBack(this);
            break;

        // Effect type list is activated
        case ControlId::txt_effect:
            update = false;
            _effectsList->activate();
            break;

        // Slider 1 for the effect parameters was moved
        case ControlId::sld_1:
            slider = (Slider*)_sliders[0];
            switch(_effectSettings->_effectType) {
                case EffectType::eff_chorus:
                    // Delay
                    _effectSettings->_chorus._delay = scale(slider->_value, 0.0, 1.0, ChorusDelayMin, ChorusDelayMax);
                    Serial.printf("SetEffect::onControl: chorus delay %0.2f -> %d\n",
                                  slider->_value, _effectSettings->_chorus._delay);
                    break;

                default:
                    break;
            }
            break;
            
        // Slider 2 for the effect parameters was moved
        case ControlId::sld_2:
            slider = (Slider*)_sliders[1];
            switch(_effectSettings->_effectType) {
                case EffectType::eff_chorus:
                    // Voices
                    _effectSettings->_chorus._voices = scale(slider->_value, 0.0, 1.0, ChorusVoicesMin, ChorusVoicesMax);
                    Serial.printf("SetEffect::onControl: chorus voices %0.2f -> %d\n",
                                  slider->_value, _effectSettings->_chorus._voices);
                    break;

                default:
                    break;
            }
            break;
            
        // Slider 3 for the effect parameters was moved
        case ControlId::sld_3:
            break;
            
        // Slider 4 for the effect parameters was moved
        case ControlId::sld_4:
            break;

        default:
            Serial.printf("##### ERROR SetEffect::onControl: unknown control ID d\n", control->_id);
            return false;
    }

    // Set the updated effect parameters
    if(update)
    {
        showTitles();
        switch(_parent->_id) {
            case wnd_set_guitar:
                _settings->_audio->updateEffects();
                break;

            case wnd_set_synth:
                _settings->_synth->updateEffects();
                break;

            default:
                break;
        }
    }

    return true;
}

//=================================================================================================
void SetEffect::onBack(Window* window) {
    Serial.printf("SetEffect::onBack: ID=%d\n", window->_id);

    _settings->_gui->_current = this;

    switch(window->_id) {
        case ControlId::wnd_effect_list: // Effect type selected
            _effectSettings->_effectType = (EffectType)_effectsList->_selectedId;
            _effectSettings->_effectName = _effectsList->_selectedString;
            _effect->_text = _effectSettings->_effectName;

            // Set new effect types either for the guitar (audio input) or for the synth
            switch(_parent->_id) {
                case wnd_set_guitar:
                    _settings->_audio->setEffects();
                    break;

                case wnd_set_synth:
                    _settings->_synth->setEffects();
                    break;

                default:
                    Serial.printf("SetEffect::onBack: unknown parent id %d\n", _parent->_id);
                    break;
            }

            showEffect();
            break;

        default:
            break;
    }

    draw();
}
