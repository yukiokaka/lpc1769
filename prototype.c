#include "LPC17xx.h"

volatile unsigned long tick;

void SysTick_Handler(void)
{
    tick ++;
    if (!(tick % 100)){                 //0.1秒(100ms)をカウント
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
    LPC_GPIO0->FIODIR |= (1 << 22);        //出力は"1"、デフォルトは入力の"0"
    //ここでは、デフォルトでpull-upの設定とオープンドレインではない設定になっているので割愛
    SysTick_Config(SystemCoreClock/1000 - 1);

    while(1);

    return 0;
}
