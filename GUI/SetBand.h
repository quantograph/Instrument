#ifndef SetBand_h
#define SetBand_h

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

    bool init(Settings* settings, Window* parent);
    void setupButtons();

    bool onControl(Control* control) override;
};

#endif
