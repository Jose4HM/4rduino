#include <DHT.h>
#include <DHT_U.h>

// Inicializamos el sensor DHT11
DHT dht(D4, DHT11);

//si usas esp32
//#include <HTTPClient.h>
//#include <WiFi.h>

//si usas esp8266
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
WiFiClient wifiClient;
 // Pines de salida
const int output5 = 5;
const int output4 = 4;

const char* ssid = "FIWI";
const char* password =  "21040411";

float temperature;
float humedity ;

float vref = 3.3;
float resolucion = vref / 1023;



void setup() {
  delay(10);
  Serial.begin(115200);


  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Inicia salidas en 0
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

}

void loop() {
  delay(5000);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    float humedity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humedity) || isnan(temperature)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      digitalWrite(output5, HIGH);
      digitalWrite(output4, LOW);
      return;
    } else {
      digitalWrite(output4, HIGH);
      digitalWrite(output5, LOW);
    }

    HTTPClient http;

    String datos_a_enviar = "temperature=" + String(temperature) + "&humedity=" + String(humedity);

    http.begin(wifiClient, "http://192.168.0.104/practica1/recibe_data.php");       //Indicamos el destino
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Preparamos el header text/plain si solo vamos a enviar texto plano sin un paradigma llave:valor.

    int codigo_respuesta = http.POST(datos_a_enviar);   //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

    if (codigo_respuesta > 0) {
      Serial.println("Código HTTP --> " + String(codigo_respuesta));   //Print return code

      if (codigo_respuesta == 200) {
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió (^_^) (^_^) (^_^) (^_^) ");
        Serial.println(cuerpo_respuesta);

      }

    } else {

      Serial.print("Error enviando POST, código: ");
      Serial.println(codigo_respuesta);

    }

    http.end();  //libero recursos

  } else {

    Serial.println("Error en la conexión WIFI");

  }

  delay(10000);
}
