#ifndef SetGuitar_h
#define SetGuitar_h

class PeakMeter;
class SetEffect;
class Slider;
class Button;
class TextBox;
class CheckBox;

class SetGuitar : public Window {
public:
    SetGuitar();

    SetEffect* _setEffect{};
    TextBox* _effect1{};
    TextBox* _effect2{};
    CheckBox* _singleCheck{};
    CheckBox* _doubleCheck{};
    Window* _current{};
    Button* _backButton{};

    virtual bool init(Settings* settings, Window* parent);
    void setupButtons();
    void updateNumber();

    void draw() override;
    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};

#endif