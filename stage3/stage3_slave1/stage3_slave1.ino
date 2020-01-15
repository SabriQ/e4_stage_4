#include <Wire.h>

int ena=2;
int dir=3;
int pul=4;
//input
int c_0=5;
int c_1=6;
int c_2=7;
//variables
byte num=5;// default to let led (if there is ) turned off
int ctx[3];
int c_ctx=0;
//int ctx[2];
void setup() {
  // put your setup code here, to run once:
pinMode(ena,OUTPUT);digitalWrite(ena,HIGH);
pinMode(dir,OUTPUT);digitalWrite(dir,LOW);
pinMode(pul,OUTPUT);digitalWrite(pul,LOW);
pinMode(c_0,INPUT);int c_0_value = 0;
pinMode(c_1,INPUT);int c_1_value = 0;
pinMode(c_2,INPUT);int c_2_value = 0;
Wire.begin(1);
Wire.onReceive(receiveinfo);
Serial.begin(9600);
}

void loop() {
  rec(num);
}

void receiveinfo() {  
  while (Wire.available()) {
    num = Wire.read();
//    Serial.println(num);
    }}


void pulse_stepper(int port_out, float Freq)
{
  digitalWrite(port_out, HIGH);
  delayMicroseconds(int(float(500000/800) / Freq));
  //细分是2,表示一圈要400个pulse;半周期是500000us/400;如果要1s2圈，那么半周期就是500000us/400/2
  //delayMicroseconds(483);
  digitalWrite(port_out, LOW);
  delayMicroseconds(int(float(500000/800) / Freq));
  //delayMicroseconds(483);
}

float Read_digital(int digital, int times) {
  float sum = 0;
  for (int i = 0; i <= times; i++) {
    int value = digitalRead(digital);
    sum = sum + value;
  }
  return sum / times;
}
void Read_ctx(){
  if (Read_digital(c_0,10)>0.9){ctx[0]=1;}else{ctx[0]=0;}
  if (Read_digital(c_1,10)>0.9){ctx[1]=1;}else{ctx[1]=0;}
  if (Read_digital(c_1,10)>0.9){ctx[2]=1;}else{ctx[2]=0;}
//  Serial.print(Read_digital(c_0,10));Serial.print(" ");
//  Serial.println(Read_digital(c_1,10));
//  delay(100);
}
void rec(int num){
  switch (num)
  {
    case 0:// go to context 0
    //0 left and right doors go left (approaching motor)
      digitalWrite(ena,LOW);
      digitalWrite(dir,LOW);
      do{Read_ctx();pulse_stepper(pul,40);}while(ctx[0]==0 && ctx[2]==0 ); // between 1-2.2
      digitalWrite(ena,HIGH);
      c_ctx=0;
      break;
    case 1://go to context 1
      digitalWrite(ena,LOW);
      if (c_ctx==0){digitalWrite(dir,HIGH);}
      else if(c_ctx==2){digitalWrite(dir,LOW);}
      else{c_ctx=1;}
      do{Read_ctx();pulse_stepper(pul, 40);}while(ctx[1]==0);
      digitalWrite(ena,HIGH);
      c_ctx=1;
      break;
    case 2://go to context 2
      digitalWrite(ena,LOW);
      digitalWrite(dir,HIGH);
      do{Read_ctx();pulse_stepper(pul,40);}while(ctx[2]==0 && ctx[0]==0);
      digitalWrite(ena,HIGH);
      c_ctx=2;
      break;
    default:
      break;
  }
}
