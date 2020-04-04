#ifndef ListWindow_h
#define ListWindow_h

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class List : public Window {
public:
    List();

    Button* _backButton{};
    char _string[32];

    virtual bool init(Settings* settings, Window* parent);
    void setupButtons();

    void draw() override;
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onControl(Control* control) override;
};

#endif
