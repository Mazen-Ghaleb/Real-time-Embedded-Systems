#pragma once
#include "types.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include <time.h>

// Definitions for ports
#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5

#define PORT_LOCK_KEY 0x4C4F434B

// Definitions may needed for PUR
#define LOW 0
#define HIGH 1

// Calls delay but gives MS instead of second
#define delayMs(numofMilliSec) delay(numofMilliSec/1000)

// Functions prototypes
void DIO_Init(int8 port , int8 pin , int8 dir , int8 isPUR);
void DIO_WritePin(int8 port , int8 pin , int8 val );
void DIO_WritePort(int8 port , int8 val );
int8 DIO_ReadPin(int8 port , int8 pin);
int8 DIO_ReadPort(int8 port);
void DIO_TogglePin(int8 port , int8 pin);
void delay(float64 numOfSec);
void delayUs(int n);

// default call without declaring PUR
#define DIO_Init_N(PORT,PIN,DIR) DIO_Init(PORT,PIN,DIR,1)
