#include "CapSensor.h"

/*
Adafruit product page: http://www.adafruit.com/product/1982
Library: http://github.com/adafruit/Adafruit_MPR121

Connections: 
On the left sensor board, pin ADDR is connected to VIN to change its address to 0x5B.
By default, address of the right sensor board is 0x5A.
SDA and SCL on both sensor boards are connected to each other. 
SDA on the boards are connected to pin 18 (SDA0) on Teensy.
SCL on the boards are connected to pin 19 (SCL0) on Teensy.
IRQ is the Interrupt Request signal pin. It is pulled up to 3.3V on the breakout and when 
the sensor chip detects a change in the touch sense switches, the pin goes to 0V until the 
data is read over i2c.
*/

#define FRONT_INTERRUPT 1
#define LEFT_INTERRUPT 2
#define RIGHT_INTERRUPT 3

//=================================================================================================
CapSensor::CapSensor() {
}

//=================================================================================================
// Initializes the sensors
bool CapSensor::Init() {
    //SetInterrupts();

    // Front sensors
    if (!_capFront.begin(0x5A)) {
        Serial.println("##### Front CAP not found");
        return false;
    }

    // Left sensors
    if (!_capLeft.begin(0x5C)) { // MPR121 'ADDR' pin is connected to 'SDA'
        Serial.println("##### Left CAP not found");
        return false;
    }

    // Right sensors
    if (!_capRight.begin(0x5B)) { // MPR121 'ADDR' pin is connected to 'VIN'
        Serial.println("##### Right CAP not found");
        return false;
    }

    GetData(_capFront, _frontData, true);
    GetData(_capLeft, _leftData, true);
    GetData(_capRight, _rightData, true);

    // Set threshholds
    /*uint8_t touch = 6;
    uint8_t release = 3;
    _capFront.setThreshholds(touch, release);
    _capLeft.setThreshholds(touch, release);
    _capRight.setThreshholds(touch, release);*/

    return true;
}

//=================================================================================================
void CapSensor::GetData(Adafruit_MPR121& sensor, Data& data, bool first) {
    for(uint8_t i = 0; i < 12; ++i) {
        if(first)
            data._values[i]._first = sensor.filteredData(i);
        else
            data._values[i]._current = sensor.filteredData(i);
    }
}

//=================================================================================================
// Get touched bits using the raw data
uint16_t CapSensor::GetTouchedData(Data& data, bool show) {
    uint16_t bits = 0;

    //Serial.printf(">>>>>>> GetTouchedData\n");
    for(uint8_t i = 0; i < 12; ++i) {
        if(show)
            Serial.printf("%04d|%04d ", data._values[i]._current, data._values[i]._first);
        
        if(data._values[i]._current < data._values[i]._first * 0.9)
            bitSet(bits, i);
    }

    if(show)
        Serial.printf("=%06X\n", bits);

    return bits;
};

//=================================================================================================
bool CapSensor::GetTouched(uint32_t& mapped, bool onlyChanged, bool show) {
    /*uint16_t front = _capFront.touched();
    uint16_t left = _capLeft.touched();
    uint16_t right = _capRight.touched();*/

    /*if(!_changed)
        return false;

    _changed = false;*/

    GetData(_capFront, _frontData);
    GetData(_capLeft, _leftData);
    GetData(_capRight, _rightData);

    uint16_t front = GetTouchedData(_frontData, show);
    uint16_t left = GetTouchedData(_leftData, show);
    uint16_t right = GetTouchedData(_rightData, show);

    mapped = 0;

    uint32_t touched = front << 16;
    touched |= (left << 8) & 0xFF00;
    touched |= right & 0xFF;

    if(onlyChanged && _touched == touched) // Nothing has changed
        return false;

    _touched = touched;

    mapped = 0;
    for(int i = 0; i < 24; ++i) {
        bitWrite(mapped, _mapping[i] - 1, bitRead(touched, i));
    }

    /*ShowRaw();
    String f = " ";
    String l = " ";
    String r = " ";
    String m = " ";
    ShowBits(front, f);
    ShowBits(left, l);
    ShowBits(right, r);
    ShowBits(mapped, m);
    if(mapped)
        Serial.printf("CAP touched: front=%s(%03X), left=%s(%03X), right=%s(%03X) -> %s(%03X)\n", 
                      f.c_str(), front, l.c_str(), left, r.c_str(), right, m.c_str(), mapped);*/

    return true;
}

//=================================================================================================
// Setup the interrupts
bool CapSensor::SetInterrupts() {
    pinMode(FRONT_INTERRUPT, INPUT);
    digitalWrite(FRONT_INTERRUPT, HIGH);
    pinMode(FRONT_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(FRONT_INTERRUPT, OnFrontTouched, FALLING);

    pinMode(LEFT_INTERRUPT, INPUT);
    digitalWrite(LEFT_INTERRUPT, HIGH);
    pinMode(LEFT_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(LEFT_INTERRUPT, OnLeftTouched, FALLING);

    pinMode(RIGHT_INTERRUPT, INPUT);
    digitalWrite(RIGHT_INTERRUPT, HIGH);
    pinMode(RIGHT_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(RIGHT_INTERRUPT, OnRightTouched, FALLING);

    return true;
}

//=================================================================================================
void CapSensor::OnTouched() {
    GetTouched(_touched, true, true);

    String s = " ";
    ShowBits(g_cap._touched, s);
    Serial.printf(">>>> CAP: %s\n", s.c_str());
    _changed = true;
}

//=================================================================================================
void CapSensor::OnFrontTouched() {
    Serial.println(">>>> CAP front touched");
    g_cap.OnTouched();
}

//=================================================================================================
void CapSensor::OnLeftTouched() {
    Serial.println(">>>> CAP left touched");
    g_cap.OnTouched();
}

//=================================================================================================
void CapSensor::OnRightTouched() {
    Serial.println(">>>> CAP right touched");
    g_cap.OnTouched();
}

//=================================================================================================
void CapSensor::Test() {
    uint32_t mapped;
    String m = " ";

    //ShowRaw();

    GetTouched(mapped, false, true);
    ShowBits(mapped, m);
    Serial.printf("CAP test: %s(%03X)\n", m.c_str(), mapped);
}

//=================================================================================================
// Shows raw data from all sensors
void CapSensor::ShowRaw() {
    String front;
    String left;
    String right;

    GetRaw(&_capFront, front);
    Serial.printf("\n");

    GetRaw(&_capLeft, left);
    Serial.printf("\n");

    GetRaw(&_capRight, right);
    Serial.printf("\n");

    Serial.printf("\n");
}

//=================================================================================================
// Gets raw sensor data from one sensor
void CapSensor::GetRaw(Adafruit_MPR121* sensor, String& data) {
    uint16_t filtered;
    uint16_t baseline;

    for(uint8_t i = 0; i < 12; ++i) {
        filtered = sensor->filteredData(i);
        baseline = sensor->baselineData(i);

        Serial.printf("%04d-%04d ", filtered, baseline);
    }
}
