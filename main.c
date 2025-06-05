#include <stdarg.h>
#include <stdint.h>

#define TRUE  1
#define FALSE 0

extern void print_message(const char* fmt, ...);

void uart_putchar(char c)
{
}

void print_message(const char* s, ...)
{
    va_list ap;
    va_start (ap, s);

    while (*s) {
        if (*s == '%' && *(s+1) == 'x') {
            unsigned long v = va_arg(ap, unsigned long);
            _Bool print_started = FALSE;
            int i;

            s += 2;
            for (i = 15; i >= 0; i--) {
                unsigned long x = (v & 0xFUL << i*4) >> i*4;
                if (print_started || x != 0U || i == 0) {
                    print_started = TRUE;
                    uart_putchar((x < 10 ? '0' : 'a' - 10) + x);
                }
            }
        } else {
            if (*s == '\n') {
                uart_putchar('\r');
            }
            uart_putchar(*s++);
        }
    }

    va_end (ap);
}

void main(void)
{
    print_message("Hello World!\n");

    while (1);
}

