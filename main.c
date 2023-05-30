// #include "api.h"
// #include "api-asm.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>

#include <stdint.h>
#include <math.h>

// #include "FreeRTOS.h"
// #include "task.h"

#include "usart.h"
#include "pwm.h"
#include "utils.h"

#ifndef M_PI
#error M_PI not defined
#endif

/*
void vApplicationStackOverflowHook(TaskHandle_t xTask __attribute((unused)),
                                   char * pcTaskName __attribute((unused))) {
	for(;;);	// Loop forever here..
}
*/

static void setup(void) {
    // Setup system clock
    rcc_clock_setup_pll(rcc_hse8mhz_configs);

    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA,
                    GPIO_MODE_OUTPUT, 
                    GPIO_PUPD_NONE, 
                    GPIO5);
    gpio_set_output_options(GPIOA,
                            GPIO_OTYPE_PP,
                            GPIO_OSPEED_2MHZ,
                            GPIO5);

    

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    systick_counter_enable();
    // systick_set_frequency(72000000, rcc_hse8mhz_configs.ahb_frequency);
    systick_interrupt_enable();
    systick_set_reload(9000000 / 1000);
    systick_clear();

    usart_setup();
    pwm_setup();
}

const int rpm = 500;
uint64_t msec = 0;
const float dt = 0.001f; // sec
const float x = rpm * 2 * M_PI / 3600; // rad / sec

// sin(dt * x + shift)
float k = dt * x;
float angle1 = 0;
float angle2 = 2 * M_PI / 3;
float angle3 = 4 * M_PI / 3;

void sys_tick_handler(void) {
    ++msec;

    angle1 += k;
    angle2 += k;
    angle3 += k;
    if (angle1 > 2 * M_PI) angle1 -= 2 * M_PI;
    if (angle2 > 2 * M_PI) angle2 -= 2 * M_PI;
    if (angle3 > 2 * M_PI) angle3 -= 2 * M_PI;

    timer_set_oc_value(TIM2, TIM_OC1, (uint32_t)(1440.0f * sin1(angle1) + 1440.0f));
    timer_set_oc_value(TIM2, TIM_OC2, (uint32_t)(1440.0f * sin1(angle2) + 1440.0f));
    timer_set_oc_value(TIM2, TIM_OC3, (uint32_t)(1440.0f * sin1(angle3) + 1440.0f));

    // gpio_toggle(GPIOA,GPIO5);
    usart_printf("Time: %d\n", msec);
}


/*
static void task1(void *args __attribute((unused))) {
        static int i = 0;
	for (;;) {
		gpio_toggle(GPIOA,GPIO5);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
*/

int main(void) {
    setup();

    /*
    while (1) {
        gpio_clear(GPIOA, GPIO5);        

        for (int i = 0; i < 20000000; ++i) {
            __asm__("nop");
        }

        gpio_set(GPIOA, GPIO5);        

        for (int i = 0; i < 5000000; ++i) {
            __asm__("nop");
        }
    }
    */

    // uint8_t fpu = SCB_CPACR & (SCB_CPACR_FULL * (SCB_CPACR_CP10 | SCB_CPACR_CP11));
    usart_printf("fpu enabled: %u\n", SCB_CPACR & (SCB_CPACR_FULL * (SCB_CPACR_CP10 | SCB_CPACR_CP11)));

    /*
    uint32_t val1, val2;
    float angle = 2 * M_PI / 3;
    int ans;
    float rpm = 500;
    float s = 500 * 2 * M_PI / 3600, dt = 0.1;
    */


    while (1) {
        // timer_set_oc_value(TIM2, TIM_OC2, (uint32_t)(2880 / 2.0f));
        // usart_printf("oc1: %d", TIM_CCR1(TIM2));
        /*
        val1 = systick_get_value();
        gpio_toggle(GPIOA, GPIO5);        

        val1 = systick_get_value();
        angle += s * dt;
        if (angle > 2 * M_PI) angle -= 2 * M_PI;
        ans = sin1(angle) * 2880;
        val2 = systick_get_value();

        usart_printf("Took %u ticks ticks to compute sin(%f) = %d\n", val1 - val2, angle, ans);

        while(!systick_get_countflag());
        for (int i = 0; i < 5000000; ++i) {
            __asm__("nop");
        }
        */
    }

    /*
    xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
    vTaskStartScheduler();

    while (1) __asm("nop");
    */

    return 0;
}
