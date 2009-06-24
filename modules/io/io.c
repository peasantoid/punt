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
              "io_error",
              NULL);

char *errstr;

/* don't want any dangling pointers */
MODULE_INIT {
  errstr = "";
}

void seterr(p_atom **vars, const int err) {
  errstr = strerror(err);
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
  check_argt("fopen", args, P_STR, P_STR, 0);

  char *path = (char *)args->value;
  char *mode = (char *)atom_getindex(args, 1)->value;
  FILE *fp = fopen(path, mode);

  if(!fp) {
    seterr(vars, errno);
    return NIL_ATOM;
  }

  return make_atom(P_FILE, "", (void *)fp);
}

MFUNC_PROTO(fclose) {
  check_argc("fclose", 1, args);
  check_argt("fclose", args, P_FILE, 0);

  if(fclose((FILE *)args->value) == EOF) {
    seterr(vars, errno);
    return NIL_ATOM;
  }

  return TRUE_ATOM;
}

MFUNC_PROTO(fget) {
  check_argc("fget", 2, args);
  check_argt("fget", args, P_FILE, P_NUM, 0);
  static char *read, *data;
    read = NULL;
    data = "";
  static long readlen;
    readlen = (long)*(p_num *)atom_getindex(args, 1)->value;
  const int blocksize = 4096;
  FILE *fp = (FILE *)args->value;

  if(readlen <= 0) {
    while(!feof(fp)) {
      read = (char *)calloc(blocksize + 1, sizeof(char));

      fgets(read, blocksize, fp);
      asprintf(&data, "%s%s", data, read);

      free(read);
    }
  } else {
    data = (char *)calloc(readlen + 2, sizeof(char));

    fgets(data, readlen + 1, fp);
  }

  return make_atom(P_STR, "", (void *)data);
}

MFUNC_PROTO(fput) {
  check_argc("fput", 2, args);
  check_argt("fput", args, P_FILE, P_STR, 0);
  
  if(fputs((char *)atom_getindex(args, 1)->value, (FILE *)args->value) == EOF ||
      fflush((FILE *)args->value) == EOF) {
    seterr(vars, errno);
    return NIL_ATOM;
  }

  return TRUE_ATOM;
}

MFUNC_PROTO(io_error) {
  return make_atom(P_STR, "", (void *)errstr);
}

