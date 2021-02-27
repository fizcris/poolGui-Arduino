#pragma once

#include <Arduino.h>
#include <MCP41_Simple.h>

class Actuator
{
private:
  byte pin;
  String name;
  String type;
  byte status;

  //digipot
  MCP41_Simple *mcp41;;
  
public:
  Actuator(byte pin, String name, String type="digital");
  void init();
  void on();
  void off();
  void setPot(long value);
}; // don't forget the semicolon at the end of the class
