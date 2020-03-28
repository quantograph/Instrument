#ifndef PeakMeter_h
#define PeakMeter_h

class TouchScreen;

class PeakMeter : public Control {
public:
    PeakMeter(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    
    void draw() override;
    void update(float left, float right);
    void drawMeterBar(uint16_t y, uint16_t height, float value);
};

#endif // Gui_h

