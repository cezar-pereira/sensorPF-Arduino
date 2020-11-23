class Temperature
{

#include "paths.h";

    Paths paths;

public:
    static const int maximumReadingsPerHour = 30;

private:
    int hourlyTemperatureReadings = 6; //default 6
    int readingCount = 0;

    float minimumTemperature = 0;
    float maximumTemperature = 0;
    float realTemperature = 0;
    float averageTemperature = 0;
    float vectorTemperatures[maximumReadingsPerHour];

public:
    void organizeTemperatures()
    {
        for (int i = 0; i < readingCount; i++)
        {
            vectorTemperatures[i] = vectorTemperatures[i + 1];
        }
    }

    float calculateAverageTemperature()
    {
        float summactionVectorTemperatures = 0;

        for (int i = 0; i <= readingCount; i++)
        {
            summactionVectorTemperatures += vectorTemperatures[i];
        };

        if (readingCount == 0)
            return summactionVectorTemperatures;
        else if (readingCount == hourlyTemperatureReadings)
            return summactionVectorTemperatures / readingCount;
        else
            return summactionVectorTemperatures / (readingCount + 1); //+1 pq inicia em 0
    }

    void updateTemperatures(float temperature)
    {
        Serial.println("atualizando temperaturas...");

        //REAL
        realTemperature = temperature;
        Firebase.setFloat(paths.REAL_TEMPERATURE, realTemperature);
        //MINIMUM
        if (realTemperature < minimumTemperature || minimumTemperature == 0)
        {
            minimumTemperature = realTemperature;
            Firebase.setFloat(paths.MINIMUM_TEMPERATURE, minimumTemperature);
        }
        //MAXIMUM
        if (realTemperature > maximumTemperature || maximumTemperature == 0)
        {
            maximumTemperature = realTemperature;
            Firebase.setFloat(paths.MAXIMUM_TEMPERATURE, maximumTemperature);
        }

        //AVERAGE
        if (readingCount >= hourlyTemperatureReadings) //readingCount comeca em ZERO
        {
            Serial.println("ENTROU PARA ORGANIZAR");
            readingCount = hourlyTemperatureReadings - 1;
            organizeTemperatures();
            vectorTemperatures[readingCount] = realTemperature;
            Serial.println("vectorTemperatures apos organizado e com nova leitura: ");

            for (int j = 0; j <= readingCount; j++)
            {
                Serial.print("Temperatura posicao ");
                Serial.print(j);
                Serial.print(": ");
                Serial.println(vectorTemperatures[j]);
            }
        }
        else
        {
            vectorTemperatures[readingCount] = realTemperature;
        }

        averageTemperature = calculateAverageTemperature();

        Firebase.setFloat(paths.AVERAGE_TEMPERATURE, averageTemperature);

        readingCount++;

        //DEPURACAO
        Serial.println("FIM atualizacao temperaturas...");
        int i = 0;
        do
        {
            Serial.print("Temperatura posicao ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(vectorTemperatures[i]);
            i++;
        } while (i < readingCount);
        Serial.print("MÉDIA: ");
        Serial.println(averageTemperature);
    }

    void setHourlyTemperatureReadings(int value)
    {
        hourlyTemperatureReadings = value;
    }

    int getHourlyTemperatureReadings()
    {
        return hourlyTemperatureReadings;
    }

    void setReadingCount(int value)
    {
        readingCount = value;
    }

    void setMinimumTemperature(float value)
    {
        minimumTemperature = 0;
    }

    void setMaximumTemperature(float value)
    {
        maximumTemperature = 0;
    }

    void setRealTemperature(float value)
    {
        realTemperature = 0;
    }
};