#ifndef Gui_h
#define Gui_h

class TouchScreen;
class Settings;
class Window;
class Main;

class Gui {
public:
    Gui();

    Settings* _settings{};
    Window* _current{};
    Main* _main{};
    uint32_t _firstTouch{0}; // Time of the first screen touch
    uint32_t _lastTouch{0}; // Time of the last screen touch
    bool _touched{}; // Whether the screen is set to "touched" state
    TS_Point _firstTouchPoint{}; // Point of the first touch
    TS_Point _lastTouchPoint{}; // Last touch point before the current touch

    bool init(Settings* settings);
    bool process();
    void onTouch(const TS_Point& point);
    void onRelease(const TS_Point& fromPoint, const TS_Point& toPoint);
    void onMove(const TS_Point& fromPoint, const TS_Point& toPoint);
    void onPeakMeter(float left, float right);
};

#endif // Gui_h

