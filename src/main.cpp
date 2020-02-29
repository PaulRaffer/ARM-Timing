#include "cpp/LCD.hpp"

/**
 * \brief		Main-function
 * 
 * 
 * 
 * \return	Error-Code
 * \retval	0		The function is successfully executed
 */
 /*
auto main() -> int
{
	using namespace LCD;
	
	auto display = Display<>{};
	
		display
		.init()
		(LCD_execute,
			0x3A,
			0x09,
			view | bottom,
			0x1E,
			0x39,
			0x1B,
			follower_control | follower_circuit_on | 0x6,
			0x57,
			0x72,
			0x38)
		.clear()
		.on() .cursor() .blink()
		<< "Hallo Welt!";
}*/

int main()
{
	LCD_init_ports();
	LCD_reset();
	GPIOB->BSRR = LCD_CS;
	GPIOB->BSRR = LCD_SCLK; wait_ms(1);
	
	SPI_start(LCD_execute);
	uint8_t program[] =
	{
		0x3A,
		0x09,
		LCD_view | LCD_bottom,
		0x1E,
		0x39,
		0x1B,
		LCD_follower_control | LCD_follower_circuit_on | 0x6,
		0x57,
		0x72,
		0x38,
		LCD_clear_display,
		LCD_display | LCD_display_on | LCD_cursor_on | LCD_blink_on,
	};
	LCD_data_count(program, sizeof(program) / sizeof(uint8_t));
	
	SPI_start(LCD_output);
	LCD_data_0term((uint8_t*)"Hallo Welt!\n");
}
