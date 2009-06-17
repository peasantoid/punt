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

/* create a function */
p_atom *blt_func(p_atom *args, p_atom **vars) {
  p_atom *rval = make_atom(P_FUNC, "", NULL);

  check_argc("func", -1, args);
  if(args->type != P_BLOCK) {
    fprintf(stderr, "func: body must be a quoted exp\n");
    exit(1);
  }
  rval->value = args->value;

  return rval;
}

/* return true */
p_atom *blt_yes(p_atom *args, p_atom **vars) {
  check_argc("yes", 0, args);
  return make_atom(P_NUM, "", atom_dupnum(1));
}

/* return false */
p_atom *blt_no(p_atom *args, p_atom **vars) {
  check_argc("yes", 0, args);
  return make_atom(P_NIL, "", NULL);
}
