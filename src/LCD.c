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
	temp |= 0x38333000; // PB11...PB13, PB15 output push pull; PB14 input
	GPIOB->CRH = temp;
	
	GPIOB->BSRR = LCD_SOD; //PB14 internen Pull Up aktivieren (ODR)
}
void LCD_reset()
{
	GPIOB->BSRR = LCD_RST; wait_ms(50);
	GPIOB->BRR  = LCD_RST; wait_ms(1);
	GPIOB->BSRR = LCD_RST;
}
void LCD_init()
{
	LCD_init_ports();
	LCD_reset();
	GPIOB->BRR = LCD_CS;
	GPIOB->BRR  = LCD_SID;
	GPIOB->BSRR = LCD_SCLK;
	wait_ms(1);
	GPIOB->BSRR  = LCD_CS;
	wait_ms(1);
}

void LCD_start_byte(uint8_t start_byte)
{
	GPIOB->BSRR = LCD_SID; wait_ms(1);
	LCD_write_byte(start_byte);
}

void LCD_write_byte(uint8_t data)
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


void LCD_write_data_byte(uint8_t cmd)
{
	LCD_write_byte(cmd & 0x0F);
	LCD_write_byte(cmd >> 4);
}

void LCD_write_data_bytes_0term(uint8_t* cmd)
{
	while(*cmd)
		LCD_write_data_byte(*cmd++);
}

void LCD_write_data_bytes_count(uint8_t* cmd, int count)
{
	while(count--)
		LCD_write_data_byte(*cmd++);
}


void LCD_init_normal()
{
	LCD_init();
	
	LCD_start_byte(LCD_start | LCD_instruction_write);
	uint8_t program[] =
	{
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display | LCD_RE_1 | LCD_normal_display,
		LCD_Extended_function_set | LCD_3line_or_4line_display,
		LCD_view | LCD_top,
		0x1E,
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display | LCD_RE_0 | LCD_instruction_set_1,
		LCD_Internal_OSC_frequency | LCD_BS0_1 | 0x3,
		LCD_Follower_control | LCD_set_divider_circuit_on | 0x6,
		LCD_Power_Icon_control_Contrast_set | LCD_set_booster_and_regulator_circuit_on | 0x3,
		LCD_Contrast_set | 0x2,
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display,
		LCD_Clear_display,
		LCD_Display_OnOff_control | LCD_display_on | LCD_cursor_on | LCD_blink_on,
		LCD_set_cursor_position(0, 2)
	};
	LCD_write_data_bytes_count(program, sizeof(program) / sizeof(uint8_t));

	LCD_start_byte(LCD_start | LCD_data_write);
}


void LCD_putchar(int character)
{
	switch (character)
	{
		case '\b':
			LCD_start_byte(LCD_start | LCD_instruction_write);
			LCD_write_data_byte(LCD_Cursor_or_display_shift | LCD_cursor_shift | LCD_shift_to_left);
			LCD_start_byte(LCD_start | LCD_data_write);
			break;
		
		case '\n':
			LCD_start_byte(LCD_start | LCD_instruction_write);
			/* TODO */
			LCD_start_byte(LCD_start | LCD_data_write);
			break;
		
		/* TODO */
		
		default:
			LCD_write_data_byte(character);
	}
}
void LCD_puts(char const * str)
{
	while (*str)
		LCD_putchar(*str++);
}

void LCD_printf(char const * format, ...)
{
	char c;
	
	unsigned int i;
	char* s;
	char str[32];
	
	va_list arg;
	va_start(arg, format);
	
	LCD_start_byte(LCD_start | LCD_data_write);
	
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

uint8_t LCD_set_cursor_position(uint8_t x, uint8_t y)
{
	return 0x80 + x * 0x1 + y * 0x20;
}
