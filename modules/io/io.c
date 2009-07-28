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
              "fgets",
              "fput",
              "feof",
              "io_error",
              "fgetc",
              "fgetl");

char *errstr;

MODULE_INIT {
  /* don't want a dangling pointer */
  errstr = "";
}

void seterr(const int err) {
  errstr = strerror(err);
}

MFUNC_PROTO(stdout) {
  return make_atom(P_UTYPE, NULL, make_utype("file", (void *)stdout));
}

MFUNC_PROTO(stderr) {
  return make_atom(P_UTYPE, NULL, make_utype("file", (void *)stderr));
}

MFUNC_PROTO(stdin) {
  return make_atom(P_UTYPE, NULL, make_utype("file", (void *)stdin));
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

  return make_atom(P_UTYPE, NULL, make_utype("file", (void *)fp));
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

MFUNC_PROTO(fgets) {
  check_argc("fgets", 2, args);
  check_argt("fgets", args, P_UTYPE, P_NUM, 0);
  check_argu("fgets", args, "file", "", NULL);

  static char *read, *data;
    data = "";
  static size_t readlen;
    readlen = (size_t)*(p_num *)atom_getindex(args, 1)->value;
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

  return make_atom(P_STR, NULL, (void *)data);
}

MFUNC_PROTO(fput) {
  check_argc("fput", 2, args);
  check_argt("fput", args, P_UTYPE, P_STR, 0);
  check_argu("fput", args, "file", "", NULL);

  static FILE *fp;
    fp = (FILE *)UTYPE(args)->value;
  
  if(fputs((char *)atom_getindex(args, 1)->value, fp) == EOF ||
      fflush(fp) == EOF) {
    seterr(errno);
    return NIL_ATOM;
  }

  return TRUE_ATOM;
}

MFUNC_PROTO(feof) {
  check_argc("feof", 1, args);
  check_argt("feof", args, P_UTYPE, 0);
  check_argu("feof", args, "file", NULL);

  return feof((FILE *)args->value) ?
    TRUE_ATOM : NIL_ATOM;
}

MFUNC_PROTO(io_error) {
  return make_atom(P_STR, NULL, (void *)errstr);
}

MFUNC_PROTO(fgetc) {
  check_argc("fgetc", 1, args);
  check_argt("fgetc", args, P_UTYPE, 0);
  check_argu("fgetc", args, "file", NULL);

  static FILE *fp;
    fp = (FILE *)UTYPE(args)->value;
  static int c;
    c = fgetc(fp);

  if(c == EOF) {
    if(feof(fp)) {
      return make_atom(P_STR, NULL, (void *)"");
    } else {
      seterr(errno);
      return NIL_ATOM;
    }
  }

  return make_atom(P_STR, NULL, vafmt("%c", c));
}

MFUNC_PROTO(fgetl) {
  check_argc("fgetl", 1, args);
  check_argt("fgetl", args, P_UTYPE, 0);
  check_argu("fgetl", args, "file", NULL);

  static char *read;
    read = "";
  static int c;
  static FILE *fp;
    fp = (FILE *)UTYPE(args)->value;

  while((c = fgetc(fp)) != EOF) {
    asprintf(&read, "%s%c", read, c);
    if(c == '\n') {
      break;
    }
  }
  /* got error */
  if(ferror(fp)) {
    seterr(errno);
    return NIL_ATOM;
  }

  return make_atom(P_STR, NULL, read);
}
