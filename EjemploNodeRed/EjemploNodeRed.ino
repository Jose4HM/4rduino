#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "credenciales.h"

const char* servidor_mqtt="192.168.1.104";//mosquitto
int puerto=1883;

WiFiClient ClientEsp;
PubSubClient client(ClientEsp);

void configuracion_wifi(){
  delay(10);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  Serial.println("Conected to WiFi");
}

void reconnect(){
  while(!client.connected()){
    Serial.println("Intentando conexion MQTT con parametros configurados...");
    if(client.connect("ESP8266Client")){
      Serial.println("Conectado!!");
    }else{
      Serial.print("Falla, Estado: ");
      Serial.print(client.state());
      Serial.println("Intentando conexión nuevamente en 5 segundos");
      delay(5000);
      }
    }
  }


void setup() {
  Serial.begin(115200);
  configuracion_wifi();
  client.setServer(servidor_mqtt, puerto);
}

void loop() {
  static float temperatura = 13.24;


  if(!client.connected()) reconnect();
  client.loop();

  client.publish("miCasa/plantaBaja/habitacion1/temperatura",String(temperatura++).c_str());
  delay(5000);

}
