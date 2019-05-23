#include <Wire.h>
//----define command set for easy use-----//
#define GET_BOARD_STATUS 0x01
#define SET_PCM_RESET 0x02
#define GET_VERSION_NUMBER 0x04
#define SET_WATCHDOG_TIMEOUT 0x06
#define RESET_SYSTEM_WATCHDOG 0x07
#define GET_NUMBER_SYSTEM_RESETS 0x09
#define SET_PDM_INITIAL_ON 0x0A
#define SET_PDM_INITIAL_OFF 0x0B
#define SET_ALL_PDM_ON 0x0C
#define SET_ALL_PDM_OFF 0x0D
#define GET_PDM_STATUS 0x0E
#define GET_PDM_INITIAL_STATE 0x0F
#define GET_BOARD_TELEM 0x10
#define GET_WATCHDOG_TIMEOUT 0x11
#define SET_PDM_ON 0x12
#define SET_PDM_OFF 0x13
#define GET_SOFT_RESET_NUM 0x14
#define GET_EXPECTED_PDM_STATE 0x16
#define GET_BOARD_TEMP 0x17
#define RESET_NODE 0x80
/*-------------- PDM Pins ----------------*/
int SW0=2;
int SW1=3;
int SW2=4;
int SW3=5;
int SW4=6;
int SW5=9;
int SW6=10;
int SW7=11;
int SW8=12;
int SW9=13;
int SW[10] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7,SW8,SW9};
//-------------- PCM Pins ----------------//
int BV = 14;
int V12 = 15;
int V5 = 16;
int V3_3 = 17;
//----- variables for board status -------//
int last_command=0;
int last_command_range=0;
int wdt=0;
int por=1;
int bor=0;
//---------------------------------------//
//---------- analog read pins ------------//
int pinRead = 0;
//------------ PDM Initial States -------//
int in0 = 0;
int in1 = 1;
int in2 = 0;
int in3 = 1;
int in4 = 1;
int in5 = 0;
int in6 = 1;
int in7 = 0;
int in8 = 0;
int in9 = 0;
//---------------------------------------//
int watchdog = 4;
int x=0;
int written=0;
int reset_freq=0;
int soft_reset_freq=0;
void setup() {
  Serial.begin(9600); 
  Serial.println("EPS powered"); 
  
  int pin_statuses[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  pinMode(SW0, OUTPUT);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);
  pinMode(SW4, OUTPUT);
  pinMode(SW5, OUTPUT);
  pinMode(SW6, OUTPUT);
  pinMode(SW7, OUTPUT);
  pinMode(SW8, OUTPUT);
  pinMode(SW9, OUTPUT);
  pinMode(BV, OUTPUT);
  pinMode(V12, OUTPUT);
  pinMode(V5, OUTPUT);
  pinMode(V3_3, OUTPUT);
  writeValue(SW9,in9);
  writeValue(SW8,in8);
  writeValue(SW7,in7);
  writeValue(SW6,in6);
  writeValue(SW5,in5);
  writeValue(SW4,in4);
  writeValue(SW3,in3);
  writeValue(SW2,in2);
  writeValue(SW1,in1);
  writeValue(SW0,in0);
  for(int i=0; i<=9; i++){
    Serial.print("State of SW");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(SW[i]));
    Serial.println();
  }
  Wire.begin(0x57); //May need to change back to 56 if necessary
  /*  Check to see if the address is 0010 1011 or if it's actually 0101 0111- where the dash is replaced by R/W control bit */
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
}
void writeValue(int pin,int high_low) {
  if(high_low>0) digitalWrite(pin,LOW);
  else digitalWrite(pin,HIGH);
}
void receiveEvent(int howMany) {
  int command = Wire.read();
  int data = Wire.read();
  
  switch(command) {
    case GET_BOARD_STATUS: 
      get_board_status();
      break;
    case SET_PCM_RESET: 
      set_pcm_reset(data);
      break;
    case GET_VERSION_NUMBER:
      get_version_number();
      break;
    case SET_WATCHDOG_TIMEOUT: 
      set_system_watchdog(data);
      break;
    case RESET_SYSTEM_WATCHDOG: 
      reset_system_watchdog();
      break;
    case GET_NUMBER_SYSTEM_RESETS: 
      get_number_resets(data);
      break;
    case SET_PDM_INITIAL_ON:
      set_PDM_Initial_State_on(data);
      break; 
    case SET_PDM_INITIAL_OFF:
      set_PDM_Initial_State_off(data);
      break;
    case SET_ALL_PDM_ON :
      set_all_PDM_on();
      break;
    case SET_ALL_PDM_OFF :
      set_all_PDM_off();
      break;
    case GET_PDM_STATUS :
      get_PDM_status(data);
      break;
    case GET_PDM_INITIAL_STATE: 
      get_PDM_initial_state(data);
      break;
    case GET_BOARD_TELEM :
      get_board_telem(data);
      break;
    case GET_WATCHDOG_TIMEOUT: 
      get_watchdog_timeout();
      break;
    case SET_PDM_ON :
      set_PDM_on(data);
      break;
    case SET_PDM_OFF :
      set_PDM_off(data);
      break;
    case GET_SOFT_RESET_NUM: 
      get_num_soft_reset();
      break;
    case GET_EXPECTED_PDM_STATE: 
      get_all_pdm_initial_state();
      break;
    case GET_BOARD_TEMP :
      get_board_temp();
      break;
    case RESET_NODE :
      reset_node();
      break;
  }
}
void requestEvent() {
  int command = Wire.read();
  switch(command){
    case GET_BOARD_STATUS: 
      get_board_status();
      break;
   
    case GET_VERSION_NUMBER:
      get_version_number();
      break;
    
    case RESET_SYSTEM_WATCHDOG: 
      reset_system_watchdog();
      break;
    
    
    case SET_ALL_PDM_ON :
      set_all_PDM_on();
      break;
    case SET_ALL_PDM_OFF :
      set_all_PDM_off();
      break;
   
    
    case GET_WATCHDOG_TIMEOUT: 
      get_watchdog_timeout();
      break;
    
    
    case GET_SOFT_RESET_NUM: 
      get_num_soft_reset();
      break;
    case GET_EXPECTED_PDM_STATE: 
      get_all_pdm_initial_state();
      break;
    case GET_BOARD_TEMP :
      get_board_temp();
      break;
    case RESET_NODE :
      reset_node();
      break;
  }
}
void get_board_status(){
  int board_status[32] = {bor , por , wdt , 0, 0, 0, last_command_range, last_command , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0};
  byte board_byte;
  
  for(int i=31; i>=0; i--){
    board_byte |= (board_status[i] << i);
  }
  
  Wire.write(board_byte);
  
}
void set_pcm_reset(int data){
  if(data == 0x01){
    digitalWrite(BV, LOW);
  }
  else if(data == 0x02){
    digitalWrite(V5, LOW);
  }
  else if(data == 0x03){
    digitalWrite(V5, LOW);
    digitalWrite(BV, LOW);
  }
  else if(data == 0x04){
    digitalWrite(V3_3, LOW);
    
  }
  else if(data == 0x05){
    digitalWrite(V3_3, LOW);
    digitalWrite(BV, LOW);
    
  }
  else if(data == 0x06){
    digitalWrite(V3_3, LOW);
    digitalWrite(V5, LOW);
   
  }
  else if(data == 0x07){
    digitalWrite(V3_3, LOW);
    digitalWrite(BV, LOW);
    digitalWrite(V5, LOW);
  }
  else if(data == 0x08){
    digitalWrite(V12, LOW);
  }
  else if(data == 0x09){
    digitalWrite(BV, LOW);
    digitalWrite(V12, LOW);
  }
  else if(data == 0x0A){
    digitalWrite(V12, LOW);
    digitalWrite(V5, LOW);
  }
  else if(data == 0x0B){
    digitalWrite(V12, LOW);
    digitalWrite(V5, LOW);
    digitalWrite(BV, LOW);
  }
  else if(data == 0x0C){
    digitalWrite(V12, LOW);
    digitalWrite(V3_3, LOW);
  }
  else if(data == 0x0D){
    digitalWrite(V12, LOW);
    digitalWrite(V3_3, LOW);
    digitalWrite(BV, LOW);
  }
  else if(data == 0x0E){
    digitalWrite(V12, LOW);
    digitalWrite(V5, LOW);
    digitalWrite(V3_3, LOW);
  }
  else if(data == 0x0F){
    digitalWrite(V12, LOW);
    digitalWrite(BV, LOW);
    digitalWrite(V3_3, LOW);
    digitalWrite(V5, LOW);
  }
  else{
    
  }
  delay(500);
  digitalWrite(V12, HIGH);
  digitalWrite(BV, HIGH);
  digitalWrite(V3_3, HIGH);
  digitalWrite(V5, HIGH);
}
void get_version_number(){
  Wire.write(0x01);
}
void set_system_watchdog(int data){
  watchdog = data;
}
void reset_system_watchdog(){
  //just kick it//
}
void get_number_resets(int data){
  Wire.write(reset_freq);
}
int switchValue(int prev) {
  if(prev>0) {
    return 1;
  }
  else return 0;
}
void get_PDM_status(int data){
  switch(data) {
    case 1:
      pinMode(SW0, INPUT);
      Wire.write(switchValue(digitalRead(SW0)));
      pinMode(SW0, OUTPUT);
      break;
    case 2:
      pinMode(SW1, INPUT);
      Wire.write(switchValue(digitalRead(SW1)));
      pinMode(SW1, OUTPUT);
      break;
    case 3:
      pinMode(SW2, INPUT);
      Wire.write(switchValue(digitalRead(SW2)));
      pinMode(SW2, OUTPUT);
      break;
    case 4:
      pinMode(SW3, INPUT);
      Wire.write(switchValue(digitalRead(SW3)));
      pinMode(SW3, OUTPUT);
      break;
    case 5:
      pinMode(SW4, INPUT);
      Wire.write(switchValue(digitalRead(SW4)));
      pinMode(SW4, OUTPUT);
      break;
    case 6:
      pinMode(SW5, INPUT);
      Wire.write(switchValue(digitalRead(SW5)));
      pinMode(SW5, OUTPUT);
      break;
    case 7:
      pinMode(SW6, INPUT);
      Wire.write(switchValue(digitalRead(SW6)));
      pinMode(SW6, OUTPUT);
      break;
    case 8:
      pinMode(SW7, INPUT);
      Wire.write(switchValue(digitalRead(SW7)));
      pinMode(SW7, OUTPUT);
      break;
    case 9:
      pinMode(SW8, INPUT);
      Wire.write(switchValue(digitalRead(SW8)));
      pinMode(SW8, OUTPUT);
      break;
    case 10:
      pinMode(SW9, INPUT);
      Wire.write(switchValue(digitalRead(SW9)));
      pinMode(SW9, OUTPUT);
      break;
  }
}
void set_PDM_Initial_State_on(int data){
  switch(data) {
    case 1:
      in0 = 0;
      break;
    case 2:
      in1 = 0;
      break;
    case 3:
      in2 = 0;
      break;
    case 4:
      in3 = 0;
      break;
    case 5:
      in4 = 0;
      break;
    case 6:
      in5 = 0;
      break;
    case 7:
      in6 = 0;
      break;
    case 8:
      in7 = 0;
      break;
    case 9:
      in8 = 0;
      break;
    case 10:
      in9 = 0;
      break;
    default:
      break;
  }
}
void set_PDM_Initial_State_off(int data){
  switch(data) {
    case 1:
      in0 = 0;
      break;
    case 2:
      in1 = 0;
      break;
    case 3:
      in2 = 0;
      break;
    case 4:
      in3 = 0;
      break;
    case 5:
      in4 = 0;
      break;
    case 6:
      in5 = 0;
      break;
    case 7:
      in6 = 0;
      break;
    case 8:
      in7 = 0;
      break;
    case 9:
      in8 = 0;
      break;
    case 10:
      in9 = 0;
      break;
    default:
      break;
  }
}
void set_all_PDM_on() {
  for (int x = 0; x<=9; x++){
    digitalWrite(x+2, LOW);
  }
}
void set_all_PDM_off() {
  for (int x = 0; x<=9; x++){
    digitalWrite(x+2, HIGH);
  }
}
void set_PDM_on(int data) {
  switch(data) {
    case 1:
      digitalWrite(SW0, LOW);
      break;
    case 2:
      digitalWrite(SW1, LOW);
      break;
    case 3:
      digitalWrite(SW2, LOW);
      break;
    case 4:
      digitalWrite(SW3, LOW);
      break;
    case 5:
      digitalWrite(SW4, LOW);
      break;
    case 6:
      digitalWrite(SW5, LOW);
      break;
    case 7:
      digitalWrite(SW6, LOW);
      break;
    case 8:
      digitalWrite(SW7, LOW);
      break;
    case 9:
      digitalWrite(SW8, LOW);
      break;
    case 10:
      digitalWrite(SW9, LOW);
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
  }
}
void set_PDM_off(int data){
  switch(data) {
    case 1:
      digitalWrite(SW0, HIGH);
      break;
    case 2:
      digitalWrite(SW1, HIGH);
      break;
    case 3:
      digitalWrite(SW2, HIGH);
      break;
    case 4:
      digitalWrite(SW3, HIGH);
      break;
    case 5:
      digitalWrite(SW4, HIGH);
      break;
    case 6:
      digitalWrite(SW5, HIGH);
      break;
    case 7:
      digitalWrite(SW6, HIGH);
      break;
    case 8:
      digitalWrite(SW7, HIGH);
      break;
    case 9:
      digitalWrite(SW8, HIGH);
      break;
    case 10:
      digitalWrite(SW9, HIGH);
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      break;
  }
}
void get_PDM_initial_state(int data){
  switch(data) {
    case 1:
      Wire.write(in0);
      break;
    case 2:
      Wire.write(in1);
      break;
    case 3:
      Wire.write(in2);
      break;
    case 4:
      Wire.write(in3);
      break;
    case 5:
      Wire.write(in4);
      break;
    case 6:
      Wire.write(in5);
      break;
    case 7:
      Wire.write(in6);
      break;
    case 8:
      Wire.write(in7);
      break;
    case 9:
      Wire.write(in8);
      break;
    case 10:
      Wire.write(in9);
      break;
    default:
      break;
  }
}
void get_watchdog_timeout(){
  Wire.write(watchdog);
}
void get_num_soft_reset(){
  Wire.write(soft_reset_freq);
}
void get_all_pdm_initial_state(){
  int pdm_status[16] = {0, 0, 0, 0, 0, in9, in8, in7, in6, in5, in4, in3, in2, in1, in0};
  byte status_byte;
  for(int i=0; i<16 ; i++){
    status_byte |= pdm_status[i] << i;
  }
  Wire.write(status_byte);
  
}
void get_board_telem(int data){
  if (data == 0x23) {
    deal_with_potentiometer_stuff();
  }
  else{
    
  }
  
}
void deal_with_potentiometer_stuff(){
  double pot_volt = .0025*(double)(analogRead(pinRead));
  pot_volt += 6;
  pot_volt *= 100;
  Wire.write((int)(pot_volt));
  
}
void get_board_temp(){
  Wire.write(52); //Dummy value
}
void reset_node(){
  
}
void loop() {
  delay(100);
}
