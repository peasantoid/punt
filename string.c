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

/* like (a)sprintf, but return the result */
char *vafmt(const char *fmt, ...) {
  va_list ap;
  char *result;

  va_start(ap, fmt);
  vasprintf(&result, fmt, ap);
  va_end(ap);

  return result;
}

/* find position of substring */
int str_pos(const char *target, const char *search, unsigned int start) {
  char *pos;

  if(strlen(search) == 0) { return -1; }
  pos = strstr(target + start, search);
  if(!pos) { return -1; }

  return pos - target;
}

/* 
 * replace substring
 *
 * This function is probably f'ing hideous.
 */
char *str_replace(const char *target, const char *search, const char *replace,
unsigned int start) {
  char *result, *left, *right;
  int pos;

  asprintf(&result, "%s", target);
  for(; (pos = str_pos(result, search, start)) != -1; ) {
    left = (char *)calloc(start + pos + 1, sizeof(char));
    right = (char *)calloc((strlen(result) - pos - strlen(search)) + 1, sizeof(char));

    strncpy(left, result, pos);
    strncpy(right, result + pos + strlen(search), strlen(result) - pos - strlen(search));
    asprintf(&result, "%s%s%s", left, replace, right);

    free(left);
    free(right);

    start = pos + strlen(replace);
  }

  return result;
}

