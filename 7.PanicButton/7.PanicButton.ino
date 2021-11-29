#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "credenciales.h"
#define PIN_BUTTON D0 //Puerto del botón
#define PIN_LED D1 //Puerto del led

const char* servidor_mqtt = "192.168.0.104"; //mosquitto
int puerto = 1883;

int ledState = 0;
int emergencyState = 0;

WiFiClient ClientEsp;
PubSubClient client(ClientEsp);

void configuracion_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Conected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Intentando conexion MQTT con parametros configurados...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado!!");
    } else {
      Serial.print("Falla, Estado: ");
      Serial.print(client.state());
      Serial.println("Intentando conexión nuevamente en 5 segundos");
      delay(5000);
    }
  }
}

void pushButton() {
  String ayuda = "Ayuda!!!!!!!!!!!!!!!!! (◕‿◕✿)";
  String Yano = "Ya no uwu (╯°□°）╯︵ ┻━┻";
  if (!client.connected()) reconnect();
  client.loop();

  if (digitalRead(PIN_BUTTON) == 1) {//Si el boton es presionado
    digitalWrite(PIN_LED, 1);//Encenderá el led
    Serial.println(ayuda);//Imprimira el texto de ayuda
    emergencyState = 1;
    client.publish("panic", String(emergencyState).c_str());
    Serial.println(emergencyState);
    delay(1000);//Esperará 1 seg
    digitalWrite(PIN_LED, ledState);//Apagará el led
    emergencyState = 0;
    Serial.println(Yano);//Print mensaje de no ayuda
    Serial.println(emergencyState);
    client.publish("panic", String(emergencyState).c_str());
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  configuracion_wifi();
  client.setServer(servidor_mqtt, puerto);
  pinMode(PIN_BUTTON, INPUT);//Leer el boton
  pinMode(PIN_LED, OUTPUT);//Definir la salida al led
  digitalWrite(PIN_LED, ledState);//Inicializar el LED a 0
}

void loop() {
  pushButton();
}
