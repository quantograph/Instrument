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

    Control(Settings::Data* settings, TouchScreen* screen, Window* parent, 
            uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    Settings::Data* _settings{};
    TouchScreen* _screen{};
    Window* _parent; // Parent window of this control
    uint16_t _x{0}; // X coordinate of the control upper left corner
    uint16_t _y{0}; // Y coordinate of the control upper left corner
    uint16_t _width{0}; // Control width
    uint16_t _height{0}; // Control height
    String _text{""}; // Control text
    Type _type{Type::none}; // Control type
    int _id{0}; // Button's menu ID

    virtual void draw();
    virtual void onTouch(TS_Point point) {}
    virtual void onRelease(TS_Point fromPoint, TS_Point toPoint) {}
    virtual void onMove(TS_Point fromPoint, TS_Point toPoint) {}
    bool inside(TS_Point point);
};

#endif // Conrol_h
