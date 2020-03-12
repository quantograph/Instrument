#ifndef Gui_h
#define Gui_h

class TouchScreen;
class Settings;
class Window;
class Main;

class Gui {
public:
    Gui(TouchScreen* screen, Settings::Data* settings);

    TouchScreen* _screen{};
    Settings::Data* _settings{};
    Window* _current{};
    Main* _main{};
    uint32_t _firstTouch{0}; // Time of the first screen touch
    uint32_t _lastTouch{0}; // Time of the last screen touch
    bool _touched{}; // Whether the screen is set to "touched" state
    TS_Point _firstTouchPoint{}; // Point of the first touch
    TS_Point _lastTouchPoint{}; // Last touch point before the current touch

    bool process();
    void onTouch(TS_Point point);
    void onRelease(TS_Point fromPoint, TS_Point toPoint);
    void onMove(TS_Point fromPoint, TS_Point toPoint);
};

#endif // Gui_h

