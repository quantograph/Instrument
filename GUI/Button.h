#ifndef Button_h
#define Button_h

class TouchScreen;

class Button : public Control {
public:
    enum ButtonId {
        none,
        back,
        guitar,
        synth,
        band,
        settings
    };

    Button(Settings* settings, Window* parent,  uint16_t x, uint16_t y, uint16_t width, uint16_t height, ButtonId id);

    ButtonId _id{none}; // Button ID
    String _text{};
    int _radius = 7; // Radius of the rectangle rounding
    uint16_t _backColor = ILI9341_NAVY; // Button's background color
    uint16_t _borderColor = ILI9341_WHITE; // Button's border color
    uint16_t _textColor = ILI9341_WHITE; // Button's text color
    int _textSize = 2; // Text size
    char _string[32];

    void init();
    void draw() override;
    void onTouch(const TS_Point& point) override;
    void onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
};

#endif
