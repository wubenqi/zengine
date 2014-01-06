// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_console/base_console.h"

#include <stdio.h>
#include <stdarg.h>   // va_list

void LocalConsole::Write(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
}

void LocalConsole::WriteNA(const char * format) {
  printf(format);
}
