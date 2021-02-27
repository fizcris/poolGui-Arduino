#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22

class Sensor
{
private:
  byte pin;
  String name;
  String sensorType;
  //DS18B20
  DallasTemperature *sensorid;
  OneWire *idBus;
  int timeToReadsensor;
  unsigned long previousMillis;
  //Pressure sensor
  float bar;
  int offset; 
  //DHT22
  float hum;  //Stores humidity value
  float temp; //Stores temperature value
  DHT *dht;

public:
  Sensor(byte pin, String name, String sensorType);
  void init();
  void read();
  float printValue(int mode=0);
  String print(int mode=0);
  int printValueIntx10 (int mode=0);
}; // don't forget the semicolon at the end of the class