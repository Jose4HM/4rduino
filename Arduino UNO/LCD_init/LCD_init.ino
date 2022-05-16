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
  InitLCD_4D();
  }
  replay=false;
}

void pulso(){
  digitalWrite(8,1);
  delay(20);
  digitalWrite(8,0);
  delay(20);
 }

void InitLCD_4D(){ 
  int variable1[10][4]={{0,0,1,1},{0,0,1,1},{0,0,1,1},{0,0,1,0},{0,0,1,0},{0,0,0,1},
  {0,0,0,0},{1,1,1,1},{1,0,0,0},{0,1,0,1}};
  int contador=3;
  for(int i=0;i<10;i++){
    contador=3;
    pulso();
    for(int j=0;j<4;j++){
      contador=contador+1;
      digitalWrite(contador,variable1[i][j]);
      delay(100);
    }
  }
  pulso();
 }