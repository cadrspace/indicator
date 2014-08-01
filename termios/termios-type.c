#include <libguile.h>
#include <termios.h>

#include "termios-type.h"

scm_t_bits termios_tag;         /* Smob tag. */

SCM
mark_termios_smob (SCM termios)
{
  return SCM_BOOL_F;
}

size_t
free_termios_smob (SCM termios)
{
  struct termios_data *td = _scm_to_termios_data (termios);
  scm_gc_free (td->termios, sizeof (struct termios_data), "termios");
  return 0;
}


/* Helper procedures. */

/* Convert object X to termios data structure. */
struct termios_data *
_scm_to_termios_data (SCM x)
{
  scm_assert_smob_type (termios_tag, x);
  return (struct termios_data *) SCM_SMOB_DATA (x);
}

/* Convert termios data structure TERMIOS to a smob. */
SCM
_scm_from_termios_data (struct termios *termios)
{
  SCM smob;
  struct termios_data *td;

  td = (struct termios_data *) scm_gc_malloc (sizeof (struct termios_data),
                                              "termios-data");
  td->termios = (struct termios *) scm_gc_malloc (sizeof (struct termios),
                                                  "termios");
  memcpy (td->termios, termios, sizeof (struct termios));
  SCM_NEWSMOB (smob, termios_tag, td);
  return smob;
}

void
init_termios_type (void)
{
  termios_tag = scm_make_smob_type ("termios", sizeof (struct termios_data));
  scm_set_smob_mark (termios_tag, mark_termios_smob);
  scm_set_smob_free (termios_tag, free_termios_smob);

#include "termios-type.x"
}
