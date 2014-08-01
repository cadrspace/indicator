#include <libguile.h>
#include <termios.h>

/* Initialzie C_CONST with the value of the constant. */
#define INIT_CONST_X(c_const)                                           \
  do {                                                                  \
    SCM obj = scm_c_define (#c_const, scm_from_int (c_const));          \
    gtermio_ ## c_const = scm_permanent_object (obj);                   \
  } while (0)


/* Baudrates */

SCM gtermio_B0;
SCM gtermio_B50;
SCM gtermio_B75;
SCM gtermio_B110;
SCM gtermio_B134;
SCM gtermio_B150;
SCM gtermio_B200;
SCM gtermio_B300;
SCM gtermio_B600;
SCM gtermio_B1200;
SCM gtermio_B1800;
SCM gtermio_B2400;
SCM gtermio_B4800;
SCM gtermio_B9600;
SCM gtermio_B19200;
SCM gtermio_B38400;


void
init_termios_const (void)
{
  INIT_CONST_X (B0);
  INIT_CONST_X (B50);
  INIT_CONST_X (B75);
  INIT_CONST_X (B110);
  INIT_CONST_X (B134);
  INIT_CONST_X (B150);
  INIT_CONST_X (B200);
  INIT_CONST_X (B300);
  INIT_CONST_X (B600);
  INIT_CONST_X (B1200);
  INIT_CONST_X (B1800);
  INIT_CONST_X (B2400);
  INIT_CONST_X (B9600);
  INIT_CONST_X (B19200);
  INIT_CONST_X (B38400);
}

/* termios-const.c ends here */
