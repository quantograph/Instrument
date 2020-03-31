#ifndef SetAudio_h
#define SetAudio_h

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class SetAudio : public Window {
public:
    SetAudio();

    PeakMeter* _peakMeter{};
    TextBox* _level{};
    TextBox* _inputText{};
    CheckBox* _micCheck{};
    CheckBox* _lineCheck{};
    Slider* _slider{};
    Button* _backButton{};
    char _string[32];

    virtual bool init(Settings* settings, Window* parent);
    void setupButtons();

    void draw() override;
    void updateInput();
    void updateLevel(bool getSlider);
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    void onPeakMeter(float left, float right) override;
    bool onButton(Button* button) override;
    bool onCheckBox(CheckBox* checkBox) override;
};

#endif
