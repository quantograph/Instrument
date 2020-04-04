#ifndef CheckBox_h
#define CheckBox_h

class CheckBox : public Control {
public:
    CheckBox(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ControlId id);

    bool _checked{};
    uint16_t _textColor = ILI9341_WHITE;
    int _textSize = 2;

    void update(bool check);
    bool onTouch(const TS_Point& point) override;
};

#endif
