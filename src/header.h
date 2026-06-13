#include <Arduino.h>
#include "driver/twai.h"
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <EEPROM.h>

#define EEPROM_SIZE 32

#ifndef HEADER_H
#define HEADER_H

//enable/disable debugging
#define DEBUG               // Comment out this Line to Remove Serial Debugging
#ifdef DEBUG
  #define DEBUG_PRINTLN  Serial.println
  #define DEBUG_PRINT    Serial.print
#else
  #define DEBUG_PRINTLN
  #define DEBUG_PRINT
#endif

//pin definitions
#define AMS 12
#define IMD 14

#define blButton 15
#define tlButton 14
#define brButton 1
#define trButton 2

#define lEncoderA 0
#define lEncoderB 39

#define rEncoderA 43
#define rEncoderB 46

#define CAN_TX 41
#define CAN_RX 40

//constant definitions
#define BOOTSCREEN_TIME 1000 //in ms
#define ENCODER_DEBOUNCE_TIME 100 //in ms
#define BUTTON_DEBOUNCE_TIME 150 //in ms
#define CAN_RX_TIMEOUT 1 //in ms
#define MAIN_SCREEN_REFRESH_TIME 250 //in ms
#define TEMP_SCREEN_REFRESH_TIME 1000//in ms
#define FAULT_SCREEN_REFRESH_TIME 1000 //in ms
#define DRIVETRAIN_SCREEN_REFRESH_TIME 100 //in ms
#define CONFIG_SCREEN_REFRESH_TIME 100 //in ms

//drivetrain definitions
//#define GEAR_RATIO 3.58333
//#define GEAR_RATIO 3.30769
#define TIRE_DIAMETER 16 //in inches
#define SLIP_RATIO_LIMIT .05 // in decimal percentage

enum vehicleState {  //which state the vehicle is in
 state0,
 state1,
 state2,
 state3
};

enum screen {  //which screen is currently shown
  main,
  faults,
  temps,
  drivetrain,
  config
};

struct faultData{
  String name;
  uint32_t bitmask;
};

struct tempData{
  String name;
  short temp;
};

struct vehicleData{  //stores data to put on the screen
  int motorSpeed; //RPM
  int groundSpeed; //mph
  unsigned int wheelAngularSpeed[4]; //fl, fr, bl, br; in RPM, from wheel speed sens.
  //if wheelAngularSpeed differs from motorSpeed more than WHEEL_SLIP_RATIO then true
  bool wheelsSlipping[4]; //fl, fr, bl, br;
  unsigned int batteryVoltage; //V
  unsigned int batteryCurrent; //A
  unsigned int chargePercent; //%
  unsigned int lowestCell; //degF
  unsigned int highestCell; //degF
  unsigned int gearRatioSelect; //0, 1, 2, or 3 depending on gear ratio selected
  unsigned int inputSelect; //for selection of config inputs
  //motor params for config
  int16_t maxDriveTorqueNm;   
  int16_t maxRegenTorqueNm;  
  int16_t maxRegenCurrent;
  // Brake Params for config
  int     brakeFilterAlphaScale;   // e.g. 38 (~0.15)
  int     brakeFilterBetaScale;    // e.g. 218 (~0.85)
  int     brakeMechThreshold;
  bool cfgChangedSinceLastConfirm = false;
  float gearRatio; 
  vehicleState state;
  screen currentScreen;
  unsigned long postFaultWord;
  unsigned long runFaultWord;
  bool unhandledFault;
};

//extern global variable declarations
//bitmasks for post faults cascadia motion CAN protocol pg. 153
extern faultData postFaultMasks[30];

//bitmasks for run faults cascadia motion CAN protocol pg. 154
extern faultData runFaultMasks[26];

extern TFT_eSPI tft;

extern vehicleData data;
extern vehicleData lastData;
extern tempData motorContTemps[11];
extern short lastMotorContTemps[11];

extern volatile int encoderPos;
extern volatile unsigned long lastEncoderTime; // debounce timer for encoder
extern volatile bool lastlEncoderA;
extern volatile unsigned long lastButtonTime;

extern twai_message_t CANmsgRX;
extern twai_message_t CANmsgTX;

//function declarations
//interrupts get attached to the 4 ISR's below for snappy button handling
void getData(); //in data_acquision.cpp
void sendData(unsigned int addr, unsigned int dataLength, unsigned long long msg); //in data_send.cpp
void sendData(twai_message_t msg);
void calculateSlippage(); //in traction_control.cpp
//in screen_draw.cpp
void drawBoot(); 
void drawMain();
void drawFaults();
void drawTemps();
void drawDrivetrainSettings();
void drawConfig();
//in buttons_encoders.cpp
void IRAM_ATTR blButtonHandler();
void IRAM_ATTR brButtonHandler();
void IRAM_ATTR tlButtonHandler();
void IRAM_ATTR trButtonHandler();
void IRAM_ATTR lEncoderHandler(); //interrupt gets attached to this for encoder
void IRAM_ATTR rEncoderHandler(); //interrupt gets attached to this for encoder

#endif