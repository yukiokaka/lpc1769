#include "LPC17xx.h"

volatile unsigned long tick;

void SysTick_Handler(void)
{
    tick ++;
    if (!(tick % 100)){                 //0.1$BIC(B(100ms)$B$r%+%&%s%H(B
        if (LPC_GPIO0->FIOPIN  & (1 << 22))
            LPC_GPIO0 -> FIOCLR = (1 << 22);
        else
            LPC_GPIO0 -> FIOSET = (1 << 22);

    }
}
int main(void)
{
    tick = 0;
    // Set P0_22 to 00 - GPIO
    LPC_PINCON->PINSEL1  &= ~(3 << 12);
    // Set GPIO - P0_22 - to be output
    LPC_GPIO0->FIODIR |= (1 << 22);        //$B=PNO$O(B"1"$B!"%G%U%)%k%H$OF~NO$N(B"0"
    //$B$3$3$G$O!"%G%U%)%k%H$G(Bpull-up$B$N@_Dj$H%*!<%W%s%I%l%$%s$G$O$J$$@_Dj$K$J$C$F$$$k$N$G3d0&(B
    SysTick_Config(SystemCoreClock/1000 - 1);

    while(1);

    return 0;
}
