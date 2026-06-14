#include "header.h"

void sendData(unsigned int addr, unsigned int dataLength, unsigned long long msg){
    CANmsgTX.identifier = addr;
    CANmsgTX.extd = 0;
    CANmsgTX.data_length_code = dataLength;
    for (int i = 0; i < dataLength; i++) {
        CANmsgTX.data[i] = (msg >> (i*8)) & 0xFF;
    }

    //Queue message for transmission
    if (twai_transmit(&CANmsgTX, pdMS_TO_TICKS(1000)) == ESP_OK) {
        DEBUG_PRINTLN("Message queued for transmission");
    } else {
        DEBUG_PRINTLN("Failed to queue message for transmission");
    }
}
void sendData(twai_message_t msg){
    if (twai_transmit(&CANmsgTX, pdMS_TO_TICKS(1000)) == ESP_OK) {
        DEBUG_PRINTLN("Message queued for transmission");
    } else {
        DEBUG_PRINTLN("Failed to queue message for transmission");
    }
}

void sendConfig(){
    twai_message_t cfg;
    cfg.identifier = 0x10;
    cfg.data_length_code = 7;
    cfg.extd = false;
    //max drive torque
    cfg.data[0] = 1;
    cfg.data[1] = data.maxDriveTorqueNm >> 8;
    cfg.data[2] = data.maxDriveTorqueNm & 0xFF;
    cfg.data[3] = data.maxRegenTorqueNm >> 8;
    cfg.data[4] = data.maxRegenTorqueNm & 0xFF;
    cfg.data[5] = data.maxRegenCurrent >> 8;
    cfg.data[6] = data.maxRegenCurrent & 0xFF;
    sendData(cfg);
    cfg.data[0] = 2;
    cfg.data[1] = data.brakeFilterAlphaScale >> 8;
    cfg.data[2] = data.brakeFilterAlphaScale & 0xFF;
    cfg.data[3] = data.brakeFilterBetaScale >> 8;
    cfg.data[4] = data.brakeFilterBetaScale & 0xFF;
    cfg.data[5] = data.brakeMechThreshold >> 8;
    cfg.data[6] = data.brakeMechThreshold & 0xFF;
    sendData(cfg);
    cfg.data_length_code = 2;
    cfg.data[0] = 10;
    cfg.data[1] = data.serialDebug;
    sendData(cfg);
}