#ifndef LCD_H
#define LCD_H

#include <stm32f10x.h>

#include <stdarg.h>

void wait_ms(int t_ms);

/**
 * \brief		Initialize ports
 * 
 * The function LCD_init
 * 
 */
void LCD_init_ports();
void LCD_reset();

void LCD_start_byte(uint8_t start_byte);
void LCD_byte(uint8_t data);

void LCD_data_byte(uint8_t cmd);
void LCD_data_bytes_0term(uint8_t cmd[]);
void LCD_data_bytes_count(uint8_t cmd[], int count);

void LCD_putchar(int character);
void LCD_puts(char const * str);
void LCD_printf(char* format, ...);

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
	LCD_start     = 0x1F,
		LCD_read    = 0x1 << 5, // R/W = 1
		LCD_write   = 0x0 << 5, // R/W = 0
		LCD_RS_1    = 0x1 << 6, // RS  = 1
		LCD_RS_0    = 0x0 << 6, // RS  = 0
	
	
		LCD_instruction_write                  = LCD_RS_0 | LCD_write,
		LCD_read_busy_flag_and_address_counter = LCD_RS_0 | LCD_read,
		LCD_data_write                         = LCD_RS_1 | LCD_write,
		LCD_data_read                          = LCD_RS_1 | LCD_read,
};

enum : uint8_t                                              //         | IS | RE
{
	LCD_Clear_display                                       = 0x1 << 0, //         |  X |  X
	
	LCD_Return_home                                         = 0x1 << 1, //         |  X |  0
	
	LCD_Power_down_mode                                     = 0x1 << 1, //         |  X |  1
		LCD_power_down_mode_set                               = 0x1 << 0, // PD  = 1 |
		LCD_power_down_mode_disable                           = 0x0 << 0, // PD  = 0 |
	
	LCD_Entry_mode_set                                      = 0x1 << 2, //         |  X | 0/1
		LCD_cursor_blink_moves_to_right                       = 0x1 << 1, // I/D = 1 |
		LCD_cursor_blink_moves_to_left                        = 0x0 << 1, // I/D = 0 |
		/* TODO */

	LCD_Display_OnOff_control                               = 0x1 << 3, //         |  X |  0
		LCD_display_on                                        = 0x1 << 2, // D   = 1 |
		LCD_display_off                                       = 0x0 << 2, // D   = 0 |
		LCD_cursor_on                                         = 0x1 << 1, // C   = 1 |
		LCD_cursor_off                                        = 0x0 << 1, // C   = 0 |
		LCD_blink_on                                          = 0x1 << 0, // B   = 1 |
		LCD_blink_off                                         = 0x0 << 0, // B   = 0 |
	
	LCD_Extended_function_set                               = 0x1 << 3, //         |  X |  1
		LCD_6dot_font_width                                   = 0x1 << 2, // FW  = 1 |
		LCD_5dot_font_width                                   = 0x0 << 2, // FW  = 0 |
		LCD_black_white_inverting_of_cursor_enable            = 0x1 << 1, // B/W = 1 |
		LCD_black_white_inverting_of_cursor_disable           = 0x0 << 1, // B/W = 0 |
		LCD_3line_or_4line_display                            = 0x1 << 0, // NW  = 1 |
		LCD_1line_or_2line_display                            = 0x0 << 0, // NW  = 0 |
	
	LCD_Cursor_or_display_shift                             = 0x1 << 4, //         |  0 |  0
		LCD_display_shift                                     = 0x1 << 3, // S/C = 1 |
		LCD_cursor_shift                                      = 0x0 << 3, // S/C = 0 |
		LCD_shift_to_right                                    = 0x1 << 2, // R/L = 1 |
		LCD_shift_to_left                                     = 0x0 << 2, // R/L = 0 |

	LCD_Double_height_Bias_Displaydot_shift                 = 0x1 << 4, //         |  0 |  1
		/* TODO */

	LCD_Internal_OSC_frequency                              = 0x1 << 4, //         |  1 |  0
		LCD_BS0_1                                             = 0x1 << 3, // BS0 = 1 |
		LCD_BS0_0                                             = 0x0 << 3, // BS0 = 0 |

	LCD_Shift_enable                                        = 0x1 << 4, //         |  1 |  1
		LCD_1st_line_display_shift_enable                     = 0x1 << 0, // DS1 = 1 |
		LCD_1st_line_display_shift_disable                    = 0x0 << 0, // DS1 = 0 |
		LCD_2nd_line_display_shift_enable                     = 0x1 << 1, // DS2 = 1 |
		LCD_2nd_line_display_shift_disable                    = 0x0 << 1, // DS2 = 0 |
		LCD_3rd_line_display_shift_enable                     = 0x1 << 2, // DS3 = 1 |
		LCD_3rd_line_display_shift_disable                    = 0x0 << 2, // DS3 = 0 |
		LCD_4th_line_display_shift_enable                     = 0x1 << 3, // DS4 = 1 |
		LCD_4th_line_display_shift_disable                    = 0x0 << 3, // DS4 = 0 |
	
