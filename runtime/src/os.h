#ifndef TI_RUNTIME_OS_H
#define TI_RUNTIME_OS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

// os - operating system abstractions

// Aliases of platform specific file access modes.
enum os_fs_access_modes {
  OS_FS_RDONLY = O_RDONLY,
  OS_FS_WRONLY = O_WRONLY,
  OS_FS_RDWR = O_RDWR
};

enum os_fs_type {
  OS_FS_REGULAR,
  OS_FS_DIRECTORY,
  OS_FS_UNKNOWN
};

struct os_fs_info {
  int size;            // total size of file in bytes
  int type;            // type of file (regular, directory, etc)
  time_t last_access;  // time of last access
  time_t last_mod;     // time of last modification
  time_t last_change;  // time of last change
};

// Open a file at the path and return a file descriptor.
int os_fs_open(const char* path, int access_mode);

int os_fs_close(int fd);

// Query information about a file from an opened descriptor.
int os_fs_get_info(int fd, struct os_fs_info* info);
int os_fs_get_info_path(const char* path, struct os_fs_info* info);

// Reads from a file descriptor into the provided data buffer.
// The number of bytes read will be less than or equal to length.
// Returns the number of bytes read or -1 on error.
int os_fs_read(int fd, void* data, int offset, int length);

#endif
