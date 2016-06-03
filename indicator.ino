#include <avr/io.h>
#include <avr/interrupt.h>

/* PIN interface */
const int LATCH = 5;
const int CLK_VERT = 3;
const int CLK_HOR = 4;
const int DATA = 2;

/* LED matrix */
const int MATRIX_SIZE = 64;

const int ROW_LEN = 8;
//const int BRIGHT_SIZE = 8;

/* LED */
            
/*
	The LED is the one chip having 3 small leds inside.
	The default led matrix has 8x8 LEDs on it and every LED is the PIXEL!
	
	There are rows and columns register drivers on the LED's board.
	We can set ON only one LED's row per time (to have the stable power's requirements)
	
	Programm loop takes LED's data from the matrix array one row per time and 
	set the outside register drivers in accordance LEDs states.
	
	We have the frame completed when the every row is drawn one time. 
	
	We must redraw every frame for every brightness index - take a look to the loop.

	Every PIXEL is described by 2 bytes (normal order):
bit's enumeration
	 0 1 2 3  4 5 6 7  8 9 10 11  12 13 14 15
bit's position
	|R|X|X|X| |G|Y|Y|Y| |B|Z|Z|Z| |0|0|0|0|
   
   This is bit's view in notation that Arduino platform use: 
   0000 XXXB XXXG XXXR
 
   where R, G, B - active status for red,green and blue leds color
   XXX, YYY, ZZZ - brightness level of R, G, B leds
*/
 
const int LEDSTAMP_SIZE = 4; // the stamp size of every LED

const int LSTATUS_OFFSET = 0;
const int LBRIGHT_OFFSET = 1;
const int BRIGHT_MASK = 0x7;
const int STATUS_MASK = 0x1;

const int LED_BLK_SZ = 4; //the size of one LED info bites block

const int RED_ID = 0; // red block number 
const int GREEN_ID = 1; // green block number 
const int BLUE_ID = 2; // blue block number 

const int BRT_OFST = 1; // LED block's offset to get the brightnes
const int RED_BLK_OFST = 0; // red block number 
const int GREEN_BLK_OFST = 4; // green block number 
const int BLUE_BLK_OFST = 8; // blue block number 

const int LRED_OFFSET = 1;
const int LGREEN_OFFSET = 4;
const int LBLUE_OFFSET = 4;

const int COUNT_RANGE = 3;

/* The array of LED's state: every element describes every led's pixel */
unsigned short matrix [MATRIX_SIZE]={0};

/* 4-level brightness gradation */
byte BRIGHT_LIST4[4] = {
  0x1,
  0x11,
  0x55,
  0xFF 
};

/* 8-level brightness gradation */
byte BRIGHT_LIST8[8] = {
  0x1,
  0x11,
  0x49,
  0x55,
  0xAD,
  0xED,
  0xFD,
  0xFF 
};

/* commands masks */
const byte MARKER_MASK = 0x80;
const byte CMD_MASK = 0x3;
const byte CMD_SIZE = 2;

/* work modes */
const byte MODE_WAIT = 0;
const byte MODE_OFF = 1;
const byte MODE_DISPLAY = 2;
const byte MODE_DEMO = 3;
const byte MODE_MAX_VALUE = MODE_DEMO;

unsigned long counter = 0;

/* 
set color of the pixel by pixel num

num - the led's number
color - colors number
brt - brightness code
sts - pixels status: ON\OFF
*/
void set_matrix_pixel_color(byte num, byte color, byte brt=7, byte sts=1 ) {
  byte pixpart = 0;
  if(num < MATRIX_SIZE) {
    pixpart |= brt & 0x7;// set 111 
    pixpart <<= 0x1; // slide it to brithness zone - 1
    pixpart |= sts & 0x1; // set first bit by sts val
    matrix[num] |= pixpart << (LED_BLK_SZ * color);
  }
}

/* get color of the pixel by pixel num + color num 
num - the led's number
clr - colors number
sts - pixels status: ON\OFF
bgt - brightness code
*/
void get_matrix_pixel_color(byte num, byte clr, byte& sts, byte& bgt ) {
  unsigned short pix = matrix[num];
  if(num < MATRIX_SIZE) {
    pix = (pix >> LED_BLK_SZ * clr) & 0xF; // retrieve the color block of the pixel
    sts = pix & BRT_OFST;
    bgt = pix >> BRT_OFST; // slide to retrieve the color 
  }
}

