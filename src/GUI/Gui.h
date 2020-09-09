#pragma once

class TouchScreen;
class Storage;
class Window;
class Main;
class SettingsFile;

class Gui {
public:
    Gui();

    SettingsFile* _settingsFile{};
    Settings* _settings{};
    Window* _current{};
    Main* _main{};
    uint32_t _firstTouch{0}; // Time of the first screen touch
    uint32_t _lastTouch{0}; // Time of the last screen touch
    bool _touched{}; // Whether the screen is set to "touched" state
    TS_Point _firstTouchPoint{}; // Point of the first touch
    TS_Point _lastTouchPoint{}; // Last touch point before the current touch

    bool init(SettingsFile* settingsFile, Settings* settings);
    bool process();
    bool onTouch(const TS_Point& point);
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint);
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint);
    void onPeakMeter(float left, float right);
};
