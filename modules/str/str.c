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

REPORT_MODULE("sfmt",
              "sfind",
              "srepl",
              "ord",
              "chr",
              "ssub",
              "slen",
              "scmp");

MFUNC_PROTO(sfmt) {
  p_atom *rval = make_atom(P_STR, NULL, (void *)"");
  p_num num;

  while(args) {
    if(args->type == P_STR) {
      asprintf((char **)&rval->value, "%s%s", (char *)rval->value, (char *)args->value);
    } else if(args->type == P_NUM) {
      num = *(p_num *)args->value;
      if((long)num == num) {
        asprintf((char **)&rval->value, "%s%ld", (char *)rval->value, (long)num);
      } else {
        asprintf((char **)&rval->value, "%s%f", (char *)rval->value, num);
      }
    } else if(args->type == P_UTYPE) {
      asprintf((char **)&rval->value, "%s(%s)", (char *)rval->value, UTYPE(args)->type);
    } else {
      asprintf((char **)&rval->value, "%s(%s)", (char *)rval->value, prim_type_name(args->type));
    }
    ATOM_NEXT(args);
  }

  return rval;
}

MFUNC_PROTO(sfind) {
  p_atom *orig = args;

  check_argc("sfind", 2, args);
  while(args) {
    if(args->type != P_STR) {
      fprintf(stderr, "sfind: all args must be strings\n");
      exit(1);
    }
    ATOM_NEXT(args);
  } args = orig;

  return make_atom(P_NUM, NULL, atom_dupnum(str_pos(
          (char *)args->value, (char *)atom_getindex(args, 1)->value, 0)));
}

MFUNC_PROTO(srepl) {
  p_atom *orig = args;

  check_argc("srepl", 3, args);
  while(args) {
    if(args->type != P_STR) {
      fprintf(stderr, "srepl: all args must be strings\n");
      exit(1);
    }
    ATOM_NEXT(args);
  } args = orig;

  return make_atom(P_STR, NULL, str_replace(
          (char *)args->value, (char *)atom_getindex(args, 1)->value,
          (char *)atom_getindex(args, 2)->value, 0));
}

MFUNC_PROTO(ord) {
  check_argc("ord", 1, args);
  check_argt("ord", args, P_STR, 0);

  return make_atom(P_NUM, NULL, atom_dupnum(*(char *)args->value));
}

MFUNC_PROTO(chr) {
  check_argc("chr", 1, args);
  check_argt("chr", args, P_NUM, 0);

  return make_atom(P_STR, NULL, vafmt("%c", (char)*(p_num *)args->value));
}

MFUNC_PROTO(ssub) {
  check_argc("ssub", 3, args);
  check_argt("ssub", args, P_STR, P_NUM, P_NUM, 0);

  static size_t start, len, i, count;
    start = (size_t)abs(*(p_num *)atom_getindex(args, 1)->value);
    len = (size_t)abs(*(p_num *)atom_getindex(args, 2)->value);
  static char *str, *rval;
    str = (char *)args->value;
    rval = (char *)calloc(len + 1, sizeof(char));
  for(i = start, count = 0;
      i < strlen(str) && count < len;
      i++, count++) {
    rval[count] = str[i];
  }

  return make_atom(P_STR, NULL, (void *)rval);
}

MFUNC_PROTO(slen) {
  check_argc("slen", 1, args);
  check_argt("slen", args, P_STR, 0);

  return make_atom(P_NUM, NULL, atom_dupnum(strlen((char *)args->value)));
}


MFUNC_PROTO(scmp) {
  #define RETNUM(n) return make_atom(P_NUM, NULL, atom_dupnum(n))

  check_argc("scmp", 2, args);
  check_argt("scmp", args, P_STR, P_STR, 0);

  static char *str1, *str2;
    str1 = (char *)args->value;
    str2 = (char *)atom_getindex(args, 1)->value;
  static size_t i, len1, len2;
    len1 = strlen(str1);
    len2 = strlen(str2);

  if(strcmp(str1, str2)) {
    for(i = 0;
        i < len1 && i < len2;
        ++i) {
      if(str1[i] < str2[i]) {
        RETNUM(1);
      } else if(str1[i] > str2[i]) {
        RETNUM(2);
      }
    }
    if(len1 < len2) {
      RETNUM(1);
    } else if(len1 > len2) {
      RETNUM(2);
    }
  }

  RETNUM(0);

  #undef RETNUM
}
