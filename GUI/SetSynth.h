#pragma once

class SetEffect;
class Slider;
class Button;
class TextBox;
class CheckBox;
class List;

class SetSynth : public Window {
public:
    SetSynth();

    SetEffect* _setEffect{};
    TextBox* _instrument{};
    List* _instrumentList{};
    TextBox* _effect1{};
    TextBox* _effect2{};
    CheckBox* _singleCheck{};
    CheckBox* _doubleCheck{};
    Window* _current{};
    Button* _backButton{};
    InputSettings* _inputSettings{};

    bool init(Settings* settings, Window* parent);
    void setupButtons();
    void updateNumber();

    void draw() override;
    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};
