#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <stdio.h>
#include <stdarg.h>

#include "usart.h"
#include "utils.h"

char buf[256];

void usart_setup(void) {
    // PB10 USART3_TX
    // PB11 USART3_RX
    
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART3);

    gpio_mode_setup(GPIOB,
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE,
                  GPIO10 | GPIO11);

    gpio_set_af(GPIOB,
                GPIO_AF7,
                GPIO10 | GPIO11);

    gpio_set_output_options(GPIOB,
                            GPIO_OTYPE_PP,
                            F3_GPIO_OSPEED_2MHZ,
                            GPIO10);


    usart_set_mode(USART3, USART_MODE_TX_RX);
    usart_set_baudrate(USART3, 115200);
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
    usart_set_databits(USART3, 8);
    usart_set_stopbits(USART3, USART_STOPBITS_1);
    usart_set_parity(USART3, USART_PARITY_NONE);
    usart_enable(USART3);

}

void usart_putc(char c) {
    usart_send_blocking(USART3, c);
    if (c == '\n') {
        usart_send_blocking(USART3, '\r');
    }
}

void usart_print(const char *str) {
    for (; (*str); ++str) {
        usart_putc(*str);
    }
}

void usart_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    usart_print(buf);
}

