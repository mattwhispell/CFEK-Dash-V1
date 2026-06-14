#include "header.h"
#include "sprites/battery.h"
#include "sprites/temp_bulb.h"
#include "sprites/cfek_logo.h"
#include "sprites/warning.h"
#include "sprites/tire.h"


void drawBoot(){
  tft.fillScreen(0x0);
  tft.setTextColor(0xFE60);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold24pt7b);
  tft.drawString("CFEK", 52, 79);
  tft.pushImage(74, 127, cfek_logoWidth, cfek_logoHeight, cfek_logo);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.drawString("WheelDash V1.0", 53, 296);
}

void drawMain(){
  //reset whole display if display state changed
  //if(data.currentScreen != lastData.currentScreen) tft.fillScreen(0x0);

  //image sprites
  tft.pushImage(108, 10, batteryWidth, batteryHeight, battery);
  tft.pushImage(120, 295, temp_bulbWidth, temp_bulbHeight, temp_bulb);
  //if(data.unhandledFault) tft.pushImage(203, 88, warningWidth, warningHeight, warning);
  if(data.unhandledFault) tft.drawBitmap(203, 148, warning, warningWidth, warningHeight, 0xFA80);

  if(!data.unhandledFault && lastData.unhandledFault) tft.fillRect(200, 144, 35, 30, 0x0);

  // tft.setFreeFont(&FreeSansBold18pt7b);
  // tft.fillRect(79, 66, 82, 31, 0x0);
  // tft.drawNumber(data.motorSpeed, 82, 95);



  // //wheel slip indicators
  // if(data.wheelsSlipping[0] && !lastData.wheelsSlipping[0]){
  //   tft.drawBitmap(5, 80, tire, tireWidth, tireHeight, 0xF800);
  // }
  // else if(!data.wheelsSlipping[0]) tft.fillRect(5, 80, 24, 24, 0x0);

  // if(data.wheelsSlipping[1] && !lastData.wheelsSlipping[1]){
  //   tft.drawBitmap(211, 80, tire, tireWidth, tireHeight, 0xF800);
  // }
  // else if(!data.wheelsSlipping[1]) tft.fillRect(211, 80, 24, 24, 0x0);

  // if(data.wheelsSlipping[2] && !lastData.wheelsSlipping[2]){
  //   tft.drawBitmap(5, 230, tire, tireWidth, tireHeight, 0xF800);
  // }
  // else if(!data.wheelsSlipping[2]) tft.fillRect(5, 230, 24, 24, 0x0);

  // if(data.wheelsSlipping[3] && !lastData.wheelsSlipping[3]){
  //   tft.drawBitmap(211, 230, tire, tireWidth, tireHeight, 0xF800);
  // }
  // else if(!data.wheelsSlipping[3]) tft.fillRect(211, 230, 24, 24, 0x0);

  

  //ground speed
  tft.setTextSize(3);
  tft.fillRect(45, 120, 112, 85, 0x0);
  tft.setTextColor(0xFFFF);
  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.drawNumber(data.groundSpeed, 55, 120);
  //battery voltage
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSans12pt7b);
  tft.fillRect(8, 6, 80, 34, 0x0);
  tft.setTextColor(0xFFFF);
  tft.drawNumber(data.batteryVoltage, 10, 9);

  //battery current
  tft.fillRect(152, 9, 81, 31, 0x0);
  tft.setTextColor(0xFFFF);
  tft.drawNumber(data.batteryCurrent, 175, 9);

  //hottest cell in red
  tft.fillRect(150, 282, 84, 30, 0x0);
  tft.setTextColor(0xF206);
  tft.drawNumber(data.highestCell, 164, 292);
  tft.drawEllipse(211, 290, 3, 3, 0xF206);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.drawString("C", 219, 296);

  //avg cell in green
  tft.setFreeFont(&FreeSans12pt7b);
  tft.fillRect(12, 282, 85, 34, 0x0);
  tft.setTextColor(0x3de7);
  tft.drawNumber(data.avgCell, 10, 292);
  tft.drawEllipse(57, 291, 3, 3, 0x3de7);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.drawString("C", 65, 296);

  //charge percentage
  tft.setFreeFont(&FreeSans18pt7b);
  // tft.setTextColor(0x0);
  // tft.drawNumber(lastData.chargePercent, 82, 38);
  tft.fillRect(80, 44, 62, 38, 0x0);
  tft.setTextColor(0xFFFF);
  tft.drawNumber(data.chargePercent, 82, 38);
  tft.drawString("%", 145, 38);

  tft.setFreeFont(&FreeSans9pt7b);
  tft.drawString("Nm", 138, 244);
  tft.fillRect(85, 237, 45, 25, 0x0);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawNumber(data.torqueFeedback, 88, 241);
  //vehicle state
