#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);//Enlace medio - protocolo
char msg[30];
void setup() 
{
  setup_wifi();
  client.setServer("18.118.126.113", 1883);//Establece los detalles del servidor
  client.setCallback(callback);
  pinMode(5,OUTPUT);
}
void callback(char* topic, byte* payload, unsigned int length)
  {
    //Serial.println("Entramos al callback");
    payload[length]='\0';
    String val=String((char*)payload);
    Serial.println(val);
    if(val == "A")
    {digitalWrite(5,HIGH);}
    if(val=="B")
    {digitalWrite(5,LOW);}
  }
void reconnect()
  {
      client.connect("ESPVirulento");//conecta al cliente
      Serial.println("Conexion exitosa");
      client.subscribe("subpriv");
  }
void loop() {
  //Comprueba si el cliente esta conectado al servidor
  if(!client.connected())
    {
      reconnect();
    }
    client.loop();
    snprintf(msg, 30,"%d",analogRead(A0));
  client.publish("canalpriv", msg);//publica publish(topic,payload)
  delay(500);
}

void setup_wifi()
{
  Serial.begin(9600);
  WiFi.begin("FIWI", "21040411");
  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
