#include "os.h"

#include <stdio.h>
#include <unistd.h>

int os_fs_open(const char* path, int access_mode) {
  return open(path, access_mode);
}

int os_fs_close(int fd) {
  return close(fd);
}

int os_fs_get_info(int fd, struct os_fs_info* info) {
  struct stat stat;
  if (fstat(fd, &stat)) {
    return -1;
  }

  info->size = stat.st_size;
  info->last_access = stat.st_atime;
  info->last_mod = stat.st_mtime;
  info->last_change = stat.st_ctime;

  return 0;
}

int os_fs_read(int fd, void* data, int offset, int length) {
  int bytes_read = pread(fd, data, length, offset);
  if (bytes_read == -1) {
    perror("last error");
  }
  return bytes_read;
}