//   tft.setFreeFont(&FreeSans9pt7b);
//   if(lastData.state != data.state) {
//   tft.fillRect(76, 240, 84, 27, 0x0);
//   switch(data.state){
//     case 0:
//       tft.setTextColor(0xF206);
//       tft.drawString("state 0", 93, 246);
//       break;
//     case 1:
//       tft.setTextColor(0xfc20);
//       tft.drawString("state 1", 93, 246);
//       break;
//     case 2:
//       tft.setTextColor(0xffe0);
//       tft.drawString("state 2", 93, 246);
//       break;
//     case 3:
//       tft.setTextColor(0x07e0);
//       tft.drawString("state 3", 93, 246);
//       break;

//   }
// }
//white units
  tft.setTextSize(1);
  tft.setTextColor(0xFFFF);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.drawString("mph", 174, 188);
  tft.drawString("V", 53, 13);
  tft.drawString("A", 218, 13);
}

void drawFaults(){
  //fault has been handled when seen
  data.unhandledFault = false;
  int faultsDisplayed = 0;
  //reset the display if the screen changed
  //if(data.currentScreen != lastData.currentScreen){
    tft.fillScreen(0x0);
    tft.setTextColor(0xF206);
    tft.setTextSize(1);
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.drawString("FAULTS", 72, 10);
    tft.setFreeFont(&FreeSans9pt7b);
  //}

  //clear faults from last frame
  // tft.setTextColor(0x0);
  // for(const auto &f : postFaultMasks){
  //   if(lastData.postFaultWord & f.bitmask){
  //     tft.drawString(f.name, 10, (36 + 20 * faultsDisplayed));
  //     faultsDisplayed++;
  //   }
  // }
  // for(const auto &f : runFaultMasks){
  //   if(lastData.runFaultWord & f.bitmask){
  //     tft.drawString(f.name, 10, (36 + 20 * faultsDisplayed));
  //     faultsDisplayed++;
  //   }
  // }
  faultsDisplayed = 0;

  //draw new frame of faults
  tft.setTextColor(0xFFFF);
  for(const auto &f : postFaultMasks){
    if(data.postFaultWord & f.bitmask){
      tft.drawString(f.name, 10, (36 + 20 * faultsDisplayed));
      faultsDisplayed++;
    }
  }
  for(const auto &f : runFaultMasks){
    if(data.runFaultWord & f.bitmask){
      tft.drawString(f.name, 10, (36 + 20 * faultsDisplayed));
      faultsDisplayed++;
    }
  }
}

void drawTemps(){
  //draw the title once
  int tempsDisplayed = 0;
  //if(data.currentScreen != lastData.currentScreen){
    tft.fillScreen(0x0);
    tft.setTextColor(0xF206);
    tft.setTextSize(1);
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.drawString("TEMPS", 78, 10);
    tft.setTextColor(0xFFFF);
    tft.setFreeFont(&FreeSans9pt7b);

    //draw the temp names once
    for(const auto &f : motorContTemps){
      tft.drawString(f.name, 10, (36 + 20 * tempsDisplayed));
      tempsDisplayed++;
    }
 // }

  tempsDisplayed = 0;
  for(const auto &f : motorContTemps){
    //draw over last temp in black for efficiency
    tft.setTextColor(0x0);
    tft.drawNumber(lastMotorContTemps[tempsDisplayed], (10 + 12 * f.name.length()), (36 + 20 * tempsDisplayed));
    //draw new temps
    tft.setTextColor(0xFFFF);
    tft.drawNumber(f.temp, (10 + 12 * f.name.length()), (36 + 20 * tempsDisplayed));
    //remember old temps for efficient redraw
    //lastMotorContTemps[tempsDisplayed] = f.temp;
    tempsDisplayed++;
  }
}

