void loop() {
  if (irrecv.decode(&results)) {
  Serial.println(results.value, HEX);
    switch(StateValue){
      case POWERON: if(results.value == START) startCommand();
                    if(results.value == STOP) stopCommand();
                    if(results.value >= 0x2C4 && results.value <=0x2FE) programmCommand();
                    break;
      case STARTED: if(results.value == STOP) stopCommand();
                    if(results.value >= 0x2C4 && results.value <=0x2FE) programmCommand();
                    break;
      case STOPPED: break;
    }
    irrecv.resume();      //Prepare to receive the next value 
  }
  //Read the battery voltage
  batteryVoltageCheck();
  //Open loop control of slope acceleration
  SpeedControl();
//  sensorSerialPrint();
  //Check fault status flags
  statusFeedback();
  //Read the sensor byte value
  SensorSum();
  //THIS LINE IS CRITICAL
  //do not remove the stateMachine.update() call, it is what makes this program 'tick'
  stateMachine.update();
}

void startCommand(){
//  Serial.println("startCommand");
  StateValue = STARTED;
  EEPROMwrite();
  DRIVERS_ENABLE();
  digitalWrite(LED, HIGH);
  SetSpeed(20, 20);
  stateMachine.immediateTransitionTo(idleRunState);
}

void stopCommand(){
//  Serial.println("stopCommand");
  motor(0, 0);
  DRIVERS_DISABLE();
  StateValue = STOPPED;
  EEPROMwrite();
  stateMachine.immediateTransitionTo(stoppedState);
}

void programmCommand(){
//  Serial.println("programmCommand");
  motor(0, 0);
  DRIVERS_DISABLE();
  StateValue = POWERON;
  EEPROMwrite();
  CommandValue = results.value - 0x200;
  EEPROM.write(CommandAddress, CommandValue);
  START = CommandValue + 0x101;
  STOP  = CommandValue + 0x100;
  stateMachine.immediateTransitionTo(powerOnState);
}

void EEPROMwrite(){
//  Serial.println("EEPROMwrite");
  EEPROM.write(StateAddress, StateValue);
}
/*FAIL STATUS FLAG RESETTER*/
void statusFeedback(){
  boolean driverStatus=digitalRead(_FS_L)&digitalRead(_FS_R);
  if (driverStatus==LOW && driverState==HIGH){
//    Serial.print  ("Fail Status: ");
//    Serial.print  (digitalRead(_FS_L), BIN);
//    Serial.println(digitalRead(_FS_R), BIN);
//    Serial.println("DRIVERS_DISABLE");
    DRIVERS_DISABLE();
//    Serial.println("DRIVERS_ENABLE");
    DRIVERS_ENABLE();
  }  
}
/*BATTERY VOLTAGE CHECK*/
void batteryVoltageCheck(){
  batteryVoltage=analogRead(BAT)*15/1024.00;
  if(batteryVoltage <= 9.2) DRIVERS_DISABLE();
}
/*SENSOR DATABYTE*/
void SensorSum(){
  sensorSum = /*sharp_sl[digitalRead(SHARP_SL)]
            + sharp_fl[digitalRead(SHARP_FL)]
            + sharp_fr[digitalRead(SHARP_FR)]
            + sharp_sr[digitalRead(SHARP_SR)]*/
            + vishay_l[digitalRead( TCRT_L )]
            + vishay_r[digitalRead( TCRT_R )]
            /*+ microswitch[Switch()]*/;
}
/*SWITCH DETECT*/
byte Switch(){
  int switchVal=analogRead(SWITCH);
  if(switchVal==0)                   return 3;
  if(switchVal>200 && switchVal<300) return 2;
  if(switchVal>400 && switchVal<600) return 1;
  if(switchVal>700 && switchVal<800) return 0;
}

void delay_100(){
  delay(100*8);
}
void LED_FLASHING(){
  digitalWrite(LED, LEDstate);
  LEDstate=!LEDstate;
  delay_100();
}
  
