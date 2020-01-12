//将所有的过程全部放在rec_py_signal中,这样可以模块化各个过程，不好的地方在于，无法杜绝嵌套产生的循环
int led = 3;
int ON = 12;
int pump_nose = 4;
int pump_left = 5;
int pump_right = 6;

int ir_nose = A0;
int ir_left = A1;
int ir_right =A2;
int ir[3];
float on_signal;
int process =0;
//in temp[60], 0 for left, 1 for right
int temp[60] = {0,1,0,1,0,1,1,0,1,0,
                1,1,1,1,0,0,0,0,1,0,
                0,1,1,0,0,1,1,1,0,0,
                1,0,0,1,1,0,1,0,1,0,
                0,1,1,0,0,1,0,1,0,1,
                0,1,0,1,0,1,1,0,1,0};
int trial_length = 60;
int i =0;
int Trial_num = 0;
int left_choice = 0;
int right_choice = 0;
int Choice_class = 0;
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
////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:  
 
  for (i=0;i<trial_length;i++){
    rec_process(process);
//    Serial.print("|");Serial.print(temp[i]);Serial.print("--");Serial.print(Choice_class);Serial.println("");
    process = temp[i]+1;  
//    Serial.print(process);Serial.print("|-");Serial.println("");
    rec_process(process);
    process=0;
//    Serial.print("-");Serial.print(temp[i]);Serial.print("--");Serial.print(Choice_class);Serial.println("");
    Serial.print("Sum: ");
    Serial.print(Trial_num);Serial.print(" ");
    Serial.print(left_choice);Serial.print(" ");
    Serial.print(right_choice);Serial.print(" ");
    if (Choice_class==1){
      i=i;
      Serial.print("correct ");}
    else if(Choice_class==0){
      i = i-1;
      Serial.print("wrong ");}
    else{
      i=0;
      Serial.print("terminated");}   
//     Serial.print("--");Serial.print(i);Serial.println("");
      Serial.print(nose_poke_time);Serial.print(" ");
      Serial.println(choice_time);
     }
   Serial.println("All done!");
}
///////////////////////////////////////////////
void rec_process(int process){
  switch (process)
  {
    case 0://waiting for nosepoke
      digitalWrite(led,LOW);
      do{Read_ir();}while(ir[0]==0);
      nose_poke_time = millis();
      Serial.println("Stat1: nose_poke");
      rec_py_signal(48);
      Trial_num =Trial_num+1;
      break;

    case 1://waiting for choice with led flash , mouse should choose left
      do{led_flash(led);}while(on_signal>0.5 && ir[1]==0 && ir[2]==0);
      choice_time = millis();    
      digitalWrite(led,LOW);
      Serial.print("Stat2: choice");    
      if (ir[1]==1){
          rec_py_signal(49);
          Serial.print("_l");
          left_choice= left_choice + 1;   
          Serial.println("  correct");
          Choice_class = 1; }
       else if (ir[2]==1){
          right_choice=right_choice + 1;
          Serial.print("_r") ;   
          Serial.println("  wrong");
          Choice_class = 0; }   
       else {
          Serial.println("_terminated");
//          Serial.print("on_signal: ");
//          Serial.println(on_signal);
          Choice_class = 2; 
       }
      break;
      
    case 2://wating for choice with led continuous on, mouse should choose right
      do{led_on(led);}while(on_signal>0.5 && ir[1]==0 && ir[2]==0);
      choice_time = millis();  
      digitalWrite(led,LOW);  
      Serial.print("Stat2: choice");    
      if (ir[1]==1){
          left_choice= left_choice + 1; 
          Serial.print("_l") ;   
          Serial.println("  wrong");
          Choice_class = 0;}
      else if (ir[2]==1){
          rec_py_signal(50);
          right_choice=right_choice + 1;
          Serial.print("_r") ;   
          Serial.println("  correct");
          Choice_class = 1; }  
       else {
          Serial.println("_terminated");
//          Serial.print("on_signal: ");
//          Serial.println(on_signal);
          Choice_class = 2; 
       }  
      break;
      default:
      break;
  }}  

void rec_py_signal(int py_signal){
  switch (py_signal)
  {
    case 48://nose_pump
      digitalWrite(led,LOW);
      water_deliver(pump_nose,10);
      break;
    case 49://left_pump
      digitalWrite(led,LOW);
      water_deliver(pump_left,10);
      break;
    case 50://right_pump
      digitalWrite(led,LOW);
      water_deliver(pump_right,10);
      break;
    default:
    break;}}
  
void led_flash(int port){
    for(int j=0;j<50;j++){
      analogWrite(led,j);
      delay(1);
      Read_ir();
      if (Serial.available()){
        int py_signal = Serial.read();
      rec_py_signal(py_signal);}
  }
    for(int j=50;j>0;j--){
      analogWrite(led,i);
      delay(1);
      Read_ir();
      if (Serial.available()){
        int py_signal = Serial.read();
      rec_py_signal(py_signal);}
  }
}

void led_on(int port){
    analogWrite(led,50);
    delay(10);
    Read_ir();
    if (Serial.available()){
      int py_signal = Serial.read();
      rec_py_signal(py_signal);}
}

void Read_ir(){
//  int ir_nose = A0;
//int ir_left = A1;
//int ir_right =A2;
  on_signal = Read_digital(ON, 20);
//  Serial.print(on_signal);Serial.print(" ");
  if(on_signal >= 0.90){ 
   if (Serial.available()){
    int py_signal = Serial.read();
    rec_py_signal(py_signal);}
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
    i = 0;
    Trial_num = 0;  
    left_choice = 0;
    right_choice = 0;
  }  }
//////////////////////////////////////////
float Read_analog(int analog, int times) {
  float sum = 0;
  for (int i = 1; i <= times; i++) {
    int value = analogRead(analog);
    sum = sum + value;
  }
  //Serial.println(sum/times);
  return sum / times;
}

float Read_digital(int digital, int times) {
  float sum = 0;
  for (int i = 1; i <= times; i++) {
    int value = digitalRead(digital);
    sum = sum + value;
  }
  return sum / times;
}
void water_deliver(int pump, int milliseconds){
digitalWrite(pump,HIGH);
delay(milliseconds);
digitalWrite(pump,LOW);  }