void drawDrivetrainSettings(){
  tft.setTextSize(1);
  tft.setTextColor(0xFFFF);
  tft.drawString("Drivetrain Settings", 12, 10);
  switch(data.gearRatioSelect){
    case 0:
      tft.fillRect(65, 118, 111, 27, 0xFFFF);
      tft.drawString("12T 3.583", 67, 151);
      tft.drawString("13T 3.308", 67, 180);
      tft.drawString("14T 3.071", 66, 209);
      tft.drawString("37T 2.846", 66, 238);
      tft.setTextColor(0x0);
      tft.drawString("11T 3.909", 66, 123);
    break;
    case 1:
      tft.fillRect(65, 148, 111, 27, 0xFFFF);
      tft.drawString("11T 3.909", 66, 123);
      tft.drawString("13T 3.308", 67, 180);
      tft.drawString("14T 3.071", 66, 209);
      tft.drawString("37T 2.846", 66, 238);
      tft.setTextColor(0x0);
      tft.drawString("12T 3.583", 67, 151);
    break;
    case 2:
      tft.fillRect(65, 176, 111, 27, 0xFFFF);
      tft.drawString("11T 3.909", 66, 123);
      tft.drawString("12T 3.583", 67, 151);
      tft.drawString("14T 3.071", 66, 209);
      tft.drawString("37T 2.846", 66, 238);
      tft.setTextColor(0x0);
      tft.drawString("13T 3.308", 67, 180);
    break;
    case 3:
      tft.fillRect(65, 204, 111, 27, 0xFFFF);
      tft.drawString("11T 3.909", 66, 123);
      tft.drawString("12T 3.583", 67, 151);
      tft.drawString("13T 3.308", 67, 180);
      tft.drawString("37T 2.846", 66, 238);
      tft.setTextColor(0x0);
      tft.drawString("14T 3.071", 66, 209);
    break;
    case 4:
      tft.fillRect(65, 232, 111, 27, 0xFFFF);
      tft.drawString("11T 3.909", 66, 123);
      tft.drawString("12T 3.583", 67, 151);
      tft.drawString("13T 3.308", 67, 180);
      tft.drawString("14T 3.071", 66, 209);
      tft.setTextColor(0x0);
      tft.drawString("37T 2.846", 66, 238);
    break;
  }
}

//
void drawConfig(){
  if(data.cfgChangedSinceLastConfirm) tft.fillRect(214, 10, 16, 16, 0xF880);
  else tft.fillRect(214, 10, 16, 16, 0x57E0);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("Config", 82, 5);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("maxDriveNm", 10, 50);
  tft.drawString("maxRegenNm", 10, 75);
  tft.drawString("maxRegenA", 10, 102);
  tft.drawString("bfAScale", 10, 127);
  tft.drawString("bfBScale", 10, 155);
  tft.drawString("brakeThresh", 10, 181);
  tft.drawString("serialDbg", 10, 205);
  tft.setTextColor(0x0);
  switch(data.inputSelect){
    case 0:
      tft.fillRect(6, 48, 116, 19, 0xFFFF);
      tft.drawString("maxDriveNm", 10, 50);    
    break;
    case 1:
      tft.fillRect(7, 75, 125, 19, 0xFFFF);
      tft.drawString("maxRegenNm", 10, 75);  
    break;
    case 2:
      tft.fillRect(9, 102, 105, 19, 0xFFFF);
      tft.drawString("maxRegenA", 10, 100);    
    break;
    case 3:
      tft.fillRect(9, 125, 82, 19, 0xFFFF);
      tft.drawString("bfAScale", 10, 127);    
    break;
    case 4:
      tft.fillRect(9, 153, 82, 19, 0xFFFF);
      tft.drawString("bfBScale", 10, 155);
    break;
    case 5:
      tft.fillRect(8, 181, 113, 19, 0xFFFF);
      tft.drawString("brakeThresh", 10, 181);
    break;
    case 6:
      tft.fillRect(8, 202, 91, 19, 0xFFFF);
      tft.drawString("serialDbg", 10, 205);
    break;
  }
  tft.setTextColor(0xFFFF);
  tft.drawNumber(data.maxDriveTorqueNm, 130, 50);
  tft.drawNumber(data.maxRegenTorqueNm, 140, 75);
  tft.drawNumber(data.maxRegenCurrent, 120, 100);
  tft.drawNumber(data.brakeFilterAlphaScale, 90, 127);
  tft.drawNumber(data.brakeFilterBetaScale, 90, 155);
  tft.drawNumber(data.brakeMechThreshold, 125, 181);
  if(data.serialDebug)tft.drawString("true", 106, 205);
  else tft.drawString("false", 106, 205);
}