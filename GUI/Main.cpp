#include <vector>
#include "../Devices/Devices.h"
#include "../Devices/TouchScreen.h"
#include "Settings.h"
#include "PeakMeter.h"
#include "Control.h"
#include "Window.h"
#include "Main.h"

//=================================================================================================
Main::Main() {

}

//=================================================================================================
bool Main::init(TouchScreen* screen, Settings::Data* settings) {
    _screen = screen;
    _settings = settings;
    Window::init(_screen, _settings);

    _peakMeter = new PeakMeter(_screen, 0, 0, _screen->_height / 2, 10);

    return true;
}

//=================================================================================================
void Main::onPeakMeter(float left, float right) {
    _peakMeter->draw(left, right);
}
