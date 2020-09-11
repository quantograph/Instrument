#include <QuantoGraph.h>

TouchScreen g_screen = TouchScreen();

// the setup function runs once when you press reset or power the board
void setup() {
    g_screen.init();
    g_screen._screen.setTextColor(ILI9341_WHITE);
    g_screen._screen.setTextSize(2);

    Serial.printf("Screen resolution: width=%d, height=%d\n\n", g_screen._width, g_screen._height);
}

// the loop function runs over and over again until power down or reset
void loop() {
    bool touched{g_screen.read()}; // Whether the screen is being touched
    String message{""};
    char buffer[128];

    g_screen._screen.fillRect(0, 0, g_screen._width, g_screen._height, ILI9341_BLACK);
    g_screen._screen.setCursor(0, 0);

    if(touched) {
        // Print the touch point to the serial port
        Serial.printf("Touched: x=%3d, y=%3d, z=%3d\n", 
                      g_screen._touchPoint.x, g_screen._touchPoint.y, g_screen._touchPoint.z);

        // Show the touch point on the screen
        sprintf(buffer, "Touched:\n x=%3d\n y=%3d\n z=%3d",
                g_screen._touchPoint.x, g_screen._touchPoint.y, g_screen._touchPoint.z);
        g_screen._screen.print(buffer);
    } else {
        g_screen._screen.print("Not touched");
    }

    delay(100);
}
