extern Credentials credentials;
extern Setting setting;
extern Temperature temperature;
extern FirebaseData firebaseDataStream;
extern FirebaseData firebaseDataSet;
extern FirebaseData firebaseDataGet;

class Email
{
public:
    bool isPauseFirebaseData = true;

    void setConfigurationSMTP()
    {
        char *title = "Alerta De Temperatura sala dos servidores da Policia Federal";
        char *content = "Sensor De Temperatura";
        SMTP.setEmail(credentials.EMAIL)
            .setPassword(credentials.EMAIL_PASSWORD)
            .Subject(title)
            .setFrom(content)
            .setForGmail();
    }

    void sendEmail()
    {
        Serial.println("Seending E-mail...");
        pauseOrContinueFirebaseData();

        setConfigurationSMTP();

        String messageToSent = "Alerta, <strong>" + setting.getSensorName() + "</strong> com temperatura: <h1>";
        messageToSent.concat(temperature.getRealTemperature());
        messageToSent.concat("</h1>");

        if (SMTP.Send(setting.getEmailToAlert(), messageToSent))
        {
            Serial.println("Alerta enviado: ");
            Serial.println(messageToSent);
        }
        else
        {
            Serial.print("Erro ao enviar e-mail: ");
            Serial.println(SMTP.getError());
        }

        pauseOrContinueFirebaseData();
    }

    void pauseOrContinueFirebaseData()
    {
        firebaseDataStream.pauseFirebase(isPauseFirebaseData);
        firebaseDataSet.pauseFirebase(isPauseFirebaseData);
        firebaseDataGet.pauseFirebase(isPauseFirebaseData);

        isPauseFirebaseData = !isPauseFirebaseData;
    }
};
