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

/* return true */
p_atom *punt_yes(p_atom *args, p_atom **vars) {
  check_argc("yes", 0, args);
  return make_atom(P_NUM, "", atom_dupnum(1));
}

/* return false */
p_atom *punt_no(p_atom *args, p_atom **vars) {
  check_argc("no", 0, args);
  return NIL_ATOM;
}

/* intelligently format args into string form */
p_atom *punt_fmt(p_atom *args, p_atom **vars) {
  char *rval = "";

  while(args) {
    if(args->type == P_NUM) {
      asprintf(&rval, "%s%f", rval, *(p_num *)args->value);
    } else if(args->type == P_STR) {
      asprintf(&rval, "%s%s", rval, (char *)args->value);
    } else {
      asprintf(&rval, "%s?", rval);
    }
    args = (p_atom *)args->next;
  }

  return make_atom(P_STR, "", (void *)rval);
}

/* print a string */
p_atom *punt_prt(p_atom *args, p_atom **vars) {
  check_argc("prt", 1, args);
  if(args->type != P_STR) {
    fprintf(stderr, "prt: string required\n");
    exit(1);
  }
  
  printf("%s", (char *)args->value);
  fflush(stdout);

  return NIL_ATOM;
}

/* return type number */
p_atom *punt_typenum(p_atom *args, p_atom **vars) {
  check_argc("typenum", 1, args);
  return make_atom(P_NUM, "", atom_dupnum(args->type));
}
