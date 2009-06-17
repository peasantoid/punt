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

/* tokenizes the file pointed to by an open FILE* */
p_atom *tokenize_fp(FILE *fp) {
  p_atom *tokens = NULL;
  const unsigned int blocksize = 4096;
  char *str = "", *read = "";

  while(!feof(fp)) {
    read = (char *)calloc(blocksize + 1, sizeof(char));
    fgets(read, blocksize, fp);
    asprintf(&str, "%s%s", str, read);
    free(read);
  }
  
  tokens = tokenize_str(str);
  free(str);

  return tokens;
}

/* tokenizes a string */
p_atom *tokenize_str(const char *str) {
  p_atom *tokens = NULL;
  static unsigned int i, bsc;
  char *strv;

  for(i = 0; i < strlen(str); i++) {
    switch(str[i]) {
      case '#':
        for(; str[i] != '\n'; i++);
        break;
      case '(':
        atom_append(&tokens, make_atom(PT_PARENL, "", NULL));
        break;
      case ')':
        atom_append(&tokens, make_atom(PT_PARENR, "", NULL));
        break;
      case '.':
        atom_append(&tokens, make_atom(PT_QUOTE, "", NULL));
        break;
      case '\'':
        /* FIXME: string parsing code is ugly and inefficient */

        strv = "";
        bsc = 0;
        /*
         * bsc: backslash count
         * if even, quote has not been escaped
         * if odd, quote has been escaped
         */
        
        for(i++; i < strlen(str); i++) {
          if(str[i] == '\'' && bsc % 2 == 0) { break; }
          else if(str[i] == '\\') {
            asprintf(&strv, "%s\\", strv);
            bsc ++;
          } else {
            if(str[i] == 'n' && bsc % 2 > 0) {
              asprintf(&strv, "%s\n", strv);
            } else {
              asprintf(&strv, "%s%c", strv, str[i]);
            }
            bsc = 0;
          }
        }
        
        /* remove leftover backslashes */
        strv = str_replace(strv, "\\\\", "\\", 0);
        strv = str_replace(strv, "\\'", "'", 0);
        strv = str_replace(strv, "\\\n", "\n", 0);

        atom_append(&tokens, make_atom(P_STR, "", (void *)strdup(strv)));
        free(strv);
        break;
      default:
        /* it seems to be a number */
        if(isdigit(str[i]) || strchr("-", str[i])) {
          strv = (char *)calloc(2, sizeof(char));
          strv[0] = str[i];

          for(i++; isdigit(str[i]) || str[i] == '.'; i++) {
            asprintf(&strv, "%s%c", strv, str[i]);
          }
          i--; /* might've overrun something important */
          
          atom_append(&tokens, make_atom(P_NUM, "", atom_dupnum(strtod(strv, NULL))));
          free(strv);
        } else if(isalnum(str[i]) || str[i] == '_') {
          strv = (char *)calloc(2, sizeof(char));
          strv[0] = str[i];

          for(i++; isalnum(str[i]) || str[i] == '_'; i++) {
            asprintf(&strv, "%s%c", strv, str[i]);
          }
          i--;

          atom_append(&tokens, make_atom(P_SYM, "", (void *)strdup(strv)));
          free(strv);
        }
        break;
    }
  }

  return tokens;
}

