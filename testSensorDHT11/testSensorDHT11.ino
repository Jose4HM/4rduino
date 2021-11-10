#include <DHT.h>
#include <DHT_U.h>
 // Pines de salida
const int output5 = 5;
const int output4 = 4;
// Inicializamos el sensor DHT11
DHT dht(D4, DHT11);
void setup() {
  // Inicializamos comunicación serie
  Serial.begin(115200);
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Inicia salidas en 0
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  // Comenzamos el sensor DHT
  dht.begin();
 
}
 
void loop() {
    // Esperamos 5 segundos entre medidas
  delay(5000);
 
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  Serial.println("Temperatura: "+String(t)+" Humedad: "+String(h)+" Temp F: "+String(f));
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    digitalWrite(output5, HIGH);
    digitalWrite(output4, LOW);
    return;
  } else{
    digitalWrite(output4, HIGH);
    digitalWrite(output5, LOW);
  }
  }
