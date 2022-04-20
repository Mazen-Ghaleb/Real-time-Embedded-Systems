#include "Init.h"

void vApplicationIdleHook();
static void BTN1_CHK_TASK(void *pvParameters);
static void BTN2_CHK_TASK(void *pvParameters);
static void UART_TASK(void *pvParameters);
xQueueHandle xQueue;
int counter = 0;

int main() 
{ 
	PortAInit();
  PortFInit();
	UART0Init();
	xQueue = xQueueCreate (5,sizeof(long));
	
	if (xQueue != NULL) {
	xTaskCreate(BTN1_CHK_TASK,"Button 1 Check", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	xTaskCreate(BTN2_CHK_TASK,"Button 2 Check", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	//xTaskCreate(UART_TASK,"UART", configMINIMAL_STACK_SIZE,NULL , 2, NULL);

	vTaskStartScheduler();
	}
	else {
	// Queue couldn't be created
	}
}

void BTN1_CHK_TASK(void *pvParameters){
  for(;;){
		//vTaskDelay(200/ portTICK_RATE_MS);
		if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
			  //vTaskDelay(200/ portTICK_RATE_MS);
				for(int i = 0; i < 700000; i++){}
				if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
						counter++;
						//printf("Counter increased to %d. \n",counter);
				}
		}
  }
}
void BTN2_CHK_TASK(void *pvParameters){
  portBASE_TYPE xStatus;
	for(;;){
		//vTaskDelay(200/ portTICK_RATE_MS);
		if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
			  //vTaskDelay(200/ portTICK_RATE_MS);
				for(int i = 0; i < 700000; i++){}
				if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
						xStatus = xQueueSendToBack(xQueue,&counter ,0);
						if (xStatus != pdPASS){
								//vPrintString( "Could not receive from the queue.\r\n" );
						}
						else {
							//printf("Value stored in the Queue is %d. \n",counter);
							counter = 0 ;
						}
				}
		}
  }
}
void UART_TASK(void *pvParameters){
	long ReceivedValue;
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100/ portTICK_RATE_MS;
  for(;;){
	xStatus = xQueueReceive(xQueue,ReceivedValue,xTicksToWait);
	if (xStatus == pdPASS) {
		//UARTCharPut(UART0_BASE, (unsigned char)* ReceivedValue));
	}
	else {
		vPrintString("Could not receive from the queue. \n");
	}
  vTaskDelay(2000/ portTICK_RATE_MS);
  }
}

void vApplicationIdleHook(){
	__asm("wfi\n");
}
