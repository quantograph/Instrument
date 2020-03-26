#ifndef SetAudio_h
#define SetAudio_h

class PeakMeter;
class Slider;

class SetAudio : public Window {
public:
    SetAudio();

    PeakMeter* _peakMeter{};
    Slider* _slider{};

    virtual bool init(TouchScreen* screen, Settings::Data* settings);
    void onPeakMeter(float left, float right);
    void onTouch(TS_Point point) override;
    void onRelease(TS_Point fromPoint, TS_Point toPoint) override;
    void onMove(TS_Point fromPoint, TS_Point toPoint) override;
};

#endif
