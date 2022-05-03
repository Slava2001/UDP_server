#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define NONE "\033[0m"

extern int debug_level;
#define log_error(fmt, ...) fprintf(stderr, "%s(): "RED fmt NONE"\n", __func__, ##__VA_ARGS__)
#define log_perror(fmt, ...) fprintf(stderr, "%s(): "RED fmt" Errno: %d(%s)"NONE"\n", __func__, ##__VA_ARGS__, errno, strerror(errno))
#define log_info(fmt, ...) fprintf(stderr, "%s(): "fmt"\n", __func__, ##__VA_ARGS__)
#define log_debug(lvl, fmt, ...) if(debug_level >= lvl) log_info(fmt, ##__VA_ARGS__)
#define log_raw(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) fprintf(stderr, "%s(): "YELLOW fmt NONE"\n", __func__, ##__VA_ARGS__)

#endif // INCLUDE_UTIL_H
