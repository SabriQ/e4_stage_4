#include <Wire.h>
byte sending;
byte num;
int led_switch = 1;

int led3 = 9;
void setup() {
  // put your setup code here, to run once:

  pinMode(led3,OUTPUT);

  digitalWrite(led3,LOW);
  Wire.begin(1);
  Wire.onReceive(receiveinfo);
//  Wire.onRequest(sendinfo);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   rec();
}
//当从机接收到主机的字符，执行
void receiveinfo() {  
  while (Wire.available()) {
    num = Wire.read();
    if (num==3){;}else{led_switch=0;}
    }   
    }
        
void sendinfo(){
  Serial.println(sending);
  Wire.write(sending);
}

void rec(){

  switch (num){
    case 3:
    while(led_switch==1){
    for (int i=1;i<10;i++){
      analogWrite(led3,i);
      delay(20);
    }
        for (int i=10;i>0;i--){
      analogWrite(led3,i);
      delay(20);
    }}
    led_switch =1;
    break;
    
    case 4:
    analogWrite(led3,25);
    break;
    
    case 5:
    digitalWrite(led3,LOW);
    break;
    
    default:
 
    break;    
  }
}
