#ifndef Window_h
#define Window_h

class TouchScreen;
class Control;
class Button;
class CheckBox;

class Window {
public:
    Window();
    virtual ~Window();

    Settings* _settings{};
    Window* _parent{};
    std::vector<Control*> _controls{};

    virtual void draw();
    virtual bool init(Settings* settings, Window* parent = nullptr);
    virtual bool onTouch(const TS_Point& point) { return false; }
    virtual bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) { return false; }
    virtual bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) { return false; }
    virtual void onPeakMeter(float left, float right) {}
    virtual bool onButton(Button* button) { return false; }
    virtual bool onCheckBox(CheckBox* checkBox) { return false; }
    virtual void onBack(Window* window) {}
};

#endif // Window_h
