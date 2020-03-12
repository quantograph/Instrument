#include "Menu.h"

//=================================================================================================
Menu::Menu() {
}

//=================================================================================================
// Initializes the menu
void Menu::init() {
    int y;
    int i;

    // Main menu
    _main._id = MENU_MAIN;
    i = 0;

    _main._buttons[i]._id = BUTTON_GUITAR;
    _main._buttons[i]._x = 0;
    y = 0;
    _main._buttons[i]._y = y;
    _main._buttons[i]._text = "Guitar";
    ++i;

    _main._buttons[i]._id = BUTTON_SYNTH;
    _main._buttons[i]._x = 0;
    y += _main._buttons[i]._height;
    _main._buttons[i]._y = y;
    _main._buttons[i]._text = "Synth";
    ++i;

    _main._buttons[i]._id = BUTTON_BAND;
    _main._buttons[i]._x = 0;
    y += _main._buttons[i]._height;
    _main._buttons[i]._y = y;
    _main._buttons[i]._text = "Band";
    ++i;

    _main._buttons[i]._id = BUTTON_SETUP;
    _main._buttons[i]._x = 0;
    y += _main._buttons[i]._height;
    _main._buttons[i]._y = y;
    _main._buttons[i]._text = "Setup";
    ++i;

    _main._buttons[i]._id = BUTTON_EXIT;
    _main._buttons[i]._x = 0;
    y += _main._buttons[i]._height;
    _main._buttons[i]._y = y;
    _main._buttons[i]._text = "Exit";

    // Guitar menu
    _guitar._id = MENU_GUITAR;
    i = 0;

    _guitar._buttons[i]._id = BUTTON_FLANGE;
    _guitar._buttons[i]._x = 0;
    y = 0;
    _guitar._buttons[i]._y = y;
    _guitar._buttons[i]._text = "Flange";
    ++i;

    _guitar._buttons[i]._id = BUTTON_CHORUS;
    _guitar._buttons[i]._x = 0;
    y += _guitar._buttons[i]._height;
    _guitar._buttons[i]._y = y;
    _guitar._buttons[i]._text = "Chorus";
    ++i;

    _guitar._buttons[i]._id = BUTTON_EXIT;
    _guitar._buttons[i]._x = 0;
    y += _guitar._buttons[i]._height;
    _guitar._buttons[i]._y = y;
    _guitar._buttons[i]._text = "Exit";
    
    // Synth menu
    _synth._id = MENU_SYNTH;
    i = 0;

    _synth._buttons[i]._id = BUTTON_EXIT;
    _synth._buttons[i]._x = 0;
    y = 0;
    _synth._buttons[i]._y = y;
    _synth._buttons[i]._text = "Exit";

    // Band menu
    _band._id = MENU_BAND;
    i = 0;

    _band._buttons[i]._id = BUTTON_EXIT;
    _band._buttons[i]._x = 0;
    y = 0;
    _band._buttons[i]._y = y;
    _band._buttons[i]._text = "Exit";

    // Setup menu
    _setup._id = MENU_SETUP;
    i = 0;

    _setup._buttons[i]._id = BUTTON_EXIT;
    _setup._buttons[i]._x = 0;
    y = 0;
    _setup._buttons[i]._y = y;
    _setup._buttons[i]._text = "Exit";
}

//=================================================================================================
// Processed the touched sensors
void Menu::touched(uint32_t touched) {
    if(_current == nullptr) { // Not in the menu mode
        if(bitRead(touched, 10) && bitRead(touched, 22)) { // Show the menu
            Serial.println("Main menu");
            _current = &_main;
            _current->draw();
        }

        return;
    }

    /*String s;
    ShowBits(touched, s);
    Serial.printf("Menu touched: %s\n", s.c_str());*/

    if(bitRead(touched, 0)) {
        Serial.println("out");
        save();
    } else if(bitRead(touched, 1)) {
        Serial.println("up");
        _current->next(false);
    } else if(bitRead(touched, 13)) {
        Serial.println("down");
        _current->next(true);
    } else if(bitRead(touched, 12)) {
        Serial.println("in");
        selected(_selectedId);
    }
}

//=================================================================================================
// Reads the touch screen
void Menu::processScreen() {
    bool process = false;

    // Every time the screen is touched, we get a lot of touch signals. Read only once here.
    uint32_t now = millis();
    if(now > _lastScreenTouch + 200)
        process = true;
    
    _lastScreenTouch = now;
    if(!process)
        return;

    Serial.println("Menu touched");
    if(_current == nullptr) { // Not in the menu mode
        _current = &_main;
        _current->draw();
        g_screen.light(1.0);
    } else {
        int id = _current->buttonPressed(g_screen._touchPoint);
        if(id != BUTTON_NONE)
            selected(id);
    }
}

