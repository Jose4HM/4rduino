#include <ESP8266WiFi.h>
#include <PubSubClient.h>  // Download and install this library first from: https://www.arduinolibraries.info/libraries/pub-sub-client
#include <WiFiClient.h>

#define SSID_NAME "Scooby-doo donde estas?"                         // Your Wifi Network name
#define SSID_PASSWORD "21040411"                 // Your Wifi network password
#define MQTT_BROKER "smartnest.cz"                    // Broker host
#define MQTT_PORT 1883                                // Broker port
#define MQTT_USERNAME "josehm"                      // Username from Smartnest
#define MQTT_PASSWORD "112318914d46082315b48d62715a54be"             // Password from Smartnest (or API key)
#define MQTT_CLIENT "63042677fb5f4769a710b71c"                       // Device Id from smartnest
#define FIRMWARE_VERSION "Door1.0"  // Custom name for this program

WiFiClient espClient;
PubSubClient client(espClient);
// Set GPIOs for LED and reedswitch
const int reedSwitch = 4; 
const int led = 2; //optional

// Detects whenever the door changed state
bool changeState = false;

// Holds reedswitch state (1=opened, 0=close)
bool state;
String doorState;

// Auxiliary variables (it will only detect changes that are 1500 milliseconds apart)
unsigned long previousMillis = 0; 
const long interval = 1500;
int doorReportSend = 0;

void startWifi();
void startMqtt();
void sendBellReport();
void checkBell();
void checkMqtt();
int splitTopic(char* topic, char* tokens[], int tokensNumber);
void callback(char* topic, byte* payload, unsigned int length);
void sendToBroker(char* topic, char* message);

// Runs whenever the reedswitch changes state
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State changed");
  changeState = true;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Read the current door state
  pinMode(reedSwitch, INPUT_PULLUP);
  state = digitalRead(reedSwitch);

  // Set LED state to match door state
  pinMode(led, OUTPUT);
  digitalWrite(led, state);
  
  // Set the reedswitch pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE); 

  startWifi();
  startMqtt();
}

void loop() {
  client.loop();
  checkBell();
  checkMqtt();
}

void callback(char* topic, byte* payload, unsigned int length) {  // This function runs when there is a new message in the subscribed topic
  Serial.print("Topic:");
  Serial.println(topic);

  // Splits the topic and gets the message
  int tokensNumber = 10;
  char* tokens[tokensNumber];
  char message[length + 1];
  splitTopic(topic, tokens, tokensNumber);
  sprintf(message, "%c", (char)payload[0]);

  for (int i = 1; i < length; i++) {
    sprintf(message, "%s%c", message, (char)payload[i]);
  }

  Serial.print("Message:");
  Serial.println(message);
}

void startWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  Serial.println("Connecting ...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    attempts++;
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

  } else {
    Serial.println('\n');
    Serial.println('I could not connect to the wifi network after 10 attempts \n');
  }

  delay(500);
}

void startMqtt() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(MQTT_CLIENT, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      if (client.state() == 5) {
        Serial.println("Connection not allowed by broker, possible reasons:");
        Serial.println("- Device is already online. Wait some seconds until it appears offline for the broker");
        Serial.println("- Wrong Username or password. Check credentials");
        Serial.println("- Client Id does not belong to this username, verify ClientId");

      } else {
        Serial.println("Not possible to connect to Broker Error code:");
        Serial.print(client.state());
      }

      delay(0x7530);
    }
  }

  char subscibeTopic[100];
  sprintf(subscibeTopic, "%s/#", MQTT_CLIENT);
  client.subscribe(subscibeTopic);  //Subscribes to all messages send to the device

  sendToBroker("report/online", "true");  // Reports that the device is online
  delay(100);
  sendToBroker("report/firmware", FIRMWARE_VERSION);  // Reports the firmware version
  delay(100);
  sendToBroker("report/ip", (char*)WiFi.localIP().toString().c_str());  // Reports the ip
  delay(100);
  sendToBroker("report/network", (char*)WiFi.SSID().c_str());  // Reports the network name
  delay(100);

  char signal[5];
  sprintf(signal, "%d", WiFi.RSSI());
  sendToBroker("report/signal", signal);  // Reports the signal strength
  delay(100);
}

int splitTopic(char* topic, char* tokens[], int tokensNumber) {
  const char s[2] = "/";
  int pos = 0;
  tokens[0] = strtok(topic, s);

  while (pos < tokensNumber - 1 && tokens[pos] != NULL) {
    pos++;
    tokens[pos] = strtok(NULL, s);
  }

  return pos;
}

void checkMqtt() {
  if (!client.connected()) {
    startMqtt();
  }
}

void checkBell() {
  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // If a state has occured, invert the current door state   
      state = !state;
      if(state) {
        doorState = "closed";
        sendBellReportClosed();
      }
      else{
        doorState = "open";
        sendBellReportOpened();
      }
      digitalWrite(led, state);
      changeState = false;
      Serial.println(state);
      Serial.println(doorState);
    }
  }
}

void sendBellReportClosed() {  //Avoids sending repeated reports. only once every 5 seconds.
  if (millis() - doorReportSend > 5000) {
    sendToBroker("report/lockedState", "ON");
    doorReportSend = millis();
  }
}
void sendBellReportOpened() {  //Avoids sending repeated reports. only once every 5 seconds.
  if (millis() - doorReportSend > 5000) {
    sendToBroker("report/lockedState", "OFF");
    doorReportSend = millis();
  }
}
void sendToBroker(char* topic, char* message) {
  if (client.connected()) {
    char topicArr[100];
    sprintf(topicArr, "%s/%s", MQTT_CLIENT, topic);
    client.publish(topicArr, message);
  }
}
