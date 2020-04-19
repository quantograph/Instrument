#pragma once

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class SetAudio : public Window {
public:
    SetAudio();

    PeakMeter* _peakMeter{};
    CheckBox* _micCheck{};
    CheckBox* _lineCheck{};
    Slider* _slider{};
    Button* _backButton{};
    char _string[32];

    bool init(Settings* settings, Window* parent);
    void setupButtons();
    void draw() override;
    void updateInput();
    void updateLevel(bool getSlider);

    void onPeakMeter(float left, float right) override;
    bool onControl(Control* control) override;
};
