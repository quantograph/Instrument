#ifndef Main_h
#define Main_h

class PeakMeter;
class Button;
class SetGuitar;
class SetSynth;
class SetBand;
class SetAudio;
class Storage;

class Main : public Window {
public:
    Main();
    ~Main();

    SetAudio* _setAudio{};
    SetGuitar* _setGuitar{};
    SetSynth* _setSynth{};
    SetBand* _setBand{};
    PeakMeter* _peakMeter{};
    std::vector<Button*> _buttons;
    Window* _current{};
    Storage* _storage{};

    virtual bool init(Storage* storage, Settings* settings);
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
