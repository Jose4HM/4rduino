#include <ESP32Servo.h>.
#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

#define WIFI_SSID "ROSA"
#define WIFI_PASSWORD "1111alex"

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 0, 105)
// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_DGR "esp/motor/Grados"
#define MQTT_PUB_DST "esp/ultrasonico/Distance"
#define MQTT_PUB_LED "esp/led/Estado"

// Digital pin connected to the DHT sensor
#define  trigPin  2
#define  echoPin  4
#define LED1  32
#define LED2  33
#define LED3  25


// Variables to hold sensor readings
float duration;
float distance;
Servo myServo;
int LED = 0;

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
float calculateDistance(){ 
      if (distance>=20){
  digitalWrite (LED1,HIGH);
  digitalWrite (LED2,LOW);
  digitalWrite (LED3,LOW);
  }

  if (distance>=10 && distance <=19){
  digitalWrite (LED1,HIGH);
  digitalWrite (LED2,HIGH);
  digitalWrite (LED3,LOW);
  
 }
  if (distance>=2 && distance <=9){
  digitalWrite (LED1,HIGH);
  digitalWrite (LED2,HIGH);
  digitalWrite (LED3,HIGH);
  LED = 1;
  // Publish an MQTT message on topic "esp/led/Estado"
  uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_LED, 1, true, String(LED).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_LED, packetIdPub3);
  LED = 0;
  uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_LED, 1, true, String(LED).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_LED, packetIdPub3);
  }
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(4);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration/58.3;
  return distance;
}
void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  Serial.begin(115200);
  myServo.attach(14); // Defines on which pin is the servo motor attached
  
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
  for(int i=40;i<=150;i++){  
  myServo.write(i);
  delay(1000);
  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
  
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(" grados   //"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  // Publish an MQTT message on topic "esp/motor/Grados"
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DGR, 1, true, String(i).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_DGR, packetIdPub1);
  Serial.printf("Message: \n", i);
    
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.println("cm"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  // Publish an MQTT message on topic "esp/ultrasonico/Distance"
  uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_DST, 1, true, String(distance).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_DST, packetIdPub2);
  Serial.printf("Message: %.2f \n", distance);

  
  }
  // Repeats the previous lines from 150 to 40 degrees
  for(int i=150;i>40;i--){  
  myServo.write(i);
  delay(1000);
  distance = calculateDistance();
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(" grados   //"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  // Publish an MQTT message on topic "esp/motor/Grados"
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DGR, 1, true, String(i).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_DGR, packetIdPub1);
  Serial.printf("Message: \n", i);
    
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.println("cm"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  // Publish an MQTT message on topic "esp/ultrasonico/Distance"
  uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_DST, 1, true, String(distance).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_DST, packetIdPub2);
  Serial.printf("Message: %.2f \n", distance);
  }
    /////////////////////////////
}
