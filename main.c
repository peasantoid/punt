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

int main(const int argc, const char **argv) {
  static FILE *fp;
  static struct stat finfo;
  static p_atom *vars;
    vars = NULL;
  register_builtins(&vars);
  static int c;
  static char *read;
    read = "";

  /* a file was specified */
  if(argc >= 2) {
     if(stat(argv[1], &finfo) == -1) {
       perror(argv[1]);
       return 1;
     } else if (!S_ISREG(finfo.st_mode)) {
       fprintf(stderr, "%s: Not a regular file.\n", argv[1]);
       return 1;
     }

    fp = fopen(argv[1], "r");
    if(!fp) {
      perror(argv[1]);
      return 1;
    }

    run_code(parse_tokens(tokenize_fp(fp)), &vars);
    fclose(fp);
  /* reading from stdin */
  } else {
    while(1) {
      c = fgetc(stdin);
      if(c == EOF) {
        break;
      }
      asprintf(&read, "%s%c", read, c);
    }
  }
  run_code(parse_tokens(tokenize_str(read)), &vars);

  return 0;
}

