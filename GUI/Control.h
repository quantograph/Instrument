#ifndef Conrol_h
#define Conrol_h

class Control {
public:
    // Control types
    enum Type {
        none,
        button,
        edit, // Edit box
        list // List of strings
    };

    Control(Settings::Data* settings);

    Type _type{Type::none}; // Control type
    int _x{0}; // X coordinate of the control upper left corner
    int _y{0}; // Y coordinate of the control upper left corner
    int _width{0}; // Control width
    int _height{0}; // Control height
    String _text{""}; // Control text
    int _id{0}; // Button's menu ID
    Settings::Data* _settings{};

    virtual void draw();
};

#endif // Conrol_h
