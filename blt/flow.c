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

/* create a function */
p_atom *blt_func(p_atom *args, p_atom **vars) {
  p_atom *rval = make_atom(P_FUNC, "", NULL);

  check_argc("func", -1, args);
  if(args->type != P_BLOCK) {
    fprintf(stderr, "func: body must be quoted exp\n");
    exit(1);
  }
  rval->value = args->value;

  return rval;
}

/* like Lisp's */
p_atom *blt_cond(p_atom *args, p_atom **vars) {
  while(args) {
    if(args->type != P_BLOCK) {
      fprintf(stderr, "cond: clause must be block\n");
      exit(1);
    } else if(atom_len((p_atom *)args->value) != 2) {
      fprintf(stderr, "cond: clause must contain exactly 2 exps\n");
      exit(1);
    }

    if(atom_true(run_exp(atom_getindex((p_atom *)args->value, 0), vars))) {
      return run_exp(atom_getindex((p_atom *)args->value, 1), vars);
    }

    args = (p_atom *)args->next;
  }
  return NIL_ATOM;
}

/* returns value of last argument */
p_atom *blt_last(p_atom *args, p_atom **vars) {
  p_atom *atom = atom_tail(args);
  if(!atom) { return NIL_ATOM; }
  return atom;
}

