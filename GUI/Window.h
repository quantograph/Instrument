#ifndef Window_h
#define Window_h

class Gui;
class Control;
class Button;
class CheckBox;
class TextBox;

class Window {
public:
    Window();
    virtual ~Window();

    Settings* _settings{};
    Window* _parent{};
    std::list<Control*> _controls{};

    virtual void draw();
    virtual bool init(Settings* settings, Window* parent);
    virtual bool onTouch(const TS_Point& point);
    virtual bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint);
    virtual bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint);
    virtual void onPeakMeter(float left, float right) {}
    virtual bool onControl(Control* control) { return false; }
    virtual void activate();
    virtual void onBack(Window* window) {}
};

#endif // Window_h
