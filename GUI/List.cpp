#include "../Devices/Devices.h"
#include "Control.h"
#include "Button.h"
#include "Window.h"
#include "List.h"

//=================================================================================================
List::List() {

}

//=================================================================================================
bool List::init(Settings* settings, Window* parent, ControlId id) {
    Window::init(settings, parent, id);

    // Get the list size
    _listWidth = _settings->_screen->_width - 2;
    _listHeight = _settings->_screen->_height - 2;
    _itemHeight = _listHeight / _itemsToShow;

    return true;
}

//=================================================================================================
// Get the current scroll
int16_t List::getScroll() {
    int16_t scroll = _scroll + _moveScroll;

    if(scroll < 0)
        scroll = 0;

    uint16_t max = _items.size() - _itemsToShow;
    if(scroll > max)
        scroll = max;

    return scroll;
}

//=================================================================================================
void List::draw() {
    uint16_t scroll = getScroll();
    //Serial.printf("draw: scroll=%d\n", scroll);
    if(scroll == _lastScroll)
        return;

    _lastScroll = scroll;
    Window::draw();

    _settings->_screen->_screen.setTextColor(_textColor);
    _settings->_screen->_screen.setTextSize(_textSize);
    uint16_t y = 2;
    for(uint16_t index = getScroll(); index < _items.size(); ++index) {
        _settings->_screen->_screen.setCursor(10, y + 17);
        _settings->_screen->_screen.print(_items[index].first);
        
        y += _itemHeight;
        if(y > (_listHeight - _itemHeight))
            break;

        _settings->_screen->_screen.drawFastHLine(0, y, _listWidth, ILI9341_NAVY);
    }

    _settings->_screen->_screen.drawRect(1, 1, _listWidth, _listHeight, ILI9341_NAVY);
}

//=================================================================================================
bool List::onTouch(const TS_Point& point) {
    Window::onTouch(point);

    _touchPoint = point;
    _touched = true;
    Serial.printf("List::onTouch\n");

    return true;
}

//=================================================================================================
bool List::onRelease(const TS_Point& fromPoint, const TS_Point& toPoint) {
    _scroll = getScroll();
    _lastScroll = _scroll;

    uint16_t move = abs(fromPoint.y - toPoint.y);
    if(move < _itemHeight && _touched) {
        int16_t index = _scroll + toPoint.y / _itemHeight;
        index = std::max(index, (int16_t)0);
        index = std::min(index, (int16_t)(_items.size() - 1));
        _selectedString = _items[index].first;
        _selectedId = (ControlId)_items[index].second;
        Serial.printf("List::onRelease, selected: %s (%d)\n", _selectedString.c_str(), _selectedId);
        _touched = false;
        _parent->onBack(this);
    }

    return true;
}

//=================================================================================================
bool List::onMove(const TS_Point& fromPoint, const TS_Point& toPoint) {
    _moveScroll = (_touchPoint.y - toPoint.y) / _itemHeight;
    //Serial.printf("Scroll: (%3d-%3d)%3d / %3d = %3d\n", _touchPoint.y, toPoint.y, _touchPoint.y - toPoint.y, _itemHeight, _moveScroll);
    draw();

    return true;
}
