#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "credenciales.h"
#define PIN_BUTTON D0 //Puerto del botón
#define PIN_R D6 //Puerto del led
#define PIN_G D5 //Puerto del led
#define PIN_Y D7 //Puerto del led
#define PIN_B D8 //Puerto del led

const char* servidor_mqtt = "192.168.0.107"; //mosquitto
int puerto = 1883;
String file;
int ledState = 0;
int emergencyState = 0;
int l = 0;
float lat=-16.376214;
float lon= -71.521747;
String nombre = "Jose";

WiFiClient ClientEsp;
PubSubClient client(ClientEsp);

void configuracion_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (l == 0) {
      digitalWrite(PIN_G, LOW);
      digitalWrite(PIN_R, HIGH);
      l = 1;
    } else if (l == 1) {
      digitalWrite(PIN_G, HIGH);
      digitalWrite(PIN_R, LOW);
      l = 0;
    }
  }
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_R, LOW);
  delay(2000);
  Serial.print("Conectado con éxito al WIFI-->");
  Serial.println(WiFi.SSID());
  digitalWrite(PIN_G, HIGH);
  delay(2000);
  digitalWrite(PIN_G, LOW);
}

void reconnect() {
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_B, HIGH);
  while (!client.connected()) {
    Serial.println("Intentando conexion MQTT con parametros configurados...");
    if (client.connect("ESP8266Client")) {
      Serial.println("MQTT conectado!!");
      digitalWrite(PIN_B, LOW);
      digitalWrite(PIN_G, HIGH);
      digitalWrite(PIN_R, LOW);
      digitalWrite(PIN_Y, HIGH);
      delay(800);
      digitalWrite(PIN_G, LOW);
    } else {
      Serial.println("Ha ocuriido un fallo al conectar MQTT, Estado: ");
      digitalWrite(PIN_B, LOW);
      digitalWrite(PIN_R, HIGH);
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
    digitalWrite(PIN_Y, LOW);
    digitalWrite(PIN_R, 1);//Encenderá el led
    Serial.println(ayuda);//Imprimira el texto de ayuda
    emergencyState = 1;
    client.publish("panic", String(emergencyState).c_str());
    client.publish("gps", file.c_str()  );

//    client.publish("gps", String("name:"+nombre+",lat:"+String(lat,6)+",lon: "+String(lon,6)).c_str());
//    client.publish("gps/name",String(nombre).c_str());
//    client.publish("gps/lat",String(lat,6).c_str());
//    client.publish("gps/lon",String(lon,6).c_str());
    
    Serial.println(emergencyState);
    delay(800);//Esperará 1 seg
    digitalWrite(PIN_R, ledState);//Apagará el led
    emergencyState = 0;
    Serial.println(Yano);//Print mensaje de no ayuda
    digitalWrite(PIN_Y, HIGH);
    Serial.println(emergencyState);
    client.publish("panic", String(emergencyState).c_str());
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_Y, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  // Inicia salidas en 0
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_Y, LOW);
  digitalWrite(PIN_B, LOW);
  configuracion_wifi();
  client.setServer(servidor_mqtt, puerto);
  pinMode(PIN_BUTTON, INPUT);//Leer el boton
  pinMode(PIN_G, OUTPUT);//Definir la salida al led
  pinMode(PIN_R, OUTPUT);//Definir la salida al led
  pinMode(PIN_B, OUTPUT);//Definir la salida al led
  pinMode(PIN_Y, OUTPUT);//Definir la salida al led
  digitalWrite(PIN_G, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_R, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_B, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_Y, ledState);//Inicializar el LED a 0

     
  DynamicJsonDocument doc(1024);       
  doc["name"] = nombre ;        
  doc["time"]   = 1351824120;            
  doc["lat"] = lat; //or you can put variables here        
  doc["lon"] = lon;          
  serializeJson(doc, file);




  
}
  
void loop() {
  pushButton();
}
