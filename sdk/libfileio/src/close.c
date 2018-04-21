#include "alpha/fileio.h"
#include "errno.h"

int close(int fd)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_CLOSE(fd, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
