#include <ESP8266WiFi.h>
#include <PubSubClient.h>  // Download and install this library first from: https://www.arduinolibraries.info/libraries/pub-sub-client
#include <WiFiClient.h>

#define SSID_NAME "COVID variante MEGATRON"              // Your Wifi Network name
#define SSID_PASSWORD "21040411"      // Your Wifi network password
#define MQTT_BROKER "smartnest.cz"         // Broker host
#define MQTT_PORT 1883                     // Broker port
#define MQTT_USERNAME "josehm"           // Username from Smartnest
#define MQTT_PASSWORD "112318914d46082315b48d62715a54be"           // Password from Smartnest (or API key)
#define MQTT_CLIENT "61e80a416a204f7b73f41cb5"            // Device Id from smartnest
#define FIRMWARE_VERSION "Tutorial-Blink"  // Custom name for this program
#define PIN_R D0 //Puerto del led
#define PIN_Y D1 //Puerto del led
#define PIN_G D2 //Puerto del led

WiFiClient espClient;
PubSubClient client(espClient);
int lightPin = 2;

void startWifi();
void startMqtt();
void checkMqtt();
int splitTopic(char* topic, char* tokens[], int tokensNumber);
void callback(char* topic, byte* payload, unsigned int length);
void sendToBroker(char* topic, char* message);

void setup() {
  pinMode(PIN_G, OUTPUT);//Definir la salida al led
  pinMode(PIN_R, OUTPUT);//Definir la salida al led
  pinMode(PIN_Y, OUTPUT);//Definir la salida al led
	pinMode(lightPin, OUTPUT);
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_Y, LOW);
  digitalWrite(PIN_G, LOW);
  delay(1000);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_Y, HIGH);
  digitalWrite(PIN_G, LOW);
  delay(1000);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_Y, LOW);
  digitalWrite(PIN_G, HIGH);
  delay(1000);
	Serial.begin(115200);
	startWifi();
	startMqtt();
}

void loop() {
	client.loop();
	checkMqtt();
}

void callback(char* topic, byte* payload, unsigned int length) {  //A new message has been received
	Serial.print("Topic:");
	Serial.println(topic);
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

	//------------------ACTIONS HERE---------------------------------
	if (strcmp(tokens[1], "directive") == 0 && strcmp(tokens[2], "powerState") == 0) {
		if (strcmp(message, "ON") == 0) {
			digitalWrite(lightPin, LOW);
			sendToBroker("report/powerState", "ON");

		} else if (strcmp(message, "OFF") == 0) {
			digitalWrite(lightPin, HIGH);
			sendToBroker("report/powerState", "OFF");
		}
	}
}

void startWifi() {
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID_NAME, SSID_PASSWORD);
	Serial.println("Connecting ...");
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_Y, HIGH);
  digitalWrite(PIN_G, LOW);
	int attempts = 0;
	while (WiFi.status() != WL_CONNECTED && attempts < 10) {
		attempts++;
		delay(500);
		Serial.print(".");
	}

	if (WiFi.status() == WL_CONNECTED) {
		Serial.println('\n');
    digitalWrite(PIN_R, LOW);
    digitalWrite(PIN_Y, LOW);
    digitalWrite(PIN_G, HIGH);
		Serial.print("Connected to ");
		Serial.println(WiFi.SSID());
		Serial.print("IP address:\t");
		Serial.println(WiFi.localIP());

	} else {
		Serial.println('\n');
		Serial.println('I could not connect to the wifi network after 10 attempts \n');
    digitalWrite(PIN_R, HIGH);
    digitalWrite(PIN_Y, LOW);
    digitalWrite(PIN_G, LOW);  
	}

	delay(500);
}

void startMqtt() {
	client.setServer(MQTT_BROKER, MQTT_PORT);
	client.setCallback(callback);

	while (!client.connected()) {
		Serial.println("Connecting to MQTT...");
      digitalWrite(PIN_R, LOW);
      digitalWrite(PIN_Y, HIGH);
      digitalWrite(PIN_G, LOW);

		if (client.connect(MQTT_CLIENT, MQTT_USERNAME, MQTT_PASSWORD)) {
			Serial.println("connected");
        digitalWrite(PIN_R, LOW);
        digitalWrite(PIN_Y, LOW);
        digitalWrite(PIN_G, HIGH);
		} else {
			if (client.state() == 5) {
				Serial.println("Connection not allowed by broker, possible reasons:");
				Serial.println("- Device is already online. Wait some seconds until it appears offline for the broker");
				Serial.println("- Wrong Username or password. Check credentials");
				Serial.println("- Client Id does not belong to this username, verify ClientId");
          digitalWrite(PIN_R, HIGH);
          digitalWrite(PIN_Y, LOW);
          digitalWrite(PIN_G, LOW);

			} else {
				Serial.println("Not possible to connect to Broker Error code:");
				Serial.print(client.state());
          digitalWrite(PIN_R, HIGH);
          digitalWrite(PIN_Y, LOW);
          digitalWrite(PIN_G, LOW);
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

void sendToBroker(char* topic, char* message) {
	if (client.connected()) {
		char topicArr[100];
		sprintf(topicArr, "%s/%s", MQTT_CLIENT, topic);
		client.publish(topicArr, message);
	}
}