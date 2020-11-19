#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "Credentials.h"

OneWire oneWire(2);
DallasTemperature sensor(&oneWire);

void setup()
{
    Serial.begin(115200);

    wifiConnect();

    sensor.begin();
}

void loop()
{
    if (!WiFi.isConnected())
    {
        Serial.println("Desconectado, reconectando...");
        WiFi.reconnect();
    }
}

void wifiConnect()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("\nconectando");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\nconectado: ");
    Serial.println(WiFi.localIP());
}

float readTemperature()
{
    sensor.requestTemperatures();
    return sensor.getTempCByIndex(0);
}
