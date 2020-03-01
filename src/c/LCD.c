#include "LCD.h"

#include <stdio.h>

void wait_ms(int t_ms)
{
	for(int i = 0; i < t_ms; i++)
		for(int j = 0; j < 1595; j++);
}

void LCD_init_ports()
{
	int temp;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	temp = GPIOB->CRH;
	temp &= 0x00000FFF;
	temp |= 0x33333000; // PB11...PB15 output push pull
	GPIOB->CRH = temp;
}
void LCD_reset()
{
	GPIOB->BSRR = LCD_RST; wait_ms(50);
	GPIOB->BRR  = LCD_RST; wait_ms(1);
	GPIOB->BSRR = LCD_RST;
}

void LCD_start_byte(uint8_t start_byte)
{
	GPIOB->BSRR = LCD_SID; wait_ms(1);
	LCD_byte(start_byte);
}

void LCD_byte(uint8_t data)
{
	for (uint8_t bit = 1; bit; bit <<= 1)
	{
		GPIOB->BRR  = LCD_SCLK;
		if (data & bit)
			GPIOB->BSRR = LCD_SID;
		else
			GPIOB->BRR  = LCD_SID;
		wait_ms(1);
		GPIOB->BSRR = LCD_SCLK;
		wait_ms(1);
	}
}

void LCD_data_byte(uint8_t cmd)
{
	LCD_byte(cmd & 0x0F);
	LCD_byte(cmd >> 4);
}

void LCD_data_bytes_0term(uint8_t* cmd)
{
	while(*cmd)
		LCD_data_byte(*cmd++);
}

void LCD_data_bytes_count(uint8_t* cmd, int count)
{
	while(count--)
		LCD_data_byte(*cmd++);
}


void LCD_putchar(int character)
{
	switch (character)
	{
		case '\b':
			LCD_start_byte(LCD_execute);
			LCD_data_byte(LCD_Cursor_or_display_shift | LCD_cursor_shift | LCD_shift_to_left);
			LCD_start_byte(LCD_output);
			break;
		
		case '\n':
			LCD_start_byte(LCD_execute);
			/* TODO */
			LCD_start_byte(LCD_output);
			break;
		
		case '\r':
			LCD_start_byte(LCD_execute);
			/* TODO */
			LCD_start_byte(LCD_output);
			break;
		
		case '\t':
			LCD_start_byte(LCD_execute);
			/* TODO */
			LCD_start_byte(LCD_output);
			break;
		
		case '\v':
			LCD_start_byte(LCD_execute);
			/* TODO */
			LCD_start_byte(LCD_output);
			break;
		
		/*case '\0':
			break;*/
		
		default:
			LCD_data_byte(character);
	}
}
void LCD_puts(char const * str)
{
	while (*str)
		LCD_putchar(*str++);
}

void LCD_printf(char* format, ...)
{
	char c;
	
	unsigned int i;
	char* s;
	char str[32];
	
	va_list arg;
	va_start(arg, format);
	
	while (c = *format++)
	{
		if (c != '%')
			LCD_putchar(c);
		else
		{
			c = *format++;
		
			switch (c)
			{
				case 'c':
					i = va_arg(arg, int);
					LCD_putchar(i);
					break;
				
				case 'd':
				case 'i':
					i = va_arg(arg, int);
					if (i < 0)
					{
						i = -i;
						LCD_putchar('-');
					}
					sprintf(str, "%d", i);
					LCD_puts(str);
					break;
					
				case 'o':
					i = va_arg(arg, unsigned int);
					sprintf(str, "%o", i);
					LCD_puts(str);
					break;
				
				case 's':
					s = va_arg(arg, char*);
					LCD_puts(s);
					break;
				
				case 'x':
					i = va_arg(arg, unsigned int);
					sprintf(str, "%x", i);
					LCD_puts(str);
			}
		}
	}
}
