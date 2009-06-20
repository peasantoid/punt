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

MFUNC_REPORT {
  char **funcs = (char **)calloc(7, sizeof(char *));

  funcs[0] = "eq";
  funcs[1] = "ne";
  funcs[2] = "lt";
  funcs[3] = "gt";
  funcs[4] = "le";
  funcs[5] = "ge";

  return funcs;
}

MFUNC_PROTO(eq) {
  check_argc("eq", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value == *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return !strcmp((char *)arg1->value, (char *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value == arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

MFUNC_PROTO(ne) {
  check_argc("ne", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value != *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return strcmp((char *)arg1->value, (char *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value != arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

MFUNC_PROTO(lt) {
  check_argc("lt", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value < *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return (strlen((char *)arg1->value) < strlen((char *)arg2->value)) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value < arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

MFUNC_PROTO(gt) {
  check_argc("gt", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value > *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return (strlen((char *)arg1->value) > strlen((char *)arg2->value)) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value > arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

MFUNC_PROTO(le) {
  check_argc("le", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value <= *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return (strlen((char *)arg1->value) <= strlen((char *)arg2->value)) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value <= arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

MFUNC_PROTO(ge) {
  check_argc("ge", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);

  if(arg1->type == P_NUM && arg2->type == P_NUM) {
    return (*(p_num *)arg1->value >= *(p_num *)arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  } else if(arg1->type == P_STR && arg2->type == P_STR) {
    return (strlen((char *)arg1->value) >= strlen((char *)arg2->value)) ?
      TRUE_ATOM : NIL_ATOM;
  } else {
    return (arg1->value >= arg2->value) ?
      TRUE_ATOM : NIL_ATOM;
  }
}

