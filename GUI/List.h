#ifndef ListWindow_h
#define ListWindow_h

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;

class List : public Window {
public:
    List();

    std::vector<std::pair<String, int> > _items;
    uint16_t _listWidth{};
    uint16_t _listHeight{};
    uint16_t _itemHeight{};
    int16_t _scroll{0};
    int16_t _lastScroll{-1};
    int16_t _moveScroll{0};
    uint16_t _itemsToShow{6};
    uint16_t _textColor = ILI9341_WHITE;
    int _textSize = 2;
    TS_Point _touchPoint{};
    String _selectedString{};
    int _selectedId{0};
    bool _ready{false};

    int16_t getScroll();

    void draw() override;
    bool init(Settings* settings, Window* parent, ControlId id) override;
    void activate() override;
    bool onTouch(const TS_Point& point) override;
    bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) override;
    bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) override;
};

#endif
