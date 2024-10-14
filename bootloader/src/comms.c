#include "comms.h"
#include "core/uart.h"

#include "core/crc.h"

typedef enum comms_state_t{
    CommsState_Length,
    CommsState_Data,
    CommsState_CRC
} comms_state_t;

static comms_state_t state = CommsState_Length;
static data_bytes_count = 0;    // To keep track of the number of data bytes read

static comms_packet_t temporary_packet = { .length = 0, .data = {0}, .crc = 0};

void comms_setup(void){

}

void comms_update(void){
    while(uart_data_available()){   // Managing the state machine
        switch (state)
        {
        case CommsState_Length:
        {
            temporary_packet.length = uart_read_byte();
            state = CommsState_Data;
        } break;
        case CommsState_Data:
        {
            temporary_packet.data[data_bytes_count++] = uart_read_byte();
            if(data_bytes_count >= PACKET_DATA_LENGTH){
                data_bytes_count = 0;
                state = CommsState_CRC;
            }
        } break;
        case CommsState_CRC:
        {
            temporary_packet.crc = uart_read_byte();
            uint8_t computed_crc = crc8()
        } break;
        default:
            break;
        }
    }
}

bool comms_packet_available(void){

}

void comms_write(comms_packet_t* packet){

}

void comms_read(comms_packet_t* packet){

}

