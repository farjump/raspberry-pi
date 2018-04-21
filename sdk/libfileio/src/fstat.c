#include "alpha/fileio.h"
#include "errno.h"

int fstat(int fd, struct stat *st)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_FSTAT(fd, (t_fileio_stat*) st, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
