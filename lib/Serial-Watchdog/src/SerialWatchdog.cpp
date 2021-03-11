#include <SerialWatchdog.h>

Serial_watchdog::Serial_watchdog(unsigned long WDTimeHard)
{
    unsigned long toSeconds = 1000;
    this->WDTimeHard = WDTimeHard * toSeconds;
    this->WDTimeSoft = WDTimeHard * toSeconds;
    reset();
}

Serial_watchdog::Serial_watchdog(unsigned long WDTimeHard, unsigned long WDTimeSoft)
{
    unsigned long toSeconds = 1000;
    this->WDTimeHard = WDTimeHard * toSeconds;
    this->WDTimeSoft = WDTimeSoft * toSeconds;
    reset();
}

void Serial_watchdog::reset()
{
    this->startMillis = millis();
    wdt_disable();
    return;
}

void Serial_watchdog::update()
{
    this->currentTime = millis();
    this->elapsedTime = abs(this->currentTime - this->startMillis);
    return;    
}
    //Serial.println(this->elapsedTime/1000);
    

void Serial_watchdog::test()
{
    if (this->elapsedTime > this->WDTimeHard)
    {
        onWDHard();
        return;
    }
    if (this->elapsedTime > this->WDTimeSoft)
    {
        onWDSoft();
        return;
    }
}

void Serial_watchdog::updateAndTest()
{
    update();
    test();
    return;
}

void Serial_watchdog::onWDHard()
{
    
    Serial.println("onWDHard!");
    Serial.println(this->startMillis);
    Serial.println(this->currentTime);
    Serial.println(this->elapsedTime);
    wdt_enable(WDTO_15MS);    
    return;
}

void Serial_watchdog::onWDSoft()
{
    Serial.println("SerialWatchdog!");
    Serial.print("Time since last sWD reset [ms]: ");
    Serial.println(this->elapsedTime);
    delay(100);
    return;
}