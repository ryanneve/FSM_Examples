#include <IRremote.h>
#include "definitions.h"
#include <EEPROM.h>

signed int STOP,
           START,
           SpeedSetValueLeft,
           SpeedSetValueRight,
           SpeedCurrentValueLeft,
           SpeedCurrentValueRight;
           
byte sensorSum,
     sensorMask,
     StateValue,
     CommandValue;
     
const byte SpeedStepValue=5;
float batteryVoltage;
word timer;

unsigned long TimeOld,
              StepTimeOldLeft,
              StepTimeOldRight;
              
boolean lastFrontSensor,
        lastLineSensor,
        lastSideSensor,
        lastMicroSwitch,
        driverState,
        LEDstate;
        
/*SENSOR WEIGHT CONSTANTS*/
const byte sharp_sl[]={1, 0};
const byte sharp_fl[]={2, 0};
const byte sharp_fr[]={4, 0};
const byte sharp_sr[]={8, 0};
const byte vishay_l[]={0, 16};
const byte vishay_r[]={0, 32};
const byte microswitch[]={0, 64, 128, 192};

//byte Inputs []={_FS_L, _FS_R, FB_L, FB_R, RECV, SW, BAT};
//byte PullUps[]={SHARP_SL, SHARP_FL, SHARP_FR, SHARP_SR};
//byte Outputs[]={IN1_L, IN1_R, IN2_L, IN2_R};

IRrecv irrecv(RECV);
decode_results results;


