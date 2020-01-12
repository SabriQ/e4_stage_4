#include <Wire.h>
byte send2slave1=0;

int ON = 13;
int pump_nose = 3;
int pump_left = 4;
int pump_right = 5;

int ir_nose = A0;
int ir_left = A1;
int ir_right =A2;
int ir[3];

int stat = 0;
int Trial_num = 0;
int left_choice = 0;
int right_choice = 0;


void setup() {
  // put your setup code here, to run once:
pinMode(ON,INPUT);
pinMode(pump_nose,OUTPUT);digitalWrite(pump_nose,LOW);
pinMode(pump_left,OUTPUT);digitalWrite(pump_left,LOW);
pinMode(pump_right,OUTPUT);digitalWrite(pump_right,LOW);
pinMode(ir_nose,INPUT);digitalWrite(ir_nose,LOW);
pinMode(ir_left,INPUT);digitalWrite(ir_left,LOW);
pinMode(ir_right,INPUT);digitalWrite(ir_right,LOW);
Wire.begin();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  do{Read_ir();}while(ir[0]==0);
  unsigned long nose_poke_time = millis();
  Serial.println("Stat1: nose_poke"); 
  Trial_num =Trial_num + 1;    

  send2slave1=2-send2slave1;
  write_data();
}
