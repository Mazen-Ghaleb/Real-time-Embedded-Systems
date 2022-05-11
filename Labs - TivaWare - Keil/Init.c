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

void TimerInit0 (void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);         // Enable timer 0
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){} // Wait for it to be ready
  TimerDisable(TIMER0_BASE, TIMER_A);                   // Disable it until it is enabled later on
  
  TimerIntDisable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);      // Sandwich
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_PERIODIC));    // Make it a periodic timer
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);        // Clear any existing interrupts
  IntPrioritySet(INT_TIMER0A , 0xe0);                   // Set priority to 2
  TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0Handler);  // Register the interrupt handler
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);       // Sandwich
}

void TimerInit1 (void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);         // Enable timer 1
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)){} // Wait for it to be ready
  TimerDisable(TIMER1_BASE, TIMER_A);                   // Disable it until it is enabled later on
  
  TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);      // Sandwich
  TimerConfigure(TIMER1_BASE, (TIMER_CFG_PERIODIC));    // Make it a periodic timer
  TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);        // Clear any existing interrupts
  IntPrioritySet(INT_TIMER1A , 0xe0);                   // Set priority to 1
  TimerIntRegister(TIMER1_BASE,TIMER_A,Timer1Handler);  // Register the interrupt handler
  TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);       // Sandwich
}


