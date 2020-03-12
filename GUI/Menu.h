#ifndef Menu_h
#define Menu_h

#include <ILI9341_t3.h>

class Menu {
    #define MAX_BUTTONS 10 // Maximum number of buttons in one menu
    // One button
    class Button {
    public:
        int _x = 0; // X coordinate of the button's upper left corner
        int _y = 0; // Y coordinate of the button's upper left corner
        int _width = 240; // Button's width
        int _height = 61; // Button's height
        int _radius = 7; // Radius of the rectangle rounding
        bool _selected = false; // Whether this button is selected
        uint16_t _backColor = ILI9341_NAVY; // Button's background color
        uint16_t _selectedBackColor = ILI9341_WHITE; // Button's background color, if selected
        uint16_t _borderColor = ILI9341_WHITE; // Button's border color
        uint16_t _selectedBorderColor = ILI9341_NAVY; // Button's border color
        uint16_t _textColor = ILI9341_WHITE; // Button's text color
        uint16_t _selectedTextColor = ILI9341_BLACK; // Button's text color
        String _text = "Button"; // Button's text
        int _textSize = 2; // Text size
        int _id = BUTTON_NONE; // Button's menu ID
        char _string[32];

        void draw();
        bool isTouched(const TS_Point& touchPoint);
    };

    // Collection of buttons
    class Selection {
    public:
        int _id = MENU_NONE;
        Button _buttons[MAX_BUTTONS];
        void draw(bool reset = true);
        int buttonPressed(const TS_Point& touchPoint);
        void next(bool next);
    };

public:
    enum MENU { // Menu ID's
        MENU_NONE,
        MENU_MAIN,
        MENU_GUITAR,
        MENU_SYNTH,
        MENU_BAND,
        MENU_SETUP
    };

    enum BUTTONS { // Button ID's
        // Main
        BUTTON_NONE,
        BUTTON_GUITAR,
        BUTTON_SYNTH,
        BUTTON_BAND,
        BUTTON_SETUP,
        BUTTON_EXIT,
        // Guitar
        BUTTON_FLANGE,
        BUTTON_CHORUS,
    };

    Menu();
    void init();
    void touched(uint32_t touched);
    void processScreen();
    void selected(int id);
    void showValue(const char* value);
    void save();
    void showInstrument();
    int _selectedId; // ID of the currently selected button
    uint32_t _lastScreenTouch = 0; // Time of the last screen touch
    Selection* _current = nullptr; // Current menu

    Selection _main; // The main menu
    Selection _guitar; // "Guitar" menu
    Selection _synth; // "Synth" menu
    Selection _band; // "Band" menu
    Selection _setup; // "Setup" menu
};

#endif
