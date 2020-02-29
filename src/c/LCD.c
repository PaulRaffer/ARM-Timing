#include "LCD.h"

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

void SPI_start(uint8_t start_byte)
{
	GPIOB->BSRR = LCD_SID; wait_ms(1);
	SPI_write(start_byte);
}

void SPI_write(uint8_t data)
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

void LCD_data(uint8_t cmd)
{
	SPI_write(cmd & 0x0F);
	SPI_write(cmd >> 4);
}

void LCD_data_0term(uint8_t* cmd)
{
	while(*cmd)
		LCD_data(*cmd++);
}

void LCD_data_count(uint8_t* cmd, int count)
{
	while(count--)
		LCD_data(*cmd++);
}
