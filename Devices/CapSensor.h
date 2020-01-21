#ifndef CapSensor_h
#define CapSensor_h

#include <Arduino.h>

class CapSensor {

// Values of one touch pad
struct Value {
    uint16_t _first = 0; // First value read from the sensor
    uint16_t _current = 0; // Current value
};

// Values of all touch pads of one sensor board
struct Data {
    Value _values[12]; // Values of all sensors
};

public:
    CapSensor();
    bool Init();
    uint16_t GetTouchedData(Data& data, bool show = false);
    bool GetTouched(uint32_t& touched, bool onlyChanged = true, bool show = false);
    bool SetInterrupts();
    void OnTouched();
    void Test();
    //void GetData(Adafruit_MPR121& sensor, Data& data, bool first = false);
    //void GetRaw(Adafruit_MPR121* sensor, String& data);
    void ShowRaw();
    static void OnFrontTouched();
    static void OnLeftTouched();
    static void OnRightTouched();
    bool _changed = false;

    Adafruit_MPR121 _capFront;
    Adafruit_MPR121 _capLeft;
    Adafruit_MPR121 _capRight;
    Data _frontData;
    Data _leftData;
    Data _rightData;
    uint32_t _touched = 0;
    uint8_t _mapping[24] = { 16, 18, 20, 22, 
                             21, 19, 17, 15, 
                              4,  6,  8, 10, 
                              9,  7,  5,  3,
                             14, 24, 23, 13,
                              2, 12, 11,  1 };
};

#endif
