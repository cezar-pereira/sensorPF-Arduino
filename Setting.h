class Setting
{
private:
    int intervalToUpdateTemperature = 10; //in minutes
    float temperatureToAlert = 30;
    String emailToAlert = "helaniocezar@gmail.com";
    String sensorName = "Sensor";

public:
    void setIntervalToUpdateTemperature(int value)
    {
        intervalToUpdateTemperature = value;
    }
    int getIntervalToUpdateTemperature()
    {
        return intervalToUpdateTemperature;
    }

    void setTemperatureToAlert(float value)
    {
        temperatureToAlert = value;
    }
    float getTemperatureToAlert()
    {
        return temperatureToAlert;
    }

    void setEmailToAlert(String value)
    {
        emailToAlert = value;
    }
    String getEmailToAlert()
    {
        return emailToAlert;
    }

    void setSensorName(String value)
    {
        sensorName = value;
    }
    String getSensorName()
    {
        return sensorName;
    }
};