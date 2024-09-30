#include "core/system.h"

#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/vector.h>
#include<libopencm3/stm32/rcc.h>

static volatile uint64_t ticks = 0;  //the reason for using a 64 bit number is to avoid overwrapping 
void sys_tick_handler(void){
  ticks++;  //note that there is a problem with this(incrementing a 64 bit number, which actually take 2 instruction cycles)
}

static void rcc_setup(void){
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

static void systick_setup(void){
  systick_set_frequency(TICK_FREQ, AHB_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();
}

uint64_t system_get_ticks(void){
  return ticks;
}

void system_setup(void){
    rcc_setup();
    systick_setup();
}