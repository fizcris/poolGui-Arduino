#include <Actuator.h>

Actuator::Actuator(byte pin, String name, String type)
{
    // Use 'this->' to make the difference between the
    // 'pin' attribute of the class and the
    // local variable 'pin' created from the parameter.
    this->pin = pin;
    this->name = name;
    this->type = type;
    init();
}

void Actuator::init()
{
    if (this->type == "digital")
    {
        pinMode(this->pin, OUTPUT);
    }
    if (this->type == "digipot")
    {
        mcp41 = new MCP41_Simple();
        mcp41->begin(this->pin);
    }
}

void Actuator::on()
{
    digitalWrite(this->pin, LOW);
}

void Actuator::off()
{
    digitalWrite(this->pin, HIGH);
}

void Actuator::setPot(long value)
{
    // Set the wiper to a point between 0 and 8570kohm
    value = map(value,0,8570,255,0);
    mcp41->setWiper(value);
}
