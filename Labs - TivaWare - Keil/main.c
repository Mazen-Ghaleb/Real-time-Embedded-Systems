#include "Init.h"

void toggle_red(void);
static void blink_red(void *pvParameters);

int main() 
{ 
  PortFInit();
	xTaskCreate(blink_red,"Blink Red", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	vTaskStartScheduler();
}

void toggle_red()
{
  if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1) & (1<<1))
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
  else
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
}

void blink_red(void *pvParameters){
  for(;;){
  toggle_red();
  vTaskDelay(1000/ portTICK_RATE_MS);
  }
}
