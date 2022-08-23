//DHT with NODE-RED-> Micro Project
//Works but payload may be corrupted when you use buttons
// Libraries
#include "DHT.h"
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>//For use the wifi in esp8266
#include <Ticker.h>//Call repeteatly function
#include <AsyncMqttClient.h>//

// Wifi Credentials
#define WIFI_SSID "Scooby-doo donde estas?"
#define WIFI_PASSWORD "21040411"

// MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 0, 103)
// For a cloud MQTT broker, type the domain name, replacing example.com
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_NAME "esp/sesion/name"
#define MQTT_PUB_TEMP "esp/dht/temperature"
#define MQTT_PUB_HUM "esp/dht/humidity"

#define RST_PIN  D3     // Configurable, see typical pin layout above
#define SS_PIN  D8     // Configurable, see typical pin layout above
// Digital pin connected to the DHT sensor
#define DHTPIN D2
// DHT model
#define DHTTYPE DHT11   // DHT 11 
// set pin numbers
const int buttonPin = D0;     // the number of the pushbutton pin
const int buttonPin2 =  D1;       // the number of the LED pin

// variable for storing the pushbutton status
int buttonState = 0;
int buttonState2 = 0;

// Initialize DHT sensor with its pin and model
DHT dht(DHTPIN, DHTTYPE);

// Variables to hold sensor readings
float temp;
float hum;

MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance
int c;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void setup() {
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(buttonPin2, INPUT);
  Serial.begin(115200);
  Serial.println();
  dht.begin();
  SPI.begin();               // Init SPI bus
  rfid.PCD_Init();        // Init MFRC522 card
  
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

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
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH
  if (buttonState == HIGH) {
    Leer();
  }else if(buttonState2==HIGH){
    Escribir();
  } 
  else {
    unsigned long currentMillis = millis();
    // Every X number of seconds (interval = 10 seconds) 
    // it publishes a new MQTT message
    if (currentMillis - previousMillis >= interval) {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      // New DHT sensor readings
      hum = dht.readHumidity();
      // Read temperature as Celsius (the default)
      temp = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      //temp = dht.readTemperature(true);
      
      // Publish an MQTT message on topic esp/dht/temperature
      uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());                            
      Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TEMP, packetIdPub1);
      Serial.printf("Message: %.2f \n", temp);
  
      // Publish an MQTT message on topic esp/dht/humidity
      uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());                            
      Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
      Serial.printf("Message: %.2f \n", hum);
    }
  }


}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}


void Escribir()
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  byte buffer[34];
  byte block;
  MFRC522::StatusCode status;
  byte tamano_codigo;
  Serial.println("Escribe nuevo codigo y finaliza con #");
  Serial.setTimeout(20000L) ;
  tamano_codigo = Serial.readBytesUntil('#', (char*) buffer, 30);

  String data3=""; //variable para guardar el vector de caracteres en una cadena
  for (int i = 0; i < tamano_codigo; i++) {
    data3+=(char)buffer[i];
  }
  Serial.println("Usuario Nuevo: ");
  Serial.println(data3);

  //llena con espacio vacio lo que no usemos de espacio para el codigo
  for (byte i = tamano_codigo; i < 10; i++) buffer[i] = ' ';
  
  //inicializamos la key que nos permite escribir en la tarjeta
  for(byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  Serial.println("Registre su tarjeta");
  //Look for new cards
  while(! rfid.PICC_IsNewCardPresent()){
    //ciclo infinito. Sale hasta que se ingresa la tarjeta  
  }

  //select one of the cards
  if( ! rfid.PICC_ReadCardSerial()){
    //return;
    Serial.println("Tarjeta no compatible");
  }

 
  for(byte i = 0; i < rfid.uid.size; i++){
  }

  //especificamos el tipo de tarjeta
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  //asignamos un bloque de escritura. 7 bloques de 30 caracteres 
  block = 1;

  //autentificacion de la tarjeta
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));
  //En caso de haber algun error con la key o con el bloque, etc. arroja un error
  if(status != MFRC522::STATUS_OK){
    return;
  }

  //realizamos la escritura en el bloque 1 lo que hay en el buffer limitado a 16 bits
  status = rfid.MIFARE_Write(block, buffer, 16);
  //si hay algun problema en la escritura lanza un error
  if(status != MFRC522::STATUS_OK){
    return;
  }

  Serial.println("Escrito correctamente");
  rfid.PICC_HaltA(); //Halt PICC
  rfid.PCD_StopCrypto1(); //stop encryption
 
}

void Leer()
{
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte tamano_codigo;
  MFRC522::StatusCode status;
  Serial.println("Pase su tarjeta para identifcarse");

  //-------------------------------------------

  // Look for new cards
  while ( ! rfid.PICC_IsNewCardPresent()) {
    //return;
    //se queda en el ciclo hasta que detecta una nueva tarjeta
  }
  // Select one of the cards
  if( ! rfid.PICC_ReadCardSerial()) {
    //return;
    Serial.println("Tarjeta No Compatible Para La Lectura");

  }
  Serial.println("Tarjeta Leida-OK");
  
  tamano_codigo = 18;
  //---------------------------------------- GET LAST NAME

  //declaramos un buffer de la misma longitud de tamano_codigo
  byte buffer2[tamano_codigo];
  block = 1;

  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(rfid.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }

  //hace una lectura del bloque y del Buffer
  status = rfid.MIFARE_Read(block, buffer2, &tamano_codigo);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }
  
  String data=""; //variable para guardar el vector de caracteres en una cadena
  for (int i = 0; i < tamano_codigo; i++) {
    data+=(char)buffer2[i];
  }
  Serial.println(data);
  Serial.println(F(" "));
  //data.replace(" ","#");
  //Send data
  //------------------------------------------
//    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_NAME, 1, true, data.c_str());                 
//    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_SES, packetIdPub1);
//    Serial.printf("Message: Sesion status \n", data);
  //-------------------------------------------
//------------------------------------------
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_NAME, 1, true, data.c_str());                      
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_NAME, packetIdPub1);
    Serial.printf("Message: Sesion status \n", data);
  //-------------------------------------------
  //Serial.println(F("\n**End Reading**\n"));
  
  delay(1000); //change value if you want to read cards faster

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}
