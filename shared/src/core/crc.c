#include "crc.h"

uint8_t crc8(uint8_t* data, uint32_t length){
    uint8_t crc = 0;

    for(uint32_t i = 0; i < length; i++){
        crc ^= data[i];
        for(uint8_t i = 0; i < 8; i++){
            if(crc & 0x80){
                crc = (crc << 1) ^ 0x07;
            }
            else{
                crc << 1;
            }
        }
    }
    return crc;
}
