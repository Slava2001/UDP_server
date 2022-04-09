#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdio.h>

extern int debug_level;
#define log_error(fmt, ...) fprintf(stderr, "%s(): "fmt, __func__, ##__VA_ARGS__)
#define log_info(fmt, ...) fprintf(stderr, "%s(): "fmt, __func__, ##__VA_ARGS__)
#define debug_log(lvl, fmt, ...) if(debug_level >= lvl) log_info(fmt, ##__VA_ARGS__)

#endif // INCLUDE_UTIL_H