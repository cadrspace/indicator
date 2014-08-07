#include <libguile.h>
#include <termios.h>

#include "termios-type.h"
#include "error.h"


SCM_DEFINE (gtermios_cfgetispeed,
            "%cfgetispeed", 1, 0, 0,
            (SCM termios),
            "")
{
  struct termios_data *td = _scm_to_termios_data (termios);
  return scm_from_int (cfgetispeed (td->termios));
}

SCM_DEFINE (gtermios_cfgetospeed,
            "%cfgetospeed", 1, 0, 0,
            (SCM termios),
            "")
{
  struct termios_data *td = _scm_to_termios_data (termios);
  return scm_from_int (cfgetospeed (td->termios));
}


SCM_DEFINE (gtermios_cfsetispeed_x,
            "%cfsetispeed!", 2, 0, 0,
            (SCM termios, SCM speed),
            "")
#define FUNC_NAME s_gtermios_cfsetispeed_x
{
  struct termios_data *td = _scm_to_termios_data (termios);
  int res;
  SCM_ASSERT (scm_is_number (speed), speed, SCM_ARG2, FUNC_NAME);

  res = cfsetispeed (td->termios, scm_to_int (speed));
  if (res < 0)
    gtermios_error (FUNC_NAME, "Unable to set speed.", speed);
  return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE (gtermios_cfsetospeed_x,
            "%cfsetospeed!", 2, 0, 0,
            (SCM termios, SCM speed),
            "")
#define FUNC_NAME s_gtermios_cfsetispeed_x
{
  struct termios_data *td = _scm_to_termios_data (termios);
  int res;
  SCM_ASSERT (scm_is_number (speed), speed, SCM_ARG2, FUNC_NAME);

  res = cfsetospeed (td->termios, scm_to_int (speed));
  if (res < 0)
    gtermios_error (FUNC_NAME, "Unable to set speed.", speed);
  return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCM_DEFINE (gtermios_tcgetattr,
            "%tcgetattr", 1, 0, 0,
            (SCM fd),
            "")
#define FUNC_NAME s_gtermios_tcgetattr
{
  struct termios termios;
  int res;
  int c_fd;

  SCM_ASSERT (scm_is_number (fd), fd, SCM_ARG1, FUNC_NAME);

  c_fd = scm_to_int (fd);

  res = tcgetattr (c_fd, &termios);
  if (res < 0)
    gtermios_error (FUNC_NAME, "Unable to get attributes.", fd);

  return _scm_from_termios_data (&termios);
}
#undef FUNC_NAME

SCM_DEFINE (gtermios_tcsetattr_x,
            "%tcsetattr!", 2, 1, 0,
            (SCM termios, SCM fd, SCM optional_actions),
            "")
#define FUNC_NAME s_gtermios_tcsetattr_x
{
  struct termios_data *td = _scm_to_termios_data (termios);
  int c_optional_actions = 0;
  int res;

  SCM_ASSERT (scm_is_number (fd), fd, SCM_ARG2, FUNC_NAME);
  if (! SCM_UNBNDP (optional_actions))
    {
      SCM_ASSERT (scm_is_number (optional_actions), optional_actions,
                  SCM_ARG3, FUNC_NAME);
      c_optional_actions = scm_to_int (optional_actions);
    }

  res = tcsetattr (scm_to_int (fd), c_optional_actions, td->termios);
  if (res < 0)
    {
      gtermios_error (FUNC_NAME, "Unable to set attributes.",
                      scm_list_3 (termios, fd, optional_actions));
    }

  return SCM_UNDEFINED;
}
#undef FUNC_NAME


void
init_termios_func (void)
{
#include "termios-func.x"
}

/* termios.c ends here */
