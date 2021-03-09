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
    this->startTime = millis();
    this->currentTime = millis();
    //wdt_disable();
}

void Serial_watchdog::update()
{
    this->currentTime = millis();
    // Make sure that millis() has not been restarted

    if (this->currentTime > this->startTime ){
        this->elapsedTime = this->currentTime - this->startTime;
    } else{
        this->startTime = millis(); 
        this->elapsedTime = 0;       
    }
    //Serial.println(this->elapsedTime/1000);
    
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
    
    Serial.println("onWDHard!");
    Serial.println(this->elapsedTime);
    Serial.println(this->startTime);
    Serial.println(this->currentTime);
    delay(50);
    wdt_enable(WDTO_15MS);    
}

void Serial_watchdog::onWDSoft()
{
    Serial.println("SerialWatchdog!");
    //Serial.print("Time since last sWD reset [ms]: ");
    //Serial.println(this->elapsedTime);
    //Serial3.end();
    Serial3.begin(9600);
    delay(100);
    
}