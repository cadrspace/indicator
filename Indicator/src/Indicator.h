#ifndef Indicator_h
#define Indicator_h

#include "Arduino.h"
#include <inttypes.h>

#define uint8_t uint8_t

const unsigned short LED_AMOUNT = 64;
const unsigned short MATRIX_SIZE = LED_AMOUNT;

const unsigned short ROW_LEN = 8;

const uint8_t MAX_BRT = 7;
const uint8_t MIN_BRT = 1;
/*
const uint8_t LATCH_PIN = 5;
const uint8_t CLK_VERT_PIN = 3;
const uint8_t CLK_HOR_PIN = 4;
const uint8_t DATA_PIN = 2;
*/
//

const uint8_t LED_BLK_SZ = 4; //the size (in bits) of one LED info bites block
const uint8_t BLK_MASK = 0xF;

const uint8_t RED_ID = 0; // red block number 
const uint8_t GREEN_ID = 1; // green block number 
const uint8_t BLUE_ID = 2; // blue block number 

const uint8_t RED_BLK_OFST = 0; // red block number 
const uint8_t GREEN_BLK_OFST = 4; // green block number 
const uint8_t BLUE_BLK_OFST = 8; // blue block number

//channel bits in registers line
const uint8_t RED_BLK_BIT = 0x1; 
const uint8_t GREEN_BLK_BIT = 0x4;
const uint8_t BLUE_BLK_BIT = 0x2;


const uint8_t COLOR_RANGE = 3;


const uint8_t NOVALID_VAL = 255;

const uint8_t BRIGHT_LIST8[8] = {
  0x0,
  0x1,
  0x11,
  0x49,
  0x55,
  0xAD,
  0xFD,
  0xFF 
};


class Indicator {
	
public:
	//Indicator();
	Indicator(uint8_t latch_pin, uint8_t clk_vert_pin, uint8_t clk_hor_pin, uint8_t data_pin);
	
	uint8_t get_pixel_color(uint8_t num, uint8_t color);
	uint8_t get_pixel(uint8_t num, uint8_t& red, uint8_t& green, uint8_t& blue );
	uint8_t set_pixel_color(uint8_t num, uint8_t color, uint8_t brt);
	uint8_t set_pixel(uint8_t num, uint8_t red, uint8_t green, uint8_t blue);
	
	unsigned long get_row( uint8_t idx, uint8_t b_idx);
	void render_line(uint8_t idx, unsigned long hor_line);
	void render_matrix();
	
	void init();
	
private:
	uint16_t matrix [MATRIX_SIZE];
	
	uint8_t latch_pin;
	uint8_t clk_vert_pin;
	uint8_t clk_hor_pin;
	uint8_t data_pin;
	
};

#endif
