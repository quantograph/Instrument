#ifndef PeakMeter_h
#define PeakMeter_h

class TouchScreen;

class PeakMeter : public Control {
public:
    PeakMeter(Settings::Data* settings, TouchScreen* screen, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    
    void draw(float left, float right);
    void drawMeterBar(uint16_t x, uint16_t y, uint16_t width, float value);
};

#endif // Gui_h