	LCD_Scroll_enable                                       = 0x1 << 4, //         |  1 |  1
		LCD_1st_line_dot_scroll_enable                        = 0x1 << 0, // HS1 = 1 |
		LCD_1st_line_dot_scroll_disable                       = 0x0 << 0, // HS1 = 0 |
		LCD_2nd_line_dot_scroll_enable                        = 0x1 << 1, // HS2 = 1 |
		LCD_2nd_line_dot_scroll_disable                       = 0x0 << 1, // HS2 = 0 |
		LCD_3rd_line_dot_scroll_enable                        = 0x1 << 2, // HS3 = 1 |
		LCD_3rd_line_dot_scroll_disable                       = 0x0 << 2, // HS3 = 0 |
		LCD_4th_line_dot_scroll_enable                        = 0x1 << 3, // HS4 = 1 |
		LCD_4th_line_dot_scroll_disable                       = 0x0 << 3, // HS4 = 0 |

	LCD_Function_set                                        = 0x1 << 5, //         |  X | 0/1
		LCD_8bit                                              = 0x1 << 4, // DL  = 1 |
		LCD_4bit                                              = 0x0 << 4, // DL  = 0 |
		LCD_2line_or_4line_display                            = 0x1 << 3, // N   = 1 |
		LCD_1line_or_3line_display                            = 0x0 << 3, // N   = 0 |
		
		LCD_double_height_font_control_for_2line_mode_enable  = 0x1 << 2, // DH  = 1 |
		LCD_double_height_font_control_for_2line_mode_disable = 0x1 << 2, // DH  = 0 |
		LCD_RE_0                                              = 0x0 << 1, // RE  = 0 |
		LCD_instruction_set_1                                 = 0x1 << 0, // IS  = 1 |
		LCD_instruction_set_0                                 = 0x0 << 0, // IS  = 0 |
		
		LCD_CGRAM_SERAM_blink_enable                          = 0x1 << 2, // BE  = 1 |
		LCD_CGRAM_SERAM_blink_disable                         = 0x0 << 2, // BE  = 0 |
		LCD_RE_1                                              = 0x1 << 1, // RE  = 1 |
		LCD_reverse_display                                   = 0x1 << 0, // REV = 1 |
		LCD_normal_display                                    = 0x0 << 0, // REV = 0 |
	
	LCD_set_CGRAM_address                                   = 0x1 << 6, //         |  0 |  0
	
	LCD_set_SEGRAM_address                                  = 0x1 << 6, //         |  1 |  0
	
	LCD_Power_Icon_control_Contrast_set                     = 0x5 << 4, //         |  1 |  0
		LCD_ICON_display_on                                   = 0x1 << 3, // Ion = 1 |
		LCD_ICON_display_off                                  = 0x0 << 3, // Ion = 0 |
		LCD_set_booster_and_regulator_circuit_on              = 0x1 << 2, // Bon = 1 |
		LCD_set_booster_and_regulator_circuit_off             = 0x0 << 2, // Bon = 0 |
	
	LCD_Follower_control                                    = 0x3 << 5, //         |  1 |  0
		LCD_set_divider_circuit_on                            = 0x1 << 3, // Don = 1 |
		LCD_set_divider_circuit_off                           = 0x0 << 3, // Don = 0 |
		
	LCD_Contrast_set                                        = 0x7 << 4, //         |  1 |  0
	
	LCD_set_DDRAM_address                                   = 0x1 << 7, //         |  X |  0
	
	LCD_set_scroll_quantity                                 = 0x1 << 7, //         |  X |  1
	
	/*LCD_Read_busy_flag_and_address_part_ID                  = 0x1 << 8, //         |  X |  X
		LCD_busy_state                                        = 0x1 << 7, // BF  = 1 |
		LCD_ready_state                                       = 0x0 << 7, // BF  = 0 |
	*/
//LCD_write_data
	
//LCD_read_data
	
	
	// Extended Instruction Set:
	
	//LCD_Temperature_Coefficient_Control
	
	//LCD_Temperature_Coefficient_Control_Settings
	
	//LCD_ROM_Selection
	
	//LCD_ROM_Selection_Settings
	
	
	
	
	LCD_view                                                = 0x1 << 2,
		LCD_bottom                                            = 0x1 << 1,
		LCD_top                                               = 0x1 << 0,
};

#endif // LCD_H
