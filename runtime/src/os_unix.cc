#include "os.h"

#include <stdio.h>
#include <unistd.h>

int os_fs_open(const char* path, int access_mode) {
  return open(path, access_mode);
}

int os_fs_close(int fd) {
  return close(fd);
}

static void stat_to_info(struct stat* stat, struct os_fs_info* info) {
  int type;
  switch(stat->st_mode & S_IFMT) {
   case S_IFREG:
     type = OS_FS_REGULAR;
     break;
   case S_IFDIR:
     type = OS_FS_DIRECTORY;
     break;
   default:
     type = OS_FS_UNKNOWN;
  }

  info->size = stat->st_size;
  info->type = type;
  info->last_access = stat->st_atime;
  info->last_mod = stat->st_mtime;
  info->last_change = stat->st_ctime;
}

int os_fs_get_info(int fd, struct os_fs_info* info) {
  struct stat st;
  if (fstat(fd, &st)) {
    return -1;
  }
  stat_to_info(&st, info);
  return 0;
}

int os_fs_get_info_path(const char* path, struct os_fs_info* info) {
  struct stat st;
  if (stat(path, &st)) {
    return -1;
  }
  stat_to_info(&st, info);
  return 0;
}

int os_fs_read(int fd, void* data, int offset, int length) {
  int bytes_read = pread(fd, data, length, offset);
  if (bytes_read == -1) {
    perror("last error");
  }
  return bytes_read;
}

