#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

/////////////////////////////////////////////////////////// LCD PINS ///////////////////////////////////////////////////////////////////
//|  1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   10   |   11   |   12   |   13   |   14   |   15   |   16   |
//| GND  |  VCC  |  V0   |  RS   |  R/W  |   E   |   D0  |   D1  |   D2  |   D3   |   D4   |   D5   |   D6   |   D7   |   A    |   K    |

// CONTROLS PINS     RS  |  R/W  |   E
// DATA BUS PINS     D0 - D7

#define LCD_CTRL_PERIPHERAL    SYSCTL_PERIPH_GPIOB
#define LCD_CTRL_PORT          GPIO_PORTB_BASE
#define RS_PIN                 GPIO_PIN_0
#define RW_PIN                 GPIO_PIN_2
#define ENABLE_PIN             GPIO_PIN_1

#define LCD_DATA_PERIPHERAL    SYSCTL_PERIPH_GPIOB
#define LCD_DATA_PORT          GPIO_PORTB_BASE
#define LCD_D7                 GPIO_PIN_7
#define LCD_D6                 GPIO_PIN_6
#define LCD_D5                 GPIO_PIN_5
#define LCD_D4                 GPIO_PIN_4

#define DELAY_1s       16666667     // =(SysCtlClockGet()/3)*1
#define DELAY_15ms     250000       // =(SysCtlClockGet()/3000)*15
#define DELAY_12ms     200000       // =(SysCtlClockGet()/3000)*12  //for example (50000000/3000)*12=200000
#define DELAY_5ms      83334        // =(SysCtlClockGet()/3000)*5
#define DELAY_2_5ms    41666        // =(SysCtlClockGet()/3000)*2.5
#define DELAY_100us    16667        // =(SysCtlClockGet()/3000000)*100
#define DELAY_40us     667          // =(SysCtlClockGet()/3000000)*40
#define DELAY_12us     200          // =(SysCtlClockGet()/3000000)*12
#define DELAY_0_6us    10           // =(SysCtlClockGet()/3000000)*0.6


///////////////////////// Comands //////////////////////////
#define LCD_CLEAR            0x01   // Clear display
#define LCD_CURSOR_ON        0x0E   // Cursor on
#define LCD_CURSOR_OFF       0x0C   // Turn off cursor
#define LCD_BLINK_CURSOR_ON  0x0F   // Blink cursor on//Turns on the LCD display and Sets a BLINKING CURSOR
#define LCD_BLINK_CURSOR_OFF 0x0E   // Cursor No Blink
#define LCD_UNDERLINE_ON     0xF1   // Underline cursor on
#define LCD_UNDERLINE_OFF    0x0D

#define LCD_TURN_ON          0x0C   // Turn Lcd display on
#define LCD_TURN_OFF         0x08   // Turn Lcd display off
#define INCREMENT            0x07   // Entry mode increment
#define DECREMENT            0x05   // Entry mode decrement
#define SHIFT_ON             0x07   // Display shift on
#define SHIFT_OFF            0x06   // Display shift off
#define SHIFT_LEFT           0x1C   // Text shift Left
#define SHIFT_RIGHT          0x18   // Display shift Right

#define FOUR_BIT             0x28  // 4-bit Interface
#define EIGHT_BIT            0x3F  // 8-bit Interface
#define LINE_5X7             0x33  // 5x7 characters, single line
#define LINE_5X10            0x37  // 5x10 characters
#define LINES_5X7            0x3B  // 5x7 characters, multiple line

/*******************************************************************
*   TYPEDEFS
*******************************************************************/

/**
 *  Creating LCD structure that will contain all its informations
 */
typedef struct LCD {

    //Control
    uint32_t ControlPeripheral, /**< Control peripheral. */
             ControlPortBase;   /**< Port base of control peripheral. */

    uint8_t rs,                 /**< RS pin. */
            rw,                 /**< R/W pin. */
            en;                 /**< Enable pin. */

    //Data
    uint32_t DataPeripheral,    /**< Data peripheral. */
             DataPortBase;      /**< Port base of data peripheral.. */

    uint8_t data[8];            /**< Data pins. */

    unsigned int line,          /**< Variable to control cursor on lines. */
                 column;        /**< Variable to control cursor on columns. */

} LCD;

LCD LCD_create(void);

/// Function that returns all control pins
uint8_t LCD_CTRL_BUS(LCD *lcd);

/// Function that returns all data pins
uint8_t LCD_DATA_BUS(LCD *lcd);

/// Function that configures LCD peripheral and pins on Tiva TM4C123GH.
void LCD_setup(LCD *lcd);

/// @brief Function that needs to be called before all code to initialize the LCD.
void LCD_init(LCD *lcd);

/// Function that clear LCD display.
void LCD_clear(LCD *lcd);

/// Function that sends a nibble to LCD.
void LCD_sendNibble(LCD *lcd, unsigned char data);

/// Function that sends a byte to LCD.
void LCD_sendByte(LCD *lcd, bool rs, char byte);

/// Function that sends a number to LCD.
void LCD_sendNum(LCD *lcd, unsigned int num);

/// Function that sends a string to LCD.
void LCD_sendString(LCD *lcd, char* word);

/// Function that controls the display cursor.
void LCD_inc_cnt(LCD *lcd);

/// Function that sets cursor position on LCD.
void LCD_setPosition(LCD *lcd, unsigned int line, unsigned int col);

#endif
