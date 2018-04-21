#include "alpha/fileio.h"
#include "errno.h"

int stat(const char* file, struct stat* st)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_STAT((t_fileio_uchar*) file, (t_fileio_stat*) st, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
