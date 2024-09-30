#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/vector.h>

#define LED_PORT (GPIOD)  
#define LED_PIN  (GPIO12)

#define TICK_FREQ   (1000)
#define AHB_FREQ    (84000000)

volatile uint64_t ticks = 0;  //the reason for using a 64 bit number is to avoid overwrapping 
void sys_tick_handler(void){
  ticks++;  //note that there is a problem with this(incrementing a 64 bit number, which actually take 2 instruction cycles)
}

static uint64_t get_ticks(void){
  return ticks;
}

static void rcc_setup(void){
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

static void gpio_setup(void){
  rcc_periph_clock_enable(RCC_GPIOD); //for every peripheral, we have to enable/initialize the clock designated for it, which eventually specifies that the peripheral is ON
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

static void systick_setup(void){
  systick_set_frequency(TICK_FREQ, AHB_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();
}

int main(void){
  rcc_setup();
  gpio_setup();

  systick_setup();

  uint64_t start_time = get_ticks();

  while(1){
    if((get_ticks() - start_time) >= 1000){
      gpio_toggle(LED_PORT, LED_PIN);
      start_time = get_ticks();
    }
    //Meanwhile, you can perform other tasks, maybe go on and blink yet another LED!!
  }

  //we will never actually return
  return 0;
}