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

    virtual bool init(Settings* settings, Window* parent);
    void setupButtons();

    void draw() override;
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onButton(Button* button) override;
    bool onCheckBox(CheckBox* checkBox) override;
};

#endif
