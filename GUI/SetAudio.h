#ifndef SetAudio_h
#define SetAudio_h

class PeakMeter;
class Slider;
class Button;

class SetAudio : public Window {
public:
    SetAudio();

    PeakMeter* _peakMeter{};
    Slider* _slider{};
    Button* _backButton{};

    virtual bool init(Settings* settings, Window* parent);
    void setupButtons();

    void draw() override;
    void save();
    void onTouch(const TS_Point& point) override;
    void onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    void onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    void onPeakMeter(float left, float right) override;
    void onButton(Button* button) override;
};

#endif
