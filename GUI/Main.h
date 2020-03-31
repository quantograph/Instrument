#ifndef Main_h
#define Main_h

class PeakMeter;
class Button;
class SetAudio;

class Main : public Window {
public:
    Main();
    ~Main();

    SetAudio* _setAudio{};
    PeakMeter* _peakMeter{};
    std::vector<Button*> _buttons;
    Window* _current{};

    virtual bool init(Settings* settings);
    void setupButtons();
    void onPeakMeter(float left, float right);

    void draw() override;
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onButton(Button* button) override;
    void onBack(Window* window) override;
};

#endif // Main_h
