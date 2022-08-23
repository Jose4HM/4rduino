#include <WiFi.h>  
#include <ArduinoJson.h>

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

#define WIFI_SSID "Di q eres pobre"
#define WIFI_PASSWORD "soypobre"

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 43, 99)
// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_MAG "esp/motor/Est"


// Digital pin connected to the DHT sensor



// Variables to hold sensor readings
const int reedSwitch = 4; //PIN INTERRUPTOR
const int led = 2; // PIN LED

bool changeState = false;// Para detectar cada vez que la puerta cambia de estado
// Mantiene el estado del interruptor de láminas (0=abierto, 1=cerrado)
bool state;
String doorState;
//Variables de tiempo de ayuda
unsigned long previousMillis = 0; 
const long interval = 1500;

//*************************CAMBIO DE ESTADO*************************
ICACHE_RAM_ATTR void changeDoorStatus() {
  //Serial.println("State changed"); Para saber el estado cambiado
  changeState = true;
}
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(reedSwitch, INPUT_PULLUP);    // Lee el estado actual de la puerta
  state = digitalRead(reedSwitch);      //Establecer el estado del LED para que coincida con el estado de la puerta
  pinMode(led, OUTPUT);
  digitalWrite(led, !state);
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE);//Configura el pin del interruptor magnetico como interrupción, asigne la función de interrupción y configure el modo de cambio

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  connectToWifi();
}

void loop() {
  // rotates the servo motor from 40 to 150 degrees
  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // Bucle iff para visualizar el estado de si esta abierto o cerrado
        state = !state;
        if(state) {
          doorState = "Abierto";
          uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_MAG, 1, true, String(doorState).c_str());                            
          Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_MAG, packetIdPub1);
          Serial.printf("Message: \n", doorState);

          //lcd.print("Abierto");
        }
        else{
          doorState = "Cerrado";
          //lcd.print("Cerrado alv");
          uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_MAG, 1, true, String(doorState).c_str());                            
          Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_MAG, packetIdPub1);
          Serial.printf("Message: \n", doorState);
        }
        digitalWrite(led, !state);
        changeState = false;
        ;
        Serial.println(doorState);
        }  
  }
  
  
    
  
    /////////////////////////////
}

