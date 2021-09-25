#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);//Enlace medio - protocolo
char msg[30];
void setup() 
{
  setup_wifi();
  client.setServer("localhost", 1883);//Establece los detalles del servidor
  
}
void reconnect()
  {
      client.connect("ESPVirulento");//conecta al cliente
      Serial.println("Conexion exitosa");
  }
void loop() {
  //Comprueba si el cliente esta conectado al servidor
  if(!client.connected())
    {
      reconnect();
    }
    snprintf(msg, 30,"%d",analogRead(A0));
  client.publish("canal", msg);//publica publish(topic,payload)
  delay(2000);

}

void setup_wifi(){
  Serial.begin(9600);
  WiFi.begin("FIWI", "21040411");
  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  }
