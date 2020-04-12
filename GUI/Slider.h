#pragma once

class TouchScreen;

class Slider : public Control {
public:
    Slider(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ControlId id = ControlId::ctl_none);

    float _value{0.0};
    int16_t _lastX{-1};
    uint16_t _left{};
    uint16_t _right{};

    void update(TS_Point point);
    void drawBar(uint16_t x, uint16_t color);
    void setValue(float value);
    bool inside(const TS_Point& point);

    void draw() override;
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
};
