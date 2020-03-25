#ifndef Slider_h
#define Slider_h

class TouchScreen;

class Slider : public Control {
public:
    Slider(Settings::Data* settings, TouchScreen* screen, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    float _value{0.0};
    int16_t _lastY{-1};

    void init();
    void draw(TS_Point point);
    void drawBar(uint16_t y, uint16_t color);
    float getValue();
    void onTouch(TS_Point point) override;
    void onRelease(TS_Point fromPoint, TS_Point toPoint) override;
    void onMove(TS_Point fromPoint, TS_Point toPoint) override;
};

#endif
