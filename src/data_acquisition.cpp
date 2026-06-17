#include "header.h"

//bitmasks for post faults cascadia motion CAN protocol pg. 153
faultData postFaultMasks[] = {
  { "hwGateDesat", 0x1 },
  { "hwOvercurrent", 0x2 },
  { "accelShort", 0x4 },
  { "accelOpen", 0x8 },
  { "currentSenseLow", 0x10 },
  { "currentSenseHigh", 0x20 },
  { "moduleTempLow", 0x40 },
  { "moduleTempHigh", 0x80 },
  { "controlPCBTempLow", 0x100 },
  { "controlPCBTempHigh", 0x200 },
  { "gateDrivePCBTempLow", 0x400 },
  { "gateDrivePCBTempHigh", 0x800 },
  { "senseLow5v", 0x1000 },
  { "senseHigh5v", 0x2000 },
  { "senseLow12v", 0x4000 },
  { "senseHigh12v", 0x8000 },
  { "senseLow2halfv", 0x10000 },
  { "senseHigh2halfv", 0x20000 },
  { "senseLow1halfv", 0x40000 },
  { "senseHigh1halfv", 0x80000 },
  { "dcBusHighV", 0x100000 },
  { "dcBusLowV", 0x200000 },
  { "prechargeTimeout", 0x400000 },
  { "prechargeVoltageFail", 0x800000 },
  { "eepromChecksumInvalid", 0x1000000 },
  { "eepromRangeErr", 0x2000000 },
  { "eepromUpdateReq", 0x4000000 },
  { "gateDriverInit", 0x10000000 },
  { "brakeShort", 0x40000000 },
  { "brakeOpen", 0x80000000 }
};

//bitmasks for run faults cascadia motion CAN protocol pg. 154
faultData runFaultMasks[] = {
  { "motorOverspeed", 0x1 },
  { "overcurrent", 0x2 },
  { "overvoltage", 0x4 },
  { "inverterOverTemp", 0x8 },
  { "accelInputShort", 0x10 },
  { "accelInputOpen", 0x20 },
  { "directionInputDissonance", 0x40 },
  { "inverterResponseTimeout", 0x80 },
  { "hwGateDesat", 0x100 },
  { "hwOverCurrent", 0x200 },
  { "underVoltage", 0x400 },
  { "CANmsgLost", 0x800 },
  { "motorOverTemp", 0x1000 },
  { "brakeInputShort", 0x10000 },
  { "brakeInputOpen", 0x20000 },
  { "moduleAovertemp", 0x40000 },
  { "moduleBovertemp", 0x80000 },
  { "moduleCovertemp", 0x100000 },
  { "PCBovertemp", 0x200000 },
  { "GDB1overtemp", 0x400000 },
  { "GDB2overtemp", 0x800000 },
  { "GDB3overtemp", 0x1000000 },
  { "currentSensorFault", 0x2000000 },
  { "gateDriverOvervoltage", 0x4000000 },
  { "hwOverVoltage", 0x10000000 },
  { "resolverFault", 0x40000000 }
};

tempData motorContTemps[] = {
  { "invModATemp", 0 },
  { "invModBTemp", 0 },
  { "invModCTemp", 0 },
  { "invGDBTemp", 0 },
  { "invContTemp", 0 },
  { "invRTD1Temp", 0 },
  { "invRTD2Temp", 0 },
  { "motorHotspot", 0 },
  { "invCoolant", 0 },
  { "invHotspot", 0 },
  { "motorTemp", 0 }
};
short lastMotorContTemps[11];

