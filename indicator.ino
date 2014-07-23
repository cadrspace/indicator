#include <avr/io.h>
#include <avr/interrupt.h>


const int PIN_DATA  = 11;
const int PIN_LATCH = 10;
const int PIN_CLK   = 13;

const int DELAY = 500; // ms


void setup()
{
  /* Configure pins */

  pinMode(PIN_DATA,  OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLK,   OUTPUT);

  /* Configure the serial port */

  Serial.begin(9600);
  Serial.println("reset");
}


/* Low-Level Procedures */

void
latch (void)
{
  digitalWrite (PIN_LATCH, LOW);
}

void
unlatch (void)
{
  digitalWrite (PIN_LATCH, HIGH);
}

/* Write DATA to the data pin. */
void
write_data (int data)
{
  digitalWrite (PIN_DATA, data);
}

/* Start shifting bits. */
void
shift_start (void)
{
  digitalWrite (PIN_CLK, HIGH);
}

/* Stop shifting bits. */
void
shift_stop (void)
{
  digitalWrite (PIN_CLK, LOW);
}


// the heart of the program
// This shifts 8 bits out MSB first,
// on the rising edge of the clock,
// clock idles low
void
shiftOut (short data_out)
{
  int i;
  int pin_state;

  //clear everything out just in case to
  //prepare shift register for bit shifting
  write_data (0);
  shift_stop ();

  for (i = 15; i >= 0; --i)
    {
      shift_stop ();

      //if the value passed to myDataOut and a bitmask result
      // true then... so if we are at i=6 and our value is
      // %11010100 it would the code compares it to %01000000
      // and proceeds to set pinState to 1.
      pin_state = data_out & (1 << i);

      //Sets the pin to HIGH or LOW depending on pinState
      write_data (pin_state);

      //register shifts bits on upstroke of clock pin  
      shift_start ();

      //zero the data pin after shift to prevent bleed through
      write_data (0);
    }

  shift_stop ();
}


/* States */

enum STATE {
  STATE_DEFAULT = 0,
  STATE_NOTICE,
  STATE_WARNING,
  STATE_TEST
};

void
state_heartbeat (void)
{
  latch ();
  shiftOut ((unsigned short) 0x05);
  unlatch ();

  delay (200);

  latch ();
  shiftOut ((unsigned short) 0x00);
  unlatch ();

  delay (1500);
}

void
state_notice (void)
{
  int i;
  for (i = 0; i < 3; ++i)
    {
      latch ();
      shiftOut ((byte) 0x05);
      unlatch ();

      delay (300);

      latch ();
      shiftOut ((byte) 0x04);
      unlatch ();

      delay (100);

      latch ();
      shiftOut ((byte) 0x00);
      unlatch ();

      delay (500);
    }
  delay (1000);
}

void
state_warning (void)
{
  int i;
  for (i = 0; i < 3; ++i)
    {
      latch ();
      shiftOut ((byte) 0x05);
      unlatch ();

      delay (100);

      latch ();
      shiftOut ((byte) 0x04);
      unlatch ();

      delay (100);
    }

  delay (1500);
}

void
state_test (void)
{

  led_demo();

/*
  int i;
  for (i = 0; i < 3; ++i)
    {
      latch ();
//      shiftOut ((unsigned short) (0x02 << 8));
      shiftOut ((unsigned short) (0xFFFF));
      unlatch ();

      delay (100);

      latch ();
      shiftOut ((unsigned short) 0x00);
      unlatch ();

      delay (500);
    }
*/
      
}


/*  LED Colors */

enum COLOR 
{
  GREEN = 1,
  BLUE,
  LBLUE,
  RED,
  ORANGE,
  PINK,
  WHITE
};

/* LED Control */

/* set current led pointed col and row to color state */
void 
set_led(int col, int row, int color)
{
     word state = 0;

     // first - let's set row bits
     state = state + color;
   
     if( row > 0 && row < 5 ) {
       if( row != 3 ) {
         state = state << ( 3 * ( row - 1 )); 
       } else {
         state = state << ( 3 * row);
       }
     }
     
    //state = state | color;

    // second - set cols one
     if( col > && col < 4 ) {
       state = state | ( ~( 0x7 & ( 0x1 << ( col-1 ) ) ) << 13); //| 0x3000;
     }
     
     digitalWrite( PIN_LATCH, LOW);
     
     // ... and using example from docs - shiftOut pushs 1 byte only!
     // we have a word type so push by parts 
     
     shiftOut( PIN_DATA, PIN_CLK, MSBFIRST, (byte) (state >> 8) );  
     shiftOut( PIN_DATA, PIN_CLK, MSBFIRST, (byte) (state) );
     digitalWrite( PIN_LATCH, HIGH);
}


/* set the light off by every led */
void
clear_led(void) 
{
    digitalWrite( PIN_LATCH, LOW);
    shiftOut( PIN_DATA, PIN_CLK, MSBFIRST, (byte) (0) );  
    shiftOut( PIN_DATA, PIN_CLK, MSBFIRST, (byte) (0) );
    digitalWrite( PIN_LATCH, HIGH);
}


/* led demo displays rotate line- one color by cycle. then unknown behavior - very impressive! */
void
led_demo()
{

  int d = 50;
  int dd = 10;
  int c = 0;
  int counter = 0;
  
  while ( true )
  {
  
    set_led( 1, 1, c);
    delay( dd );
    set_led( 2, 2, c);
    delay( dd );
    set_led( 3, 3, c);
    
    delay( dd );
    clear_led();
    delay( dd );
    delay( d );
    
    set_led( 2, 1, c);
    delay( dd );
    set_led( 2, 2, c);
    delay( dd );
    set_led( 2, 3, c);
    
    delay( dd );
    clear_led();
    delay( dd );
    delay( d );
    
    set_led( 3, 1, c);
    delay( dd );
    set_led( 2, 2, c);
    delay( dd );
    set_led( 1, 3, c);
    
    delay( dd );
    clear_led();
    delay( dd );
    delay( d );
     
    set_led( 1, 2, c );
    delay( dd );
    set_led( 2, 2, c);
    delay( dd );
    set_led( 3, 2, c);
    
    delay( dd );
    clear_led();
    delay( dd );
    delay( d );
    
    if( !(counter % 10) ) {
      c++;
    }
    counter++;
 
  }

}



/* Main loop */

void
loop (void)
{
  byte state = STATE_DEFAULT;
  while (true)
    {
      if (Serial.available ())
        state = Serial.parseInt ();
      else
        state = STATE_DEFAULT;

      switch (state)
        {
        case STATE_DEFAULT:
          state_heartbeat ();
          break;

        case STATE_NOTICE:
          state_notice ();
          state = STATE_DEFAULT;
          break;

        case STATE_WARNING:
          state_warning ();
          state = STATE_DEFAULT;
          break;
          
        case STATE_TEST:
          state_test ();
          state = STATE_DEFAULT;
          break;   
        }
    }
}

/* indicator.ino ends here */
