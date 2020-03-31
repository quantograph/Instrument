#ifndef TextBox_h
#define TextBox_h

class TextBox : public Control {
public:
    TextBox(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int id = 0);

    String _text{};
    uint16_t _textColor = ILI9341_WHITE;
    int _textSize = 2;

    void update(String text);
    bool onTouch(const TS_Point& point) override;
};

#endif
