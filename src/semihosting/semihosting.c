#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

// Helper Macros
#define LOG_EXAMPLE(FMT, ...) printf(EXAMPLE_LOGGER_PREFIX FMT, ##__VA_ARGS__)
#define EXAMPLE_LOGGER_PREFIX "\n\x1b[1;46m Exemple " STR(__COUNTER__) " \x1b[0m "
#define STR(a) STR_(a)
#define STR_(a) #a

// Helper functions
static void print_stat(struct stat *sb);
static void panic(const char* fmt, ...);

void main(void) {
  // open
  const char* a_file = "gdb-fileio-example.svg";
  LOG_EXAMPLE("open(): create and open file `%s`\n", a_file);
  int fd = open(a_file,
                O_WRONLY | O_CREAT | O_TRUNC,
                S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
  if (fd == -1) {
    panic("open(): %s", strerror(errno));
  }

  // write
  LOG_EXAMPLE("write(): write an SVG image in `%s`\n", a_file);
  extern const char* farjump_logo_svg;
  if (write(fd, farjump_logo_svg, strlen(farjump_logo_svg)) == -1) {
    panic("write(): %s", strerror(errno));
  }

  // close
  LOG_EXAMPLE("close(): close the file descriptor of `%s`\n", a_file);
  if (close(fd) == -1) {
    panic("close(): %s", strerror(errno));
  }

  // rename
  const char* new_filename = "farjump.svg";
  LOG_EXAMPLE("rename(): rename `%s` into `%s`\n", a_file, new_filename);
  if (rename(a_file, new_filename) != 0) {
    panic("rename(): %s", strerror(errno));
  }

  // system
  LOG_EXAMPLE("system(): run a shell script to check the existence of file `%s`\n", new_filename);
  char cmd[100];
  int rc;
  rc = snprintf(cmd, sizeof (cmd), "set -x && echo using system && test -e %s", new_filename);
  if (rc >= sizeof (cmd)) {
    panic("snprintf(): shell command buffer too small");
  } else if (rc <= 0) {
    panic("snprintf(): error code `%d`", rc);
  }
  rc = system(cmd);
  if (rc != 0) {
    panic("system(): \"%s\" returned `%d`", cmd, rc);
  }

  // stat
  LOG_EXAMPLE("stat(): get and print the file status of `%s`\n", new_filename);
  struct stat sb;
  if (stat(new_filename, &sb) == -1) {
    panic("stat(): %s", strerror(errno));
  }
  print_stat(&sb);

  // open
  LOG_EXAMPLE("open(): open file `%s` in read-only mode\n", new_filename);
  fd = open(new_filename, O_RDONLY);
  if (fd == -1) {
    panic("open(): %s", strerror(errno));
  }

  // read
  LOG_EXAMPLE("read(): partly read file `%s` and compare the retrieved data with the original data\n", new_filename);
  char buffer[126];
  int n = read(fd, buffer, sizeof (buffer));
  if (n == -1) {
    panic("read(): %s", strerror(errno));
  }
  if (n == 0) {
    panic("read() returned 0 bytes");
  }
  if (strncmp(buffer, farjump_logo_svg, n) != 0) {
    panic("bytes read from file `%s` are different from the original data", new_filename);
  }

  // lseek
  LOG_EXAMPLE("lseek(): partly read file `%s` and compare the retrieved data with the original data\n", new_filename);
  off_t offset = lseek(fd, 33, SEEK_CUR);
  if (offset == -1) {
    panic("lseek(): %s", strerror(errno));
  }
  printf("file cursor moved at %d", offset);
  n = read(fd, buffer, sizeof (buffer));
  if (n == -1) {
    panic("read(): %s", strerror(errno));
  }
  if (n == 0) {
    panic("read() returned 0 bytes");
  }
  if (strncmp(buffer, &farjump_logo_svg[offset], n) != 0) {
    panic("bytes read from file `%s` are different from the original data", new_filename);
  }

  // isatty
  LOG_EXAMPLE("isatty(): check that `%s` is indeed not a tty\n", new_filename);
  if (isatty(fd) != 0) {
    panic("isatty() did not return 0: %s", strerror(errno));
  }

  // close
  LOG_EXAMPLE("close(): closing file `%s`\n", new_filename);
  if (close(fd) == -1) {
    panic("close(): %s", strerror(errno));
  }

  // isatty
  LOG_EXAMPLE("isatty(): check that stdin is a TTY\n");
  if (isatty(STDIN_FILENO) != 1) {
    panic("isatty() did not return 1: %s", strerror(errno));
  }

  // unlink
  LOG_EXAMPLE("unlink(): removing file `%s`\n", new_filename);
  if (unlink(new_filename) == -1) {
    panic("unlink(): %s", strerror(errno));
  }

  // gettimeofday
  LOG_EXAMPLE("gettimeofday(): display current date and time\n");
  struct timeval tv;
  n = gettimeofday(&tv, NULL);
  if (n != 0) {
    panic("gettimeofday(): return code `%d`", n);
  }
  time_t nowtime = tv.tv_sec;
  printf("GMT-0 time is %s\n", ctime(&nowtime));

  // File I/Os with special files
  const char* dev = "/dev/random";
  LOG_EXAMPLE("using the File I/O extension with special file `%s`\n", dev);

  printf("open(): open special file `%s`\n", dev);
  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    panic("open(): %s", strerror(errno));
  }

  printf("fstat(): get the file status of `%s`\n", dev);
  if (fstat(fd, &sb) == -1) {
    panic("fstat(): %s", strerror(errno));
  }
  print_stat(&sb);

  printf("read(): read some data from special file `%s`\n", dev);
  int x;
  n = read(fd, &x, sizeof (x));
  if (n == -1) {
    panic("read(): %s", strerror(errno));
  }
  if (n == 0) {
    panic("read() returned 0 bytes");
  }
  buffer[n] = 0;
  printf("read %d bytes `%d`", n, x);

  if (close(fd) == -1 ) {
    panic("close(): %s", strerror(errno));
  }

  // File I/Os with special files
  dev = "/dev/stdout";
  LOG_EXAMPLE("using the File I/O extension with special file `%s`\n", dev);

  printf("open(): open special file `%s`\n", dev);
  fd = open(dev, O_WRONLY);
  if (fd == -1) {
    panic("open(): %s", strerror(errno));
  }

  printf("fstat(): get the file status of `%s`\n", dev);
  if (fstat(fd, &sb) == -1) {
    panic("fstat(): %s", strerror(errno));
  }
  print_stat(&sb);

  const char data[] = "Hello, File I/O!\n";
  printf("write(): write `%s` to special file `%s`\n", data, dev);
  n = write(fd, data, sizeof (data));
  if (n == -1) {
    panic("write(): %s", strerror(errno));
  }
  if (n == 0) {
    panic("write() returned 0 bytes");
  }

  if (close(fd) == -1 ) {
    panic("close(): %s", strerror(errno));
  }

  exit(EXIT_SUCCESS);
}

