#include <Arduino.h>
#include "header.h"

//enable/disable debugging in the header.h file
TFT_eSPI tft = TFT_eSPI(240, 320);  // Invoke TFT library

//create objects for state and data
vehicleData data;
vehicleData lastData;

volatile int encoderPos;
volatile unsigned long lastButtonTime = 0;
volatile unsigned long lastEncoderTime = 0; // debounce timer for encoder
volatile bool lastlEncoderA;
volatile unsigned long lastMainTime = 0;
volatile unsigned long lastTempsTime = 0;
volatile unsigned long lastFaultTime = 0;
volatile unsigned long lastDrivetrainTime = 0;
volatile unsigned long lastConfigTime = 0;
volatile unsigned int secretCount = 0, etchx = 0, etchy = 0;

twai_message_t CANmsgRX;
twai_message_t CANmsgTX;

void setup() {
  // start serial early for debug
  Serial.begin(115200);
  DEBUG_PRINTLN("serial open");
  //setting pin modes
  pinMode(AMS, OUTPUT);
  pinMode(IMD, OUTPUT);
  pinMode(blButton, INPUT);
  pinMode(tlButton, INPUT);
  pinMode(brButton, INPUT);
  pinMode(trButton, INPUT);
  pinMode(lEncoderA, INPUT);
  pinMode(lEncoderB, INPUT);
  pinMode(rEncoderA, INPUT);
  pinMode(rEncoderB, INPUT);

  //attach interrupts to the 4 buttons
  attachInterrupt(digitalPinToInterrupt(blButton), blButtonHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(brButton), brButtonHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(tlButton), tlButtonHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(trButton), trButtonHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(lEncoderA), lEncoderHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(rEncoderA), rEncoderHandler, FALLING);

  //initialize tft
  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(2);
  tft.fillScreen(0x000);

  //boot sequence
  drawBoot();
  delay(500);
  digitalWrite(AMS, HIGH);
  digitalWrite(IMD, HIGH);
  delay(BOOTSCREEN_TIME);
  digitalWrite(AMS, LOW);
  digitalWrite(IMD, LOW);
  tft.fillScreen(0x0);
  data.currentScreen = main;

  //CANbus setup
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  //Look in the api-reference for other speed sets.
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver installed");
  } else {
    Serial.println("Failed to install driver");
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK) {
    Serial.println("Driver started");
  } else {
    Serial.println("Failed to start driver");
    return;
  }

  EEPROM.begin(EEPROM_SIZE);
  data.gearRatioSelect = EEPROM.read(0);
  data.maxDriveTorqueNm = EEPROM.read(1) << 8 | EEPROM.read(2);
  data.maxRegenTorqueNm = EEPROM.read(3) << 8 | EEPROM.read(4);
  data.maxRegenCurrent = EEPROM.read(5) << 8 | EEPROM.read(6);
  data.brakeFilterAlphaScale = EEPROM.read(7) << 8 | EEPROM.read(8);
  data.brakeFilterBetaScale = EEPROM.read(9) << 8 | EEPROM.read(10);
  data.brakeMechThreshold = EEPROM.read(11) << 8 | EEPROM.read(12);
  data.serialDebug = EEPROM.read(13);
  EEPROM.end();
  switch(data.gearRatioSelect){
    case 0:
      data.gearRatio = 3.909;
    break;
    case 1:
      data.gearRatio = 3.583;
    break;
    case 2:
      data.gearRatio = 3.308;
    break;
    case 3:
      data.gearRatio = 3.071;
    case 4:
      data.gearRatio = 2.84615;
    break;
  }

}

void loop() {
  switch(data.currentScreen){
    case 0:
      //main screen
      if(millis()-lastMainTime > MAIN_SCREEN_REFRESH_TIME){
        //calculateSlippage();
        drawMain(); 
        lastMainTime = millis();
      }     
    break;
    case 1:
      //faults screen
      if(millis()-lastFaultTime > FAULT_SCREEN_REFRESH_TIME){
        drawFaults();
        lastFaultTime = millis();
      }
    break;
    case 2:
      //temps screen
      if(millis()-lastTempsTime > TEMP_SCREEN_REFRESH_TIME){
        drawTemps(); 
        lastTempsTime = millis();
      }
    break;
    case 3:
      if(millis()-lastDrivetrainTime > DRIVETRAIN_SCREEN_REFRESH_TIME){
        drawDrivetrainSettings();
        lastDrivetrainTime = millis();
      }
      break;
    case 4:
      if(millis()-lastConfigTime > CONFIG_SCREEN_REFRESH_TIME){
        drawConfig();
        lastConfigTime = millis();
      }
    break;
    case 5:
      tft.fillRect(etchx, etchy, 3, 3, 0xfe60);
    break;
}
  getData();
  //remember what we drew so we can efficiently overwrite next frame
  lastData = data; 
}
