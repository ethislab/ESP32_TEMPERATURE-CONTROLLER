
#define BUTTON    // button enable
#include <PID_v1.h>
#include "WiFi.h"
#include "ArduinoNvs.h"
//#include <TM1638lite.h>
#include <TM1638.h>
#include <ADS1232.h>
#include "tc.h"
#include "th.h"
#define pdwn 18    //for ADS1232
#define dout 19   //for ADS1232
#define sclk  21   //for ADS1232
//#define CH_SEL 25  //for ADS1232 CH selection
//#define SSR_PIN 13  // Relay output
#define strobe 27
#define clk 26
#define datapin 25
#define CH_SEL 22 
#define SSD_OUT 23
const uint8_t MODE_BUTTON_PIN = 32;  // mode button
const uint8_t BACK_BUTTON_PIN = 32;  // back button
const uint8_t UP_BUTTON_PIN   = 35;  // up button
const uint8_t DOWN_BUTTON_PIN = 34;  // down button
//#define clk_type CLOCK_INVERT 
//#define clk_delay 3
const float ADC_RESOL = 0.061035163e-6 ; 
//const float ADC_RESOL = 0.122070327e-6 ;    //gain =2  GAIN0 =1,GAIN1=0 
//const float ADC_RESOL = 0.244140654e-6 ; 
//TM1638esp32 tm(strobe, clk, datapin,clk_type,clk_delay);   // strobe,clock,data
//TM1638lite tm(strobe, clk, datapin); 
//const double  KP=2, KI=5, KD=1;
double  Kp, Ki, Kd;
double temp_Pv;
uint16_t currentMillis3, previousMillis3=0,currentMillis4,previousMillis4=0;
uint8_t old_state1,old_state2;
uint8_t thSel_ok=0,tcSel_ok=0 ; 
float val_tc;
double pid_out;
float Rt;
double temp_Sv =25; uint8_t bPress;
bool res;
bool upkey(void);
bool downkey(void);
//const uint32_t WindowSize = 300;
uint32_t WindowSize =1000;
uint32_t windowStartTime = 0;
#define POnE 1   // Proportional on Error.
#define Cont_Direct 0  // Controller action is reverse (PV-SP) ; 
TM1638 tm(datapin, clk, strobe,true,7);  
ADS1232 tc(pdwn, sclk, dout);
PID myPID(&temp_Pv, &pid_out, &temp_Sv, Kp, Ki, Kd,POnE,Cont_Direct);

void setup() {
  // put your setup code here, to run once:
  tc.beginAdc();
  Serial.begin(9600);
  pinMode(CH_SEL, OUTPUT);      // ADC Channel select pin as potput
  //pinMode (SSR_PIN, OUTPUT);    // solid state realy out put pin as output.
  tc.power_up();
  //sensors.begin();
   WiFi.mode(WIFI_OFF);
   btStop();
   //attachAllButtonInterrupts();
   NVS.begin();
  //res=NVS.setInt("Kp", (uint64_t)KP);
  Kp = (float)NVS.getInt("Kp"); 
  //serial_printf(Serial,"Kp_set:%f\n",Kp);
  //res=NVS.setInt("Ki",(uint64_t) KI);
  Ki = (float) NVS.getInt("Ki"); 
  //serial_printf(Serial,"Ki_set:%f\n",Ki);
  //res=NVS.setInt("Kd", (uint64_t)KD);
  Kd = (float) NVS.getInt("Kd");
  //serial_printf(Serial,"Kd_set:%f\n",Kd);
  //res=NVS.setInt("WindowSize",  WindowSize);
  //WindowSize=NVS.getInt(" WindowSize");
  temp_Sv = (float) NVS.getInt("temp_Sv");
  //res= NVS.setInt("temp_Sv",(uint64_t)temp_Sv);
  //res= NVS.getInt("temp_Sv",(uint64_t)temp_Sv);
 //tc.offsetcalibrateADC() ;
 //tc.offset_cal();
 myPID.SetTunings(Kp, Ki, Kd, POnE);
 myPID.SetMode(AUTOMATIC);
}

void loop() {
  // put your main code here, to run repeatedly:
  temp_Pv = read_Tc()+read_5k();
  //Serial.print("Temp_PV: " ); 
  //Serial.println(temp_Pv);
  
 //serial_printf(Serial,"Kp_set: %f,Ki_set: %f,Kd_set: %f,windowSize: %i\n",Kp,Ki,Kd,WindowSize);
//serial_printf(Serial,"temp_sv%f\n",temp_Sv);
//serial_printf(Serial,"Kp%d\n",Kp);
//serial_printf(Serial,"Ki%d\n",Ki);
//serial_printf(Serial,"Kd%d\n",Kd);
  if(upkey() | downkey()){
    bPress = 1;
  if(upkey()){
   temp_Sv++;
   res= NVS.setInt("temp_Sv",(uint64_t)temp_Sv);
   }
   if(downkey()){
   temp_Sv--;
   res= NVS.setInt("temp_Sv",(uint64_t)temp_Sv);
   }
   tm.setDisplayToDecNumber(temp_Sv, 0, false);}
   else{
   if(bPress){delay(500); bPress =0;}  // 500 ms delay for SP visibility
   tm.setDisplayToDecNumber(temp_Pv*10, 1 << 1, false);}
    myPID.Compute();
    bool ssdCom = timeProp(pid_out);
    if(ssdCom){
      tm.setLEDs(2);
    }else{
     tm.setLEDs(0); 
    }
    digitalWrite(SSD_OUT,ssdCom);
    Serial.println( millis());
//serial_printf(Serial,"tempSV:%f,temp_PV:%f,PID_OUT:%f,SSD_COM:%b\n",temp_Sv,temp_Pv,pid_out,ssdCom); // for serialPort  
serial_printf(Serial,"%f,%f,%f,%b\n",temp_Sv,temp_Pv,pid_out,ssdCom);
  }

// Time proportional control function
  bool timeProp (long pidVal){    
  bool output;
  unsigned long tnow = millis();
  //serial_printf(Serial,"tnow:%d\n",tnow);
  // Time Proportional-PID algorithm
  if(tnow - windowStartTime > WindowSize){
    //time to shift the Relay Window
    windowStartTime += WindowSize;
     //serial_printf(Serial,"wst:%d\n",windowStartTime);
  }
  if(pidVal> (tnow - windowStartTime)){
    //digitalWrite(SSR_PIN, HIGH);
    output = true;
   //serial_printf(Serial,"o/p:%d\n",output);
  }
  else{
    //digitalWrite(SSR_PIN, LOW);
    output = false;
    //serial_printf(Serial,"o/p:%d\n",output);
  }
//  Serial.print(output);
//  Serial.print(" ");
//  Serial.println(tnow -windowStartTime);  
  return output; 
}

bool upkey(void) {
    if (tm.getButtons()==2){
     // Serial.print("******");
        old_state1 = 1;}
        delay(2);
    if (old_state1 & (tm.getButtons()==2)) {
        old_state1 = 0;     
        return 1;}
        else {
        old_state1 = 0;   
        return 0;}
}
bool downkey(void) {
    if (tm.getButtons()==8){
        old_state2 = 1;}
        delay(2);
    if (old_state2 & (tm.getButtons()==8)) {
        old_state2 = 0;     
        return 1;}
        else {
        old_state2 = 0;   
        return 0;}
}
