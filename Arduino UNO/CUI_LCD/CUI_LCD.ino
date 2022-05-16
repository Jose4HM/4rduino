boolean replay=true;

void setup() {
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

void loop() {
  if(replay==true){
    matriz();
    numeros();
  }
  replay=false;
}
void habilitacion(){
  digitalWrite(8,1);
  delay(25);
  digitalWrite(8,0);
  delay(25);
 }
void matriz(){
  int variable1[10][4]={{0,0,1,1},{0,0,1,1},{0,0,1,1},{0,0,1,0},{0,0,1,0},{0,0,0,1},
  {0,0,0,0},{1,1,1,1},{1,0,0,0},{0,1,0,1}};
  int contador=3;
  for(int i=0;i<10;i++){
    contador=3;
    habilitacion();
    for(int j=0;j<4;j++){
      contador=contador+1;
      digitalWrite(contador,variable1[i][j]);
      delay(100);
    }
  }
 }
 void numeros(){
  digitalWrite(10,1);
  int CUI[8][4]={{0,0,1,1},{0,0,1,1},{0,0,1,1},{0,0,1,0},{0,0,1,1},{0,1,1,1},
  {0,0,1,1},{0,0,0,0}};//Si nos vamos al datasheet, especifica que para los números, los MSB tienen que ser 0010 y luego el numero en binario.
  int contador2=3;
  for(int i=0;i<8;i++){
    contador2=3;
    habilitacion();
    for(int j=0;j<4;j++){
      contador2=contador2+1;
      digitalWrite(contador2,CUI[i][j]);
      delay(100);
    }
  }
  habilitacion();
 }