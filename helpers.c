/*
 * This file is part of punt.
 *
 * punt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * punt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with punt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common.h"

char **_report_module(void *nullarg, ...) {
  va_list ap;
  static char **funcs, *fname;
  static int len, i;

  len = 0;
  funcs = NULL;
  fname = "";

  va_start(ap, nullarg);
  while(1) {
    fname = va_arg(ap, char *);
    if(!fname) { break; }
    len++;
  }
  va_end(ap);

  if(!len) {
    fprintf(stderr, "module registers 0 functions\n");
    exit(1);
  }

  funcs = (char **)calloc(len + 1, sizeof(char *));

  va_start(ap, nullarg);
  for(i = 0; i < len; i++) {
    funcs[i] = va_arg(ap, char *);
  }
  va_end(ap);

  return funcs;
}

