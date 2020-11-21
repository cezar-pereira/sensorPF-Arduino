#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "Credentials.h"
#include "Setting.h"
#include "Paths.h"

OneWire oneWire(2);
DallasTemperature sensor(&oneWire);

Setting setting;
Paths paths;
Credentials credentials;

void setup()
{
    Serial.begin(115200);

    wifiConnect();

    Firebase.begin(credentials.FIREBASE_HOST, credentials.FIREBASE_AUTH);

    sensor.begin();

    readingSettingsValuesFirebase();
}

void loop()
{
    if (!WiFi.isConnected())
    {
        Serial.println("Desconectado, reconectando...");
        wifiConnect();
    }

    Serial.println(readTemperature());
}

void wifiConnect()
{
    WiFi.begin(credentials.WIFI_SSID, credentials.WIFI_PASSWORD);
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

void readingSettingsValuesFirebase()
{
    setting.setIntervalToUpdateTemperature(Firebase.getInt(paths.SETTINGS_INTERVAL_TO_UPDATE));
    setting.setTemperatureToAlert(Firebase.getFloat(paths.SETTINGS_TEMPERATURE_TO_ALERT));
    setting.setEmailToAlert(Firebase.getString(paths.EMAIL_TO_ALERT));

    Serial.println("CONFIGURACOES: ");
    Serial.print("Intervalo: ");
    Serial.println(setting.getIntervalToUpdateTemperature());
    Serial.print("Temperatura de alerta: ");
    Serial.println(setting.getTemperatureToAlert());
    Serial.print("Email para envio: ");
    Serial.println(setting.getEmailToAlert());
}
