#include "Init.h"

static void BTN1_CHK_TASK(void *pvParameters);
static void BTN2_CHK_TASK(void *pvParameters);
static void UART_TASK(void *pvParameters);

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}

xQueueHandle xQueue;
xTaskHandle xBTN1_Handle;
xTaskHandle xBTN2_Handle;
xTaskHandle xUART_Handle;


uint32_t counter = 0;

int main() 
{
	PortAInit();
  PortFInit();
	UART0Init();
	xQueue = xQueueCreate (5,sizeof(long));
	
	if (xQueue != NULL) {
	xTaskCreate(BTN1_CHK_TASK,"Button 1 Check", configMINIMAL_STACK_SIZE,NULL , 1, &xBTN1_Handle);
	xTaskCreate(BTN2_CHK_TASK,"Button 2 Check", configMINIMAL_STACK_SIZE,NULL , 1, &xBTN2_Handle);
	xTaskCreate(UART_TASK,"UART", configMINIMAL_STACK_SIZE,NULL , 2, &xUART_Handle);
	
	vTaskStartScheduler();
	}
	else {
	// Queue couldn't be created
	}
}

void BTN1_CHK_TASK(void *pvParameters){
  for(;;){
		if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0){	
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 1);
				counter++;
				while ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0)) {}
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)== 0);
				taskYIELD()
		}
  }
}
void BTN2_CHK_TASK(void *pvParameters){
	portBASE_TYPE xStatus;
	unsigned portBASE_TYPE uxPriority;
	uxPriority = uxTaskPriorityGet( xBTN2_Handle );
	for(;;){
		if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)== 0){	
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)== 1);
				vTaskPrioritySet( xUART_Handle, ( uxPriority - 1 ) );
				xStatus = xQueueSendToBack(xQueue,&counter,0);
				if (xStatus == pdPASS){
						counter = 0 ;
						}
				vTaskPrioritySet( xUART_Handle, ( uxPriority + 1 ) );
				while ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)== 0)) {}
				do {
					delay(20);
				} while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)== 0);
					taskYIELD()
		}
	}
}

void UART_TASK(void *pvParameters){
	long ReceivedValue;
	portBASE_TYPE xStatus;
	unsigned char buffer [50];
  for(;;){
		xStatus = xQueueReceive(xQueue,&ReceivedValue,portMAX_DELAY);
		if (xStatus == pdPASS) {
					sprintf (buffer, "%ld",ReceivedValue);
					for(int i = 0; buffer[i] != '\0'; i++){
						UARTCharPut(UART0_BASE, buffer[i]);
					}
					UARTCharPut(UART0_BASE, '\r');
					UARTCharPut(UART0_BASE, '\n');
				}
		else {
		}
		//vTaskDelay(1000/ portTICK_RATE_MS);
  }
}

