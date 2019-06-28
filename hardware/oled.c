#include "oled.h"
#include "font_ascii.h"

#define WIDTH	128
#define HEIGHT	64

#define  CMD  0
#define  DAT  1

uint16_t oled_pos_x = 0, oled_pos_y = 0; 
char oled_str[50];

/**
 * @brief Oled I/O drive function
 */
void oled_gpio_init(void)
{
    GPIO_QuickInit(HW_GPIOD,0,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,1,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,2,kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOD,3,kGPIO_Mode_OPP); 
    GPIO_QuickInit(HW_GPIOD,4,kGPIO_Mode_OPP);   
}


/**
 *  @brief Write a byte to oled
 *  @com 1 command, 0 data
 *  @dat A byte of data to be sent 
 */
void oled_write_byte(uint8_t com, uint8_t dat)
{
	uint8_t i = 8;
	OLED_DC = com;
	while(i--){
		OLED_SCL = 0;
		OLED_SDA = (dat&0x80) ? 1 : 0;
		OLED_SCL = 1;
		dat <<= 1; 
	}
}


/**
 *  @brief Clear the oled screen
 *  @dat 0x00 Clear to be black screen, 0xff Clear to be white screen
 */
void oled_clear_all(uint8_t dat)
{
	uint8_t x,y;
	
	for(y = 0;y<8;y++){
		oled_write_byte(CMD,0xb0+y);
		oled_write_byte(CMD,0x00);
		oled_write_byte(CMD,0x10);
		for(x = 0;x<WIDTH;x++)	oled_write_byte(DAT,dat); 
	}	
}


/**
 *  @brief Initialize the oled
 */
void oled_init(void)
{
	oled_gpio_init();
	
	OLED_CS  = 0;
	OLED_RES = 0; 
	OLED_RES = 1; 
	
	oled_write_byte(CMD,0xA8);    //Set Multiplex Ratio
    oled_write_byte(CMD,0x3F);  
    
    oled_write_byte(CMD,0xD3);    //Set offset of display
    oled_write_byte(CMD,0x00);
    
    oled_write_byte(CMD,0x40);    //Set the first line of display 
    
    oled_write_byte(CMD,0xA1);    //Set segment remapping  A0h/A1h
    
    oled_write_byte(CMD,0xC8);    //Set the scan direction of COM  C0h/C8h
    
    oled_write_byte(CMD,0xDA);    //Sets the hardware configuration of the COM pin
    oled_write_byte(CMD,0x12);
    
    oled_write_byte(CMD,0x81);    //Set the contrast
    oled_write_byte(CMD,0x7F);
    
    oled_write_byte(CMD,0xA4);    //Disable screen display
    
    oled_write_byte(CMD,0xA6);    //Enable screen display
    
    oled_write_byte(CMD,0xD5);    //Set the OSC frequency
    oled_write_byte(CMD,0x80);
    
    oled_write_byte(CMD,0x8D);    //Enable charge pupm regulator
    oled_write_byte(CMD,0x14);
    
    oled_write_byte(CMD,0xAF);    //Start to display
	oled_clear_all(0x00);         //The initial screen clearing
	
}


/**
 *  @brief Set display position
 *  @x Position x
 *  @y Position y
 */
void oled_set_pos(uint8_t x, uint8_t y)
{
	oled_write_byte(CMD,0xb0+y);
	oled_write_byte(CMD,x&0x0F); 
	oled_write_byte(CMD,(x>>4) | 0x10);
}


/**
 *  @brief Draw a point on oled
 *  @x Position x
 *  @y Position y
 */
void oled_draw_point(uint16_t x, uint16_t y)
{
	uint8_t dat;
	
	/* Get the point data, get the actual y display, and write x information */
	dat = 0x01<<(y%8);
	oled_write_byte(CMD,0xb0+(y>>3));		
	
	oled_write_byte(CMD,(x&0x0f)|0x00);
	oled_write_byte(CMD,((x&0xf0)>>4)|0x10);
	oled_write_byte(DAT,dat);
}


/**
 *  @brief Display a character
 *  @x Position x
 *  @y Position y
 *  @ch The character to display
 */
void oled_print_char(uint8_t x, uint8_t y, char ch)
{
	oled_set_pos(x,y);
	uint8_t temp,i;
    temp = (ch - 32);
    for(i = 0;i<6;i++)
        oled_write_byte(DAT,ASCII_6X8[temp][i]);
}


/**
 *  @brief Display string
 *  @str The point string to display
 */
void oled_print_string(const char* str)
{
	while(*str){
		if(*str == '\r'){ str++; continue; }
		if(*str == '\n'){
		   oled_pos_x = 0;oled_pos_y += 1;
		   str++; continue;
		}
		if(oled_pos_x> WIDTH-6){oled_pos_x = 0;oled_pos_y+=1;}
		if(oled_pos_y> 8){oled_clear_all(0x00);oled_pos_y = oled_pos_x = 0;}
		oled_print_char(oled_pos_x,oled_pos_y,*str++);
		oled_pos_x += 6;
	}
}

