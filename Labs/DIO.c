#include "DIO.h"

void DIO_Init(int8 port, int8 pin, int8 dir, int8 isPUR) {
    // intialize as GPIO
    Set_Bit(SYSCTL_RCGCGPIO_R, port)
        while (Get_Bit(SYSCTL_PRGPIO_R, port) == 0) {};
    switch (port)
    {
    case PORTA:
        GPIO_PORTA_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTA_CR_R, pin)
            if (dir) {
                Set_Bit(GPIO_PORTA_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTA_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTA_PDR_R, pin)
                            Set_Bit(GPIO_PORTA_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTA_PUR_R, pin)
                            Set_Bit(GPIO_PORTA_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTA_DEN_R, pin)
            break;
    case PORTB:
        GPIO_PORTB_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTB_CR_R, pin)
            if (dir) {
                Set_Bit(GPIO_PORTB_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTB_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTB_PDR_R, pin)
                            Set_Bit(GPIO_PORTB_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTB_PUR_R, pin)
                            Set_Bit(GPIO_PORTB_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTB_DEN_R, pin)
            break;
    case PORTC:
        GPIO_PORTC_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTC_CR_R, pin)
            if (dir) {
                Set_Bit(GPIO_PORTC_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTC_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTC_PDR_R, pin)
                            Set_Bit(GPIO_PORTC_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTC_PUR_R, pin)
                            Set_Bit(GPIO_PORTC_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTC_DEN_R, pin)
            break;
    case PORTD:
        GPIO_PORTD_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTD_CR_R, pin)
            if (dir) {
                Set_Bit(GPIO_PORTD_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTD_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTD_PDR_R, pin)
                            Set_Bit(GPIO_PORTD_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTD_PUR_R, pin)
                            Set_Bit(GPIO_PORTD_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTD_DEN_R, pin)
            break;
    case PORTE:
        GPIO_PORTE_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTE_CR_R, pin)
            if (dir) {
                Set_Bit(GPIO_PORTE_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTE_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTE_PDR_R, pin)
                            Set_Bit(GPIO_PORTE_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTE_PUR_R, pin)
                            Set_Bit(GPIO_PORTE_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTE_DEN_R, pin)
            break;
    case PORTF:
        GPIO_PORTF_LOCK_R = PORT_LOCK_KEY;
        Set_Bit(GPIO_PORTF_CR_R, pin)
            //GPIO_PORTF_PUR_R = 0x11;
            if (dir) {
                Set_Bit(GPIO_PORTF_DIR_R, pin)
            }
            else {
                Clear_Bit(GPIO_PORTF_DIR_R, pin)
                    if (isPUR) {
                        Clear_Bit(GPIO_PORTF_PDR_R, pin)
                            Set_Bit(GPIO_PORTF_PUR_R, pin)
                    }
                    else {
                        Clear_Bit(GPIO_PORTF_PUR_R, pin)
                            Set_Bit(GPIO_PORTF_PDR_R, pin)
                    }
            }
        Set_Bit(GPIO_PORTF_DEN_R, pin)
            break;
    }
}

void DIO_WritePin(int8 port, int8 pin, int8 val) {
    switch (port)
    {
    case PORTA:
        if (val)
            Set_Bit(GPIO_PORTA_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTA_DATA_R, pin)
            break;
    case PORTB:
        if (val)
            Set_Bit(GPIO_PORTB_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTB_DATA_R, pin)
            break;
    case PORTC:
        if (val)
            Set_Bit(GPIO_PORTC_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTC_DATA_R, pin)
            break;
    case PORTD:
        if (val)
            Set_Bit(GPIO_PORTD_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTD_DATA_R, pin)
            break;
    case PORTE:
        if (val)
            Set_Bit(GPIO_PORTE_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTE_DATA_R, pin)
            break;
    case PORTF:
        if (val)
            Set_Bit(GPIO_PORTF_DATA_R, pin)
        else
            Clear_Bit(GPIO_PORTF_DATA_R, pin)
            break;
    }
}
void DIO_WritePort(int8 port, int8 val) {
    switch (port)
    {
    case PORTA:
        GPIO_PORTA_DATA_R = val;
        break;
    case PORTB:
        GPIO_PORTB_DATA_R = val;
        break;
    case PORTC:
        GPIO_PORTC_DATA_R = val;
        break;
    case PORTD:
        GPIO_PORTD_DATA_R = val;
        break;
    case PORTE:
        GPIO_PORTE_DATA_R = val;
        break;
    case PORTF:
        GPIO_PORTF_DATA_R = val;
        break;
    }
    return;
}

int8 DIO_ReadPin(int8 port, int8 pin) {
    switch (port)
    {
    case PORTA:
        return Get_Bit(GPIO_PORTA_DATA_R, pin);
    case PORTB:
        return Get_Bit(GPIO_PORTB_DATA_R, pin);
    case PORTC:
        return Get_Bit(GPIO_PORTC_DATA_R, pin);
    case PORTD:
        return Get_Bit(GPIO_PORTD_DATA_R, pin);
    case PORTE:
        return Get_Bit(GPIO_PORTE_DATA_R, pin);
    case PORTF:
        return Get_Bit(GPIO_PORTF_DATA_R, pin);
    }
}

int8 DIO_ReadPort(int8 port) {
    switch (port)
    {
    case PORTA:
        return GPIO_PORTA_DATA_R;
    case PORTB:
        return GPIO_PORTB_DATA_R;
    case PORTC:
        return GPIO_PORTC_DATA_R;
    case PORTD:
        return GPIO_PORTD_DATA_R;
    case PORTE:
        return GPIO_PORTE_DATA_R;
    case PORTF:
        return GPIO_PORTF_DATA_R;
    }
}

void DIO_TogglePin(int8 port, int8 pin) {
    switch (port)
    {
    case PORTA:
        Toggle_Bit(GPIO_PORTA_DATA_R, pin)
            break;
    case PORTB:
        Toggle_Bit(GPIO_PORTB_DATA_R, pin)
            break;
    case PORTC:
        Toggle_Bit(GPIO_PORTC_DATA_R, pin)
            break;
    case PORTD:
        Toggle_Bit(GPIO_PORTD_DATA_R, pin)
            break;
    case PORTE:
        Toggle_Bit(GPIO_PORTE_DATA_R, pin)
            break;
    case PORTF:
        Toggle_Bit(GPIO_PORTF_DATA_R, pin)
            break;
    }
    return;
}

void delay(float64 numOfSec)
{
    float64 numOfMilliSec = 100 * numOfSec;
    time_t startTime = clock();
    while (clock() < startTime + numOfMilliSec);
    return;
}

void delayUs(int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3; j++)
        {
        } /* do nothing for 1 us */
}
