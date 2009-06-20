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

char **_punt_report_funcs(void) {
  char **funcs = (char **)calloc(4, sizeof(char *));

  funcs[0] = "sfmt";
  funcs[1] = "sfind";
  funcs[2] = "srepl";

  return funcs;
}

MFUNC_PROTO(sfmt) {
  p_atom *rval = make_atom(P_STR, "", (void *)"");

  while(args) {
    switch(args->type) {
      case P_STR:
        asprintf((char **)&rval->value, "%s%s", (char *)rval->value, (char *)args->value);
        break;
      case P_NUM:
        asprintf((char **)&rval->value, "%s%f", (char *)rval->value, *(p_num *)args->value);
        break;
      default:
        asprintf((char **)&rval->value, "%s?", (char *)rval->value);
        break;
    }
    ATOM_NEXT(args);
  }

  return rval;
}

MFUNC_PROTO(sfind) {
  return NIL_ATOM;
}

MFUNC_PROTO(srepl) {
  return NIL_ATOM;
}

