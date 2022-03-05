#include "types.h"
#include <time.h>
#include "stdint.h"
#include "stdbool.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "tm4c123gh6pm.h"
#include <driverlib/systick.h>

uint32 CalcTicks(uint32 milliseconds);
void PortFInit(void);
void Systick_Init(uint32 delayMs);
void Systick_Handler(void);

extern unsigned int tickcounter;
