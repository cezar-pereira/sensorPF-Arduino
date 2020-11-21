class Setting
{
private:
    int intervalToUpdateTemperature = 10;           //in minutes - default
    float temperatureToAlert = 30;                  //default
    String emailToAlert = "helaniocezar@gmail.com"; //default

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
};