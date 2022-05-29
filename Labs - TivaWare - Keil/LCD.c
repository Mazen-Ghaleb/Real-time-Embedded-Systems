#include "LCD.h"

LCD LCD_create(void)
{
    LCD lcd;
    lcd.ControlPeripheral = LCD_CTRL_PERIPHERAL;
    lcd.ControlPortBase = LCD_CTRL_PORT;
    lcd.en = ENABLE_PIN;
    lcd.rs = RS_PIN;
    lcd.rw = RW_PIN;

    lcd.DataPeripheral = LCD_DATA_PERIPHERAL;
    lcd.DataPortBase = LCD_DATA_PORT;

    unsigned int i;
    for(i = 0; i < 8; i++)
        lcd.data[i] = 0;

    lcd.data[7] = LCD_D7;
    lcd.data[6] = LCD_D6;
    lcd.data[5] = LCD_D5;
    lcd.data[4] = LCD_D4;

    lcd.column = 0;
    lcd.line = 1;

    return lcd;
}

uint8_t LCD_CTRL_BUS(LCD *lcd)
{
    return (lcd->en | lcd->rs | lcd->rw );
}

 uint8_t LCD_DATA_BUS(LCD *lcd)
{
    uint8_t pins = 0;
    unsigned int i;
    for(i = 4; i < 8; i++)  //Using two nibbles to comunication
        pins = pins | lcd->data[i];
    return pins;
}

void LCD_setup(LCD *lcd)
{
    //Enable CTRL Peripheral
    SysCtlPeripheralEnable(lcd->ControlPeripheral);
    GPIOPinTypeGPIOOutput(lcd->ControlPortBase, LCD_CTRL_BUS(lcd));

    //Enable DATA Peripheral
    SysCtlPeripheralEnable(lcd->DataPeripheral);
    GPIOPinTypeGPIOOutput(lcd->DataPortBase, LCD_DATA_BUS(lcd));
}

void LCD_init(LCD *lcd)
{
            LCD_setup(lcd);
            //Set control pins to low level
            GPIOPinWrite(LCD_CTRL_PORT, LCD_CTRL_BUS(lcd), 0x00);
            //Ensures that pins RS, RW and ENABLE will be in 0 (low)
            //Set data pins to low level
            GPIOPinWrite(LCD_DATA_PORT, LCD_DATA_BUS(lcd), 0x00);
            //Ensures that data bus will be in 0 (low)
            SysCtlDelay(DELAY_1s/2);            //Time to stabilize the LCD

            // Commands sequence to initialize and configure the LCD
            LCD_sendNibble(lcd, 0x03);          // Send first command to initialize the display
            SysCtlDelay(DELAY_5ms);             // Wait 5ms to stabilize the LCD
            LCD_sendNibble(lcd, 0x03);          // Send second command to initialize the display
            SysCtlDelay(DELAY_5ms);             // Wait 5ms to stabilize the LCD
            LCD_sendNibble(lcd, 0x03);          // Send third command to initialize the display
            SysCtlDelay(DELAY_5ms);             // Wait 5ms to stabilize the LCD
            LCD_sendNibble(lcd, 0x02);          // CURSOR HOME - Send commando to go to initial position (0x80)
            SysCtlDelay(DELAY_5ms);             // Wait 5ms to stabilize the LCD
            LCD_sendByte(lcd, 0, FOUR_BIT);     // FUNCTION SET - Configures the LCD for 4 bits and 2 lines, and font size 5X7.
            LCD_clear(lcd);                     // Clear the display
            LCD_sendByte(lcd, 0, LCD_CURSOR_ON);  // DISPLAY CONTROL - Turn on display without cursor
            LCD_sendByte(lcd, 0, SHIFT_OFF);    // ENTRY MODE SET - The cursor will move to right when a new character is being write on display
}


void LCD_clear(LCD *lcd) //Clear the display
{
    LCD_sendByte(lcd, 0, LCD_CLEAR);
    SysCtlDelay(DELAY_5ms);
}


void LCD_sendNibble(LCD *lcd, unsigned char data)
{
	int i;
    for (i = 3; i >= 0; i--)
    {
 	   if (data & (1 << i))
       {
        	GPIOPinWrite(lcd->DataPortBase, lcd->data[4+i], lcd->data[4+i]); //Bit equals 1
       }
       else
       {
        	GPIOPinWrite(lcd->DataPortBase, lcd->data[4+i], 0); //Bit equals 0
       }
    }
    GPIOPinWrite(LCD_CTRL_PORT, lcd->en, lcd->en); //Enable = 1
    SysCtlDelay(50);
    GPIOPinWrite(LCD_CTRL_PORT, lcd->en, 0); //Enable = 0
    SysCtlDelay(500);
}


void LCD_sendByte(LCD *lcd, bool rs, char byte)
{
    GPIOPinWrite(LCD_CTRL_PORT, lcd->en, 0); //Ensure that enable is in low level

    if(rs)
    {
        LCD_inc_cnt(lcd);
        GPIOPinWrite(LCD_CTRL_PORT, lcd->rs, lcd->rs); //RS = 1 Data
    }
    else
    {
        GPIOPinWrite(LCD_CTRL_PORT, lcd->rs, 0); //RS = 0 Intruction
    }

    LCD_sendNibble(lcd, byte >> 4); // Send HIGH part of data/command
    LCD_sendNibble(lcd, (byte & 0x0f));  // Clean the HIGH part and send the LOW part of data/command
}


void LCD_sendString(LCD *lcd, char* word)
{
    unsigned int i, len = strlen(word);

    for(i = 0; i < len; i++)
    {
        LCD_sendByte(lcd, 1, word[i]);
    }
}

/**
 * Counter Increment.
 * This function controls the LCD cursor.
 * @param lcd Pointer to a lcd structure.
 * @see LCD_create()
 */
void LCD_inc_cnt(LCD *lcd)
{
    if(++(lcd->column) > 16)
    {
        lcd->column = 1; //Counting the new character

        if(lcd->line == 1)
        {
            LCD_sendByte(lcd, 0, 0xC0); //jump for second line
            lcd->line = 2;
        }
        else if(lcd->line == 2)
        {
            LCD_sendByte(lcd, 0, 0x80); //jump for first line
            lcd->line = 1;
        }
    }
}

void LCD_setPosition(LCD *lcd, unsigned int line, unsigned int col) //Defines the position of cursor
{
    //The first position begins with value 1
    if(line == 1)
    {
        line = 0x80;     //If 1, initial address is 0x80
        lcd->line = 1;
    }
    else if(line == 2)
    {
        line = 0xC0;     //If 2, initial address is 0xC0
        lcd->line = 2;
    }

    lcd->column = col; 

    line = line + lcd->column;     //add to initial address the number of column


    LCD_sendByte(lcd, 0, (char)line);

    return;
}

void LCD_sendNum(LCD *lcd, unsigned int num)
{
		int count=(num==0)?1:log10(num)+1;  
    int number = pow(10,count-1);
    int r, q, i;

    for(i=0;i<count;i++)
    {
        q = num / number;
        r = num % number;
        LCD_sendByte(lcd, 1, (char)(q+48));
        number /= 10;
        num = r;
    }
}
