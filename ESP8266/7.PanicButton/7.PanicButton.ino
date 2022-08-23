//Importación de librerias
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "credenciales.h"//lee el archivo de credenciales
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define PIN_BUTTON D0 //Puerto del botón
#define PIN_R D6 //Puerto del led
#define PIN_G D5 //Puerto del led
#define PIN_Y D7 //Puerto del led
#define PIN_B D8 //Puerto del led

//Creación de constantes
const char* servidor_mqtt = "192.168.0.107"; //mosquitto
int puerto = 1883;//puerto del servidor mqtt
int ledState = 0;//estado de led apagado
int emergencyState = 0;//estado 0
int l = 0;//constantes auxiliares
int c = 0;//constantes auxiliares
//vector de coordenadas
float lat[5]={-16.376214, -16.376121, -16.376630, -16.376687, -16.377862};
float lon[5]= {-71.521747, -71.521786,  -71.522362, -71.521826, -71.521274};
String nombre = "Jose";
//crea la instancia del cliente
WiFiClient ClientEsp;
PubSubClient client(ClientEsp);
//Funcion para conectarse al Wifi
void configuracion_wifi() {
  delay(10);
  WiFi.begin(ssid, password);//intentar la coenexión a wifi con las credenciales
  while (WiFi.status() != WL_CONNECTED) {//bucle para conectarase a la red
    delay(500);
    Serial.print(".");
    //alternar los estados de los leds
    if (l == 0) {
      digitalWrite(PIN_G, LOW);
      digitalWrite(PIN_R, HIGH);
      l = 1;
    } else if (l == 1) {
      digitalWrite(PIN_G, HIGH);
      digitalWrite(PIN_R, LOW);
      l = 0;
    }
  }
  //una vez conectado poner los led apagados
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_R, LOW);
  delay(2000);
  Serial.print("Conectado con éxito al WIFI-->");
  lcd.clear();//limpiar la pantalla e imprimir los textos
  lcd.setCursor(0,0);
  lcd.print("   Conectado a   ");
  lcd.setCursor(0,1);
  lcd.print("    "+WiFi.SSID());//imprimir la red wifi conectada
  Serial.println(WiFi.SSID());
  digitalWrite(PIN_G, HIGH);//poner el LED verde encendido para representar el estado
  delay(2000);
  digitalWrite(PIN_G, LOW);
}
//Funcion reconnect mqtt para conectar el MQTT
void reconnect() {
  //Encender el led para indicar que comenzo a conectarse al servidor mqtt
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_B, HIGH);
  //mientras no este conectado
  while (!client.connected()) {
    Serial.println("Intentando conexion MQTT con parametros configurados...");
    //mostrar los estados en el LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Conectando a");
    lcd.setCursor(0,1);
    lcd.print("  Broker MQTT");
    //Si se lográ conectar, mostrar en el display el texto y prender los LED B y Y para indicar una conexión exitosa
    if (client.connect("ESP8266Client")) {
      Serial.println("MQTT conectado!!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("      MQTT ");
      lcd.setCursor(0,1);
      lcd.print("     Conectado");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("      TODO      ");
      lcd.setCursor(0,1);
      lcd.print("     Listo    ");
      delay(1000);
      digitalWrite(PIN_B, LOW);
      digitalWrite(PIN_G, HIGH);
      digitalWrite(PIN_R, LOW);
      digitalWrite(PIN_Y, HIGH);
      delay(800);
      digitalWrite(PIN_G, LOW);
    } else {//Si no se conecta, se encenderá el led rojo indicando un error y se mostrará en el display. Y se intentará la conexión en 5 min
      Serial.println("Ha ocuriido un fallo al conectar MQTT, Estado: ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("     Error con");
      lcd.setCursor(0,1);
      lcd.print("    MQTT");
      digitalWrite(PIN_B, LOW);
      digitalWrite(PIN_R, HIGH);
      Serial.print(client.state());
      Serial.println("Intentando conexión nuevamente en 5 segundos");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Intentando en");
      lcd.setCursor(0,1);
      lcd.print("      5 seg");
      delay(5000);
    }
  }
}
//Funcion para enviar los estados de los LEDs
void pushButton() {
  //Se definen los textos(son variables temporales que se mostraran en el monitos display)
  String ayuda = "Ayuda!!!!!!!!!!!!!!!!! (◕‿◕✿)";
  String Yano = "No es necesaria una ayuda(╯°□°）╯︵ ┻━┻";
  //Si se desconecta el servidor mqtt, se llama a la función reconnect para volver a intentar la conexión
  if (!client.connected()) reconnect();
  client.loop();
  //Si el boton es presionado(1)
  if (digitalRead(PIN_BUTTON) == 1) {//Si el boton es presionado
    String file;//Se crea el payload tipo JSON
    DynamicJsonDocument doc(1024);       
    doc["name"] = nombre ;
    doc["time"]   = 1351824120;            
    doc["lat"] = lat[c]; //or you can put variables here        
    doc["lon"] = lon[c];          
    serializeJson(doc, file);
    //El LED Y se apaga y se prende el led R, indicando que ha sucedido una emergencia
    digitalWrite(PIN_Y, LOW);//Apagamos el LED Y por que estaba encendido
    digitalWrite(PIN_R, HIGH);//Encenderá el led
    Serial.println(ayuda);//Imprimira el texto de ayuda
    //Se muestran los textos en el LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    Ayuda!! ");
    lcd.setCursor(0,1);
    lcd.print("  Ayuda pedida");
    emergencyState = 1;//Se define el estado 1
    client.publish("panic", String(emergencyState).c_str());//Se publica el payload con su topico y el mensaje en tipo string
    client.publish("gps", file.c_str());//Se publica el payload con las coordenadas en tipo JSON
    c++;//Se establece un limite para el vector de coordenadas(si se tuviera un GPS no es necesario esta parte)
    if (c==5){
      c=0;
    }
    Serial.println(emergencyState);
    delay(800);
    digitalWrite(PIN_R, ledState);//Apagará el led
    emergencyState = 0;
    Serial.println(Yano);//Print mensaje de no ayuda
    lcd.clear();//Se muestra en el LCD los estados
    lcd.setCursor(0,0);
    lcd.print("   Ya no >:v ");
    lcd.setCursor(0,1);
    lcd.print("  Tranquilidad");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("      TODO      ");
    lcd.setCursor(0,1);
    lcd.print("      Listo    ");
    digitalWrite(PIN_Y, HIGH);//Se vuelve a encender el LED Y
    Serial.println(emergencyState);
    client.publish("panic", String(emergencyState).c_str());//Y se envia el estado 0
  }
}

void setup() {
  //Se inicializa los baudios
  Serial.begin(115200);
  //Se definen los puertos y su modo de operación
  pinMode(PIN_G, OUTPUT);//Definir la salida al led
  pinMode(PIN_R, OUTPUT);//Definir la salida al led
  pinMode(PIN_B, OUTPUT);//Definir la salida al led
  pinMode(PIN_Y, OUTPUT);//Definir la salida al led
  digitalWrite(PIN_G, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_R, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_B, ledState);//Inicializar el LED a 0
  digitalWrite(PIN_Y, ledState);//Inicializar el LED a 0
  //Inicialización del LCD y texto inicial
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Conectandose a");
  lcd.setCursor(0,1);
  lcd.print("      WIFI");
  //Se llama a las funciones
  configuracion_wifi();
  //Se establece el servidor y el puerto
  client.setServer(servidor_mqtt, puerto);
  pinMode(PIN_BUTTON, INPUT);//Leer el boton
}
  
void loop() {
  //Se llama en un bucle a las función pushButton
  pushButton();
}
