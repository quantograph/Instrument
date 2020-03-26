#ifndef Main_h
#define Main_h

class PeakMeter;
class Slider;
class Button;

class Main : public Window {
public:
    Main();

    PeakMeter* _peakMeter{};
    Slider* _slider{};
    std::vector<Button*> _buttons;

    virtual bool init(TouchScreen* screen, Settings::Data* settings);
    void setupButtons();
    void onPeakMeter(float left, float right);
    void onTouch(TS_Point point) override;
    void onRelease(TS_Point fromPoint, TS_Point toPoint) override;
    void onMove(TS_Point fromPoint, TS_Point toPoint) override;
    void onButton(Button* button) override;
};

#endif // Main_h
