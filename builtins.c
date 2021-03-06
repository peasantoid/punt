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

/* load module */
p_atom *blt_use(p_atom *args, p_atom **vars) {
  static char *path;
  static size_t i;
    i = 1;

  while(args) {
    if(args->type != P_STR) {
      func_err("use", vafmt("non-string argument %ld", i));
    }

    asprintf(&path, "%s/%s.so", MOD_DIR, basename((char *)args->value));
    if(!load_module(path, vars)) {
      fprintf(stderr, "use: %s\n", dlerror());
      exit(1);
    }

    free(path);
    ATOM_NEXT(args);
    i++;
  }

  return NIL_ATOM;
}

/* register built-in variables */
void register_builtins(p_atom **vars) {
  atom_setname(vars, make_atom(P_MFUNC, "use", &blt_use));
}
