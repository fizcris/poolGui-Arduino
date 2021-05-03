#include <Sensor.h>

Sensor::Sensor(byte pin, String name, String sensorType)
{
    // Use 'this->' to make the difference between the
    // 'pin' attribute of the class and the
    // local variable 'pin' created from the parameter.
    this->pin = pin;
    this->name = name;
    this->sensorType = sensorType;

    this->timeToReadsensor = 800; //ms
    this->previousMillis = 0;

    this->bar = 0; //lectura a 0atm
    //DHT22
    this->temp = 0;
    this->hum = 0;
    init();
}

void Sensor::init()
{
    avgTemp = new movingAvg(10); // define the moving average object
    avgTemp->begin();

    if (this->sensorType == "DS18B20")
    {
        idBus = new OneWire(pin);
        sensorid = new DallasTemperature(idBus);
        sensorid->begin();
        sensorid->setResolution(12);
        //     10bits   187.5ms Resolution 0.250°C
        // max 12bits	750ms   Resolution 0.0625°C
        sensorid->setWaitForConversion(false); // Makes it async! Need to handle wait manually
    }
    else if (this->sensorType == "DHT22")
    {
        dht = new DHT(this->pin, DHTTYPE);
        dht->begin();
    }
}

void Sensor::read()
{
    if (this->sensorType == "DS18B20")
    {
        if (millis() - this->previousMillis > this->timeToReadsensor)
        {
            sensorid->requestTemperaturesByIndex(0);
            this->previousMillis = millis();
        }
    }
    if (this->sensorType == "MBS3000")
    {
        this->bar = map(analogRead(this->pin), 184, 982, 0, 10000) / 1000.0;
        this->bar = (this->bar < 0) ? 0.0 : this->bar; //Remove negative values
    }
    if (this->sensorType == "DHT22")
    {
        this->hum = dht->readHumidity();
        this->temp = dht->readTemperature();
    }
}

float Sensor::printValue(int mode)
{
    float lastRead;

    if (this->sensorType == "DS18B20")
    {
        lastRead = sensorid->getTempCByIndex(0);
    }
    if (this->sensorType == "DHT22")
    {
        if (mode == 0)
            lastRead = this->temp;
        else
            return this->hum; //No need for filtering
    }
    if (this->sensorType == "MBS3000")
    {
        lastRead = this->bar;
    }
    else
    {
        return 0.0; //TODO implent error case
    }

    // Filtering and error handling
    if (lastRead > 0)
    {
        this->lastAvg = this->avgTemp->reading(lastRead);
        return this->lastAvg;
    }
    else
    {
        return 0.0; //TODO implent error case
    };
}

String Sensor::print(int mode)
{
    if (mode == 0)
        return (
            this->name + " " + String(this->printValue(mode)) + " ");
    else
        return (
            this->name + "_" + mode + " " + String(this->printValue(mode)) + " ");
}

int Sensor::printValueIntx10(int mode)
{

    return (int)(printValue(mode) * 10);
}

bool Sensor::hasValueChanged(int mode)
{

    if (mode == 0)
    {

        if (this->lastRead != this->printValueIntx10(0))
        {
            lastRead = this->printValueIntx10(0);
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (mode == 1)
    {
        if (this->lastRead1 != this->printValueIntx10(1))
        {
            lastRead1 = this->printValueIntx10(1);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}