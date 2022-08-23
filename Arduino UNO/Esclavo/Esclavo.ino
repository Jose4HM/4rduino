///////////////////// ARDUINO ESCLAVO /////////////////
//Incluir librerias
//#include <LiquidCrystal.h>
//#include <Wire.h> 
#include <Adafruit_LiquidCrystal.h>
//LiquidCrystal lcd(13, 12, 11, 10, 1, 0); //Modo de 4 bits
Adafruit_LiquidCrystal lcd(0);
//Definición de variables
char buffer[10]={};//Buffer
int var1;//Varible de envio
int var2;//variable de envio
int c=1;//Contador inicial de bits

void setup(){
//Pines de lectura
  //LCD configuración
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  delay(200);
  //Puertos de datos
  DDRD=0b00001111;//Definir puerto D como salida(1) y entrada(0)
  DDRB=0b11110000;//Definir puerto B como salida(1) y entrada(0)
  //Busy 
  pinMode(1, OUTPUT);
  digitalWrite(1,LOW);
  //nStrobe
  pinMode(2, INPUT);
  //nACK
  pinMode(3, OUTPUT);
  digitalWrite(3,HIGH);
  // initialize serial:
  Serial.begin(9600);
}
void loop(){
  while(c<9){//Repetirá hasta que se envien los 8 bits
    if(digitalRead(2)==LOW){//Si el mensaje esta inicializado
      delay(100);//esperamos un momento
      {delay(10);} while (digitalRead(2) == LOW);//Mientras Strobe sea bajo, esperamos que termine de enviar el mensaje
      digitalWrite(1,HIGH);//Indicamos que estamos ocupados
      //Interpretamos el mensaje
      delay(100);
      var1=PIND & 0b11110000;//Hacemos una compuerta AND para solo salvar el nible alto
      var2=PINB & 0b00001111;//Lo mismo con el nimble bajo
      var2=var2|var1;//OR entre ambos nibles
      buffer[c]=var2;//Almacenmaos el valor
      digitalWrite(3,LOW);//Indicamos que lo hemos leido
      delay(100);
      c++;//Contador más 1
      lcd.setCursor(0, 0);
      lcd.print("#");//Imprimimos un asterisco para indicar que salio todo bien
      delay(100);
      digitalWrite(1,LOW);//Indicamos que estamos libres
      delay(200);
      digitalWrite(3,HIGH);//Volvemos a la normalidad (quitarmos la confirmacion de haber leido)
    }
    delay(200);
  }
  Disp();//Cuando termine el bucle while vamos a la función disp
}
void Disp(){
  for (int j=0;j<=8;j++){
    lcd.setCursor(0, 0);//Cursor en la primera linea
    lcd.print("Recibido");//Imprimimos recibido
    lcd.setCursor(j, 1);//Cursor segunda linea
    lcd.print(buffer);//Imprimimos el buffer
    Serial.print(buffer[j]);//Lo confirmamso en el monitor series(opcional)
  }
}