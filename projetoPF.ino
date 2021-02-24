#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "FirebaseESP8266.h"

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

FirebaseData firebaseDataStream;
FirebaseData firebaseDataSet;
FirebaseData firebaseDataGet;

unsigned long counterMillisToUpdateTemperature = 0;

unsigned long antiFloodFirebaseCheckTemperature = 0;
int intervalAntiFloodFirebaseCheckTemperature = 30; //seconds

void setup()
{
    Serial.begin(115200);

    wifiConnect();

    Firebase.begin(credentials.FIREBASE_HOST, credentials.FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    if (!Firebase.beginStream(firebaseDataStream, paths.PATH))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't begin stream connection...");
        Serial.println("REASON: " + firebaseDataStream.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    Firebase.setBool(firebaseDataSet, paths.CHECK_TEMPERATURE, false);

    sensor.begin();

    readingSettingsValuesFirebase();

    temperature.updateTemperatures(readTemperature());
}

void loop()
{
    if (!Firebase.readStream(firebaseDataStream))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't read stream data...");
        Serial.println("REASON: " + firebaseDataStream.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
        return;
    }

    if (firebaseDataStream.streamTimeout())
    {
        Serial.println("Stream timeout, resume streaming...");
        Serial.println();
    }

    if (firebaseDataStream.streamAvailable())
    {
        Serial.println("STREAM PATH: " + firebaseDataStream.streamPath());
        Serial.println("EVENT PATH: " + firebaseDataStream.dataPath());
        Serial.println("DATA TYPE: " + firebaseDataStream.dataType());
        Serial.println("EVENT TYPE: " + firebaseDataStream.eventType());

        checkUpdateTemperatureRequested();
        readingSettingsValuesFirebase();
    }
    checkTemperature();
    checkUpdateTemperaturePeriod();
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
    Firebase.getInt(firebaseDataGet, paths.SETTINGS_INTERVAL_TO_UPDATE);
    if (firebaseDataGet.dataType() == "int")
        setting.setIntervalToUpdateTemperature(firebaseDataGet.intData());

    Firebase.getFloat(firebaseDataGet, paths.SETTINGS_TEMPERATURE_TO_ALERT);
    if (firebaseDataGet.dataType() == "float" || firebaseDataGet.dataType() == "int")
        setting.setTemperatureToAlert(firebaseDataGet.floatData());

    Firebase.getString(firebaseDataGet, paths.EMAIL_TO_ALERT);
    if (firebaseDataGet.dataType() == "string")
        setting.setEmailToAlert(firebaseDataGet.stringData());

    Firebase.getString(firebaseDataGet, paths.SENSOR_NAME);
    if (firebaseDataGet.dataType() == "string")
        setting.setSensorName(firebaseDataGet.stringData());

    calculateHourlyTemperatureReadings(setting.getIntervalToUpdateTemperature());

    Serial.println("CONFIGURACOES: ");
    Serial.print("Intervalo: ");
    Serial.println(setting.getIntervalToUpdateTemperature());
    Serial.print("Temperatura de alerta: ");
    Serial.println(setting.getTemperatureToAlert());
    Serial.print("Email para envio: ");
    Serial.println(setting.getEmailToAlert());
    Serial.print("Nome do sensor: ");
    Serial.println(setting.getSensorName());
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
    if (counterMillisToUpdateTemperature > millis())
        counterMillisToUpdateTemperature = millis();

    if ((millis() - counterMillisToUpdateTemperature) >= (setting.getIntervalToUpdateTemperature() * 60000))
    {
        counterMillisToUpdateTemperature = millis();
        temperature.updateTemperatures(readTemperature());
        Serial.println("Atualizou temperatura automaticamente de acordo com o tempo definido");
    }
}

void checkUpdateTemperatureRequested()
{
    Firebase.getBool(firebaseDataGet, paths.CHECK_TEMPERATURE);

    if (firebaseDataGet.boolData())
    {
        temperature.updateTemperatures(readTemperature());
        Serial.println("Atualizou temperatura conforme solicitado");
        Firebase.setBool(firebaseDataSet, paths.CHECK_TEMPERATURE, false);
    }
}

void checkTemperature()
{
    if (readTemperature() >= setting.getTemperatureToAlert())
    {
        if ((millis() - antiFloodFirebaseCheckTemperature) >= (intervalAntiFloodFirebaseCheckTemperature * 1000))
        {
            antiFloodFirebaseCheckTemperature = millis();
            temperature.updateTemperatures(readTemperature());
        }
    }
}