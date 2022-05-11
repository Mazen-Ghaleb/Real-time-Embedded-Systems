#include "Init.h"

static void INIT_TASK(void *pvParameters);
void vApplicationIdleHook(void);
static void vHandlerTask1( void *pvParameters );
static void vHandlerTask2( void *pvParameters );
static void vPeriodicTask( void *pvParameters );
void delay (int n);

void Timer0delay(uint32 milliseconds){
  TimerDisable(TIMER0_BASE, TIMER_A);                           // Sandwich
  TimerLoadSet(TIMER0_BASE, TIMER_A, CalcTicks(milliseconds));  // Load the ticks value
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);               // Clear any existing interrupts
  TimerEnable(TIMER0_BASE, TIMER_A);                            // Sandwich
}

void Timer1delay(uint32 milliseconds){
  TimerDisable(TIMER1_BASE, TIMER_A);                           // Sandwich
  TimerLoadSet(TIMER1_BASE, TIMER_A, CalcTicks(milliseconds));  // Load the ticks value
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);               // Clear any existing interrupts
  TimerEnable(TIMER1_BASE, TIMER_A);                            // Sandwich
}

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}

xTaskHandle xHandlerTask1_Handle;
xTaskHandle xHandlerTask2_Handle;
xTaskHandle xPeriodicTask_Handle;
xSemaphoreHandle xBinarySemaphore1;
xSemaphoreHandle xBinarySemaphore2;


uint32_t counter1 = 0;
uint32_t counter2 = 0;


int main() 
{
	vSemaphoreCreateBinary(xBinarySemaphore1)
	vSemaphoreCreateBinary(xBinarySemaphore2)
	
	if( xBinarySemaphore1 != NULL && xBinarySemaphore2!= NULL) {

		xTaskCreate(vHandlerTask1,"Handler Task 1", configMINIMAL_STACK_SIZE,NULL , 1, &xHandlerTask1_Handle);
		xTaskCreate(vHandlerTask2,"Handler Task 2", configMINIMAL_STACK_SIZE,NULL , 1, &xHandlerTask2_Handle);
		xTaskCreate(vPeriodicTask,"Periodic Task", configMINIMAL_STACK_SIZE,NULL , 2, &xPeriodicTask_Handle);
		xTaskCreate(INIT_TASK,"Intialization Task", configMINIMAL_STACK_SIZE,NULL , 4, NULL);
		
		vTaskStartScheduler();
	}
	else {
		// Semaphore couldn't be created
	}
}

void INIT_TASK(void *pvParameters){
	PortFInit();
	TimerInit0();
	TimerInit1();
	
	vTaskDelete(NULL);
}

static void vHandlerTask1( void *pvParameters ){
    xSemaphoreTake( xBinarySemaphore1, 0 );
    for( ;; )
    {
        xSemaphoreTake( xBinarySemaphore1, portMAX_DELAY );
				counter1++;
    }
}

static void vHandlerTask2( void *pvParameters ){
    xSemaphoreTake( xBinarySemaphore2, 0 );
    for( ;; )
    {
        xSemaphoreTake( xBinarySemaphore2, portMAX_DELAY );
				counter2++;
    }
}

static void vPeriodicTask( void *pvParameters ){
    for( ;; )
    {
			*((unsigned long volatile *) (0xE000E200)) |= (0x1<<19) | (0x1<<21);
			vTaskDelay(1000/portTICK_RATE_MS);
      //Timer0delay(100);
			//Timer1delay(100);
    }
}

// Timer 0 handler
void Timer0Handler(void){
	TimerDisable(TIMER0_BASE, TIMER_A);
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( xBinarySemaphore1, &xHigherPriorityTaskWoken );
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);               // Clear any existing interrupts
}

// Timer 1 handler
void Timer1Handler(void){
	TimerDisable(TIMER1_BASE, TIMER_A);
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( xBinarySemaphore2, &xHigherPriorityTaskWoken );
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);               // Clear any existing interrupts
}

void vApplicationIdleHook(){
	__asm("wfi\n");
}
