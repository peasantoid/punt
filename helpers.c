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

char **_report_module(const void *nullarg, ...) {
  va_list ap;
  static char **funcs, *fname;
  static int len, i;

  len = 0;
  funcs = NULL;
  fname = "";

  va_start(ap, nullarg);
  while(1) {
    fname = va_arg(ap, char *);
    if(!fname) { break; }
    len++;
  }
  va_end(ap);

  if(!len) {
    fprintf(stderr, "module registers 0 functions\n");
    exit(1);
  }

  funcs = (char **)calloc(len + 1, sizeof(char *));

  va_start(ap, nullarg);
  for(i = 0; i < len; i++) {
    funcs[i] = va_arg(ap, char *);
  }
  va_end(ap);

  return funcs;
}

void func_err(const char *func, const char *msg) {
  fprintf(stderr, "%s: %s\n", func, msg);
  exit(1);
}

void check_argc(const char *func, const int minlen, p_atom *args) {
  /* must be exactly minlen args */
  if(minlen >= 0) {
    if(atom_len(args) != minlen) {
      func_err(func, vafmt("exactly %d %s required, %d given", minlen,
          minlen == 1 ? "argument" : "arguments", atom_len(args)));
      exit(1);
    }
  /* must be at least minlen args */
  } else {
    if(atom_len(args) < abs(minlen)) {
      func_err(func, vafmt("at least %d %s required, %d given", abs(minlen),
          minlen == -1 ? "argument" : "arguments", atom_len(args)));
      exit(1);
    }
  }
}

/*
 * check argument types (primitive)
 *
 * 0 to end list
 */
void check_argt(const char *func, p_atom *args, ...) {
  va_list ap;
  static p_type type;
    type = 0;
  static size_t i;
    i = 0;

  va_start(ap, args);
  while(1) {
    type = va_arg(ap, p_type);
    if(type == 0) {
      break;
    }
    
    if(args->type != type && type != P_ANY) {
      func_err(func, vafmt("incorrect type '%s' for argument %ld (should be '%s')", 
            prim_type_name(args->type), i + 1, prim_type_name(type)));
    }

    ATOM_NEXT(args);
    i++;
  }
  va_end(ap);
}

/*
 * check argument types (user-defined)
 *
 * "" to skip, NULL to end list
 */
void check_argu(const char *func, p_atom *args, ...) {
  va_list ap;
  static char *type;
    type = "";
  static size_t i;
    i = 0;

  va_start(ap, args);
  while(1) {
    type = va_arg(ap, char *);
    /*
     * order of these two is very important, as strlen() segfaults
     * if NULL
     *
     */
    if(!type) { break; }
    else if(strlen(type) == 0) { continue; }

    if(args->type != P_UTYPE || strcmp(UTYPE(args)->type, type)) {
      func_err(func, vafmt("incorrect type '%s' for argument %ld (should be '%s')",
            (args->type != P_UTYPE) ? prim_type_name(args->type) : UTYPE(args)->type,
            i + 1, type));
    }

    ATOM_NEXT(args);
    i++;
  }
}

/* get primitive type name */
char *prim_type_name(p_type type) {
  switch(type) {
    case P_NIL:
      return "nil";
      break;
    case P_NUM:
      return "number";
      break;
    case P_STR:
      return "string";
      break;
    case P_LIST:
      return "list";
      break;
    case P_SYM:
      return "symbol";
      break;
    case P_BLOCK:
      return "block";
      break;
    case P_MFUNC:
      return "module_function";
      break;
    case P_FUNC:
      return "function";
      break;
    case P_UTYPE:
      return "custom";
      break;
    default:
      return "undefined_type";
      break;
  }
}

