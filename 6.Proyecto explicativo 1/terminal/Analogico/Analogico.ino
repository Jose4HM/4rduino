const int analogInPin = A0;

float SensorValue = 0;
float vref = 3.3;
float resolucion = vref/1023;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
SensorValue = analogRead(analogInPin)*resolucion;

Serial.println(SensorValue);

delay(10);
}
