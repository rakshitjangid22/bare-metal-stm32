#include "core/uart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "core/ring_buffer.h"

#define BAUD_RATE           (115200)
#define RING_BUFFER_SIZE    (128)   // For maximum of around 10ms of latency

static ring_buffer_t rb = {0U}; 

static uint8_t data_buffer[RING_BUFFER_SIZE] = {0U};

void usart1_isr(void){
    const bool overrun_occurred = usart_get_flag(USART1, USART_FLAG_ORE) == 1;
    const bool data_received = usart_get_flag(USART1, USART_FLAG_RXNE) == 1;

    if(data_received || overrun_occurred){
        if(!ring_buffer_write(&rb, (uint8_t)usart_recv(USART1))){
            // Handle failure...
        }
    }
}

void uart_setup(void){
    ring_buffer_setup(&rb, data_buffer, RING_BUFFER_SIZE);

    rcc_periph_clock_enable(RCC_USART1);

    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, 1);
    usart_set_baudrate(USART1, BAUD_RATE);
    usart_set_parity(USART1, 0);
    
    usart_enable_rx_interrupt(USART1);
    nvic_enable_irq(NVIC_USART1_IRQ);

    usart_enable(USART1);
}

void uart_write(uint8_t* data, const uint32_t length){
    for(uint32_t i = 0; i < length; i++){
        uart_write_byte(data[i]);
    }
}

void uart_write_byte(uint8_t data){
    usart_send_blocking(USART1, (uint16_t)data);
}

uint32_t uart_read(uint8_t* data, const uint32_t length){    
    if(length == 0) return 0;

    for(uint32_t i = 0; i < length; i++){
        if(!ring_buffer_read(&rb, &data[i])){
            return i;
        }
    }
    return length;
}   

uint8_t uart_read_byte(void){   // The implementation assumes that the user has made sure that a byte will surely be read upon calling this function
    uint8_t byte_read = 0;
    (void)uart_read(&byte_read, 1);
    return byte_read;
}

bool uart_data_available(void){
    return !ring_buffer_empty(&rb);
}