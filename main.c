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
  static char *read, *str;
    str = "";
  static int blocksize;
    blocksize = 4096;

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
      read = (char *)calloc(blocksize + 1, sizeof(char));
      if(!fgets(read, blocksize, stdin)) {
        break;
      }
      asprintf(&str, "%s%s", str, read);
      free(read);
    }
    run_code(parse_tokens(tokenize_str(str)), &vars);
  }

  return 0;
}

