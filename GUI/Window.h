#ifndef Window_h
#define Window_h

class TouchScreen;

class Window {
public:
    Window();
    ~Window();

    TouchScreen* _screen{};
    Settings::Data* _settings{};
    std::vector<Control*> _controls{};

    virtual void draw();
    virtual bool init(TouchScreen* screen, Settings::Data* settings);
    virtual void onPeakMeter(float left, float right) {}
};

#endif // Window_h
