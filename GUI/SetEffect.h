#pragma once

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;
class List;
class Control;

class SetEffect : public Window {
public:
    SetEffect();

    EffectSettings* _effectSettings{};
    TextBox* _effect{};
    List* _effectsList{};
    Button* _backButton{};

    bool init(Settings* settings, Window* parent);
    void showEffect();
    void activate(EffectSettings* effectSettings);
    void setupButtons();

    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};
