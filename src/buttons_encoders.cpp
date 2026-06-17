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
    else data.currentScreen = config; //loop around feature
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
    DEBUG_PRINT("bl:");
    DEBUG_PRINTLN(digitalRead(blButton));
    DEBUG_PRINT("br:");
    DEBUG_PRINTLN(digitalRead(brButton));
    if(data.currentScreen == drivetrain){
        EEPROM.begin(EEPROM_SIZE);
        if(EEPROM.read(0) != data.gearRatioSelect){
          EEPROM.write(0, data.gearRatioSelect);
          EEPROM.commit();
          EEPROM.end();
        }
      }
    lastData.currentScreen = data.currentScreen;
    if(data.currentScreen < 4 ){
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
    if(digitalRead(blButton) == false && digitalRead(brButton) == false){
      if(data.currentScreen != etch){
        data.currentScreen = etch;
        tft.fillScreen(0x0);
      }
      else if(data.currentScreen == etch) data.currentScreen = main;
    }
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
    if(data.currentScreen == config){
      //save data to eeprom
      EEPROM.begin(EEPROM_SIZE);
      EEPROM.write(1, data.maxDriveTorqueNm >> 8);
      EEPROM.write(2, data.maxDriveTorqueNm);
      EEPROM.write(3, data.maxRegenTorqueNm >> 8);
      EEPROM.write(4, data.maxRegenTorqueNm);
      EEPROM.write(5, data.maxRegenCurrent >> 8);
      EEPROM.write(6, data.maxRegenCurrent);
      EEPROM.write(7, data.brakeFilterAlphaScale >> 8);
      EEPROM.write(8, data.brakeFilterAlphaScale);
      EEPROM.write(9, data.brakeFilterBetaScale >> 8);
      EEPROM.write(10, data.brakeFilterBetaScale);
      EEPROM.write(11, data.brakeMechThreshold >> 8);
      EEPROM.write(12, data.brakeMechThreshold);
      EEPROM.write(13, data.serialDebug);
      EEPROM.commit();
      EEPROM.end();
      //send config messages to ecu
      sendConfig();

      //data.cfgChangedSinceLastConfirm = false;
    }
    else if(data.currentScreen == etch) tft.fillScreen(0x0);
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
        tft.fillScreen(0x0);
        drawDrivetrainSettings();
      }
      else if(data.currentScreen == config && data.inputSelect > 0){
        tft.fillScreen(0x0);
        data.inputSelect--;
        drawConfig();
      }
      else if(data.currentScreen == etch && etchx > 0){
        etchx-=3;
      }
    } else {
      encoderPos++;   // CW
      //DEBUG_PRINTLN("CW");
      if(data.currentScreen == drivetrain && data.gearRatioSelect < 4){
        data.gearRatioSelect++;
        tft.fillScreen(0x0);
        drawDrivetrainSettings();
      }
      else if(data.currentScreen == config && data.inputSelect < 6){
        tft.fillScreen(0x0);
        data.inputSelect++;
        drawConfig();
      }
      else if(data.currentScreen == etch && etchx < 237){
        etchx+=3;
      }
    }
    if(data.currentScreen == drivetrain){
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
        case 4:
          data.gearRatio = 2.84615;
          break;
      }
    }
    DEBUG_PRINTLN(encoderPos);
    lastEncoderTime = millis();
  }
}

void IRAM_ATTR rEncoderHandler() {
  if(millis()-lastEncoderTime > 50){
    //Serial.println("CHANGE DETECTED");
    if (digitalRead(rEncoderB)) {
      encoderPos--;   // CCW
      //DEBUG_PRINTLN("CCW");
      if(data.currentScreen == config){
        switch(data.inputSelect){
          case 0:
            tft.fillRect(127, 42, 46, 22, 0x0);
            if(data.maxDriveTorqueNm > 0) data.maxDriveTorqueNm--;
          break;
          case 1:
            tft.fillRect(137, 67, 46, 22, 0x0);
            if(data.maxRegenTorqueNm > 0) data.maxRegenTorqueNm--;
          break;
          case 2:
            tft.fillRect(117, 92, 46, 22, 0x0);
            if(data.maxRegenCurrent > 0) data.maxRegenCurrent--;
          break;
          case 3:
            tft.fillRect(87, 127, 46, 22, 0x0);
            if(data.brakeFilterAlphaScale > 0) data.brakeFilterAlphaScale--;
          break;
          case 4:
            tft.fillRect(87, 168, 46, 22, 0x0);
            if(data.brakeFilterBetaScale > 0) data.brakeFilterBetaScale--;
          break;
          case 5:
            tft.fillRect(122, 185, 46, 22, 0x0);
            if(data.brakeMechThreshold > 0) data.brakeMechThreshold--;
          break;
          case 6:
            tft.fillRect(104, 202, 45, 19, 0x0);
            data.serialDebug = !data.serialDebug;
            if(data.serialDebug){
              Serial.begin(115200);
            }
            else{
              Serial.end();
            }
          break;
        }
        drawConfig();
      }
      else if(data.currentScreen == etch && etchy > 0){
        etchy-=3;
      }
    } else {
      encoderPos++;   // CW
      //DEBUG_PRINTLN("CW");
      if(data.currentScreen == config){
        switch(data.inputSelect){
          case 0:
            tft.fillRect(127, 42, 46, 22, 0x0);
            if(data.maxDriveTorqueNm < 256) data.maxDriveTorqueNm++;
          break;
          case 1:
            tft.fillRect(137, 67, 46, 22, 0x0);
            if(data.maxRegenTorqueNm < 256) data.maxRegenTorqueNm++;
          break;
          case 2:
            tft.fillRect(117, 92, 46, 22, 0x0);
            if(data.maxRegenCurrent < 256) data.maxRegenCurrent++;
          break;
          case 3:
            tft.fillRect(87, 127, 46, 22, 0x0);
            if(data.brakeFilterAlphaScale < 256) data.brakeFilterAlphaScale++;
          break;
          case 4:
            tft.fillRect(87, 168, 46, 22, 0x0);
            if(data.brakeFilterBetaScale < 256) data.brakeFilterBetaScale++;
          break;
          case 5:
            tft.fillRect(122, 185, 46, 22, 0x0);
            if(data.brakeMechThreshold < 256) data.brakeMechThreshold++;
          break;
          case 6:
            tft.fillRect(104, 202, 45, 19, 0x0);
            data.serialDebug = !data.serialDebug;
            if(data.serialDebug){
              Serial.begin(115200);
            }
            else{
              Serial.end();
            }
          break;
        }
        drawConfig();
      }
      else if(data.currentScreen == etch && etchy < 317){
        etchy+=3;
      }
    }
    DEBUG_PRINTLN(encoderPos);
    data.cfgChangedSinceLastConfirm = true;
    lastEncoderTime = millis();
  }
}