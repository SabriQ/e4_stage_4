//将所有的过程全部放在rec_py_signal中,这样可以模块化各个过程，不好的地方在于，无法杜绝嵌套产生的循环
//nano在D3、D5、D6、D9、D10、D11引脚上有正弦波符号，通俗来讲就是可以输出PWM波的引脚注释
//A5 SCl； A4 SDA  are used for IIC communication


int ir_ll = A0;// infra red left-left lick
int ir_lr = A1;// infra red left-right lick
int ir_ls = A2;// infra red left-start of context
int ir_rl = A3;// infra red right-left lick  Serial.print(ll);Serial.print(" ");
int ir_rr = A6;// infra red right-right lick
int ir_rs = A7;// infra red right-start of context
int ir[6];

int ON = 13;
int pump_ll = 2;
int pump_lr = 3;
int pump_rl = 4;
int pump_rr = 5;

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

unsigned long np_time;
unsigned long ctx_ent_time;
unsigned long ctx_ext_time;
unsigned long choice_time;
unsigned long ctx_Rent_time;
unsigned long ctx_Rext_time;

void setup() {
  // put your setup code here, to run once:
pinMode(ON,INPUT);digitalWrite(ON,LOW);
pinMode(pump_ll,OUTPUT);digitalWrite(pump_ll,LOW);
pinMode(pump_lr,OUTPUT);digitalWrite(pump_lr,LOW);
pinMode(pump_rl,OUTPUT);digitalWrite(pump_rl,LOW);
pinMode(pump_rr,OUTPUT);digitalWrite(pump_rr,LOW);

pinMode(ir_ll,INPUT);
pinMode(ir_lr,INPUT);
pinMode(ir_ls,INPUT);
pinMode(ir_rl,INPUT);
pinMode(ir_rr,INPUT);
pinMode(ir_rs,INPUT);

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
      Serial.print("terminated ");}   
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
        do{Read_ir();}while(ir[0]==0);
        np_time = millis();
        Serial.println("Stat1: nose_poke");
        rec_py_signal(48);//water_deliver for nosepoke @ left-left
        //rec_py_signal(49);//water_deliver for nosepoke @ left-right
        Trial_num =Trial_num+1;
        break;
    case 1:// waiting for ctx_enter
        do{Read_ir();}while(on_signal > 0.5 && ir[2]==0);
        ctx_ent_time = millis();
        Serial.println("Stat2: context_enter");
        break;
    case 2:
        do{Read_ir();}while(on_signal > 0.5 && ir[5]==0);
        ctx_ext_time = millis();
        Serial.println("Stat3: context_exit");
        break;
    case 3:
        do{Read_ir();}while(on_signal > 0.5 &&ir[3]==0 && ir[4]==0);
        choice_time = millis();
        Serial.println("Stat4: choice");   
        if (ir[3]==1){
            rec_py_signal(49);
            Serial.print("_l");
            left_choice= left_choice + 1;   
            Serial.println(" correct");
            Choice_class = 1; }
         else if (ir[4]==1){
            right_choice=right_choice + 1;
            Serial.print("_r") ;   
            Serial.println(" wrong");
            Choice_class = 0; }   
         else {
            Serial.println(" terminated");
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
          Serial.println(" wrong");
          Choice_class = 0;}
      else if (ir[2]==1){
          rec_py_signal(50);
          right_choice=right_choice + 1;
          Serial.print("_r") ;   
          Serial.println(" correct");
          Choice_class = 1; }  
       else {
          Serial.println(" terminated");
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
  on_signal = Read_digital(ON, 10);
//  Serial.print(on_signal);Serial.print(" ");
  if(on_signal >= 0.90){ 
   if (Serial.available()){
    int py_signal = Serial.read();
    rec_py_signal(py_signal);}
    
  float ll = Read_analog(ir_ll,10);
  float lr = Read_analog(ir_lr,10);
  float ls = Read_analog(ir_ls,10);
  float rl = Read_analog(ir_rl,10);
  float rr = Read_analog(ir_rr,10);
  float rs = Read_analog(ir_rs,10);
  
  if (ll < 500 && ll >5){ir[0] = 1;}else{ir[0] = 0;}  
  if (lr < 500 && lr >5){ir[1] = 1;}else{ir[1] = 0;}  
  if (ls < 500 && ls >5){ir[2] = 1;}else{ir[2] = 0;}  
  if (rl < 500 && rl >5){ir[3] = 1;}else{ir[3] = 0;}  
  if (rr < 800 && rr >5){ir[4] = 1;}else{ir[4] = 0;}  
  if (rs < 500 && rs >5){ir[5] = 1;}else{ir[5] = 0;} 
  
  Serial.print(ll);Serial.print(" ");
  Serial.print(lr);Serial.print(" ");
  Serial.print(ls);Serial.print(" ");
  Serial.print(rl);Serial.print(" ");
  Serial.print(rr);Serial.print(" ");
  Serial.println(rs);  
  delay(100);
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
