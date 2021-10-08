#include <stdlib.h>
#include "LCD.h"

/**
 * \brief		Main-Funktion
 * 
 * Beispielhauptprogramm zum Testen der LCD-Library
 * 
 * \return	Error-Code
 * \retval	0	The function is successfully executed
 * \retval	1 Error
 */
int main()
{
	LCD_init_normal();
	
	LCD_putchar('a'); // Zeichen 'a' wird ausgageben
	LCD_printf("%c %d %o %x", 111, 111, 111, 111); // 111 wird in unterschiedlichen Formaten ausgegeben

	return EXIT_SUCCESS;
}
