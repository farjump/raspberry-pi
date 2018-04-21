#include "alpha/fileio.h"
#include "errno.h"

int isatty(int fd)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_ISATTY(fd, &errnum, &rc);

  if (rc == 0) {
    errno = errnum;
  }
  return rc;
}
