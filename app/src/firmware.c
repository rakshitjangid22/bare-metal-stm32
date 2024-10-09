#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/cm3/scb.h>

#include "core/system.h"
#include "timer.h"
#include "core/uart.h"

#define BOOTLOADER_SIZE (0x10000U)

#define LED_PORT  (GPIOD)   
#define LED_PIN   (GPIO12) 

#define UART_PORT (GPIOA)
#define RX_PIN    (GPIO10)
#define TX_PIN    (GPIO9)


static void vector_table_setup(void){
  SCB_VTOR = BOOTLOADER_SIZE;
}

static void gpio_setup(void){
  rcc_periph_clock_enable(RCC_GPIOD); //for every peripheral, we have to enable/initialize the clock designated for it, which eventually specifies that the peripheral is ON
  gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
  gpio_set_af(LED_PORT, GPIO_AF2, LED_PIN);

  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, RX_PIN | TX_PIN);  
  gpio_set_af(UART_PORT, GPIO_AF7, RX_PIN | TX_PIN);
}

int main(void){
  vector_table_setup();
  system_setup();
  gpio_setup();
  timer_setup();
  uart_setup();

  uint64_t start_time = system_get_ticks();
  float duty_cycle = 0.0f;

  timer_pwm_set_duty_cycle(duty_cycle);

  while(1){
    if((system_get_ticks() - start_time) >= 10){
      duty_cycle += 1.0f;
      if(duty_cycle > 100.0f) duty_cycle = 0.0f;

      timer_pwm_set_duty_cycle(duty_cycle);

      start_time = system_get_ticks();
    }
    //Meanwhile, you can perform other tasks, maybe go on and blink yet another LED!!
    if(uart_data_available()){
      uint8_t data = uart_read_byte();
      uart_write_byte(data + 1);
    }
  }

  //We will never actually return
  return 0;
}