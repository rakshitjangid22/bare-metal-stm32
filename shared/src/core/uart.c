#include "core/uart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>


static uint8_t data_buffer;
static bool data_available = false;

void usart1_isr(void){
    const bool overrun_occurred = usart_get_flag(USART1, USART_FLAG_ORE) == 1;
    const bool data_received = usart_get_flag(USART1, USART_FLAG_RXNE) == 1;

    if(data_received || overrun_occurred){
        data_buffer = (uint8_t)usart_recv(USART1);
        data_available = true;
    }

}

void uart_setup(void){
    rcc_periph_clock_enable(RCC_USART1);

    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, 1);
    usart_set_baudrate(USART1, 115200);
    usart_set_parity(USART1, 0);
    
    usart_enable_rx_interrupt(USART1);
    nvic_enable_irq(NVIC_USART1_IRQ);

    usart_enable(USART1);
}

void uart_teardown(void){

}

void uart_write(uint8_t* data, const uint32_t length){
    for(uint32_t i = 0; i < length; i++){
        uart_write_byte(data[i]);
    }
}

void uart_write_byte(uint8_t data){
    usart_send_blocking(USART1, (uint16_t)data);
}

uint32_t uart_read(uint8_t* data, const uint32_t length){       // This implementation is only for reading 1 byte at a time
    if(length > 0 && data_available){
        *data = data_buffer;
        data_available = false;
        return 1;
    }
    return 0;
}

uint8_t uart_read_byte(void){
    data_available = false;
    return data_buffer;
}

bool uart_data_available(void){
    return data_available;
}