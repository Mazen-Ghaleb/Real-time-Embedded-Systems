#define PART_TM4C123GH6PM
#include "types.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <math.h>
#include "stdbool.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include <driverlib/systick.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

uint32 CalcTicks(uint32 milliseconds);
void PortAInit(void);
void PortFInit(void);
void UART0Init(void);
void TimerInit0 (void);
void TimerInit1 (void);
void SwitchInterruptInit(void);
void SwitchHandler(void);
void Timer0Handler(void);
void Timer1Handler(void);
void Systick_Init(uint32 delayMs);
void Systick_Handler(void);


extern unsigned int tickcounter;
