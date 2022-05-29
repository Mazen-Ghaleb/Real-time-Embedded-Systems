#include "Init.h"

static void INIT_TASK(void *pvParameters);
static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
void UART_PrintBuffer(void);
void toggle_blue(void);
void toggle_red(void);
void toggle_green(void);
void vApplicationIdleHook(void);
void delay (int n);

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}

unsigned char buffer [50];
xTaskHandle xTask1_Handle;
xTaskHandle xTask2_Handle;
xSemaphoreHandle xMutex;
xSemaphoreHandle xMutex2;

int main() 
{
	xMutex = xSemaphoreCreateMutex();
	xMutex2 = xSemaphoreCreateMutex();
	
	if( xMutex != NULL && xMutex2!= NULL) {

		xTaskCreate(vTask1,"Task 1", configMINIMAL_STACK_SIZE,"Task 1" , 2, &xTask1_Handle);
		xTaskCreate(vTask2,"Task 2", configMINIMAL_STACK_SIZE,"Task 2" , 2, &xTask2_Handle);
		xTaskCreate(INIT_TASK,"Intialization Task", configMINIMAL_STACK_SIZE,NULL , 4, NULL);
		
		vTaskStartScheduler();
	}
	else {
		// Semaphore or Mutex couldn't be created
	}
}

void INIT_TASK(void *pvParameters){
	PortAInit();
	PortFInit();
	UART0Init();
	
	vTaskDelete(NULL);
}

static void vTask1( void *pvParameters ){
	portBASE_TYPE xStatus;	
	for( ;; )
		{
			xStatus = xSemaphoreTake( xMutex, portMAX_DELAY );
			if (xStatus == pdPASS) {
					taskYIELD()
					sprintf (buffer, "%s","This is Task 1");
					UART_PrintBuffer();
					xStatus = xSemaphoreTake( xMutex2, portMAX_DELAY );
					if (xStatus == pdPASS) {
						sprintf (buffer, "%s","Deadlock didn't happen");
						UART_PrintBuffer();
						xSemaphoreGive(xMutex2);
					}
				}
			xSemaphoreGive(xMutex);
		}
}

static void vTask2( void *pvParameters ){
		portBASE_TYPE xStatus;
    for( ;; )
    {
      xStatus = xSemaphoreTake( xMutex2, portMAX_DELAY );
			if (xStatus == pdPASS) {
					taskYIELD()
					sprintf (buffer, "%s","This is Task 2");
					UART_PrintBuffer();
					xStatus = xSemaphoreTake(xMutex, portMAX_DELAY );
					if (xStatus == pdPASS) {
						sprintf (buffer, "%s","Deadlock didn't happen");
						UART_PrintBuffer();
						xSemaphoreGive(xMutex);
					}
				}
			xSemaphoreGive(xMutex2);
    }
}

void vApplicationIdleHook(){
	sprintf (buffer, "%s","Reached Idle Hook --> Tasks are blocked");
	UART_PrintBuffer();
	__asm("wfi\n");
}

void UART_PrintBuffer(void){
	for(int i = 0; buffer[i] != '\0'; i++){
				UARTCharPut(UART0_BASE, buffer[i]);
				}
				UARTCharPut(UART0_BASE, '\r');
				UARTCharPut(UART0_BASE, '\n');
}
