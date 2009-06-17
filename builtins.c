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
#include "blt/flow.c"

p_atom *register_builtins(void) {
  p_atom *vars = NULL;

  atom_setname(&vars, make_atom(P_MFUNC, "func", &blt_func));
  atom_setname(&vars, make_atom(P_MFUNC, "cond", &blt_cond));
  atom_setname(&vars, make_atom(P_MFUNC, "yes", &blt_func));
  atom_setname(&vars, make_atom(P_MFUNC, "no", &blt_cond));

  return vars;
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
