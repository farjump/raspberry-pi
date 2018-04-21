#include "alpha/fileio.h"
#include "errno.h"

int unlink(char *name)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_UNLINK((t_fileio_uchar*) name, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
