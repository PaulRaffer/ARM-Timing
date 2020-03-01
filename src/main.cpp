#include "cpp/LCD.hpp"

/**
 * \brief		Main-function
 * 
 * 
 * 
 * \return	Error-Code
 * \retval	0		The function is successfully executed
 */
/*auto main() -> int
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
	
	LCD_start_byte(LCD_start | LCD_instruction_write);
	uint8_t program[] =
	{
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display | LCD_RE_1 | LCD_normal_display,
		LCD_Extended_function_set | LCD_3line_or_4line_display,
		LCD_view | LCD_bottom,
		0x1E,
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display | LCD_RE_0 | LCD_instruction_set_1,
		LCD_Internal_OSC_frequency | LCD_BS0_1 | 0x3,
		LCD_Follower_control | LCD_set_divider_circuit_on | 0x6,
		LCD_Power_Icon_control_Contrast_set | LCD_set_booster_and_regulator_circuit_on | 0x3,
		LCD_Contrast_set | 0x2,
		LCD_Function_set | LCD_8bit | LCD_2line_or_4line_display,
		LCD_Clear_display,
		LCD_Display_OnOff_control | LCD_display_on | LCD_cursor_on | LCD_blink_on,
	};
	LCD_data_bytes_count(program, sizeof(program) / sizeof(uint8_t));
	
	LCD_printf("%c %d %i %o %s%x\b", 111, 111, 111, 111, "Hex:", 111);
}

