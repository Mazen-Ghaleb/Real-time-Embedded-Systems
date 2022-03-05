#include "Init.h"
#define Systick_DELAY 100
unsigned int tickcounter = 0;

void delayMs (int delay) {
int start = tickcounter;
while (start < tickcounter + int(delay/ Systick_DELAY)) {}
}

//void toggle_red()
//{
//  if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1) & (1<<1))
//    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
//  else
//    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
//}

void toggle_red(void){  
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1));
}

void toggle_blue(void){  
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2));
}

void blink_red(void){
  while(1){
  toggle_red();
  delayMs(500);
  }
}

void blink_blue(void){
  while(1){
  toggle_blue();
  delayMs(1000);
  }
}

void main()
{ 
  PortFInit()
  __asm("CPSID I");
  Systick_Init(Systick_DELAY);
  __asm("CPSIE I");
  blink_red();

  while(1){}
}

  void Systick_Handler(void) {
    tickcounter++;
  }
