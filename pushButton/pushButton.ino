#define PIN_BUTTON D0
#define PIN_LED D1

int ledState = 1;

int newButtonState = 0;
int oldButtonState = 0;

void fnControlButton() {
  newButtonState = digitalRead(PIN_BUTTON);
  if (newButtonState != oldButtonState) {
    oldButtonState = newButtonState;
    if (newButtonState == LOW) {
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, ledState);

}

void loop() {
  //  Serial.println(digitalRead(PIN_BUTTON));
  //  digitalWrite(PIN_LED,digitalRead(PIN_BUTTON));
  //  delay(1000);
  fnControlButton();

}
