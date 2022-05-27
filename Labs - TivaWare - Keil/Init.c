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
  HWREG(GPIO_PORTF_BASE+GPIO_O_CR) = GPIO_PIN_0;
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); 
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); 
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);    
}
void PortAInit(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}
	GPIOPinTypeUART(GPIO_PORTA_BASE , GPIO_PIN_0 | GPIO_PIN_1);
}

void PortEInit(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}
	//HWREG(GPIO_PORTE_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
  //HWREG(GPIO_PORTE_BASE+GPIO_O_CR) = GPIO_PIN_3;
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	//GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
}

void ADCInit(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}
	PortEInit();
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);
	//ADCProcessorTrigger(ADC0_BASE, 3);
  //while(!ADCIntStatus(ADC0_BASE, 3, false)){}
}

void UART0Init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
	UARTDisable(UART0_BASE);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
	(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	UART_CONFIG_PAR_NONE));
	UARTEnable(UART0_BASE);
}
void Systick_Init(uint32 delayMs) {
  SysTickDisable();
  SysTickIntDisable();
	SysTickIntRegister(Systick_Handler);
  SysTickPeriodSet(CalcTicks(delayMs));
  SysTickIntEnable();
  SysTickEnable();
}
