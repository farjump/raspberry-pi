//
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.
//

#ifndef LIBALPHA_FILEIO_H_
# define LIBALPHA_FILEIO_H_

# ifdef __cplusplus
extern "C" {
# endif

# include <gdb/fileio.h>

extern void FILEIO_OPEN (/* in  */ const char*                 path,
                         /* in  */ fio_uint32_t                flags,
                         /* in  */ fio_uint32_t                mode,
                         /* out */ fio_uint32_t*               errno,
                         /* out */ fio_int32_t*                return_code);

extern void FILEIO_CLOSE (/* in  */ fio_int32_t                fd,
                          /* out */ fio_uint32_t*              errno,
                          /* out */ fio_int32_t*               return_code);

extern void FILEIO_READ (/* in  */ fio_int32_t                 fd,
                         /* in  */ void*                       dst,
                         /* in  */ fio_uint32_t                count,
                         /* out */ fio_uint32_t*               errno,
                         /* out */ fio_int32_t*                return_code);

extern void FILEIO_WRITE (/* in  */ fio_int32_t                fd,
                          /* in  */ const void*                src,
                          /* in  */ fio_uint32_t               count,
                          /* out */ fio_uint32_t*              errno,
                          /* out */ fio_int32_t*               return_code);

extern void FILEIO_LSEEK (/* in  */ fio_int32_t                fd,
                          /* in  */ fio_int32_t                offset,
                          /* in  */ fio_uint32_t               flag,
                          /* out */ fio_uint32_t*              errno,
                          /* out */ fio_int32_t*               return_code);

extern void FILEIO_RENAME (/* in  */ const char*               old_name,
                           /* in  */ const char*               new_name,
                           /* out */ fio_uint32_t*             errno,
                           /* out */ fio_int32_t*              return_code);

extern void FILEIO_UNLINK (/* in  */ const char*               path,
                           /* out */ fio_uint32_t*             errno,
                           /* out */ fio_int32_t*              return_code);

extern void FILEIO_STAT (/* in  */ const char*                 path,
                         /* in  */ struct fio_stat*            stat,
                         /* out */ fio_uint32_t*               errno,
                         /* out */ fio_int32_t*                return_code);

extern void FILEIO_FSTAT (/* in  */ fio_int32_t                fd,
                          /* in  */ struct fio_stat*           stat,
                          /* out */ fio_uint32_t*              errno,
                          /* out */ fio_int32_t*               return_code);

extern void FILEIO_GETTIMEOFDAY (/* in  */ struct fio_timeval* timeval,
                                 /* in  */ void*               timezone,
                                 /* out */ fio_uint32_t*       errno,
                                 /* out */ fio_int32_t*        return_code);

extern void FILEIO_ISATTY (/* in  */ fio_int32_t               fd,
                           /* out */ fio_uint32_t*             errno,
                           /* out */ fio_int32_t*              return_code);

extern void FILEIO_SYSTEM (/* in  */ const char*               command,
                           /* out */ fio_uint32_t*             errno,
                           /* out */ fio_int32_t*              return_code);

# ifdef __cplusplus
}
# endif

#endif /* !LIBALPHA_FILEIO_H_ */
