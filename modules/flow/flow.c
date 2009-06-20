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

MFUNC_REPORT {
  char **funcs = (char **)calloc(4, sizeof(char *));

  funcs[0] = "func";
  funcs[1] = "cond";
  funcs[2] = "last";

  return funcs;
}

/* create a function */
MFUNC_PROTO(func) {
  p_atom *rval = make_atom(P_FUNC, "", NULL);

  check_argc("func", 1, args);
  if(args->type != P_BLOCK) {
    fprintf(stderr, "func: body must be quoted exp\n");
    exit(1);
  }
  rval->value = args->value;

  return rval;
}

/* like Lisp's */
MFUNC_PROTO(cond) {
  while(args) {
    if(args->type != P_BLOCK) {
      fprintf(stderr, "cond: clause must be block\n");
      exit(1);
    } else if(atom_len((p_atom *)args->value) != 2) {
      fprintf(stderr, "cond: clause must contain exactly 2 exps\n");
      exit(1);
    }

    if(atom_true(run_exp((p_atom *)args->value, vars))) {
      return run_exp(atom_getindex((p_atom *)args->value, 1), vars);
    }

    args = (p_atom *)args->next;
  }
  return NIL_ATOM;
}

/* returns value of last argument */
MFUNC_PROTO(last) {
  return atom_tail(args);
}

