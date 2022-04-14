#include "Init.h"

void vApplicationIdleHook();
void toggle_red(void);
static void blink_red(void *pvParameters);
static void blink_blue(void *pvParameters);
static void blink_green(void *pvParameters);

int main() 
{ 
  PortFInit();
	xTaskCreate(blink_red,"Blink Red", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	xTaskCreate(blink_green,"Blink Green", configMINIMAL_STACK_SIZE,NULL , 1, NULL);
	
	vTaskStartScheduler();
}

void blink_red(void *pvParameters){
  for(;;){
  toggle_red();
  vTaskDelay(1000/ portTICK_RATE_MS);
  }
}
void blink_green(void *pvParameters){
  for(;;){
  toggle_green();
  vTaskDelay(2000/ portTICK_RATE_MS);
  }
}

void vApplicationIdleHook(){
	toggle_blue();
}
