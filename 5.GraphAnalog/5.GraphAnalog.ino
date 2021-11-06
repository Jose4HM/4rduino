const int analogInPin=A0;
int SensorValue=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  SensorValue=analogRead(analogInPin);
  Serial.println(SensorValue);
}
