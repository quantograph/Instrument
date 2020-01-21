#include "Misc.h"

// Shows a number as bits
/*void ShowBits(uint32_t value, String& s) {
    char buffer[8];

    if(value == 0) {
        s = ",";
        return;
    }

    for(int i = 0; i < 32; ++i) {
        if(bitRead(value, i)) {
            sprintf(buffer, "%d,", i + 1);
            s = s + buffer;
        }
    }
}*/

// Writes to the top of the screen
/*void LogScreen(const char* line) {
    g_screen._screen.setTextColor(ILI9341_WHITE);
    g_screen._screen.setTextSize(2);
    g_screen._screen.setCursor(0, 0);
    g_screen._screen.fillRect(0, 0, 300, 40, ILI9341_BLACK);
    g_screen._screen.print(line);
}*/