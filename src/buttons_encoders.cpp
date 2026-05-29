#include "header.h"


//bottom left button decrements current screen
void IRAM_ATTR blButtonHandler(){
  if(millis()-lastButtonTime > BUTTON_DEBOUNCE_TIME){
    lastData.currentScreen = data.currentScreen;
    if(data.currentScreen == drivetrain){
        EEPROM.begin(EEPROM_SIZE);
        if(EEPROM.read(0) != data.gearRatioSelect){
          EEPROM.write(0, data.gearRatioSelect);
          EEPROM.commit();
          EEPROM.end();
        }
      }
    if(data.currentScreen > 0){
      data.currentScreen = static_cast<screen>(static_cast<int>(data.currentScreen) - 1);
    }
    else data.currentScreen = drivetrain; //loop around feature
    lastButtonTime = millis();
    tft.fillScreen(0x0);

    DEBUG_PRINT("CURRENT SCREEN: ");
    DEBUG_PRINTLN(data.currentScreen);
    DEBUG_PRINT("LAST SCREEN: ");
    DEBUG_PRINTLN(lastData.currentScreen);
    DEBUG_PRINTLN("");
  }
}
//bottom right button increments current screen
void IRAM_ATTR brButtonHandler(){
  if(millis()-lastButtonTime > BUTTON_DEBOUNCE_TIME){
    if(data.currentScreen == drivetrain){
        EEPROM.begin(EEPROM_SIZE);
        if(EEPROM.read(0) != data.gearRatioSelect){
          EEPROM.write(0, data.gearRatioSelect);
          EEPROM.commit();
          EEPROM.end();
        }
      }
    lastData.currentScreen = data.currentScreen;
    if(data.currentScreen < 3 ){
      data.currentScreen = static_cast<screen>(static_cast<int>(data.currentScreen) + 1);
    }
    else data.currentScreen = main; //loop around feature
    
    lastButtonTime = millis();
    tft.fillScreen(0x0);
    DEBUG_PRINT("CURRENT SCREEN: ");
    DEBUG_PRINTLN(data.currentScreen);
    DEBUG_PRINT("LAST SCREEN: ");
    DEBUG_PRINTLN(lastData.currentScreen);
    DEBUG_PRINTLN("");
  }
}
//top left button decrements current state
void IRAM_ATTR tlButtonHandler(){
  if(millis()-lastButtonTime > BUTTON_DEBOUNCE_TIME){
    // lastData.state = data.state;
    // if(data.state > 0){
    //   data.state = static_cast<vehicleState>(static_cast<int>(data.state) - 1);
    // }
    // //else data.state = state3; //loop around feature
    lastButtonTime = millis();

    // DEBUG_PRINT("CURRENT STATE: ");
    // DEBUG_PRINTLN(data.state);
  }
}
//top right button increments current state
void IRAM_ATTR trButtonHandler(){
  //lastData.state = data.state;
  if(millis()-lastButtonTime > BUTTON_DEBOUNCE_TIME){
    // if(data.state < 3){
    //   data.state = static_cast<vehicleState>(static_cast<int>(data.state) + 1);
    // }
    // //else data.state = state0; //loop around feature
    lastButtonTime = millis();

    // DEBUG_PRINT("CURRENT STATE: ");
    // DEBUG_PRINTLN(data.state);
  }
}

void IRAM_ATTR lEncoderHandler() {
  if(millis()-lastEncoderTime > 50){
    //Serial.println("CHANGE DETECTED");
    if (digitalRead(lEncoderB)) {
      encoderPos--;   // CCW
      //DEBUG_PRINTLN("CCW");
      if(data.currentScreen == drivetrain && data.gearRatioSelect > 0){
        data.gearRatioSelect--;
      }
    } else {
      encoderPos++;   // CW
      //DEBUG_PRINTLN("CW");
      if(data.currentScreen == drivetrain && data.gearRatioSelect < 3){
        data.gearRatioSelect++;
      }
    }
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
      break;
    }
    tft.fillScreen(0x0);
    DEBUG_PRINTLN(encoderPos);
    lastEncoderTime = millis();
  }
}