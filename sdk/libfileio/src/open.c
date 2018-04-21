#include "alpha/fileio.h"
#include "errno.h"

int open(char* file, int flags, int mode)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_OPEN((t_fileio_uchar*) file, flags, mode, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