void getData(){
  if(twai_receive(&CANmsgRX, CAN_RX_TIMEOUT) == ESP_OK){
    //print complete CAN message for debugging
    // #ifdef DEBUG
    //   char buf[50];
    //   unsigned long long fullMsg;
    //   for(int i = 0; i < 8; i++) fullMsg = (fullMsg << 8)+ CANmsgRX.data[i];
    //   sprintf(buf, "Address: %02x  Length: %1d  Data: %016llx", CANmsgRX.identifier, CANmsgRX.data_length_code, fullMsg);
    //   Serial.println(buf);
    // #endif

    //Cascadia motion CAN protocol pg.143
    switch(CANmsgRX.identifier){
      //ID 0xA7: Voltage information
      case 0xA7:
      //high voltages *10
        lastData.batteryVoltage = data.batteryVoltage;
        data.batteryVoltage = (CANmsgRX.data[1] * 256 + CANmsgRX.data[0]) / 10;
        DEBUG_PRINT("VOLTAGE DATA RECIEVED: ");
        DEBUG_PRINTLN(data.batteryVoltage);
        break;
      //ID 0xA6: Current information
      case 0xA6:
      //currents *10
        data.batteryCurrent = (CANmsgRX.data[7] * 256 + CANmsgRX.data[6]) / 10;
        DEBUG_PRINT("CURRENT DATA RECIEVED: ");
        DEBUG_PRINTLN(data.batteryCurrent);
        break;
      //ID 0xA5: Motor position information
      case 0xA5:
      //actual velocity in rpm
        noInterrupts();
        data.motorSpeed = CANmsgRX.data[3] * 256 + CANmsgRX.data[2];
        if(data.motorSpeed < 10) data.groundSpeed = 0;
        else data.groundSpeed = (int)((float)data.motorSpeed * 0.0167238);
        interrupts();
        DEBUG_PRINT("MOTOR SPEED DATA RECIEVED: ");
        DEBUG_PRINTLN(data.motorSpeed);
        DEBUG_PRINT("CALCULATED GROUND SPEED: ");
        DEBUG_PRINTLN(data.groundSpeed);
        break;
      //ID 0xAB: Motor controller fault codes
      case 0xAB:
        data.postFaultWord = CANmsgRX.data[0] + (CANmsgRX.data[1] << 8) + (CANmsgRX.data[2] << 16) + (CANmsgRX.data[3] << 24);
        data.runFaultWord = CANmsgRX.data[4] + (CANmsgRX.data[5] << 8) + (CANmsgRX.data[6] << 16) + (CANmsgRX.data[7] << 24);
        if((data.postFaultWord || data.runFaultWord) &&
        (lastData.postFaultWord != data.postFaultWord || lastData.runFaultWord != data.runFaultWord)) {
          data.unhandledFault = true;
        }
        DEBUG_PRINT("POST FAULT DATA RECIEVED: ");
        DEBUG_PRINTLN(data.postFaultWord, HEX);
        DEBUG_PRINT("RUN FAULT DATA RECIEVED: ");
        DEBUG_PRINTLN(data.runFaultWord, HEX);
      break;
      case 0xAC:
        data.torqueFeedback  = (CANmsgRX.data[3] * 256 + CANmsgRX.data[2]) / 10;
      break;
      //ID 0x420: ECU fault codes
      case 0x420:
      break;
      //ID 0xA0: Temps #1
      case 0xA0:
        motorContTemps[0].temp = CANmsgRX.data[0] * 256 + CANmsgRX.data[1];
        motorContTemps[1].temp = CANmsgRX.data[2] * 256 + CANmsgRX.data[3];
        motorContTemps[2].temp = CANmsgRX.data[4] * 256 + CANmsgRX.data[5];
        motorContTemps[3].temp = CANmsgRX.data[6] * 256 + CANmsgRX.data[7];
      break;
      //ID 0xA1: Temps #2
      case 0xA1:
        motorContTemps[4].temp = CANmsgRX.data[0] * 256 + CANmsgRX.data[1];
        motorContTemps[5].temp = CANmsgRX.data[2] * 256 + CANmsgRX.data[3];
        motorContTemps[6].temp = CANmsgRX.data[4] * 256 + CANmsgRX.data[5];
        motorContTemps[7].temp = CANmsgRX.data[6] * 256 + CANmsgRX.data[7];
      break;
      //ID 0xA2: Temps #3
      case 0xA2:
        motorContTemps[8].temp = CANmsgRX.data[0] * 256 + CANmsgRX.data[1];
        motorContTemps[9].temp = CANmsgRX.data[2] * 256 + CANmsgRX.data[3];
        motorContTemps[10].temp = CANmsgRX.data[4] * 256 + CANmsgRX.data[5];
      break;
      case 0x03:
      //dtc flags 1 bytes 4, 5, dtc flags 3 bytes 6, 7
        data.avgCell = CANmsgRX.data[2];
        data.highestCell = CANmsgRX.data[1];
        data.chargePercent = (int)(CANmsgRX.data[0] / 2);
      break;
      case 0x10:
        data.cfgChangedSinceLastConfirm = false;
      break;
      case 0xA9:
        data.glvVoltage = CANmsgRX.data[7] * 256 + CANmsgRX.data[6];
        data.glvVoltage = (float)((int)(data.glvVoltage * 10)) / 10;
      break;
      case 0xF2:
        data.state = static_cast<vehicleState>(CANmsgRX.data[0]);
      break;
    }
    //lastData = data;
  }
}