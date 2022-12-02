
/*****************************************************
 * Read Thermo Couple routine*
 ****************************************************/
float read_Tc(void){      // read thermocouple routine
  float temp_tc;
  //float val;
   uint32_t value;
  if(!tcSel_ok & !thSel_ok){
 //Serial.println("#####");  
  digitalWrite(CH_SEL, LOW);   // ADC CH-0 Selected
 // digitalWrite(LED, LOW);
  previousMillis3 = millis();
  tcSel_ok =1;}                // ADC CH disabled during ADC conversion
  // Time delay 100 msed Routine non blocking
  if(tcSel_ok){
  currentMillis3 = millis();                         // for timeout function
  //Serial.print("currentMillis: ");
  //Serial.println(currentMillis3);  
  if((currentMillis3-previousMillis3)>5U){
  //Serial.print("currentMillis@timeout: ");
  //Serial.println(currentMillis);                               
  previousMillis3= currentMillis3;
  /********************************/   
                     
    value  = tc.raw_read(2);  // read ADC 
    //serial_printf(Serial,"%d",millis());
    //serial_printf(Serial,"valueHex:%x\n",value);     
  if(value<=0x7FFFFF){          // TC --> positive voltage  range
    val_tc = (value+tc.OFFSET)*ADC_RESOL*tc.GAIN;
   
    val_tc = val_tc*1000;  //volatage in mv
    //val_tc = val_tc+0.06;    // offset error of 0.06mv
     //serial_printf(Serial,"val_tc:%f\n",val_tc);
  }
  else{
    //value = value-0xFF800000;      // range conversion for negative voltage 800000-->0000,1000000 -->7FFFFFF
     //serial_printf(Serial,"valueHex:%x\n",value);
     //serial_printf(Serial,"valuedec:%d\n",value);  
    val_tc = -(value+tc.OFFSET)*ADC_RESOL *tc.GAIN;
    val_tc = - val_tc;
    //serial_printf(Serial,"val_tc:%f\n",val_tc);
  }
  tcSel_ok = 0;                  // channel select enabled
  //Serial.print("value_tc_hex:");
  //Serial.println(value);
  //Serial.print("val_tc:");
  //Serial.println(val_tc);
  
  //sensors.requestTemperatures(); // Send the command to get temperatures
  //sensors.getTempCByIndex(0)
  //return (T_tck(val)+ sensors.getTempCByIndex(0));
  //float temp_tc = T_tck(val);
  //float temp_th = read_5k();
   //Serial.print("temp_th:");
  // Serial.println(temp_th);
  // Serial.print("temp_tc:");
  // Serial.println(temp_tc);
  //return (T_tck(val)+ read_5k());
  //return temp_tc;
      }
  }
   if(val_tc >=20.644){
    temp_tc = T_tck2(val_tc);
   }
  else{ temp_tc = T_tck1(val_tc);
  }
 //  Serial.print("temp_tc:");
 //  Serial.println(temp_tc);
  return temp_tc;
}

/*****************************************************
 * Read thermistor routine*
 ****************************************************/
 
float read_5k(void){    // read thermistor routine
  float val5k;
  if(!thSel_ok & !tcSel_ok){
      //Serial.println("@@@@@@@@@"); 
  digitalWrite(CH_SEL, HIGH);  // ADC CH-1 Selected 
  //digitalWrite(LED, HIGH);
  previousMillis4 = millis();
  thSel_ok = 1;}
  //delay(100);
  /**Delay Routine for 100ms non blocking****/
  if(thSel_ok){
  currentMillis4 = millis();                         // for timeout function
   //Serial.print("currentMillis4:");
   //Serial.println(currentMillis4);
  if((currentMillis4-previousMillis4) >5U){
  //Serial.print("currentMillis@timeout: ");
  //Serial.println(currentMillis);                               
  previousMillis4 = currentMillis4;
  //Serial.print("previousMillis4:");
  // Serial.println(previousMillis4);
  //Serial.println("^^^");
     
  thSel_ok = 0;
  /******************************************/
  long value  = tc.raw_read(2);  // read ADC 
  // Serial.print("Value_hex_th:");
  // Serial.println(value);
  if(value<=0x7FFFFF){          // TC --> positive voltage  range
    val5k = (value+tc.OFFSET)*ADC_RESOL*tc.GAIN;
  }
  else{
    value = value-0x800000;      // range conversion for negative voltage 800000-->0000,1000000 -->7FFFFFF
    val5k = -(value+tc.OFFSET)*ADC_RESOL *tc.GAIN;
  }
    Rt = 1000*(2.048/val5k-1);   //rt = rs*(vref/v-1) 
    
   //Serial.print("Rt:");
  // Serial.println(Rt);
 
  //return temp_th;
      }
  }
   //Serial.println("!!!!");
   //Serial.print("Rt:");
   //Serial.println(Rt);
  float temp_th = T_th5k(Rt); 
 // Serial.print("temp_th:");
 // Serial.println(temp_th);
  //return( T_th5k(Rt));   // room temperature compensation
 //serial_printf(Serial,"temp_th:%f\n",temp_th);
  return temp_th;
}
