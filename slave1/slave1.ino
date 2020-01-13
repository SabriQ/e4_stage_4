#include <Wire.h>
byte sending;
byte num;
int led2 = 2;
int led3 = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  Wire.begin(1);
  Wire.onReceive(receiveinfo);
  Wire.onRequest(sendinfo);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
rec(num);
}
//当从机接收到主机的字符，执行
void receiveinfo() {  
  while (Wire.available()) {
    num = Wire.read();
//    Serial.println(num);
    }   
    }
        
void sendinfo(){
  Serial.println(sending);
  Wire.write(sending);
}

void rec(int num){
  switch (num){
    case 0:
    while(true){
    for (int i=1;i<10;i++){
      analogWrite(led3,i);
      delay(20);
    }
        for (int i=10;i>0;i--){
      analogWrite(led3,i);
      delay(20);
    }}
    break;
    
    case 1:
    digitalWrite(led3,HIGH);
    break;
    case 2:
    digitalWrite(led3,LOW);
    
    default:
 
    break;    
  }
}
