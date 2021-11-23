#define PIN_BUTTON D0 //Puerto del botón
#define PIN_LED D1 //Puerto del led

int ledState = 0;
int emergencyState=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT);//Leer el boton
  pinMode(PIN_LED, OUTPUT);//Definir la salida al led
  digitalWrite(PIN_LED, ledState);//Inicializar el LED a 0
}

void loop() {
  pushButton();
}

void pushButton() {
  String ayuda= "Ayuda!!!!!!!!!!!!!!!!! (◕‿◕✿)";
  String Yano= "Ya no uwu (╯°□°）╯︵ ┻━┻";
  if (digitalRead(PIN_BUTTON) == 1) {//Si el boton es presionado
    digitalWrite(PIN_LED, 1);//Encenderá el led
    Serial.println(ayuda);//Imprimira el texto de ayuda
    emergencyState=1;
    Serial.println(emergencyState);
    delay(1000);//Esperará 1 seg
    digitalWrite(PIN_LED, ledState);//Apagará el led
    emergencyState=0;
    Serial.println(Yano);//Print mensaje de no ayuda
    Serial.println(emergencyState);
  }
}
