//IR control PINs
#define RECV 9 //Infrared receiver signal pin

//SONY RMT-814 IR Remote Control codes
#define FWD   0x2C9B //Forward, as button T
#define BWD   0x6C9B //Backward, as button W
#define POWER 0x4C9D //Turn off 0 as RED "Power" button
#define RIGHT 0x6BC  //Turn right button >>|
#define LEFT  0xEBC  //Turn left  button |<<
#define ZERO  0xE3C  //Stop, as button ZERO

//Freescale Semiconductor DC Motor driver MC33887 Control pins
#define _FS_L 2  //Left  Fail Status pin
#define _FS_R 3  //Right Fail Status pin
#define IN1_L 5  //Left  Input 1 pin
#define IN1_R 6  //Right Input 1 pin
#define IN2_L 4  //Left  Input 2 pin
#define IN2_R 7  //Right Input 2 pin
#define EN    8  //MC33887 motor driver Enable pin
#define FB_L  A0 //Left  Current Feedback pin
#define FB_R  A1 //Right Current Feedback pin

//Vishay Telefunken TCRT1000 Sensor pins
#define SWITCH A2 //Two microswitches with R-2R ladder
#define TCRT_L A4 //Left  TCRT1000 pin
#define TCRT_R A3 //Right TCRT1000 pin
#define BAT    A5 //Battery reference
#define LED    A7 //Start Module Status LED

//Sharp 340K Sensor pins
#define SHARP_SL 10 //SHARP 340K of the Left  Side  pin 
#define SHARP_FL 11 //SHARP 340K of the Front Left  pin
#define SHARP_FR 12 //SHARP 340K of the Front Right pin
#define SHARP_SR 13 //SHARP 340K of the Right Side  pin

//Last Front Sensor
#define Left  0
#define Right 1

//EEPROM ADDRESS
#define StateAddress   0
#define CommandAddress 1

//EEPROM StateValues
#define POWERON 0
#define STARTED 1
#define STOPPED 2

//DRIVERS CMD
#define DRIVERS_ENABLE()  driverState=HIGH; digitalWrite(EN, driverState);
#define DRIVERS_DISABLE() driverState=LOW; digitalWrite(EN, driverState);
