#include <ESP8266WiFi.h>
const char* ssid="WIFI ESP8266";//Name of your red
const char* password="12345678";//Your password, remember, at least 8 characters
void setup() {
  Serial.begin(115200);
  Serial.print("\nSetting Ap");//Status message, you can view it in monitor serie
  WiFi.softAP(ssid, password);
  Serial.println("WiFi is ready");

}

void loop() {
  int device= WiFi.softAPgetStationNum();//It conunts numbers of users connected to your red
  Serial.printf("Devices connected =%d\n",device);
  delay(5000);

}
