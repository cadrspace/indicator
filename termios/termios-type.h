#ifndef __TERMIOS_TYPE_H__
#define __TERMIOS_TYPE_H__

#include <termios.h>
#include <libguile.h>

extern scm_t_bits termios_tag;

struct termios_data {
  struct termios *termios;
};

extern struct termios_data *_scm_to_termios_data (SCM x);
extern SCM _scm_from_termios_data (struct termios *termios);

#endif  /* ifndef __TERMIOS_TYPE_H__ */