//=================================================================================================
// Draws one menu button, inicating its selection
void Menu::Button::draw() {
    uint16_t backColor;
    uint16_t borderColor;
    uint16_t textColor;

    // Set all colors, for selected state or not
    backColor = _selected ? _selectedBackColor : _backColor;
    borderColor = _selected ? _selectedBorderColor : _borderColor;
    textColor = _selected ? _selectedTextColor : _textColor;

    // Draw the button's frame
	g_screen._screen.fillRoundRect(_x, _y, _width, _height, _radius, backColor);
	g_screen._screen.drawRoundRect(_x, _y, _width, _height, _radius, borderColor);

    // Draw the text
    g_screen._screen.setCursor(_x + 10, _y + 20);
    g_screen._screen.setTextColor(textColor);
    g_screen._screen.setTextSize(_textSize);
    g_screen._screen.print(_text);
}

//=================================================================================================
bool Menu::Button::isTouched(const TS_Point& touchPoint) {
    if(touchPoint.x >= _x && touchPoint.y >= _y && 
       touchPoint.x <= _x + _width && touchPoint.y <= _y + _height)
        return true;
    else
        return false;
}

//=================================================================================================
// Draws the current menu
void Menu::Selection::draw(bool reset /* = true */) {
    for(int i = 0; i < 10; ++i) {
        Button* button = &_buttons[i];
        if(button->_id == BUTTON_NONE)
            continue;

        // Select the first button for the new menu
        if(reset) {
            if(i == 0) {
                button->_selected = true;
                g_menu._selectedId = button->_id;
            } else {
                button->_selected = false;
            }
        }

        button->draw();
    }
}

//=================================================================================================
// Selects the next or the previous button
void Menu::Selection::next(bool next) {
    Button* button = nullptr;
    Button* nextButton = nullptr;
    int i;

    // Find the currently selected button
    for(i = 0; i < MAX_BUTTONS; ++i) {
        button = &_buttons[i];
        if(button->_selected)
            break;
    }

    if(!button) {
        Serial.println("##### No selected button");
        return;
    }

    Serial.printf("Current id=%d\n", button->_id);
    if(next) { // Select the next button
        if(i == MAX_BUTTONS) { // End of all buttons
            Serial.println("##### i == MAX_BUTTONS");
            return;
        }

        nextButton = &_buttons[i + 1];
        if(nextButton->_id == BUTTON_NONE) { // No next button defined
            Serial.println("##### No next button defined");
            return;
        }
    } else { // Select the previous button
        if(i == 0) { // No previous button
            Serial.println("##### No previous button");
            return;
        }

        nextButton = &_buttons[i - 1];
    }

    button->_selected = false;
    nextButton->_selected = true;
    g_menu._selectedId = nextButton->_id;
    Serial.printf("Next id=%d\n", nextButton->_id);

    draw(false);
}

//=================================================================================================
int Menu::Selection::buttonPressed(const TS_Point& touchPoint) {
    for(int i = 0; i < 10; ++i) {
        Button* button = &_buttons[i];
        if(button->_id == BUTTON_NONE)
            continue;

        if(button->isTouched(g_screen._touchPoint)) {
            return button->_id;
        }
    }

    return BUTTON_NONE;
}

//=================================================================================================
void Menu::selected(int id) {
    Serial.printf("Selected %d\n", id);

    switch(id) {
        case BUTTON_GUITAR:
            _current = &_guitar;
            break;

        case BUTTON_SYNTH:
            _current = &_synth;
            break;

        case BUTTON_BAND:
            _current = &_band;
            break;

        case BUTTON_SETUP:
            _current = &_setup;
            break;

        case BUTTON_FLANGE:
            //g_audioInput.flange(0.5);
            save();
            return;

        case BUTTON_CHORUS:
            //g_audioInput.chorus();
            save();
            return;

        case BUTTON_EXIT:
            save();
            return;
    }

    g_screen.clear();
    _current->draw();
    /*if(_current->_id == MENU_PRESET_INSTRUMENTS) {
        showInstrument();
    }*/
}

//=================================================================================================
void Menu::save() {
    //Serial.printf("Menu::save(): %d\n", g_settings._data._midiInstrument);
    _current = nullptr;
    g_screen.clear();
    g_screen.light(0.0);
    //g_settings.Write();

    //g_keys.SetInstrument();
    //g_midi.init();
}

//=================================================================================================
void Menu::showValue(const char* value) {
	g_screen._screen.fillRect(0, 0, 240, 50, ILI9341_BLACK);
    g_screen._screen.setCursor(0, 0);
    g_screen._screen.setTextColor(ILI9341_WHITE);
    g_screen._screen.setTextSize(2);
    g_screen._screen.println(value);
}

//=================================================================================================
void Menu::showInstrument() {
    char string[64];
    //sprintf(string, "%s", g_midi._melodic[g_settings._data._midiInstrument - 1]._name);
    showValue(string);
    //g_midi.SetInstrument(0, g_settings._data._midiInstrument);
}
