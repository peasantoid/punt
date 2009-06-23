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

#define P_FILE str_hash("file")

REPORT_MODULE("stdout",
              "stderr",
              "stdin",
              "fopen",
              "fclose",
              "fget",
              "fput",
              NULL);

void seterr(p_atom **vars, int err) {
  atom_setname(vars, make_atom(P_NUM, "__io_errno", atom_dupnum(err)));
  atom_setname(vars, make_atom(P_STR, "__io_errstr", strerror(err)));
}

MFUNC_PROTO(stdout) {
  return make_atom(P_FILE, "", (void *)stdout);
}

MFUNC_PROTO(stderr) {
  return make_atom(P_FILE, "", (void *)stderr);
}

MFUNC_PROTO(stdin) {
  return make_atom(P_FILE, "", (void *)stdin);
}

MFUNC_PROTO(fopen) {
  check_argc("fopen", 2, args);

  return NIL_ATOM;
}

MFUNC_PROTO(fclose) {
  check_argc("fclose", 1, args);
  return NIL_ATOM;
}

MFUNC_PROTO(fget) {
  return NIL_ATOM;
}

MFUNC_PROTO(fput) {
  check_argc("fput", 2, args);
  if(args->type != P_FILE) {
    fprintf(stderr, "fput: arg 1 must be file\n");
    exit(1);
  } else if(atom_getindex(args, 1)->type != P_STR) {
    fprintf(stderr, "fput: arg 2 must be string\n");
    exit(1);
  }
  seterr(vars, 0);
  if(fputs((char *)atom_getindex(args, 1)->value, (FILE *)args->value) == EOF ||
      fflush((FILE *)args->value) == EOF) {
    seterr(vars, errno);
    return NIL_ATOM;
  }
  return TRUE_ATOM;
}