static void panic(const char* fmt, ...) {
  printf("\x1b[1;41mPanic\x1b[0m ");
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
  exit(EXIT_FAILURE);
}

static void print_stat(struct stat *sb) {
  printf("File type:                ");

  switch (sb->st_mode & S_IFMT) {
  case S_IFBLK:  printf("block device\n");            break;
  case S_IFCHR:  printf("character device\n");        break;
  case S_IFDIR:  printf("directory\n");               break;
  case S_IFIFO:  printf("FIFO/pipe\n");               break;
  case S_IFLNK:  printf("symlink\n");                 break;
  case S_IFREG:  printf("regular file\n");            break;
  case S_IFSOCK: printf("socket\n");                  break;
  default:       printf("unknown?\n");                break;
  }

  printf("I-node number:            %ld\n", (long) sb->st_ino);

  printf("Device ID:                0x%lx\n", (long) sb->st_rdev);
  printf("Container Device ID:      0x%lx\n", (long) sb->st_dev);


  printf("Mode:                     %lo (octal)\n",
         (unsigned long) sb->st_mode);

  printf("Link count:               %ld\n", (long) sb->st_nlink);
  printf("Ownership:                UID=%ld   GID=%ld\n",
         (long) sb->st_uid, (long) sb->st_gid);

  printf("Preferred I/O block size: %ld bytes\n",
         (long) sb->st_blksize);
  printf("File size:                %lld bytes\n",
         (long long) sb->st_size);
  printf("Blocks allocated:         %lld\n",
         (long long) sb->st_blocks);

  printf("Last status change:       %s", ctime(&sb->st_ctime));
  printf("Last file access:         %s", ctime(&sb->st_atime));
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
}
