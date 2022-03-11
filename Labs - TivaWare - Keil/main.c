#include "Init.h"
#define Systick_DELAY 100

void delayMs (unsigned int delay);
void toggle_red(void);
void toggle_blue(void);
void blink_red(void);
void blink_blue(void);
int main(void) __attribute__((noreturn));

int main() 
{ 
  PortFInit();
  __asm("CPSID I");
  Systick_Init(Systick_DELAY);
  __asm("CPSIE I");
	blink_red();
	blink_blue();
  while(1){}
}

void delayMs (unsigned int delay){
unsigned int start = tickcounter;
while (tickcounter - start < (delay/Systick_DELAY)) {}
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

