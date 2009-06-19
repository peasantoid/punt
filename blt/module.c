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

p_atom *blt_use(p_atom *args, p_atom **vars) {
  char *path;

  while(args) {
    if(args->type != P_STR) {
      fprintf(stderr, "use: all arguments must be strings\n");
      exit(1);
    }

    asprintf(&path, "%s/%s.so", MOD_DIR, basename((char *)args->value));
    if(!load_module(path, vars)) {
      fprintf(stderr, "use: %s\n", dlerror());
      exit(1);
    }

    args = (p_atom *)args->next;
  }

  return NIL_ATOM;
}

p_atom *blt_load(p_atom *args, p_atom **vars) {
  return NIL_ATOM;
}

