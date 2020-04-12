#pragma once

class Fingers {
    struct Key {
        int _x = -1;
        int _y = -1;
    };

    InstrumentInfo* _instrument = nullptr;
    int _x = -1;
    int _y = -1;
    Key _keys[24];
    //uint16_t _backColor = ILI9341_YELLOW;
    //uint16_t _pressedColor = ILI9341_YELLOW;
    //uint16_t _freeColor = ILI9341_BLACK;
    //uint16_t _errorColor = ILI9341_RED;
    int _keyStep = 0;
    int _shift = 0;
    int _radius = 0;
    int _smallRadius = 0;
    int _keyCount = 0;

public:
    Fingers();
    bool init(InstrumentInfo* instrument, int x, int y);
    void initSaxophone();
    void showSaxophone(uint32_t keys, bool error);
};
