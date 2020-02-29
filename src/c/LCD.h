#ifndef LCD_H
#define LCD_H

#include <stm32f10x.h>

void wait_ms(int t_ms);

/**
 * \brief		Initialize ports
 * 
 * The function LCD_init
 * 
 */
void LCD_init_ports();
void LCD_reset();

void SPI_start(uint8_t start_byte);
void SPI_write(uint8_t data);

void LCD_data(uint8_t cmd);
void LCD_data_0term(uint8_t cmd[]);
void LCD_data_count(uint8_t cmd[], int count);



enum
{
	LCD_RST  = 0x1 << 11,
	LCD_CS   = 0x1 << 12,
	LCD_SCLK = 0x1 << 13,
	LCD_SOD  = 0x1 << 14,
	LCD_SID  = 0x1 << 15,
};

enum : uint8_t
{
	LCD_execute = 0x1F, // R/W = 0; RS = 0
	LCD_output  = 0x5F, // R/W = 0; RS = 1
};

enum : uint8_t
{
	LCD_clear_display					= 0x1 << 0,
	
	LCD_return_home						= 0x1 << 1,

	LCD_view									= 0x1 << 2,
		LCD_bottom							= 0x1 << 1,
		LCD_top									= 0x1 << 0,
	
	LCD_display								= 0x1 << 3,
		LCD_display_on					= 0x1 << 2,
		LCD_cursor_on						= 0x1 << 1,
		LCD_blink_on						= 0x1 << 0,
	
	LCD_follower_control			= 0x3 << 5,
		LCD_follower_circuit_on	= 0x1 << 3,
};

#endif // LCD_H
