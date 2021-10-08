/**
 * \file
 */

#ifndef LCD_H
#define LCD_H

#include <stm32f10x.h>

#include <stdarg.h>

/**
 * \brief		Wartet fuer t_ms ms
 * 
 * Die Funktion wait_ms haelt das Programm fuer t_ms ms an.
 */
void wait_ms(int t_ms);

/**
 * \brief		Initialisiert Portleitungen
 * 
 * Die Funktion LCD_init_ports initialisiert die fuenf Portleitungen die zum
 * kommunizieren mit der LCD-Anzeige notwendeg sind. (RST, CS, SCLK, SOD, SID)
 */
void LCD_init_ports();

/**
 * \brief		Reset LCD-Display
 * 
 * Fuehrt einen Reset der LCD-Anzeige aus. (Reset-Leitung fuer 1ms auf low setzen)
 */
void LCD_reset();

/**
 * \brief		Initialisiert LCD-Display
 * 
 * Die Funktion LCD_init initialisiert die Portleitungen, fuehrt einen Reset aus
 * und initialisiert die restlichen Portleitungen mit einem Defaultwert.
 */
void LCD_init();

/**
 * \brief			Sendet ein Startbyte zur Anzeige
 * 
 * Die Funktion LCD_start_byte unterbricht normalen Datenverkehr mit einer
 * Regelverletzung (SID wird gesetzt wenn Clock = 1) und sendet dann ein Byte
 * mit der Funktion LCD_write_byte.
 * 
 * \param[in]	start_byte	Byte dass nach der Regelverletzung uebertragen wird.
 *												Dies ist folgendermassen aufgebaut: 0 RS R/W 1 1 1 1 1
 */
void LCD_start_byte(uint8_t start_byte);

/**
 * \brief									Sendet ein Byte zur Anzeige
 * 
 * Die Funktion LCD_write_byte sendet ein Byte zur LCD-Anzeige (LSB zuerst).
 * 
 * \param[in]	data				Byte dass uebertragen wird
 */
void LCD_write_byte(uint8_t data);

/**
 * \brief									Sendet ein Datenbyte zur Anzeige
 * 
 * Die Funktion LCD_write_data_byte bringt zurest das Das Datenbyte in die gewuenschte
 * Form (D7 D6 D5 D4 D3 D2 D1 D0 => 0 0 0 0 D3 D2 D1 D0   0 0 0 0 D7 D6 D5 D4) und
 * sendet die zwei entstandenen Bytes mithilfe der Funktion LCD_write_byte an die Anzeige.
 * 
 * \param[in]	cmd				Datenbyte das uebertragen wird
 */
void LCD_write_data_byte(uint8_t cmd);

/**
 * \brief									Ruft die Funktion LCD_write_data_byte fuer jedes
													Element im Nullterminierten Array cmd auf.
 * 
 * Die Funktion LCD_write_data_byte_0term ruft die Funktion LCD_write_data_byte fuer jedes
 * Element im Nullterminierten Array cmd auf.
 * 
 * \param[in]	cmd					Datenbytes die uebertragen werden
 */
void LCD_write_data_bytes_0term(uint8_t cmd[]);

/**
 * \brief									Ruft die Funktion LCD_write_data_byte fuer die ersten
 *												count Elemente im Array cmd auf.
 * 
 * Die Funktion LCD_write_data_byte_count ruft die Funktion LCD_write_data_byte fuer die ersten count Elemente im Array cmd auf.
 * 
 * \param[in]	cmd					Datenbytes die uebertragen werden
 * \param[in] count				Anzahl der Elemente die uebertagen werden
 */
void LCD_write_data_bytes_count(uint8_t cmd[], int count);


void LCD_init_normal();


/**
 * \brief									Gibt ein Zeichen auf der Anzeige aus
 * 
 * Die Funktion LCD_putchar gibt ein Zeichen auf der LCD-Anzeige aus.
 * ACHTUNG: FUNKTIONIERT NUR WENN VORHER DER BEFEHL LCD_start_byte(LCD_start | LCD_data_write) AUSGWFUEHRT WURDE!
 * 
 * \param[in]	character		Zeichen das ausgegeben werden soll
 */
void LCD_putchar(int character);

/**
 * \brief									Gibt eine Zeichenkette auf der Anzeige aus
 * 
 * Die Funktion LCD_puts gibt alle Zeichen der nullterminierten Zecihenkette str auf der LCD-Anzeige aus.
 * ACHTUNG: FUNKTIONIERT NUR WENN VORHER DER BEFEHL LCD_start_byte(LCD_start | LCD_data_write) AUSGWFUEHRT WURDE!
 * 
 * \param[in]	str					Zeichenkette die ausgegeben werden soll
 */
void LCD_puts(char const * str);

/**
 * \brief									Siehe https://www.cplusplus.com/reference/cstdio/printf/
 *
 * \param[in]	format			Formatstring
 * \param[in]	...					weitere Argumente
 */
void LCD_printf(char const * format, ...);

/**
 * \brief Konstantendefinitionen fuer Portleitungen
 */
enum
{
	LCD_RST  = 0x1 << 11,
	LCD_CS   = 0x1 << 12,
	LCD_SCLK = 0x1 << 13,
	LCD_SOD  = 0x1 << 14,
	LCD_SID  = 0x1 << 15,
};

/**
 * \brief Konstantendefinitionen fuer Startbytes
 */
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


/**
 * \brief Konstantendefinitionen fuer Befehle
 */
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
		LCD_3rd_line                                          = 0x0 << 2, //
		LCD_2nd_line                                          = 0x1 << 2, //
		LCD_1st_line                                          = 0x2 << 2, //
		LCD_both_lines                                        = 0x3 << 2, //

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
	
	
	LCD_view                                                = 0x1 << 2,
		LCD_bottom                                            = 0x1 << 1,
		LCD_top                                               = 0x1 << 0,
};

/**
 * \brief		Berechnet einen Befehl um den Cursor an die Position (x, y) zu setzen
 *
 * \return	Befehl um den Cursor an die Position (x, y) zu setzen
 */
uint8_t LCD_set_cursor_position(uint8_t x, uint8_t y);

#endif // LCD_H
