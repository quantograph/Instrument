#ifndef Window_h
#define Window_h

class TouchScreen;
class Control;
class Button;

class Window {
public:
    Window();
    virtual ~Window();

    Settings* _settings{};
    Window* _parent{};
    std::vector<Control*> _controls{};

    virtual void draw();
    virtual bool init(Settings* settings, Window* parent = nullptr);
    virtual void onTouch(const TS_Point& point) {}
    virtual void onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {}
    virtual void onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {}
    virtual void onPeakMeter(float left, float right) {}
    virtual void onButton(Button* button) {}
    virtual void onBack(Window* window) {}
};

#endif // Window_h
