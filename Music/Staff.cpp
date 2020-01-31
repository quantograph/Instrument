#include "Misc.h"
#include "Keys.h"
#include "Staff.h"

//=================================================================================================
Staff::Staff() {

}
/*
//=================================================================================================
bool Staff::init(int x, int y) {
    _x = x;
    _y = y;
    DrawStaff();
    
    return true;
}

//=================================================================================================
// Draws music staff
void Staff::DrawStaff() {
    int mainLength; // Length of the main staff's lines
    int extraLength; // Length of the extra (above and below) lines
    int offset; // Left offset of the whole staff
    int extraOffset; // Left offset of the extra lines
    int mainColor = ILI9341_WHITE; // Color for the main staff lines
    int extraColor = ILI9341_DARKGREY; // Color for the extra staff lines
    int lines = 0;
    int i;
    int x = _x;
    int y = _y;
    int cleffOffsetX = 0;
    int cleffOffsetY = 0;

    // Set all sizes
    mainLength = g_screen._width * 0.6;
    offset = (g_screen._width - mainLength) / 2;
    extraLength = mainLength * 0.5;
    extraOffset = (mainLength - extraLength) / 2;
    _lineSpace = 15;

    // Extra lines above
    y = _y + _lineSpace;
    x = _x + offset + extraOffset;
    for(i = 0; i < 2; i++) {
        g_screen._screen.drawLine(x, y, x + extraLength, y, extraColor);
        y += _lineSpace;
        lines++;
    }

    // Treble cleff lines
    x = _x + offset;
    for(i = 0; i < 5; i++) {
        g_screen._screen.drawLine(x, y, x + mainLength, y, mainColor);
        y += _lineSpace;
        lines++;
    }

    // Treble cleff
    cleffOffsetX = 30;
    cleffOffsetY = 50;
    TrebleClef(_x + cleffOffsetX, _y);
    TrebleClef(_x + cleffOffsetX, _y + 1);
    TrebleClef(_x + cleffOffsetX + 1, _y);

    // Middle C line
    x = _x + offset + extraOffset;
    g_screen._screen.drawLine(x, y, x + extraLength, y, extraColor);
    _middleC = y;
    y += _lineSpace;
    lines++;

    // Bass cleff lines
    x = _x + offset;
    for(i = 0; i < 5; i++) {
        g_screen._screen.drawLine(x, y, x + mainLength, y, mainColor);
        y += _lineSpace;
        lines++;
    }

    // Bass clef
    cleffOffsetX = 46;
    cleffOffsetY = 135;
    BassClef(_x + cleffOffsetX, _y + cleffOffsetY);
    BassClef(_x + cleffOffsetX, _y + cleffOffsetY + 1);
    BassClef(_x + cleffOffsetX + 1, _y + cleffOffsetY);

    // Extra lines below
    x = _x + offset + extraOffset;
    for(i = 0; i < 2; i++) {
        g_screen._screen.drawLine(x, y, x + extraLength, y, extraColor);
        y += _lineSpace;
        lines++;
    }

    _staffBottom = y + _lineSpace;
}

//=================================================================================================
void Staff::TrebleClef(int x, int y) {
    Point points[] = { {2.0,6.6}, {1.4,7.0}, {2.0,7.6}, {2.6,7.0}, {1.8,2.0}, {2.2,1.0}, {2.6,2.0}, 
        {1.0,4.5}, {1.2,5.6}, {2.0,6.0}, {2.8,5.8}, {3.1,5.0}, {2.3,3.9}, {1.6,4.8}, {2.0,5.3} };
    float scale = 120.0 / 8.0;

    for(size_t i = 0; i < (sizeof(points) / sizeof(Point)) - 1; ++i) {
        g_screen._screen.drawLine(x + points[i]._x   * scale, y + points[i]._y   * scale, 
                                  x + points[i+1]._x * scale, y + points[i+1]._y * scale, ILI9341_CYAN);
    }
}

//=================================================================================================
void Staff::BassClef(int x, int y) {
    Point points[] = { {0.2,3.3}, {1.3,2.8}, {2.2,1.6}, {2.2,0.5}, {1.4,0.0}, {0.5,0.5}, {0.8,1.3}, {1.2,0.8} };
    float scale = 120.0 / 8.0;

    for(size_t i = 0; i < (sizeof(points) / sizeof(Point)) - 1; ++i) {
        g_screen._screen.drawLine(x + points[i]._x   * scale, y + points[i]._y   * scale, 
                                  x + points[i+1]._x * scale, y + points[i+1]._y * scale, ILI9341_CYAN);
    }

    g_screen._screen.fillCircle((int16_t)(x + 2.7 * scale), (int16_t)(y + 0.5 * scale), (int16_t)(0.2 * scale), ILI9341_CYAN);
    g_screen._screen.fillCircle((int16_t)(x + 2.7 * scale), (int16_t)(y + 1.5 * scale), (int16_t)(0.2 * scale), ILI9341_CYAN);
}

//=================================================================================================
void Staff::DrawNote(int note, bool draw) {
    int y;
    int x = 160;
    int radius = 6;
    float line;
    bool onLine = false; // Whether this note is on the staff line or between

    if(note == NO_NOTE)
        return;

    // Get the line number for this note and see if it's located on the line
    line = GetNoteLine(note);
    if(abs(line - (int)line) < 0.1f)
        onLine = true;

    // Make sure adjacent notes don't overlap
    if(onLine)
        x += radius * 2;

    //Serial.printf("------ Line %f, draw=%d\n", draw);
    y = _middleC - line * (_lineSpace);
    if(draw) {
        g_screen._screen.fillCircle(x, y, radius, ILI9341_RED);
    } else {
        g_screen._screen.fillCircle(x, y, radius, ILI9341_BLACK);
        if(onLine)
            g_screen._screen.drawLine(x - radius, y, x + radius, y, ILI9341_WHITE);
    }
}

//=================================================================================================
// Gets note's line number on the staff. Middle C line is 0.0
// Whole numbers for the notes on the line, .5 for notes between the lines. 
float Staff::GetNoteLine(int note) {
    float line = 0.0; // Line number to return
    int from;
    int to;
    int i;
    bool bass;

    // Get note's position relative to middle C
    if(note >= Keys::_cNumber) { // Treble clef
        from = Keys::_cNumber;
        to = note;
        bass = false;
    } else { // Bass clef
        from = note;
        to = Keys::_cNumber;
        bass = true;
    }

    // Move through the note range
    for(i = from; i < to; i++) {
        if(!IsSharp(i))
            line += 0.5f;
    }

    if(bass) // Negative for bass cleff
        line = -line;

    return line;
}

//=================================================================================================
// Finds whether a note is a sharp/flat
// 0  1      2  3      4  5  6      7  8      9  10    11
// C  C#/Db  D  D#/Eb  E  F  F#/Gb  G  G#/Ab  A  A#/Bb  B
bool Staff::IsSharp(char note) {
    int number; // Note number in C Major scale

    number = note % 12;
    switch(number) {
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            sprintf(_string, "Sharp %d (%d)", note, number);
            Serial.println(_string);
            return true;

        default:
            sprintf(_string, "Not Sharp %d (%d)", note, number);
            Serial.println(_string);
            return false;
    }
}

//=================================================================================================
// Shows saxophone keys pressed
void Staff::test() {
}
*/