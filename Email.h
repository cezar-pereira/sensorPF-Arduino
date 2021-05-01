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

    void sendEmail()
    {
        EMailSender emailSend(credentials.EMAIL, credentials.EMAIL_PASSWORD);
        Serial.println("Seending E-mail...");
        pauseOrContinueFirebaseData();

        String messageToSent = "Alerta, <strong>" + setting.getSensorName() + "</strong> com temperatura: <h1>";
        messageToSent.concat(temperature.getRealTemperature());
        messageToSent.concat("</h1>");

        EMailSender::EMailMessage message;
        message.subject = "teste";
        message.message = messageToSent;

        EMailSender::Response resp = emailSend.send(setting.getEmailToAlert(), message);

        Serial.println("Sending status: ");

        Serial.println(resp.status);
        Serial.println(resp.code);
        Serial.println(resp.desc);
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
