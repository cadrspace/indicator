#include "Indicator.h"

byte Indicator::get_pixel_color(byte num, byte color) {
	if(num < MATRIX_SIZE) {
		return (matrix[num] >> (LED_BLK_SZ * color)) & BLK_MASK;
	}
	return NOVALID_VAL;
}

byte Indicator::get_pixel(byte num, byte& red, byte& green, byte& blue ) {
	if(num < MATRIX_SIZE) {
		red = (matrix[num] >> RED_BLK_OFST) & BLK_MASK;
		green = (matrix[num] >> GREEN_BLK_OFST) & BLK_MASK;
		blue = (matrix[num] >> BLUE_BLK_OFST) & BLK_MASK;
		return 0;
	}
	return NOVALID_VAL;
}

byte Indicator::set_pixel_color(byte num, byte color, byte brt = MAX_BRT) {
	
	uint16_t pixpart = 0;
	if(num < MATRIX_SIZE) {
		pixpart |= (brt & BLK_MASK);
		matrix[num] &= ~(BLK_MASK << (LED_BLK_SZ * color));
		matrix[num] |= (pixpart << (LED_BLK_SZ * color));
		return 0;
	}
	return NOVALID_VAL;
}


byte Indicator::set_pixel(byte num, byte red_brt, byte green_brt, byte blue_brt) {

	uint16_t x = 0;
	if(num < MATRIX_SIZE) {
		x |= (red_brt & BLK_MASK) << RED_BLK_OFST;
		x |= (green_brt & BLK_MASK) << GREEN_BLK_OFST;
		x |= (blue_brt & BLK_MASK) << BLUE_BLK_OFST;
		matrix[num] = x;
		return 0;
	}
	return NOVALID_VAL;
}
	
uint32_t Indicator::get_row( byte idx, byte b_idx) {
	
  uint32_t hor_line = 0;
  uint32_t proc_row = 0;
  uint16_t start_pix = idx * ROW_LEN;
 
  byte s_r = 0;
  byte s_g = 0;
  byte s_b = 0;

  uint16_t pix = 0;
  
  for ( int j = 0; j < ROW_LEN; j++ ) {
      proc_row  = 0;
	  
	  get_pixel( (start_pix + j ), s_r, s_g, s_b );

		if( b_idx & BRIGHT_LIST8[s_r])  {
			proc_row |= RED_BLK_BIT;
		}
		if( b_idx & BRIGHT_LIST8[s_g])  {
			proc_row |= GREEN_BLK_BIT;
		} 
		if(b_idx & BRIGHT_LIST8[s_b]) {
			proc_row |= BLUE_BLK_BIT;
		}
		proc_row <<= j*COLOR_RANGE;
		hor_line |= proc_row;
  }
  return hor_line;
}
	
void Indicator::render_line(byte idx, uint32_t hor_line) {
	
	digitalWrite(latch_pin, LOW);
	shiftOut(data_pin, clk_vert_pin, MSBFIRST, ~(1<<idx) );

	shiftOut(data_pin, clk_hor_pin, MSBFIRST, (hor_line>>16)&0xff);
	shiftOut(data_pin, clk_hor_pin, MSBFIRST, (hor_line>>8)&0xff);
	shiftOut(data_pin, clk_hor_pin, MSBFIRST, hor_line&0xff);
  
	digitalWrite(latch_pin, HIGH);
}

void Indicator::render_matrix() {

	uint32_t hor_line = 0;
	for(byte u = 0; u < 8; u++) {
		for(byte idx = 0; idx < ROW_LEN; idx++) {
			hor_line = get_row(idx, (1 << u) );
			render_line(idx, hor_line);
		}
	}
}


void Indicator::init()
{

	//randomSeed(analogRead(0));
 
	pinMode(latch_pin, OUTPUT);
	pinMode(clk_vert_pin, OUTPUT);
	pinMode(clk_hor_pin, OUTPUT);
	pinMode(data_pin, OUTPUT);
	delayMicroseconds(50000);
	
}

Indicator::Indicator(byte _latch_pin, byte _clk_vert_pin, byte _clk_hor_pin, byte _data_pin) {
	latch_pin = _latch_pin;
	clk_vert_pin = _clk_vert_pin;
	clk_hor_pin = _clk_hor_pin;
	data_pin = _data_pin;
}

