#include "../Devices/Devices.h"
#include "Settings.h"
#include "Control.h"

//=================================================================================================
Control::Control(Settings::Data* settings) : _settings(settings) {

}

//=================================================================================================
// Draw this control
void Control::draw() {
    switch(_type) {
        case button:
            break;

        case edit:
            break;

        case list:
            break;

        default:
            Serial.printf("Unknown control type: %d\n", _type);
    }
}
