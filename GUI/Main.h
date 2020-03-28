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
    void onTouch(const TS_Point& point) override;
    void onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    void onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    void onButton(Button* button) override;
    void onBack(Window* window) override;
};

#endif // Main_h
