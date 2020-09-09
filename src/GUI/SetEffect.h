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
    virtual ~SetEffect();

    EffectSettings* _effectSettings{};
    TextBox* _effect{};
    List* _effectsList{};
    Button* _backButton{};
    std::vector<Slider*> _sliders{};

    bool init(Settings* settings, Window* parent);
    void reset();
    void showEffect();
    void showTitles();
    void activate(EffectSettings* effectSettings);
    void setupButtons();
    void showSliders(int number);
    // Virtuals
    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};
