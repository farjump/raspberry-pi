#include "alpha/fileio.h"
#include "errno.h"

int gettimeofday(struct timeval* tv, void* tz)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  (void) tz; // not implemented
  FILEIO_GETTIMEOFDAY((t_fileio_timeval*) tv, FILEIO_NULL, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}

