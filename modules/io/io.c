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

REPORT_MODULE("stdout",
              "stderr",
              "stdin",
              "fopen",
              "fclose",
              "fget",
              "fput",
              "io_error",
              NULL);

char *errstr;

MODULE_INIT {
  /* don't want a dangling pointer */
  errstr = "";
}

void seterr(const int err) {
  errstr = strerror(err);
}

MFUNC_PROTO(stdout) {
  return make_atom(P_UTYPE, "", make_utype("file", (void *)stdout));
}

MFUNC_PROTO(stderr) {
  return make_atom(P_UTYPE, "", make_utype("file", (void *)stderr));
}

MFUNC_PROTO(stdin) {
  return make_atom(P_UTYPE, "", make_utype("file", (void *)stdin));
}

MFUNC_PROTO(fopen) {
  check_argc("fopen", 2, args);
  check_argt("fopen", args, P_STR, P_STR, 0);

  char *path = (char *)args->value;
  char *mode = (char *)atom_getindex(args, 1)->value;
  FILE *fp = fopen(path, mode);

  if(!fp) {
    seterr(errno);
    return NIL_ATOM;
  }

  return make_atom(P_UTYPE, "", make_utype("file", (void *)fp));
}

MFUNC_PROTO(fclose) {
  check_argc("fclose", 1, args);
  check_argt("fclose", args, P_UTYPE, 0);
  check_argu("fclose", args, "file", NULL);

  if(fclose((FILE *)UTYPE(args)->value) == EOF) {
    seterr(errno);
    return NIL_ATOM;
  }

  return TRUE_ATOM;
}

MFUNC_PROTO(fget) {
  check_argc("fget", 2, args);
  check_argt("fget", args, P_UTYPE, P_NUM, 0);
  check_argu("fget", args, "file", "", NULL);

  static char *read, *data;
    read = NULL;
    data = "";
  static long readlen;
    readlen = (long)*(p_num *)atom_getindex(args, 1)->value;
  const int blocksize = 4096;
  FILE *fp = (FILE *)UTYPE(args)->value;

  if(readlen <= 0) {
    while(!feof(fp)) {
      read = (char *)calloc(blocksize + 1, sizeof(char));

      if(!fgets(read, blocksize, fp)) {
        seterr(errno);
        return NIL_ATOM;
      }
      asprintf(&data, "%s%s", data, read);

      free(read);
    }
  } else {
    data = (char *)calloc(readlen + 2, sizeof(char));

    if(!fgets(data, readlen + 1, fp)) {
      seterr(errno);
      return NIL_ATOM;
    }
  }

  return make_atom(P_STR, "", (void *)data);
}

MFUNC_PROTO(fput) {
  check_argc("fput", 2, args);
  check_argt("fput", args, P_UTYPE, P_STR, 0);
  check_argu("fput", args, "file", "", NULL);

  FILE *fp = (FILE *)UTYPE(args)->value;
  
  if(fputs((char *)atom_getindex(args, 1)->value, fp) == EOF ||
      fflush(fp) == EOF) {
    seterr(errno);
    return NIL_ATOM;
  }

  return TRUE_ATOM;
}

MFUNC_PROTO(io_error) {
  return make_atom(P_STR, "", (void *)errstr);
}

