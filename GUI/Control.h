#ifndef Conrol_h
#define Conrol_h

class Window;

class Control {
public:
    // Control types
    enum Type {
        none,
        button,
        edit, // Edit box
        list // List of strings
    };

    Control(Settings* settings, Window* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, ControlId id);

    virtual ~Control() {}

    Settings* _settings{};
    Window* _parent{}; // Parent window of this control
    uint16_t _x{0}; // X coordinate of the control upper left corner
    uint16_t _y{0}; // Y coordinate of the control upper left corner
    uint16_t _width{0}; // Control width
    uint16_t _height{0}; // Control height
    String _text{""}; // Control text
    Type _type{Type::none}; // Control type
    ControlId _id{ControlId::ctl_none}; // Control ID

    virtual void draw() {}
    virtual bool onTouch(const TS_Point& point) { return false; }
    virtual bool onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) { return false; }
    virtual bool onMove(const TS_Point& fromPoint, const TS_Point& toPoint) { return false; }
    bool inside(const TS_Point& point);
};

#endif // Conrol_h
