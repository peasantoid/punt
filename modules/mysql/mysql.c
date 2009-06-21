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

#include "../../common.h"
#include <mysql.h>

#define P_MYSQL str_hash("mysql")

MFUNC_REPORT {
  char **funcs = (char **)calloc(3, sizeof(char *));

  funcs[0] = "mysql_connect";

  return funcs;
}

MFUNC_PROTO(mysql_connect) {
  check_argc("mysql_connect", 4, args);
  p_atom *orig = args;
  while(args) {
    if(args->type != P_STR) {
      fprintf(stderr, "mysql_connect: all arguments must be strings\n");
      exit(1);
    }
    ATOM_NEXT(args);
  } args = orig;
  
  MYSQL *rval = mysql_init(NULL);

  return make_atom(P_MYSQL, "", (void *)rval);
}

