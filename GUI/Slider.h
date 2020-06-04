#pragma once

class TouchScreen;

class Slider : public Control {
public:
    // Data ---------------------------------------------------------------------------------------
    String _title{};
    float _value{0.0};
    int16_t _lastX{-1};
    uint16_t _barTop{};
    uint16_t _barLeft{};
    uint16_t _barRight{};
    uint16_t _titleHeight{20};
    uint16_t _barHeight{};
    uint16_t _textColor = ILI9341_WHITE;
    int _textSize = 2;
    bool _ready{false};

    // Functions ----------------------------------------------------------------------------------
    Slider(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, 
           uint16_t height, ControlId id = ControlId::ctl_none);
    void activate();
    void update(TS_Point point);
    void drawBar(uint16_t x, uint16_t color);
    void setValue(float value, float min = 0.0, float max = 1.0);
    bool inside(const TS_Point& point);
    // Virtuals
    void draw() override;
    void setTitle(const char* title);
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
};
