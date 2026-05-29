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