#include <LiquidCrystal.h>
// PANTALLA LCD 16X2
// AQUI SE CONFIGURAN LOS PINES PARA LA COMUNICACION CON LA PANTALLA
LiquidCrystal lcd(10, 8, 7, 6, 5, 4);//Puertos de pines
void setup(){
  // INDICAMOS QUE TENEMOS CONECTADA UNA PANTALLA DE 16X2
  lcd.begin(16, 2);
  // MOVER EL CURSOR A LA PRIMERA POSICION DE LA PANTALLA (0, 0)
  lcd.home();

  lcd.print("microcontroladores y microprocesadores");
  // MOVER EL CURSOR A LA SEGUNDA LINEA (1) PRIMERA COLUMNA (0)
  lcd.setCursor ( 0, 1 );
  // IMPRIMIR OTRA CADENA EN ESTA POSICION
  lcd.print("laboratorio 2");
  delay(1000);
  }
void loop(){
  // EN EL CICLO PRINCIPAL SOLAMENTE RECORREMOS EL MENSAJE DE UN LADO A OTRO
  int i;
  // DESPLAZAR LA PANTALLA A LA DERECHA 
  for ( int i = 0; i < 23; i++ ) {
    lcd.scrollDisplayLeft();
    delay (150);
  }
  // DESPLAZAR LA PANTALLA A LA IZQUIERDA
  for ( int i = 0; i < 23; i++ ) {
    lcd.scrollDisplayRight();
    delay (150);
  }
}