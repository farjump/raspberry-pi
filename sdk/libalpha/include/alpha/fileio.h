//
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.
//

#ifndef LIBALPHA_FILEIO_H_
# define LIBALPHA_FILEIO_H_

# define FILEIO_NULL ((void*) 0)

// Base types
typedef unsigned char      t_fileio_uchar;
typedef long               t_fileio_int32;
typedef unsigned long      t_fileio_uint32;
typedef unsigned long long t_fileio_uint64;

// File open flags
typedef enum {
  FILEIO_O_RDONLY = 0,
  FILEIO_O_WRONLY = 1,
  FILEIO_O_RDWR   = 2,
  FILEIO_O_APPEND = 8,
  FILEIO_O_CREAT  = 512,
  FILEIO_O_TRUNC  = 1024,
  FILEIO_O_EXCL   = 2048
} t_fileio_open_flags;

// File Open modes
typedef enum {
  FILEIO_S_IXOTH = 1,
  FILEIO_S_IWOTH = 2,
  FILEIO_S_IROTH = 4,
  FILEIO_S_IXGRP = 8,
  FILEIO_S_IWGRP = 16,
  FILEIO_S_IRGRP = 32,
  FILEIO_S_IXUSR = 64,
  FILEIO_S_IWUSR = 128,
  FILEIO_S_IRUSR = 256,
  FILEIO_S_IFDIR = 16384,
  FILEIO_S_IFREG = 32768
} t_fileio_mode;

// Return code of FileIO functions
typedef enum {
  FILEIO_OK           = 0,
  FILEIO_EPERM        = 1,
  FILEIO_ENOENT       = 2,
  FILEIO_EINTR        = 4,
  FILEIO_EBADF        = 9,
  FILEIO_EACCES       = 13,
  FILEIO_EFAULT       = 14,
  FILEIO_EBUSY        = 16,
  FILEIO_EEXIST       = 17,
  FILEIO_ENODEV       = 19,
  FILEIO_ENOTDIR      = 20,
  FILEIO_EISDIR       = 21,
  FILEIO_EINVAL       = 22,
  FILEIO_ENFILE       = 23,
  FILEIO_EMFILE       = 24,
  FILEIO_EFBIG        = 27,
  FILEIO_ENOSPC       = 28,
  FILEIO_ESPIPE       = 29,
  FILEIO_EROFS        = 30,
  FILEIO_ENAMETOOLONG = 91,
  FILEIO_EUNKNOWN     = 9999
} t_fileio_errno;

// Seek commands
typedef enum {
  FILEIO_SEEK_SET = 0,
  FILEIO_SEEK_CUR = 1,
  FILEIO_SEEK_END = 2
} t_fileio_lseek_flag;

// File Stat structure
typedef struct {
  t_fileio_uint32 st_dev;
  t_fileio_uint32 st_ino;
  t_fileio_mode   st_mode;
  t_fileio_uint32 st_nlink;
  t_fileio_uint32 st_uid;
  t_fileio_uint32 st_gid;
  t_fileio_uint32 st_rdev;
  t_fileio_uint64 st_size;
  t_fileio_uint64 st_blksize;
  t_fileio_uint64 st_block;
  t_fileio_uint32 st_time;
  t_fileio_uint32 st_mtime;
  t_fileio_uint32 st_ctime;
} t_fileio_stat;

// Time value structure
typedef struct {
  t_fileio_uint32 tv_sec;
  t_fileio_uint32 tv_usec;
} t_fileio_timeval;

extern void FILEIO_OPEN (/* in  */ t_fileio_uchar*           path,
                         /* in  */ t_fileio_open_flags       flags,
                         /* in  */ t_fileio_mode             mode,
                         /* out */ t_fileio_errno*           err_number,
                         /* out */ t_fileio_int32*           return_code);

extern void FILEIO_CLOSE (/* in  */ t_fileio_int32           fd,
                          /* out */ t_fileio_errno*          err_number,
                          /* out */ t_fileio_int32*          return_code);

extern void FILEIO_READ (/* in  */ t_fileio_int32            fd,
                         /* in  */ t_fileio_uchar*           dst,
                         /* in  */ t_fileio_uint32           count,
                         /* out */ t_fileio_errno*           err_number,
                         /* out */ t_fileio_int32*           return_code);

extern void FILEIO_WRITE (/* in  */ t_fileio_int32           fd,
                          /* in  */ t_fileio_uchar*          src,
                          /* in  */ t_fileio_uint32          count,
                          /* out */ t_fileio_errno*          err_number,
                          /* out */ t_fileio_int32*          return_code);

extern void FILEIO_LSEEK (/* in  */ t_fileio_int32           fd,
                          /* in  */ t_fileio_int32           offset,
                          /* in  */ t_fileio_lseek_flag      flag,
                          /* out */ t_fileio_errno*          err_number,
                          /* out */ t_fileio_int32*          return_code);

extern void FILEIO_RENAME (/* in  */ t_fileio_uchar*         old_name,
                           /* in  */ t_fileio_uchar*         new_name,
                           /* out */ t_fileio_errno*         err_number,
                           /* out */ t_fileio_int32*         return_code);

extern void FILEIO_UNLINK (/* in  */ t_fileio_uchar*         path,
                           /* out */ t_fileio_errno*         err_number,
                           /* out */ t_fileio_int32*         return_code);

extern void FILEIO_STAT (/* in  */ t_fileio_uchar*           path,
                         /* in  */ t_fileio_stat*            stat,
                         /* out */ t_fileio_errno*           err_number,
                         /* out */ t_fileio_int32*           return_code);

extern void FILEIO_FSTAT (/* in  */ t_fileio_int32           fd,
                          /* in  */ t_fileio_stat*           stat,
                          /* out */ t_fileio_errno*          err_number,
                          /* out */ t_fileio_int32*          return_code);

extern void FILEIO_GETTIMEOFDAY (/* in  */ t_fileio_timeval* timeval,
                                 /* in  */ void*             timezone,
                                 /* out */ t_fileio_errno*   err_number,
                                 /* out */ t_fileio_int32*   return_code);

extern void FILEIO_ISATTY (/* in  */ t_fileio_int32          fd,
                           /* out */ t_fileio_errno*         err_number,
                           /* out */ t_fileio_int32*         return_code);

extern void FILEIO_SYSTEM (/* in  */ t_fileio_uchar*         command,
                           /* out */ t_fileio_errno*         err_number,
                           /* out */ t_fileio_int32*         return_code);

#endif /* !LIBALPHA_FILEIO_H_ */
