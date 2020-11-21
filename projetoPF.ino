#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "Credentials.h"
#include "Setting.h"
#include "Paths.h"
#include "Temperature.h"

OneWire oneWire(2);
DallasTemperature sensor(&oneWire);

Setting setting;
Paths paths;
Credentials credentials;
Temperature temperature;

unsigned long currentMillis = 0;
unsigned long counterMillisToUpdateTemperature = 0;

void setup()
{
    Serial.begin(115200);

    wifiConnect();

    Firebase.begin(credentials.FIREBASE_HOST, credentials.FIREBASE_AUTH);

    sensor.begin();

    readingSettingsValuesFirebase();
    temperature.updateTemperatures(readTemperature());

    Firebase.stream("/");
}

void loop()
{
    if (!WiFi.isConnected())
    {
        Serial.println("Desconectado, reconectando...");
        wifiConnect();
    }

    currentMillis = millis();

    if (Firebase.failed())
    {
        Serial.println("Streaming error");
        Serial.println(Firebase.error());
        return;
    }
    checkUpdateTemperaturePeriod();

    if (Firebase.available())
    {
        FirebaseObject event = Firebase.readEvent();
        String eventType = event.getString("type");
        eventType.toLowerCase();
        //     Serial.print("Evento: ");
        //     Serial.println(eventType);

        if (eventType == "patch")
        {
            String path = event.getString("path");
            Serial.print("Endereco: ");
            Serial.println(path);

            if (path == "/CheckTemperature")
                checkUpdateTemperatureRequested();

            else if (path == "/Settings")
                readingSettingsValuesFirebase();
        }
    }
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

    calculateHourlyTemperatureReadings(setting.getIntervalToUpdateTemperature());

    Serial.println("CONFIGURACOES: ");
    Serial.print("Intervalo: ");
    Serial.println(setting.getIntervalToUpdateTemperature());
    Serial.print("Temperatura de alerta: ");
    Serial.println(setting.getTemperatureToAlert());
    Serial.print("Email para envio: ");
    Serial.println(setting.getEmailToAlert());
    Serial.print("Leitura por hora: ");
    Serial.println(temperature.getHourlyTemperatureReadings());
}

void calculateHourlyTemperatureReadings(int interval)
{
    temperature.setHourlyTemperatureReadings((60 / interval));
    //caso tenha algum erro de validacao no front, pois o maximo de leituras por hora definido é de 30
    if (temperature.getHourlyTemperatureReadings() > temperature.maximumReadingsPerHour)
        temperature.setHourlyTemperatureReadings(temperature.maximumReadingsPerHour);
}

void checkUpdateTemperaturePeriod()
{
    if ((currentMillis - counterMillisToUpdateTemperature) >= (setting.getIntervalToUpdateTemperature() * 60000))
    {
        counterMillisToUpdateTemperature = currentMillis;
        temperature.updateTemperatures(readTemperature());
        Serial.println("Atualizou temperatura automaticamente de acordo com o tempo definido");
    }
}

void checkUpdateTemperatureRequested()
{
    Firebase.setBool(paths.CHECK_TEMPERATURE, false);
    temperature.updateTemperatures(readTemperature());
    Serial.println("Atualizou temperatura conforme solicitado");
}