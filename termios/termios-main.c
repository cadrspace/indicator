#include "termios-type.h"
#include "termios-func.h"
#include "termios-const.h"

void
init_termios (void)
{
  init_termios_type ();
  init_termios_func ();
  init_termios_const ();
}
