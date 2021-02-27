#include <SerialWatchdog.h>

Serial_watchdog::Serial_watchdog(unsigned long WDTimeHard)
{
    this->WDTimeHard = WDTimeHard * 1000;
    this->WDTimeSoft = WDTimeHard * 1000;

    reset();
}

Serial_watchdog::Serial_watchdog(unsigned long WDTimeHard, unsigned long WDTimeSoft)
{
    this->WDTimeHard = WDTimeHard * 1000;
    this->WDTimeSoft = WDTimeSoft * 1000;
    reset();
}

void Serial_watchdog::reset()
{
    this->startTime = millis();
    this->currentTime = millis();
    wdt_disable();
}

void Serial_watchdog::update()
{
    this->currentTime = millis();
    this->elapsedTime = this->currentTime - this->startTime;
    Serial.print("Time since last sWD reset: ");
    Serial.println(this->elapsedTime/1000);
}

void Serial_watchdog::test()
{
    if (this->elapsedTime > this->WDTimeHard)
    {
        onWDHard();
    }
    if (this->elapsedTime > this->WDTimeSoft)
    {
        onWDSoft();
    }
}

void Serial_watchdog::updateAndTest()
{
    update();
    test();
}

void Serial_watchdog::onWDHard()
{
    wdt_enable(WDTO_15MS);
    Serial.println("onWDHard!");
}

void Serial_watchdog::onWDSoft()
{
    Serial.println("SerialWatchdog!");
}
