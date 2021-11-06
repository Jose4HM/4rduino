//si usas platformio, si usas arduino ide comentar esta línea.
//#include <Arduino.h>

//si usas esp32
//#include <HTTPClient.h>
//#include <WiFi.h>

//si usas esp8266
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
WiFiClient wifiClient;

/* Credenciales de tu conexión WiFi*/
const char* ssid = "WIFI";
const char* password =  "21040411";

/*Configuraciones iniciales*/
void setup() {
  delay(10);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Comprobamos la conexión 
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());

}

/*Bucle de ejecución*/
void loop() {

  if(WiFi.status()== WL_CONNECTED){   //Comprobamos el estado de la conexión WiFi

    HTTPClient http;
   //Indicamos el destino
    http.begin(wifiClient,"http://192.168.0.104/practica1/api_get.php?user=Joss&pass=12345");    
   //Preparamos el header text/plain si solo vamos a enviar texto plano sin un paradigma llave:valor.    
    http.addHeader("Content-Type", "plain-text"); 
   //Enviamos el post pasándole, los datos que queremos enviar. 
   //(esta función nos devuelve un código que guardamos en un int)
    int codigo_respuesta = http.GET();   

    if(codigo_respuesta>0){
      //Imprimimos el código de respuesta
      Serial.println("Código HTTP -> " + String(codigo_respuesta));   

      if(codigo_respuesta == 200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió (^_^) (^_^) (^_^) ");
        Serial.println(cuerpo_respuesta);

      }

    }else{

     Serial.print("Error enviando POST, código: ");
     Serial.println(codigo_respuesta);

    }

    http.end();  //libero recursos

  }else{

     Serial.println("Error en la conexión WiFi");

  }

   delay(2000);
}
