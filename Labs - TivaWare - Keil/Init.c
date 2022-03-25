#include "Init.h"

unsigned int tickcounter	= 0U;

uint32 CalcTicks(uint32 milliseconds){
return ((milliseconds) * (16 * 1000))-1 ;
}	

void Systick_Handler(void) {
    tickcounter++;
  }

void PortFInit(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
  HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE+GPIO_O_CR) = GPIO_PIN_0 | GPIO_PIN_4;
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void Systick_Init(uint32 delayMs) {
  SysTickDisable();
  SysTickIntDisable();
	SysTickIntRegister(Systick_Handler);
  SysTickPeriodSet(CalcTicks(delayMs));
  SysTickIntEnable();
  SysTickEnable();
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
void toggle_green()
{
  if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_3) & (1<<3))
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);
  else
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3);
}
