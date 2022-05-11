#include "Init.h"

static void INIT_TASK(void *pvParameters);
static void BTN1_CHK_TASK(void *pvParameters);
static void BTN2_CHK_TASK(void *pvParameters);
static void UART_TASK(void *pvParameters);
void delay (int n);

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}

xQueueHandle xQueue;
xTaskHandle xBTN1_Handle;
xTaskHandle xBTN2_Handle;
xTaskHandle xUART_Handle;

xSemaphoreHandle xCountingSemaphore;

uint32_t counter = 0;

int main() 
{
	xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );
	xQueue = xQueueCreate (5,sizeof(long));

	if( xCountingSemaphore != NULL ) {

		if (xQueue != NULL) {
		xTaskCreate(BTN1_CHK_TASK,"Button 1 Check", configMINIMAL_STACK_SIZE,NULL , 1, &xBTN1_Handle);
		xTaskCreate(UART_TASK,"UART", configMINIMAL_STACK_SIZE,NULL , 2, &xUART_Handle);
		xTaskCreate(INIT_TASK,"Intialization Task", configMINIMAL_STACK_SIZE,NULL , 3, NULL);
		
		vTaskStartScheduler();
		}
		else {
		// Queue couldn't be created
		}
	}
	else {
		// Semaphore couldn't be created
	}
}

void INIT_TASK(void *pvParameters){
	PortAInit();
  PortFInit();
	UART0Init();
	vTaskDelete(NULL);
}

void BTN1_CHK_TASK(void *pvParameters){
	portBASE_TYPE xStatus;
	for(;;){
		if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0){	
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 1);
				counter++;
				xStatus = xQueueSendToBack(xQueue,&counter,0);
				if (xStatus != pdPASS){
				}
				xSemaphoreGive(xCountingSemaphore);
				while ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0)) {}
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0);
		}
  }
}


void UART_TASK(void *pvParameters){
	long ReceivedValue;
	portBASE_TYPE xStatus;
	unsigned char buffer [50];
  for(;;){
		xSemaphoreTake( xCountingSemaphore, portMAX_DELAY );
		xStatus = xQueueReceive(xQueue,&ReceivedValue,portMAX_DELAY);
		if (xStatus == pdPASS) {
					sprintf (buffer, "%ld",ReceivedValue);
					for(int i = 0; buffer[i] != '\0'; i++){
						UARTCharPut(UART0_BASE, buffer[i]);
					}
					UARTCharPut(UART0_BASE, '\r');
					UARTCharPut(UART0_BASE, '\n');
		}
  }
}
