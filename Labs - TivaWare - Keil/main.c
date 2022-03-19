#include "Init.h"

void toggle_red(void);
void toggle_blue(void);
static void blink_red(void *pvParameters);
static void blink_blue(void *pvParameters);

int main() 
{ 
  PortFInit();
	xTaskCreate(blink_red,"Blink Red", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	xTaskCreate(blink_blue,"Blink Blue", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	vTaskStartScheduler();
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

void blink_red(void *pvParameters){
  for(;;){
  toggle_red();
  vTaskDelay(1000);
  }
}

void blink_blue(void *pvParameters){
  for(;;){
  toggle_blue();
  vTaskDelay(1000);
  }
}
