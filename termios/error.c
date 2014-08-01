#include <libguile.h>

void
gtermios_error (const char *proc, const char *msg, SCM args)
{
  SCM KEY = scm_from_locale_symbol ("termios-error");
  scm_error (KEY, proc, msg, args, SCM_BOOL_F);
}

