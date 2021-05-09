///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             INCLUDES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <frameProcessor.h>
#include <NoDelay.h>

#include <Sensor.h>
#include <Actuator.h>
#include <SerialWatchdog.h>
#include <freemem.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            TODO
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Change Serial readString for non blocking
//Test and tune daqs (prop valves)
//Test and tune digipot
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Test vars
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Memory initialization
Sensor *sensor1;
Sensor *sensor2;
Sensor *sensor3;
Sensor *sensor4;
Sensor *sensor5;
Sensor *sensor6;
Sensor *sensor9;
Sensor *sensor10;
// TODO test and impelement
//const int PWMPin1 = 46; // Pin of the PWM for the DAC 1
//const int PWMPin2 = 44; // Pin of the PWM for the DAC 2
//digital
Actuator *actuator1;
Actuator *actuator2;
Actuator *actuator3;
Actuator *actuator4;
Actuator *actuator5;
Actuator *actuator6;
Actuator *actuator7;
Actuator *actuator8;
//digipot
//Actuator *actuator9;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Global Variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Placeholders
int desiredTempPool = 150;    // Temp degrees * 10 [Initial]
int desiredTempFloor = 150;   // Temp degrees * 10 [Initial]
int desiredState = 00;        // [00 -60] Current State [Initial]
//Control
int hysteresisTemp = 20;      // Temp degrees * 10 [Initial] pm 1 deg
//Alarms
int alarmHighTemp = 840;      // Temp degrees * 10  //any sensor
int alarmHighTempImp = 840;   // Temp degrees * 10  //Pool imp sensor
bool alarmState = false;      //MCU -> RPI    Alarm state
//Non blocking updates
noDelay periodicUpdate(1000); //MCU -> RPI    Periodic update timmer in ms
noDelay criticalUpdate(100);  //MCU -> RPI    Periodic update timme in ms

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             Helper Global Functions 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <scenarios.h>
#include <parseCommander.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Setup
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    //PC
    Serial.begin(9600);
    delay(100);
    Serial.println("Hi");

    //Rpi
    Serial3.begin(9600);
    delay(100);

    //PWMout
    //pinMode(PWMPin1, OUTPUT); // Sets the pin as output
    //pinMode(PWMPin2, OUTPUT); // Sets the pin as output

    sensor1 = new Sensor(22, "st_a", "DS18B20");
    sensor2 = new Sensor(23, "st_b", "DS18B20");
    sensor3 = new Sensor(24, "st_c", "DS18B20");
    sensor4 = new Sensor(25, "st_d", "DS18B20");
    sensor5 = new Sensor(26, "st_e", "DS18B20");
    sensor6 = new Sensor(27, "st_f", "DS18B20");
    //sensor7 = new Sensor(28, "st_g", "DS18B20");
    //sensor8 = new Sensor(29, "sa_a", "DS18B20");
    sensor9 = new Sensor(30, "st_b", "DHT22");

    sensor10 = new Sensor(A0, "sp_a", "MBS3000");

    actuator1 = new Actuator(38, "vk1");
    actuator2 = new Actuator(39, "vk2");
    actuator3 = new Actuator(40, "vk3");
    actuator4 = new Actuator(41, "vk4");

    actuator5 = new Actuator(43, "mPool");
    actuator6 = new Actuator(47, "mFloor");
    actuator7 = new Actuator(48, "mHeatEx");
    actuator8 = new Actuator(49, "mENABLE");
    //Digipot
    //actuator9 = new Actuator(53, "digipot10k","digipot");

    //Deactivate motors at startup
    scenario_STOP();
    actuator8->on(); //It goes the other way arround 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Loop
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{   
    //Serial.println("About to read serial");
    parseSerialCommand();        //Read and parse incoming data

    if (periodicUpdate.update()) //Checks to see if set time has past
    {
        // Update sensor values
        //Serial.println("About to read sensors");
        sensor1->read();
        sensor2->read();
        sensor3->read();
        sensor4->read();
        sensor5->read();
        sensor6->read();
        //Read DHT22
        sensor9->read();
        //Read Pressure Sensor
        sensor10->read();
        //Serial.println("***********");
        if (sensor1->hasValueChanged()){
            //Serial.println("TEMP_HEATER");
            SendFrameWord(TEMP_HEATER, sensor1->lastRead);
        }
        if (sensor2->hasValueChanged()){
            //Serial.println("TEMP_POOL");
            SendFrameWord(TEMP_POOL, sensor2->lastRead);
        }
        if (sensor9->hasValueChanged()){
            //Serial.println("TEMP_FLOOR");
            SendFrameWord(TEMP_FLOOR, sensor9->lastRead);
        }
        if (sensor9->hasValueChanged(1)){
            //Serial.println("HG_ROOM");
            SendFrameWord(HG_ROOM, sensor9->printValueIntx10(1));
        }
        if (sensor3->hasValueChanged()){
            //Serial.println("TEMP_POOL_IMP");
            SendFrameWord(TEMP_POOL_IMP, sensor3->printValueIntx10());
        }
        if (sensor4->hasValueChanged()){
            //Serial.println("TEMP_FLOOR_IMP");
            SendFrameWord(TEMP_FLOOR_IMP, sensor4->printValueIntx10());
        }
        if (sensor5->hasValueChanged()){
            //Serial.println("TEMP_RETURN");
            SendFrameWord(TEMP_RETURN, sensor5->printValueIntx10());
        }
        if (sensor6->hasValueChanged()){
            //Serial.println("TEMP_SERIES");
            SendFrameWord(TEMP_SERIES, sensor6->printValueIntx10());

        }
        if (sensor10->hasValueChanged()){
            //Serial.println("PRESS_RETURN");
            SendFrameWord(PRESS_RETURN, sensor10->printValueIntx10());

        }
        //Serial.println("***********");
    }
    if (criticalUpdate.update())
    {
        //Serial.println("Send criticals");
        SendFrameWord(DESIRED_TEPM_POOL, desiredTempPool);
        SendFrameWord(DESIRED_TEMP_FLOOR, desiredTempFloor);
        SendFrameWord(DESIRED_STATE, desiredState);
        SendFrameWord(CMD_STATE_EMERGENCE, alarmState);
    }
    if (freeMemory() < 100){
        Serial.println(freeMemory());
    };
    SWDupdateAndTest();
}
