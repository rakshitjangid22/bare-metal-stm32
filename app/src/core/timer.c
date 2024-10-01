#include "core/timer.h"

#include<libopencm3/stm32/timer.h>
#include<libopencm3/stm32/rcc.h>

// 84_000_000
// freq = system_freq / ((prescaler - 1) * (arr - 1))

#define PRESCALER (84)
#define ARR_VALUE (1000)

void timer_setup(void){
    rcc_periph_clock_enable(RCC_TIM4);

    // High level timer configuration
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    // Setup PWM Mode
    timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);

    // Enable PWM output
    timer_enable_counter(TIM4);
    timer_enable_oc_output(TIM4, TIM_OC1);

    // Setup frequency and resolution
    timer_set_prescaler(TIM4, PRESCALER - 1);
    timer_set_period(TIM4, ARR_VALUE - 1);
    
}

void timer_pwm_set_duty_cycle(float duty_cycle){
    // duty_cycle = (ccr / arr) * 100
    // crr = (duty_cycle * arr / 100)
    const float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);
    timer_set_oc_value(TIM4, TIM_OC1, (uint32_t)raw_value);
}