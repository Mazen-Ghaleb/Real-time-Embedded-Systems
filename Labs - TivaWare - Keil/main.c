#include "Init.h"

static void INIT_TASK(void *pvParameters);
void toggle_red(void);
void toggle_blue(void);
static void vCounterTask( void *pvParameters );
static void vLedTogglerTask( void *pvParameters );
void vApplicationIdleHook(void);
void delay (int n);

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}

unsigned char buffer [50];
xTaskHandle xCounterTask_Handle;
xTaskHandle xLedTogglerTask_Handle;
xSemaphoreHandle xBinarySemaphore;
xSemaphoreHandle xMutex;

int main() 
{
	vSemaphoreCreateBinary(xBinarySemaphore)
	xMutex = xSemaphoreCreateMutex();
	
	if( xBinarySemaphore != NULL && xMutex!= NULL) {

		xTaskCreate(vCounterTask,"CounterTask", configMINIMAL_STACK_SIZE,"This is the CounterTask\n" , 2, &xCounterTask_Handle);
		xTaskCreate(vLedTogglerTask,"Led Toggler Task", configMINIMAL_STACK_SIZE,"This is LedToggler Task\n" , 3, &xLedTogglerTask_Handle);
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
	SwitchInterruptInit();
	UART0Init();
	
	vTaskDelete(NULL);
}

static void vCounterTask( void *pvParameters ){
		for( ;; )
		{
			xSemaphoreTake( xMutex, portMAX_DELAY );
			sprintf (buffer, "%s",(char * ) pvParameters);
			for(int i = 0; buffer[i] != '\0'; i++){
						UARTCharPut(UART0_BASE, buffer[i]);
			}
			UARTCharPut(UART0_BASE, '\r');
			UARTCharPut(UART0_BASE, '\n');
			for (int i =0;i<11;i++)
			{
			sprintf (buffer, "%d",i);
			for(int i = 0; buffer[i] != '\0'; i++){
						UARTCharPut(UART0_BASE, buffer[i]);
			}
			UARTCharPut(UART0_BASE, '\r');
			UARTCharPut(UART0_BASE, '\n');
			}
			xSemaphoreGive(xMutex);
		}
}

static void vLedTogglerTask( void *pvParameters ){
    xSemaphoreTake( xBinarySemaphore, 0 );
		portBASE_TYPE xStatus;
    for( ;; )
    {
      xStatus = xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
			if (xStatus == pdPASS) {
				xSemaphoreTake( xMutex, portMAX_DELAY );
				sprintf (buffer, "%s",(char * ) pvParameters);
				for(int i = 0; buffer[i] != '\0'; i++){
						UARTCharPut(UART0_BASE, buffer[i]);
				}
				UARTCharPut(UART0_BASE, '\r');
				UARTCharPut(UART0_BASE, '\n');
				
				toggle_red();
				xSemaphoreGive(xMutex);
				vTaskDelay(500/portTICK_RATE_MS);
				}
    }
}

// Switch 0 handler
void SwitchHandler(void) {
    //uint32 switches = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4); // Read the switches
    // (switches & 0x10) // If switch 2 has been pressed
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4); // Clear the Interrupt
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}


void vApplicationIdleHook(){
	__asm("wfi\n");
}

void toggle_red()
{
  if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1) & (1<<1))
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
  else
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
}

void toggle_blue()
{
  if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2) & (1<<2))
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
  else
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
}
