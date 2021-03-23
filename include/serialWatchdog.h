#include <Arduino.h>
#include <avr/wdt.h>


//WATCHDOG SERIAL
unsigned long WDTimeHard = 30*1000; //in secods (max: 4,294,967*1000)
unsigned long WDTimeSoft = 5*1000; //in secods (max: 4,294,967*1000)
unsigned long startMillis = millis();
unsigned long currentTime = millis();
unsigned long elapsedTime = 0;

/*************************************************************************************************************************************************/
//NAME:         onWDSoft
//DESCRPTION:   Actions to do after a period of Serial desconexion 
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void onWDSoft(){
    Serial.println("SerialWatchdog!");
    Serial.print("Time since last sWD reset [ms]: ");
    Serial.println(elapsedTime);
    //delay(100);
};

/*************************************************************************************************************************************************/
//NAME:         onWDHard
//DESCRPTION:   Actions to do after a period of critical Serial desconexion 
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void onWDHard(){
    Serial.println("onWDHard!");
    Serial.println(startMillis);
    Serial.println(currentTime);
    Serial.println(elapsedTime);
    wdt_enable(WDTO_15MS);    
};

/*************************************************************************************************************************************************/
//NAME:         SWDreset
//DESCRPTION:   Reset Serial watchdog countdown 
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void SWDreset(){
    startMillis = millis();
};

/*************************************************************************************************************************************************/
//NAME:         SWDupdate
//DESCRPTION:   Update elapsedTime since last reset
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void SWDupdate(){
    currentTime = millis();
    elapsedTime = abs(currentTime - startMillis);
};

/*************************************************************************************************************************************************/
//NAME:         SWDtest
//DESCRPTION:   Test if WD conditons are to be triggered
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void SWDtest(){
    if (elapsedTime > WDTimeHard)
    {
        onWDHard();
    } else if (elapsedTime > WDTimeSoft)
    {
        onWDSoft();
    } else {
        // Do nothing
    };
};

/*************************************************************************************************************************************************/
//NAME:         SWDupdateAndTest
//DESCRPTION:   SWDupdate() & SWDtest()
//INPUT:        Void
//RETURN:       Void
//NOTE:         
/*************************************************************************************************************************************************/
void SWDupdateAndTest(){
    SWDupdate();
    SWDtest();
};