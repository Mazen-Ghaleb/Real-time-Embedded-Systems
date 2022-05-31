#include "Init.h"

void INIT_TASK(void *pvParameters);
static void vDisplayTask( void *pvParameters );
static void vADCTask( void *pvParameters );
static void vChangeThresholdTask( void *pvParameters );
void vApplicationIdleHook(void);

void threshold_LED(unsigned int ReadTemperature);
void UART_PrintBuffer(void);

LCD lcd;

xSemaphoreHandle xMutex;
xSemaphoreHandle xBinarySemaphore;

xQueueHandle xQueue;


unsigned int threshold;
unsigned char buffer [50];


int main() 
{
	vSemaphoreCreateBinary(xBinarySemaphore)
	xMutex = xSemaphoreCreateMutex();
	xQueue = xQueueCreate (5,sizeof(unsigned int));
	if (xQueue != NULL && xMutex!= NULL && xBinarySemaphore != NULL)  {
		xTaskCreate(vADCTask,"ADC Task", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
		xTaskCreate(vDisplayTask,"Display Task", configMINIMAL_STACK_SIZE,NULL , 2, NULL);
		xTaskCreate(vChangeThresholdTask,"Change Threshold Task", configMINIMAL_STACK_SIZE,NULL , 3, NULL);
		xTaskCreate(INIT_TASK,"Intialization Task", configMINIMAL_STACK_SIZE,NULL , 4, NULL);		
		vTaskStartScheduler();
	}
	else {
	//Semaphore or Queue or Mutex couldn't be created 
	}
}

void INIT_TASK(void *pvParameters){
	PortAInit();
	PortFInit();
	UART0Init();
	SwitchInterruptInit();

	ADCInit();
	
	lcd = LCD_create();
	LCD_init(&lcd);
	
	vTaskDelete(NULL);
}
static void vChangeThresholdTask( void *pvParameters ){
	xSemaphoreTake( xBinarySemaphore, 0 );
	portBASE_TYPE xStatus;
	threshold = 30; //default
	for(;;){
		xStatus = xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
		if (xStatus == pdPASS) {
			xSemaphoreTake( xMutex, portMAX_DELAY );
				// Prints on Console to Enter Threshold
			sprintf (buffer, "%s","\nPlease Enter the threshold Temperature: ");
			UART_PrintBuffer();
		
			//Reads Threshold
			int j = 0;
			do {
					buffer[j] = UARTCharGet(UART0_BASE);
					UARTCharPut(UART0_BASE, buffer[j]);
					j++;
			}
			while((buffer[j-1] != '\n') && (buffer[j-1] != '\r') && (j < 50));
			UARTCharPut(UART0_BASE, '\r');
			UARTCharPut(UART0_BASE, '\n');
	//		if (j != 50) {
	//			buffer[j] = '\0';
	//		}
			threshold = atoi(buffer);
			xSemaphoreGive(xMutex);
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	}
}


static void vDisplayTask( void *pvParameters ){
		unsigned int ReceivedValue;
		portBASE_TYPE xStatus;
		for( ;; )
		{
			xStatus = xQueueReceive(xQueue,&ReceivedValue,portMAX_DELAY);
			if (xStatus == pdPASS) {
				xSemaphoreTake( xMutex, portMAX_DELAY );
				LCD_clear(&lcd);
				//Display using LCD
				LCD_setPosition(&lcd, 1, 0);
				LCD_sendString(&lcd, "SetPoint:");
				LCD_sendNum(&lcd,threshold);
				LCD_sendString(&lcd, " C");
				
				LCD_setPosition(&lcd, 2, 0);
				LCD_sendString(&lcd, "Measured:");
				LCD_sendNum(&lcd,ReceivedValue);
				LCD_sendString(&lcd, " C");
				
				
				// Display using UART
				sprintf (buffer, "%s %d%s %.2lf%s %.2lf%s","Temprature is:",ReceivedValue,"C",((double)ReceivedValue * 9/5)+ 32,"F", ReceivedValue + 273.15,"K");
			  UART_PrintBuffer();
				
				//Changing LED
				threshold_LED(ReceivedValue);
				xSemaphoreGive(xMutex);
				vTaskDelay(1000/portTICK_RATE_MS);
				}
		}
}

static void vADCTask( void *pvParameters ){
	uint32_t pui32ADC0Value[1];
	unsigned int ui32TempValueC;
	portBASE_TYPE xStatus;  
	for( ;; )
    {
			xSemaphoreTake( xMutex, portMAX_DELAY );
			ADCProcessorTrigger(ADC0_BASE, 3);
			while(!ADCIntStatus(ADC0_BASE, 3, false)){}
			
			ADCIntClear(ADC0_BASE, 3);
			ADCSequenceDataGet(ADC0_BASE, 3, &pui32ADC0Value);
			ui32TempValueC = (unsigned int) (ceil((( pui32ADC0Value[0] *3.3)/4096)*100)); // Convert to Celcius
			xStatus = xQueueSendToBack(xQueue,&ui32TempValueC,0);
			xSemaphoreGive(xMutex);
			vTaskDelay(1000/portTICK_RATE_MS);
    }
}

// Switch 0 and 4 handler
void SwitchHandler(void) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4); // Clear the Interrupt
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

void vApplicationIdleHook(){
	__asm("wfi\n");
}


void threshold_LED(unsigned int ReadTemperature){
	if (ReadTemperature > threshold) {
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1); // Turns Red LED and turn off Heater
	}
	else if (ReadTemperature == threshold) {
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_3); // Turns Yellow LED
	}
	else if (ReadTemperature < threshold) {
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_2); // Turns Blue LED
	}
}
	
void UART_PrintBuffer(void){
	for(int i = 0; buffer[i] != '\0'; i++){
				UARTCharPut(UART0_BASE, buffer[i]);
				}
				UARTCharPut(UART0_BASE, '\r');
				UARTCharPut(UART0_BASE, '\n');
}
