
uint32 CalcTicks(uint32 milliseconds){
return ((milliseconds) * (16 * 1000))-1 ;
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

void Systick_Init(int delayMs) {
  SysTickDisable();
  SysTickIntDisable();
  SysTickPeriodSet(CalcTicks(delayMs));
  SysTickIntEnable();
  SysTickEnable();
}