void setup(){

/* registers configuration - we use one LATCH and one DATA pins for rows and columns registers.
   but CLK are isolated!
*/
	pinMode(LATCH, OUTPUT);
	pinMode(CLK_VERT, OUTPUT);
	pinMode(CLK_HOR, OUTPUT);
	pinMode(DATA, OUTPUT);
 
 /* This is color's gradient example #2 */

    for(byte a = 0; a < ROW_LEN/2; a++) {
  		for(byte b = 0; b < ROW_LEN; b++) {
  			set_matrix_pixel_color(a*ROW_LEN+b, BLUE_ID,b );
  			set_matrix_pixel_color(a*ROW_LEN+b, GREEN_ID,7-b );
  			set_matrix_pixel_color(a*ROW_LEN+b, RED_ID,a*2 );
  		}
    }
    for(byte a = ROW_LEN/2; a < ROW_LEN; a++) {
  		for(byte b = 0; b < ROW_LEN; b++) {
  			set_matrix_pixel_color(a*ROW_LEN+b, BLUE_ID,b );
  			set_matrix_pixel_color(a*ROW_LEN+b, GREEN_ID,7-b );
  			set_matrix_pixel_color(a*ROW_LEN+b, RED_ID,7-a*2);
  		}
    }

}

/**
 * Get horizontal line that describes which one of LED's color would be drawn on a LED panel.
 * idx - rows index
 * b_idx - brightness index (0-7 for 7-levels)
 * returned value is the multibite value that have every LED's color state (ON or OFF).
 * i.e 100010 - means LED#1 has red color is set ON and LED#2 has green color set ON but other colors are * set OFF
 */
unsigned long get_row( byte idx, byte b_idx) {

	unsigned long hor_line = 0;
	unsigned long lopatka_row = 0;
	unsigned short start_pix = idx * ROW_LEN;
  
	byte s_r = 0;
	byte s_g = 0;
	byte s_b = 0;

	unsigned short pix = 0;
	// for every LED from matrix array
	for ( int j = 0; j < ROW_LEN; j++ ) {
		lopatka_row = 0;

		pix = matrix[ start_pix + j ];

		// get color block for every color
		s_r = (pix >> RED_BLK_OFST) & 0xf; //1 
		s_g = (pix >> GREEN_BLK_OFST) & 0xf; // 4
		s_b = (pix >> BLUE_BLK_OFST) & 0xf; //8

		// add color's status set ON if the brightness level has valid mask for current brightness render index
		if( (b_idx & BRIGHT_LIST8[ (s_r >> 0x1) ]) && (s_r & 0x1) ) {
			lopatka_row |= 0x1;
		}
		if( (b_idx & BRIGHT_LIST8[ (s_g >> 0x1) ]) && (s_g & 0x1) ) {
			lopatka_row |= 0x4;
		} 
		if( (b_idx & BRIGHT_LIST8[ (s_b >> 0x1) ]) && (s_b & 0x1) ) {
			lopatka_row |= 0x2;
		}
		// shifting to j position before union operation
		lopatka_row <<= j*3;
		// and to update returned value with the clculated color status
		hor_line |= lopatka_row;

  }
  return hor_line;
}

/*
render procedure without the brihtness feature - it is formal untested code
*/
/*
void render_simple_loop() {
  byte idx = 0; // rows index
  unsigned long hor_line = 0; // данные о ряде

  while (idx < ROW_LEN ) {
    hor_line = get_row(idx);
    render_line(idx, hor_line);
    idx++;
    delay(LOOP_DELAY);
  }
}
*/

/*
* fill the outsides registers with the current row number and row's data
*/
void render_line(byte idx, unsigned long hor_line) {

	digitalWrite(LATCH, LOW);
	shiftOut(DATA, CLK_VERT, MSBFIRST, ~(1<<idx) );

	shiftOut(DATA, CLK_HOR, MSBFIRST, (hor_line>>16)&0xff);
	shiftOut(DATA, CLK_HOR, MSBFIRST, (hor_line>>8)&0xff);
	shiftOut(DATA, CLK_HOR, MSBFIRST, hor_line&0xff);
  
	digitalWrite(LATCH, HIGH);
}

void loop() {

// this is "WOW" example: vertical autoscrolling
/*
  counter++;
  unsigned short tm[ROW_LEN]={};
  
  if (counter > 2) {
    //unsigned short matrix
    memcpy(tm,matrix,ROW_LEN*sizeof(unsigned short));
    for(int a = 0; a< ROW_LEN-1; a++) {
      memcpy(matrix+a*ROW_LEN,matrix+(a+1)*ROW_LEN,ROW_LEN*sizeof(unsigned short));
    }
     memcpy(matrix+7*ROW_LEN,tm,ROW_LEN*sizeof(unsigned short));
    counter=0;
  }
 */
 
	unsigned long hor_line = 0;

	// for every brightness level
	for(byte u = 0; u < 8; u++) {
		// for every row of the LED matrix
		for(byte idx = 0; idx < ROW_LEN; idx++) {
			//get current row set having one bit per LED color state for the current brightness render index
			hor_line = get_row(idx, (1 << u) );
			// and push it to the outside board registers 
			render_line(idx, hor_line);
		}
	}

}
