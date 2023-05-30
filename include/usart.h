#ifndef USART_H
#define USART_H

void usart_setup(void);
void usart_putc(char c);
void usart_print(const char *str);
void usart_printf(const char *fmt, ...);

#endif
