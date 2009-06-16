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
  static unsigned int i;
  static FILE *fp;
  p_atom *tokens = NULL,
         *vars =   NULL;
  static struct stat finfo;

  for(i = 1; i < argc; i++) {
    /*
     * check for directory: code contributed by vkumar
     * for some reason, we're allowed to fopen directories
     */
    if(lstat(argv[i], &finfo) == -1 || S_ISDIR(finfo.st_mode)) {
      if(S_ISDIR(finfo.st_mode)) {
        fprintf(stderr, "%s: Is a directory\n", argv[i]);
      } else {
        perror(argv[i]);
      }
      return 1;
    }

    fp = fopen(argv[i], "r");
    if(!fp) {
      perror(argv[i]);
      return 1;
    }

    tokens = tokenize_fp(fp);
    run_tokens(tokens, vars);

    fclose(fp);
  }
  printf(MOD_DIR "\n");

  return 0;
}

