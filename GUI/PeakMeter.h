#pragma once

class TouchScreen;

class PeakMeter : public Control {
public:
    PeakMeter(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ControlId id = ControlId::ctl_none);
    
    void draw() override;
    void update(float left, float right);
    void drawMeterBar(uint16_t y, uint16_t height, float value);
};
