#include "Init.h"
#define Systick_DELAY 100

void delayMs (unsigned int delay);
void toggle_red(void);
void toggle_blue(void);

uint32_t stack_blinkRed[40];
uint32_t *sp_blinkRed = &stack_blinkRed[40];
void blink_red(void);

uint32_t stack_blinkBlue[40];
uint32_t *sp_blinkBlue = &stack_blinkBlue[40];
void blink_blue(void);

int main(void) __attribute__((noreturn));

int main() 
{ 
  PortFInit();
  __asm("CPSID I");
  Systick_Init(Systick_DELAY);
	
		/* fabricate Cortex-M ISR stack frame for blink red */
    *(--sp_blinkRed) = (1U << 24);  						/* xPSR */
    *(--sp_blinkRed) = (uint32_t)&blink_red;		/* PC */
    *(--sp_blinkRed) = 0x0000000EU; 						/* LR  */
    *(--sp_blinkRed) = 0x0000000CU; 						/* R12 */
    *(--sp_blinkRed) = 0x00000003U; 						/* R3  */
    *(--sp_blinkRed) = 0x00000002U; 						/* R2  */
    *(--sp_blinkRed) = 0x00000001U; 						/* R1  */
    *(--sp_blinkRed) = 0x00000000U; 						/* R0  */
	
    /* fabricate Cortex-M ISR stack frame for blink blue */
    *(--sp_blinkBlue) = (1U << 24);  						/* xPSR */
    *(--sp_blinkBlue) = (uint32_t)&blink_blue;	/* PC */
    *(--sp_blinkBlue) = 0x0000000EU; 						/* LR  */
    *(--sp_blinkBlue) = 0x0000000CU; 						/* R12 */
    *(--sp_blinkBlue) = 0x00000033U; 						/* R3  */
    *(--sp_blinkBlue) = 0x00000022U; 						/* R2  */
    *(--sp_blinkBlue) = 0x00000011U; 						/* R1  */
    *(--sp_blinkBlue) = 0x00000000U; 						/* R0  */
		
  __asm("CPSIE I");
	//blink_red();
	//blink_blue();
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

