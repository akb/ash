#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "debug.h"

bool flag_debug = false;

void log_debug(char* format, ...) {
  if (flag_debug == false) return;
  va_list args;
  va_start(args, format);
  fprintf(stderr, "DEBUG: ");
  vfprintf(stderr, format, args);
  fputc('\n', stderr);
  va_end(args);
}