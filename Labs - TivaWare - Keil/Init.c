#include "Init.h"

uint32 CalcTicks(uint32 milliseconds){
return ((milliseconds) * (16 * 1000))-1 ;
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
	GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE , GPIO_PIN_0 | GPIO_PIN_1);
}

void PortEInit(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
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

void SwitchInterruptInit(void){
  GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);        // Sandwich
  GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);          // Clear any existing interrupts
  NVIC_PRI7_R = 6 << 21;
	//IntPrioritySet(INT_GPIOF , 0xe0);                             // Set priority to 7
  GPIOIntRegister(GPIO_PORTF_BASE,SwitchHandler);               // Register the interrupt handler
  GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_FALLING_EDGE); // Set the switches to trigger once pressed (since the switches are configured as Pull-down)
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);        // Sandwich
}
