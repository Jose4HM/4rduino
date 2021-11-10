#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
WiFiClient wifiClient;
/************************ Adafruit IO Configuration *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME    "Yasperterian"
#define IO_KEY         "aio_eJLI76SEvNOL6NZPnqnjvTLZFdXt"

/******************************* WIFI Configuration **************************************/

#define WIFI_SSID       "FIWI"
#define WIFI_PASS       "21040411"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************ Main Program Starts Here *******************************/
#include <ESP8266WiFi.h>
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include <ArduinoHttpClient.h>
int l = 0;
#define LED_PIN 5
#define OTHER_PIN 4

// button state
int current = 0;
int last = 0;

// set up the 'command' feed
AdafruitIO_Feed *command = io.feed("lights");

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(OTHER_PIN, OUTPUT);

  // start the serial connection
  Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'command' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  command->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (l == 0) {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(OTHER_PIN, HIGH);
      l = 1;
    } else if (l == 1) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(OTHER_PIN, LOW);
      l = 0;
    }
  }
  digitalWrite(LED_PIN, LOW);
  digitalWrite(OTHER_PIN, LOW);
  // we are connected
  Serial.println();
  delay(2000);
  // Imprime el IP asignado
  Serial.println("");
  Serial.println(io.statusText());
  digitalWrite(OTHER_PIN, HIGH);
  delay(3000);
  digitalWrite(OTHER_PIN, LOW);


}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();


}

// this function is called whenever a 'command' message
// is received from Adafruit IO. it was attached to
// the command feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  int command = data->toInt();

  if (command == 1) { //light up the LED
    Serial.print("received <- ");
    Serial.println(command);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(OTHER_PIN, HIGH);
  } else {
    Serial.print("received <- ");
    Serial.println(command);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(OTHER_PIN, LOW);
  }
}
