#define B_H 5
#define B_M 6
#define B_S 7
int Hstatus=0;
int conf=0;
int status=1;//1 hora, 2 minuto 3 segundo
int h;
int m;
int s;
void time(){
  char buffer [40];
  if (digitalRead(B_H) == LOW){//Si es presionado
    Serial.println("Presionado.....");
    delay(1000);
    h=00;
    m=00;
    while (h<=23){
      while(m<=59){
        for (s=00; s<=59; s++){
          sprintf(buffer,"La hora es %d:%d:%d",h,m,s);
          // Serial.println(s);
          Serial.println(buffer);
          delay(1000);
        }
        m=m+1;
        if(m==60){
          m=00;
          h=h+1;
          if (h==24){
            h=0;
            } 
          }
        }
      }
    } 
  }

void setup() {
  // put your setup code here, to run once:
  pinMode(B_H, INPUT_PULLUP);
  Serial.begin(19200);
  Serial.println("Ready");
  
}
void loop() {
  // put your main code here, to run repeatedly:
  time();
}
