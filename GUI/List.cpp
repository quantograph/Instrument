#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "Window.h"
#include "List.h"

//=================================================================================================
List::List() {

}

//=================================================================================================
bool List::init(Settings* settings, Window* parent) {

    // Buttons
    setupButtons();

    return true;
}

//=================================================================================================
// Makes all menu buttons
void List::setupButtons() {
    int width = _settings->_screen->_width / 4;
    _backButton = new Button(_settings, this, _settings->_screen->_width - width, 
                             _settings->_screen->_height - width, width, width, ControlId::btn_back);
    _backButton->init();
}

//=================================================================================================
void List::draw() {
    _settings->_screen->_screen.fillScreen(ILI9341_BLACK);

}

//=================================================================================================
bool List::onTouch(const TS_Point& point) {
/*    if(_micCheck->onTouch(point) || _lineCheck->onTouch(point)) {
        //Serial.printf("===== List::onTouch check boxes: %dx%d\n", point.x, point.y);
        updateInput();
    } else if (_slider->onTouch(point)) {
        //Serial.printf("===== List::onTouch slider: %dx%d\n", point.x, point.y);
        updateLevel(true);
    } else {
        //Serial.printf("===== List::onTouch button: %dx%d\n", point.x, point.y);
        _backButton->onTouch(point);
    }
*/
    return true;
}

//=================================================================================================
bool List::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {

    return true;
}

//=================================================================================================
bool List::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {

    return true;
}

//=================================================================================================
bool List::onControl(Control* control) {
    //Serial.printf("List::: %s\n", button->_text.c_str());

    switch(control->_id) {
        case ControlId::btn_back:
            _parent->onBack(this);
            break;

        default:
            Serial.printf("##### ERROR: unknown button ID: d\n", control->_id);
            return false;
    }

    return true;
}
