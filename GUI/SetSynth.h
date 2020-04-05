#ifndef SetSynth_h
#define SetSynth_h

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class SetSynth : public Window {
public:
    SetSynth();

    Button* _backButton{};
    char _string[32];

    bool init(Settings* settings, Window* parent);
    void setupButtons();

    bool onControl(Control* control) override;
};

#endif
