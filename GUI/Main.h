#ifndef Main_h
#define Main_h

class PeakMeter;

class Main : public Window {
public:
    Main();

    virtual bool init(TouchScreen* screen, Settings::Data* settings);
    void onPeakMeter(float left, float right);

    PeakMeter* _peakMeter{};
};

#endif // Main_h
