#include "Init.h"

void INIT_TASK(void *pvParameters);
void toggle_red(void);
void toggle_blue(void);
static void vLCDTask( void *pvParameters );
static void vADCTask( void *pvParameters );
void vApplicationIdleHook(void);
void delay (int n);
void threshold_LED(unsigned int ReadTemperature);
void turn_BlueLED(void);
void turn_YellowLED(void);
void turn_RedLED(void);

//unsigned char buffer [50];
LCD lcd;

xSemaphoreHandle xMutex;
xQueueHandle xQueue;

xTaskHandle xLCDTask_Handle;
xTaskHandle xACDTask_Handle;

unsigned int threshold = 30;

int main() 
{
	xMutex = xSemaphoreCreateMutex();
	xQueue = xQueueCreate (5,sizeof(unsigned int));
	if (xQueue != NULL && xMutex!= NULL)  {
	xTaskCreate(vLCDTask,"LCD Task", configMINIMAL_STACK_SIZE,"This is the LCD Task\n" , 3, &xLCDTask_Handle);
	xTaskCreate(vADCTask,"ADC Task", configMINIMAL_STACK_SIZE,"This is the ADC Task\n" , 2, &xACDTask_Handle);
	xTaskCreate(INIT_TASK,"Intialization Task", configMINIMAL_STACK_SIZE,NULL , 4, NULL);		
	vTaskStartScheduler();
	}
	else {
	// Queue or Mutex couldn't be created 
	}
}

void INIT_TASK(void *pvParameters){
	PortAInit();
	PortFInit();
	//UART0Init();
	ADCInit();
	
	lcd = LCD_create();
	LCD_init(&lcd);
	vTaskDelete(NULL);
}

static void vLCDTask( void *pvParameters ){
		unsigned int ReceivedValue;
		portBASE_TYPE xStatus;
		for( ;; )
		{
			xStatus = xQueueReceive(xQueue,&ReceivedValue,portMAX_DELAY);
			if (xStatus == pdPASS) {
				xSemaphoreTake( xMutex, portMAX_DELAY );
				LCD_setPosition(&lcd, 1, 0);
				LCD_sendString(&lcd, "Temperature:");
				LCD_setPosition(&lcd, 2, 0);
				LCD_sendNum(&lcd,ReceivedValue);
				LCD_sendString(&lcd, " C");
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

void vApplicationIdleHook(){
	__asm("wfi\n");
}

void delay(int n ){
	for (int i = 0; i<n;i++)
			for (int j = 0; j<3180;j++);
}


void threshold_LED(unsigned int ReadTemperature){
	if (ReadTemperature > threshold) {
		turn_RedLED();
	}
	else if (ReadTemperature == threshold) {
		turn_YellowLED();
	}
	else if (ReadTemperature < threshold) {
		turn_BlueLED();
	}
}
	
void turn_BlueLED(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_2);
}

void turn_YellowLED(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_3);
}

void turn_RedLED(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1);
}
