#pragma once

#include <Arduino.h>
#include <avr/wdt.h>

class Serial_watchdog
{
private:
    unsigned long WDTimeSoft; // Coutndown in seconds up to about 40 days
    unsigned long WDTimeHard; // Coutndown in seconds up to about 40 days
    unsigned long startMillis;
    unsigned long currentTime;
    unsigned long elapsedTime;

public:
    Serial_watchdog(unsigned long WDTimeHard);
    Serial_watchdog(unsigned long WDTimeHard, unsigned long WDTimeSoft);
    void reset();
    void update();
    void test();
    void updateAndTest();
    void onWDSoft();
    void onWDHard();
};
