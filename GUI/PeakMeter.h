#ifndef PeakMeter_h
#define PeakMeter_h

class TouchScreen;

class PeakMeter {
public:
    PeakMeter(TouchScreen* screen, int x, int y, int height, int width);

    TouchScreen* _screen{};
    int _x;
    int _y;
    int _height; 
    int _width;
    
    void draw(float left, float right);
    void drawMeterBar(int x, int y, int width, float value);
};

#endif // Gui_h

