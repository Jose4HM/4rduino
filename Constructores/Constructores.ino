#include <PubSubClient.h>

//PubSubClient;
/*
  void setup() {
  client.setClient(ethClient);
  client.setServer("broker.exmaple.cpm",1883);
  }
*/




PubSubClient client(clienteDeRed);
/*
  void setup(){
  client.setServer("broker.example.com",1883);
  }
*/


//PubSubClient client(servidor, puerto, [callback], cliente, stream);
void reconnect() {
  while (!client.connected()) {
    //la funcion devuelve true o false
    if (client.connect("Client_ID")) {
      //devuelve true o false
      client.subscribe("miCasa/plantaBaja/cocina/temperatura");
      //parametros: topic-cont char[]   qos-int(0 o 1)
    } else {
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length){
  String payload_;
    for(int i=0;i<length;i++){
        payload_+=(char)payload[i];
      }
}

void setup(){
  client.setServer(servidor_mqtt, 1883);
  client.setCallback(callback);
  }

void loop(){
  String payload = "Hola mundo"
  if(!client.connected()) reconnect();
  if(!client.loop)) client.connect("Client_ID");
  client.publish("topic",payload);
  }
