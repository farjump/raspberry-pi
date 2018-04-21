#include "alpha/fileio.h"
#include "errno.h"

int lseek(int fd, int offset, int dir)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_LSEEK(fd, offset, dir, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
