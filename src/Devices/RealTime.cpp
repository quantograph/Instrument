#include "Devices.h"
#include "RealTime.h"

//=================================================================================================
bool RealTime::init() {
    setSyncProvider(getTeensy3Time);
    if (timeStatus() != timeSet) {
        Serial.println("##### ERROR: can't sync with RTC");
    }

    return true;
}

//=================================================================================================
bool RealTime::show() {
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d %02d/%02d/%d", hour(), minute(), second(), day(), month(), year());
    Serial.println(buffer);

    return true;
}

//=================================================================================================
bool RealTime::sync() {
    #define TIME_HEADER  "T"   // Header tag for serial time sync message
    time_t pcTime = 0L;
    const time_t VALID_TIME = 1357041600; // Jan 1 2013 

    Serial.printf("sync\n");
    if(!Serial.available()) {
        Serial.printf("no serial\n");
        return false;
    }

    Serial.printf("Serial.find\n");
    if(Serial.find(TIME_HEADER)) {
        pcTime = Serial.parseInt();
        Serial.printf("pcTime=%d\n", pcTime);
        if( pcTime > VALID_TIME) { // check the value is a valid time (greater than Jan 1 2013)
            Teensy3Clock.set(pcTime); // set the RTC
            setTime(pcTime);
            Serial.printf("Set PC time to %d\n", pcTime);
        }
    }

    return true;
}

//=================================================================================================
time_t RealTime::getTeensy3Time()
{
  return Teensy3Clock.get();
}
