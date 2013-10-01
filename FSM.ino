//http://www.arduino.cc/playground/uploads/Code/FSM_1-6.zip
#include <FiniteStateMachine.h>

State powerOnState = State(powerOnUpdate); //According to RobotChallenge rules
State idleRunState = State(idleRunUpdate);
State spinState = State(spinEnter, spinUpdate, NULL);
State lineAvoidState = State(lineAvoidEnter, lineAvoidUpdate, NULL);
//State attackState = State(attackEnter, attackUpdate, NULL);
//State overfallAvoidState = State(overfallAvoidEnter, overfallAvoidUpdate, NULL);
State stoppedState = State(stoppedSafe,stoppedUpdate, NULL); //According to RobotChallenge rules

FiniteStateMachine stateMachine = FiniteStateMachine(powerOnState);




/*POWER ON STATE*/ 
void powerOnEnter(){
//  Serial.println("PowerOnState");
  digitalWrite(LED, LOW);
}

void powerOnUpdate(){
}

/*STOPPED_SAFE STATE*/
void stoppedSafe(){
//  Serial.println("StoppedState");
  for(byte i=0; i<10; i++){
    LED_FLASHING();
  }
  StateValue = POWERON;
  EEPROMwrite();
}
/*STOPPED STATE*/
void stoppedUpdate(){
  while(1){
    LED_FLASHING();
  }
}

/*IDLE RUN STATE*/  
void idleRunUpdate(){
//  Serial.println("idleRunUpdate");
  //Conditions for transition to a Spin State
//  if(sensorSum==1||sensorSum==3||sensorSum==8||sensorSum==12){
//    stateMachine.transitionTo(spinState); 
//  }
  //Conditions for transition to a Line Avoid State
  if((sensorSum>=16 && sensorSum<=63)||(sensorSum>=80 && sensorSum<=127)||(sensorSum>=208 && sensorSum<=255)){
    stateMachine.transitionTo(lineAvoidState);
  }
  //Conditions for transition to a Attack State
//  if(sensorSum==2||sensorSum==4||sensorSum==6||sensorSum==7||sensorSum==14){
//    stateMachine.transitionTo(attackState);
//  }
  //Conditions for transition to a Overfall Avoid State
//  if((sensorSum>=64 && sensorSum<=79)||(sensorSum>=128 && sensorSum<=143)||(sensorSum>=192 && sensorSum<=207)){
//    stateMachine.transitionTo(overfallAvoidState);
//  }
}

/*LINE AVOID STATE*/
void lineAvoidEnter(){
//  Serial.println("LineAvoid");
  //Trim other sensoric values  
  switch(sensorSum & 48){                        
    case 16: //Set mask for Left Line sensor
             sensorMask=!16;
             //Remember the Left Line sensor, to track the position
             lastLineSensor=Left;
             //Set timer
             timer=500;
             SetSpeed(10, -5);
             break;
    case 32: //Set mask for Right Line sensor
             sensorMask=!32;
             //Remember the Right Line sensor, to track the position
             lastLineSensor=Right;
             //Set timer
             timer=500;
             SetSpeed(-5, 10);
             break;
    case 48: //Set mask for Both Line sensors
             sensorMask=!48;
             // Set timer for 600ms
             timer=1000;
             SetSpeed(-10, -10);
             break;      
  }
  //Reset timer
  TimeOld=millis();
}
void lineAvoidUpdate(){
  //Check on other active sensor or timeout
  if(sensorSum & sensorMask > 0 || millis()-TimeOld >=timer*8){
    //If no sensor activated
    if(sensorSum & sensorMask==0 && sensorMask==!48 && millis()-TimeOld>=timer*8){
      //Find out which was the last
      switch(lastLineSensor){
        case Left: //Set diferential motor speed to 25%
                   SetSpeed(+25, -25); break;
        case Right://Set diferential motor speed to 25%
                   SetSpeed(-25, +25); break;
      }
      //Transit to Spin State to make 180 degrees spin
      stateMachine.immediateTransitionTo(spinState);
    }
    //Find out which was the last
    switch(lastLineSensor){
      case Left: //Set diferential motor speed to 25%
                 SetSpeed(+30, +15); break;
      case Right://Set diferential motor speed to 25%
                 SetSpeed(+15, +30); break;
    }
    //Transit to Idle Run State
    stateMachine.immediateTransitionTo(idleRunState);
  }
}

/*SPIN STATE*/
void spinEnter(){
  //Trim other sensoric values
  switch(sensorSum & 9){
    case 1: //Set mask for SHARP_SL sensor
            sensorMask=!(sensorSum & 1);
            // Set timer for 600ms
            timer=600;
            // Set diferential motor speed to 25%
            SetSpeed(-25, +25); break;
    case 8: sensorMask=!(sensorSum & 8);
            // Set timer for 600ms
            timer=600;
            // Set diferential motor speed to 25%
            SetSpeed(+25, -25); break; //Set timer for 1000ms and both motor cross reverse speed to 50
  }
  //Reset timer
  TimeOld=millis();
}
void spinUpdate(){
  //Read the sensor byte value
  SensorSum();
  //Check on other active sensor or timeout
  if(sensorSum & sensorMask > 0 || millis()-TimeOld >=timer*8){
    //Transit to Idle Run State to make another decition
    stateMachine.immediateTransitionTo(idleRunState);
  }
}

/*ATTACK STATE*/
void attackEnter(){
  //Trim other sensoric values
  switch(sensorSum & 6){
    case 2: //Set mask for SHARP front left sensor
            sensorMask=!13;
            //Remember the left front sensor, to track the opponent
            lastFrontSensor=Left;
            //Set motor differential speed forward 20/30%
            SetSpeed(+20, +30); break;
    case 4: //Set mask for SHARP front right sensor
            sensorMask=!11;
            //Remember the right front sensor, to track the opponent
            lastFrontSensor=Right;
            //Set motor differential speed forward 30/20%
            SetSpeed(+30, +20); break;
    //No need memory for this condition
    //Impossible to disappear at the same time for both sensors
    case 6: //Set mask for SHARP Front both sensors
            sensorMask=!15;
            //Set both motor speed forward 30%
            SetSpeed(+30, +30); break;
  }
}
void attackUpdate(){
  //Read the sensor byte value
  SensorSum();
  //Prevent target lost
  if(sensorSum == 0){
    // Set timer for 300ms
    timer=300;
    //Recall the last Front sensor from the memory
    switch(lastFrontSensor){
      case Left: //Set left motor speed reverse 30%
                 SetSpeed(-30, 0); break;
      case Right://Set right motor speed reverse 30%
                 SetSpeed(0, -30); break;
    }
    //Transit to Spin State to detect opponent again
    stateMachine.immediateTransitionTo(spinState);
  }
  //Check calibration
  if(sensorSum & sensorMask>0){
    //Transit to Idle Run State to make another decition
    stateMachine.immediateTransitionTo(idleRunState);
  }
  //Check if one of front sensors deactivated
  if(sensorMask==!15 && sensorSum & 6 <6){
    //Transit to Idle Run State to make another decition
    stateMachine.immediateTransitionTo(idleRunState);
  }
}

void overfallAvoidEnter(){
}
void overfallAvoidUpdate(){
}
void overfallAvoidExit(){
}
