#ifndef _OLED_H_
#define _OLED_H_
#include <stdio.h>
#include "common.h"
#include "gpio.h"

#define OLED_SCL   PDout(3) 
#define OLED_SDA   PDout(2) 
#define OLED_RES   PDout(1)
#define OLED_DC    PDout(0)
#define OLED_CS    PDout(4)

/** @brief Print data directly in the current location. */
#define oled_printf(...)	{ sprintf (oled_str,__VA_ARGS__); oled_print_string(oled_str); }
		
/**
 * @brief Prints data at the specified location.
 * @y  Position y, 0~7
 * @x  Position x, 0~128
 */		
#define oled_display(y,x,...)	{ oled_pos_x=x; oled_pos_y=y; oled_printf (__VA_ARGS__) }


extern char oled_str[50];		
extern uint16_t oled_pos_x;
extern uint16_t oled_pos_y; 

/*---function interface---*/
void oled_draw_point(uint16_t x, uint16_t y);
void oled_init(void);
void oled_print_char(uint8_t x, uint8_t y, char ch);
void oled_set_pos(uint8_t x, uint8_t y);
void oled_clear_all(uint8_t dat);
void oled_write_byte(uint8_t com, uint8_t dat);
void oled_print_string(const char* str);
#endif
