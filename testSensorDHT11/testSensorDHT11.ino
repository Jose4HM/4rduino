#include <DHT.h>
#include <DHT_U.h>
 
// Inicializamos el sensor DHT11
DHT dht(D4, DHT11);
void setup() {
  // Inicializamos comunicación serie
  Serial.begin(115200);
 
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
 
}
