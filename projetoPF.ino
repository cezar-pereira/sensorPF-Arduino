#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(2);
DallasTemperature sensor(&oneWire);

void setup()
{
    Serial.begin(115200);
    sensor.begin();
}

void loop()
{
    Serial.println(readTemperature());
}

float readTemperature()
{
    sensor.requestTemperatures();
    return sensor.getTempCByIndex(0);
}
