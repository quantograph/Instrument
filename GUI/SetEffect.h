#ifndef SetEffect_h
#define SetEffect_h

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;
class Control;

class SetEffect : public Window {
public:
    SetEffect();

    EffectSettings* _effectSettings{};
    TextBox* _effect{};
    Button* _backButton{};

    virtual bool init(Settings* settings, Window* parent);
    void activate(EffectSettings* effectSettings);
    void setupButtons();

    void draw() override;
    bool onControl(Control* control) override;
};

#endif
