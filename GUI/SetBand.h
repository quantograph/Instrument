#pragma once

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class SetBand : public Window {
public:
    SetBand();

    Button* _backButton{};
    char _string[32];

    bool init(Settings* settings, Window* parent, ControlId id);
    void setupButtons();

    bool onControl(Control* control) override;
};
