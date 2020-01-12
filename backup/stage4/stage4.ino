int led = 2;
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
pinMode(led,OUTPUT);digitalWrite(led,LOW);
pinMode(pump_nose,OUTPUT);digitalWrite(pump_nose,LOW);
pinMode(pump_left,OUTPUT);digitalWrite(pump_left,LOW);
pinMode(pump_right,OUTPUT);digitalWrite(pump_right,LOW);
pinMode(ir_nose,INPUT);digitalWrite(ir_nose,LOW);
pinMode(ir_left,INPUT);digitalWrite(ir_left,LOW);
pinMode(ir_right,INPUT);digitalWrite(ir_right,LOW);
Serial.begin(9600);
}

float Read_analog(int analog, int times) {
  float sum = 0;
  for (int i = 0; i <= times; i++) {
    int value = analogRead(analog);
    sum = sum + value;
  }
  //Serial.println(sum/times);
  return sum / times;
}

float Read_digital(int digital, int times) {
  float sum = 0;
  for (int i = 0; i <= times; i++) {
    int value = digitalRead(digital);
    sum = sum + value;
  }
  return sum / times;
}
void led_on(int port, int on_time,int off_time){
    digitalWrite(led,port);
    delay(on_time);
    digitalWrite(led,LOW);
    delay(off_time);
}

void Read_ir(){
//  int ir_nose = A0;
//int ir_left = A1;
//int ir_right =A2;
  float on_signal = Read_digital(ON, 10);
   if(on_signal >= 0.90){ 
  float ir_nose_value = Read_analog(ir_nose,10);
  float ir_left_value = Read_analog(ir_left,10);
  float ir_right_value = Read_analog(ir_right,10); 
  if (ir_nose_value< 500 && ir_nose_value>5) {ir[0] = 1;}else{ir[0] = 0;} 
  if (ir_left_value< 500 && ir_left_value>5) {ir[1] = 1;}else{ir[1] = 0;} 
  if (ir_right_value< 500 && ir_right_value>5) {ir[2] = 1;}else{ir[2] = 0;} 
//  Serial.print(ir_nose_value);Serial.print(" ");
//  Serial.print(ir_left_value);Serial.print(" ");
//  Serial.print(ir_right_value);Serial.print(" ");
//  Serial.print(ir[0]);Serial.print(" ");
//  Serial.print(ir[1]);Serial.print(" ");
//  Serial.println(ir[2]); 
//  delay(200);
  }else{
    stat=0;
    Trial_num = 0;  
    left_choice = 0;
    right_choice = 0;
  }  }
void water_deliver(int pump, int milliseconds){
digitalWrite(pump,HIGH);
delay(milliseconds);
//Serial.println("test");
digitalWrite(pump,LOW);  }
void rec_py_signal(){
  int py_signal =Serial.read();
  switch (py_signal)
  {
    case 48://led continuous on
      do{Read_ir();led_on(led,10,0);}while(ir[1]==0 && ir[2]==0);
      break;
    case 49://led flash
      do{Read_ir();led_on(led,50,50);}while(ir[1]==0 && ir[2]==0);
      break;
    case 50://led off
      do{Read_ir();led_on(led,0,10);}while(ir[1]==0 && ir[2]==0);
      break;
    case 51://nose_pump
      water_deliver(pump_nose,10);
      break;
    case 52://left_pump
      water_deliver(pump_left,10);
      break;
    case 53://right_pump
      water_deliver(pump_right,10);
      break;
    default:
      break;
  } }
void loop() {
  // put your main code here, to run repeatedly:
//  float on_signal = Read_digital(ON, 10);
//  Serial.println(on_signal);
//  if(on_signal>=0.9){
        
    do{Read_ir();}while(ir[0]==0);
    unsigned long nose_poke_time = millis();
    Serial.println("Stat1: nose_poke"); 
    Trial_num =Trial_num + 1;    
    
    rec_py_signal();
    unsigned long choice_time = millis();    
    Serial.print("Stat2: choice");    
    if (ir[1]==1){
      Serial.println("_l");
     left_choice= left_choice + 1;      
      stat = 1;}
     else if (ir[2]==1){
      right_choice=right_choice + 1;
      Serial.println("_r") ;   
      stat = 2;
     }
//    rec_py_signal();
//    Serial.print("Sum: ");
//    Serial.print(Trial_num);
//    Serial.print(" ");
//    if (stat==1){
//      Serial.print("l");
//      Serial.print(" ");
//      Serial.print(left_choice);
//      Serial.print(" ");
//    }else if(stat ==2){
//      Serial.print("r");
//      Serial.print(" ");
//      Serial.print(right_choice);
//      Serial.print(" ");}  
//    Serial.print(nose_poke_time);
//    Serial.print(" ");
//    Serial.println(choice_time);
//    stat=0;
//  }else{
//    digitalWrite(pump_nose,LOW);
//    digitalWrite(pump_left,LOW);
//    digitalWrite(pump_right,LOW);
//    stat = 0;
//    Trial_num = 0;
//    left_choice = 0;
//    right_choice = 0;}
}
