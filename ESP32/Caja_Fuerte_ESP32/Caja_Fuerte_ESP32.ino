// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#define EEPROM_SIZE 24
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
#define button 15
#define led 33
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
int counter=0;
int attempts=0;
//String Pass ="";
byte pin_rows[ROW_NUM]      = {23, 22, 21, 19}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = {18, 5, 4, 2};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
//Keypad configuration
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
byte password[6];//Array which contain the password from EEPROM
const String pass2config = "123456"; // change your password here
String input_password;
String input_password2;
String newPassStr;
String bkp_pass;
String bkp_passC;
String Pass="193270";
char ban[4]={'A','B','C','D'};
bool locked=true;
bool noLetter=false;
int menuLvl=0;
//LCD constant
const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Configura LCD
void EEPROMCon(){
  //Write EEPROM
  EEPROM.begin(EEPROM_SIZE);//Inite EEPROM
  //Default password
  EEPROM.write(0,'1');
  EEPROM.write(1,'9');
  EEPROM.write(2,'3');
  EEPROM.write(3,'2');
  EEPROM.write(4,'7');
  EEPROM.write(5,'0');
}
void setup() {
  EEPROMCon();
  Serial.begin(9600);//baudios
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Password");
  lcd.print("LOCKED");
  // for(int i=0;i<6;i++){
  //   char character=(char)EEPROM.read(i);
  //   Pass+=character;
  // }
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  //Serial.println(Pass);
}

void loop() {
  if(menuLvl==0){
    unlock();
  }else if(menuLvl==1){
    menu();
  }
  else if(menuLvl==2){
    lock();
  }
}

void menu(){
  char key = keypad.getKey();
    if(attempts==2){
      lcd.clear();
      lcd.print("Locked by");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("5 minutes");
      delay(2000);
      for (int d=0;d<16;d++){
        lcd.setCursor(d,0);
        lcd.print("#");
        delay(3000/16);
      }
      attempts=0;
      clearLCD();
    }
    else{
      if (key) {
          lcd.setCursor(counter,1);
          lcd.print(key);
          delay(500);
          lcd.setCursor(counter,1);
          lcd.print("*");
          counter++;
          if (key == '#') {
            clearLCD();
            input_password=bkp_pass;
            for (int j=0;j<bkp_pass.length();j++){
              lcd.setCursor(j,1);
              lcd.print('*');
            }
            counter=bkp_pass.length();
          }
          else if (key == '*') {
            if (Pass == input_password) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("ACCESS GRANTED!");
              //lcd.setCursor(0,1);
              //lcd.print("Close: button + * ");
              counter=0;
              menuLvl=2;

            }else if(input_password== pass2config){
              lcd.clear();
              lcd.print("Change pass");
              delay(1500);
              getData();
              loop();
            }
            else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(1-attempts);
              lcd.setCursor(3,0);
              lcd.print("lifes");
              lcd.setCursor(0,1);
              lcd.print("Remaining");
              attempts++;
              delay(3000);
              clearLCD();
            }
            input_password = ""; // clear input password
          }
          else{
            bkp_pass=input_password;
            input_password += key; // append new character to input password string
          }
        }
    }
}

void getData(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Write New Pass:");
  int c1=0;
  newPassStr = ""; // clear input password
  bool confirm=false;
  while (confirm==false){
    char key3 = keypad.getKey();
    if (key3){
      if(lett(key3)==false){
        lcd.setCursor(c1,1);
        lcd.print(key3);
        delay(500);
        lcd.setCursor(c1,1);
        lcd.print("*");
        c1++;
        if (key3 == '#') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("New password:");
          newPassStr=bkp_passC;
          for (int j=0;j<bkp_passC.length();j++){
            lcd.setCursor(j,1);
            lcd.print('*');
          }
          c1=bkp_passC.length();
        }
        else if(key3=='*'){
          if ((newPassStr.length())!=6){
            lcd.clear();
            lcd.print("Only 6 numbers");
            delay(1000);
            lcd.clear();
            lcd.print("Try again");
            delay(1000);
            lcd.clear();
            lcd.print("New password:");
            c1=0;
            newPassStr = ""; // clear input password
          }
          else {
            confirm=true;
            lcd.clear();
            lcd.print("Are you sure?");
            //Serial.println(newPassStr);
            bool confirmChange=false;
            while(confirmChange==false){
              char key3 = keypad.getKey();
              if(key3=='A'){
                lcd.clear();
                lcd.print("Done");
                delay(1500);
                confirmChange=true;
                WriteEPR();
              }
              else if(key3=='B'){
                lcd.clear();
                lcd.print("Canceled");
                delay(1500);
                confirmChange=true;
                menuLvl=0;
                setup();
              }
              else{
                lcd.setCursor(0,1);
                lcd.print("A:yes  B:no");
                delay(100);
              }
            }
          }
        }
        else {
          bkp_passC=newPassStr;
          newPassStr += key3; // append new character to input password string
        }
      }else{
        lcd.clear();
        lcd.print("No symbols");
        delay(1000);
        lcd.clear();
        lcd.print("New password:");
        c1=0;
        newPassStr="";
      }
      
}
  }
//  return newPassStr;
}

void clearLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password");
  lcd.setCursor(0,1);
  counter=0;
  input_password = "";
}

void WriteEPR(){
  //Serial.println("xxxxxxxxxxxxxxx");
  // for (int l=0;l<6;l++){
  //   EEPROM.write(newPassStr.charAt(l),l);
  // }
  // lcd.clear();
  // lcd.print("Changed");
  // lcd.setCursor(0,1);
  // lcd.print("Done");
  // delay(1000);
  //Serial.println(newPassStr);
  Pass=newPassStr;
  Serial.println(Pass);
  menuLvl=0;
  setup();
}

void unlock(){
  char keyE = keypad.getKey();
  if(keyE=='*'){
    menuLvl=1;
    clearLCD();
  }
}

void lock(){
  if(digitalRead(button)==LOW){
    digitalWrite(led, HIGH);
    delay(1500);
    bool ClosedConf=false;
    while(ClosedConf==false){
      char cls = keypad.getKey();
      if(cls=='*'){
        ClosedConf=true;
      }
    }
    ClosedConf=false;
    //Serial.println("Cerrado");
    menuLvl=0;
    lcd.clear();
    clearLCD();
    setup();
  }
  else{
    digitalWrite(led,LOW);
  }
}

bool lett(char evaluate){
  //Serial.println(evaluate);
  for (int comp=0;comp<4;comp++){
    if (evaluate==ban[comp]){
      return true;
    }
  }
  return false;
}
