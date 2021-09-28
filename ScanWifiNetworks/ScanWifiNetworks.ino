#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);//Esta instrucción le indica al Arduino que inicie comunicación con la computadora (o cualquier dispositivo conectado a los pines RX y TX) con una velocidad de comunicación serial de 9600 bits por segundo (baudios)
  WiFi.mode(WIFI_STA);//Empezar la ESP en modo estacion
  WiFi.disconnect();//Si ya se hizo funcionar el ESP como un cliente de una red es posible que se conecte automaticamente a nuestra red, asi que nos desconectamos de cualquier red
  delay(1000);
}

void loop() {
  Serial.println("\nWifi scan begin");//imprimir mensaje de estado
  int n=WiFi.scanNetworks();//Esta funcion escanea las redes en formas de punteros. Esta función devuelve un numero entero
  if(n == 0){
    Serial.println("No networks");
  }
  //Numero nombre Potencia MAC
  for(int i=0; i<n; i++){
    Serial.print(i+1);
    Serial.print(" - ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" ) MAC: ");
    Serial.println(WiFi.BSSIDstr(i));
  }
  delay(5000);
  }
