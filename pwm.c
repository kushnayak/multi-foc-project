// #include "api.h"
// #include "api-asm.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "pwm.h"
#include "utils.h"

/*
 * PA0, PA1, PA2 -> TIM_2 CH1, CH2, CH3
 */
void pwm_setup(void) {
    // Setup system clock
    
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    GPIO0 | GPIO1 | GPIO2);

    gpio_set_af(GPIOA,
                GPIO_AF1,
                GPIO0 | GPIO1 | GPIO2);

    gpio_set_output_options(GPIOA,
                            GPIO_OTYPE_PP,
                            F3_GPIO_OSPEED_50MHZ,
                            GPIO0 | GPIO1 | GPIO2);
                
    
    timer_disable_counter(TIM2);
    rcc_periph_reset_pulse(RST_TIM2);

    timer_set_mode(TIM2,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, // Center aligned PWM better for motors? 
                   TIM_CR1_DIR_UP);


    // timer_set_prescaler(TIM2, 72 - 1); // 1 MHz clock, 1 tick / microsecond
    // 72 MHz clock -> 72e6 ticks / sec
    // Need 25 KHz PWM freq -> 72e6 ticks / sec / (25e3 cycles / sec) -> 2880 ticks / cycle
    timer_enable_preload(TIM2);
    timer_enable_update_event(TIM2);
    timer_generate_event(TIM2, TIM_EGR_UG);
    timer_continuous_mode(TIM2);
    timer_set_period(TIM2, 4500);

    timer_disable_oc_output(TIM2, TIM_OC1);
    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_disable_oc_output(TIM2, TIM_OC3);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
    timer_enable_oc_preload(TIM2, TIM_OC1);
    timer_enable_oc_preload(TIM2, TIM_OC2);
    timer_enable_oc_preload(TIM2, TIM_OC3);
    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_enable_oc_output(TIM2, TIM_OC2);
    timer_enable_oc_output(TIM2, TIM_OC3);


    timer_set_counter(TIM2, 0);
    timer_enable_counter(TIM2);
}

