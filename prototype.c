/* Kernel includes. */ 
#include "FreeRTOS.h" 
#include "task.h" 


#define LED_STACK_SIZE 64
#define LED_TASK_PRIO tskIDLE_PRIORITY + 1

void LedTask(void *param) 
{
	static uint32_t state = 0;
    
    while(1) {
        vTaskDelay(500);
        LPC_GPIO0 -> FIOCLR = (1 << 22);
    }
}

void LedTask2(void *param) 
{
	static uint32_t state = 0;
    
    while(1) {
        vTaskDelay(1000);
        LPC_GPIO0 -> FIOSET = (1 << 22);
	}
}

int main(void) 
{
    // LED初期化
    LPC_PINCON->PINSEL1  &= ~(3 << 12);
    // Set GPIO - P0_22 - to be output
    LPC_GPIO0->FIODIR |= (1 << 22);    
    LPC_GPIO0 -> FIOSET = (1 << 22);

        
    xTaskCreate( LedTask, ( signed portCHAR * ) "LedTask", LED_STACK_SIZE, NULL, LED_TASK_PRIO, NULL );
    xTaskCreate( LedTask2, ( signed portCHAR * ) "LedTask2", LED_STACK_SIZE, NULL, LED_TASK_PRIO-1, NULL );

	vTaskStartScheduler();
    while(1);
	return 0;
}
