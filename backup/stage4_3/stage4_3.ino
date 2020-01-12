


int led = 2;
int ON = 13;
float on_signal=0;
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
unsigned long nose_poke_time;
unsigned long choice_time;
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

void loop() {
  // put your main code here, to run repeatedly:


}

void Trial(on_signal,int statement,int choice){
  while(True){
    Read_ir();
    if (on_signal==1){
      rec(statement);
      return Trial(on_signal,statement,choice);
      }else{
        return Trial(on_signal,statement,choice);
      }      
  }
}
void rec(int statement){
  switch(statement){
    case 1:
      while(True){
        if (ir[0]==1){
          nose_poke_time = millis();
          Serial.println("Stat1: nose_poke");
          water_deliver(pump_nose,10); 
          statement = 1-statement;;
          break;}else{Read_ir();}
      }
      break;
    case 2:
      while(True){
        if (ir[1]==1 || ir[2]==1){        
        statement=1-statement;
        break;}else{ Read_ir();}
      }
      break;
     default:
      break;
  }



  
}
void rec_py_signal(int py_signal){
  switch (py_signal)
  {
    case 48://nosepoke
    while(True){
      Read_ir();
      if (on==0 || ir[0]==1){
        nose_poke_time = millis();
        Serial.println("Stat1: nose_poke");
        water_deliver(pump_nose,10);
        break;
      }
    }
    
      digitalWrite(led,LOW);
      do{Read_ir();}while(ir[0]==0);
      nose_poke_time = millis();
      Serial.println("Stat1: nose_poke"); 
      Trial_num =Trial_num + 1;       
      break;
    case 49://context enter
      break;
    case 50://context exit
      break;
    case 51://choice with led continuou on
      do{Read_ir();led_on(led,5);led_off(led,5);}while(ir[1]==0 && ir[2]==0);
      choice_time = millis();    
      digitalWrite(led,LOW);
      Serial.print("Stat2: choice");    
      if (ir[1]==1){
          Serial.println("_l");
          left_choice= left_choice + 1;      
          stat = 1;}
       else if (ir[2]==1){
          right_choice=right_choice + 1;
          Serial.println("_r") ;   
          stat = 2; }
      break;
    case 52://choice with led flash
      do{Read_ir();led_on(led,5);led_off(led,145);}while(ir[1]==0 && ir[2]==0);
      choice_time = millis();    
      Serial.print("Stat2: choice");    
      if (ir[1]==1){
          Serial.println("_l");
          left_choice= left_choice + 1;      
          stat = 1;}
       else if (ir[2]==1){
          right_choice=right_choice + 1;
          Serial.println("_r") ;   
          stat = 2; }
       
      break;

    case 53://nose_pump
      digitalWrite(led,LOW);
      water_deliver(pump_nose,10);
      break;
    case 54://left_pump
      digitalWrite(led,LOW);
      water_deliver(pump_left,10);
      break;
    case 55://right_pump
      digitalWrite(led,LOW);
      water_deliver(pump_right,10);
      break;
    default:
      break;
  } }

  
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
void water_deliver(int pump, int milliseconds){
digitalWrite(pump,HIGH);
delay(milliseconds);
digitalWrite(pump,LOW);  }

void led_on(int port,int milliseconds){
    digitalWrite(port,HIGH);
//    delay(10);Read_ir();
    int start = millis();    
    int diff = 0;
    while(diff<milliseconds){
      Read_ir();int finish = millis();int diff = finish-start;
      if (diff > milliseconds ){break;}}
//    do{Read_ir();int finish = millis();int diff = finish-start;if (diff > milliseconds ){break;}}while(diff<milliseconds);
}
void led_off(int port,int milliseconds){
    digitalWrite(port,LOW);
//    delay(10);Read_ir();
    int start = millis();    
    int diff = 0;
    while(diff<milliseconds){
      Read_ir();int finish = millis();int diff = finish-start;
      if (diff > milliseconds ){break;}}
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
