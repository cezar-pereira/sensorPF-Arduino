<h1 align="center"> 
	TempSensor
</h1>

<p align="center">
 <a href="#-sobre-o-projeto">Sobre</a> •
 <a href="#-funcionalidades">Funcionalidades</a> •
 <a href="#-tecnologias">Tecnologias</a> 
</p>


## 💻 Sobre o projeto

TempSensor - é um projeto desenvolvido durante o meu estágio na Superintendência Regional em Alagoas - Polícia Federal que tem como propósito monitorar a temperatura da sala do servidor e alertar por e-mail e enviar as temperaturas para o servidor (Firebase) para serem lidas por outro sistema.

---

## ⚙️ Funcionalidades

- Verifica temperatura de alerta
  - Compara temperatura atual com a obtida do servidor, caso seja maior que a permitida envia um e-mail de alerta;
- Enviar e-mail de alerta;
  - E-mail definido no app
- Temperatura mínima;
- Temperatura média;
  - média das ultimas x leituras por hora, x a ser definida pelo usuário no app;
- Temperatura máxima;
- Temperatura atual;

---

## 🛠 Tecnologias

As seguintes ferramentas foram usadas na construção do projeto:

#### **Server**  ([Firebase](https://firebase.google.com/))
#### **Sensor Temperatura**  ([DS18B20](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf))
#### **NodeMCU ESP8266**  ([NodeMCU ESP8266](https://components101.com/development-boards/nodemcu-esp8266-pinout-features-and-datasheet))

#### **Utilitários**
-   Editor:  **[Visual Studio Code](https://code.visualstudio.com/)**  → Extensions:  **[Arduino](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)**

---

## 📘 Bibliotecas

Bibliotecas utilizadas no projeto:

### E-mail
- #### **ESP8266SMTP.h & ESP8266SMTP.cpp**  ([ESP8266SMTP](https://github.com/CosmicBoris/ESP8266SMTP/tree/master/src))

### Sensor de temperatura
- #### **OneWire**  ([OneWire](https://blogmasterwalkershop.com.br/arquivos/libs/OneWire.zip))
- #### **DallasTemperature**  ([DallasTemperature](https://blogmasterwalkershop.com.br/arquivos/libs/DallasTemperature.zip))

### ESP8266
- #### **ESP8266WiFi**  ([ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi))
### FirebaseArduino
- #### **FirebaseArduino**  ([FirebaseArduino](https://github.com/FirebaseExtended/firebase-arduino))
### ArduinoJson
- #### **ArduinoJson**  ([ArduinoJson](https://github.com/bblanchon/ArduinoJson))


