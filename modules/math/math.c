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
  char **funcs = (char **)calloc(3, sizeof(char *));

  funcs[0] = "add";
  funcs[1] = "sub";

  return funcs;
}

p_atom *punt_add(p_atom *args, p_atom **vars) {
  check_argc("add", 2, args);
  if(args->type != P_NUM || atom_getindex(args, 1)->type != P_NUM) {
    fprintf(stderr, "add: arguments must be numbers\n");
  }
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value + *(p_num *)atom_getindex(args, 1)->value));
}

p_atom *punt_sub(p_atom *args, p_atom **vars) {
  check_argc("sub", 2, args);
  if(args->type != P_NUM || atom_getindex(args, 1)->type != P_NUM) {
    fprintf(stderr, "sub: arguments must be numbers\n");
  }
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value - *(p_num *)atom_getindex(args, 1)->value));
}

