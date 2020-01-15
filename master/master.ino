//Master
#include <Wire.h>

byte send2slave1_num=3;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  send2slave1_num = 7-send2slave1_num;
  Serial.println(send2slave1_num);
  write_data(1,send2slave1_num);
  Serial.println("tst");
//  read_data(1,1);

  delay(4000);
//write data to slave1


//if (recfslave1_string.length()>5){
// Serial.println(recfslave1_string);}
// else{
//  Serial.println("no recieving");
// }
}

void write_data(int slave,byte num){
Wire.beginTransmission(slave);
Wire.write(num);
Wire.endTransmission();
}
