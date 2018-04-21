#include "alpha/fileio.h"
#include "errno.h"

int write(int fd, char* buf, int count)
{
  t_fileio_errno errnum;
  t_fileio_int32 rc;

  FILEIO_WRITE(fd, (t_fileio_uchar*) buf, count, &errnum, &rc);

  if (rc == -1) {
    errno = errnum;
  }
  return rc;
}
