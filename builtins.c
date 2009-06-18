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
  atom_setname(&vars, make_atom(P_MFUNC, "yes", &blt_yes));
  atom_setname(&vars, make_atom(P_MFUNC, "no", &blt_no));
  atom_setname(&vars, make_atom(P_MFUNC, "fmt", &blt_fmt));
  atom_setname(&vars, make_atom(P_MFUNC, "prt", &blt_prt));

  return vars;
}

/* return true */
p_atom *blt_yes(p_atom *args, p_atom **vars) {
  check_argc("yes", 0, args);
  return make_atom(P_NUM, "", atom_dupnum(1));
}

/* return false */
p_atom *blt_no(p_atom *args, p_atom **vars) {
  check_argc("no", 0, args);
  return NIL_ATOM;
}

/* intelligently format args into string form */
p_atom *blt_fmt(p_atom *args, p_atom **vars) {
  char *rval = "";

  while(args) {
    if(args->type == P_NUM) {
      asprintf(&rval, "%s%f", rval, *(p_num *)args->value);
    } else if(args->type == P_STR) {
      asprintf(&rval, "%s%s", rval, (char *)args->value);
    }
    args = (p_atom *)args->next;
  }

  return make_atom(P_STR, "", (void *)rval);
}

/* print a string */
p_atom *blt_prt(p_atom *args, p_atom **vars) {
  check_argc("prt", -1, args);
  if(args->type != P_STR) {
    fprintf(stderr, "prt: string required\n");
    exit(1);
  }
  
  printf("%s", (char *)args->value);

  return NIL_ATOM;
}

