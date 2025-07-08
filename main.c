#include <stdarg.h>
#include <stdint.h>
#include "armv8util.h"
#include "gicv2.h"

#define TRUE  1
#define FALSE 0

#define GenCounterFreq  (0x5F5E100/20)   //5M
#define TICK_CYCLES     (16*GenCounterFreq)
#define GEN_TIMER_INTID  30
#define CNTP_CTL_EL0_ENABLE   (1U<<0)

extern void ActivateInterrupt(uint32_t intID, uint32_t ipri, int type);
extern void SetupGIC(void);
extern void EnableInt(void);
extern void DisableInt(void);
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

void Timer(void)
{
    print_message("Timer\n");

    do {
        WriteSysReg(CNTP_CVAL_EL0, ReadSysReg(CNTPCT_EL0) + TICK_CYCLES );
    } while ((long)(ReadSysReg(CNTPCT_EL0) - ReadSysReg(CNTP_CVAL_EL0)) >= 0);
}

static void StartTimer(void)
{
    ActivateInterrupt(GEN_TIMER_INTID, 16U, FALSE);

    WriteSysReg(CNTP_CVAL_EL0, ReadSysReg(CNTPCT_EL0) + TICK_CYCLES );
    WriteSysReg(CNTP_CTL_EL0, CNTP_CTL_EL0_ENABLE);
}

static void clearbss(void)
{
    unsigned char *p;
    extern unsigned char _bss_start[];
    extern unsigned char _bss_end[];

    for (p = _bss_start; p < _bss_end; p++) {
        *p = 0LL;
    }
}

void main(void)
{
    print_message("Hello World!\n");

    clearbss();
    SetupGIC();
    StartTimer();

    while (1);
}

