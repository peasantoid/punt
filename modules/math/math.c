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
#include <math.h>

REPORT_MODULE("add",
              "sub",
              "mul",
              "div",
              "mod",
              "pow",

              "abs",
              "floor",
              "ceil");

void check_types(p_atom *args, char *func) {
  while(args) {
    if(args->type != P_NUM) {
      fprintf(stderr, "%s: arguments must be numeric\n", func);
      exit(1);
    }
    args = (p_atom *)args->next;
  }
}

MFUNC_PROTO(add) {
  check_argc("add", 2, args);
  check_types(args, "add");
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value + *(p_num *)atom_getindex(args, 1)->value));
}

MFUNC_PROTO(sub) {
  check_argc("sub", 2, args);
  check_types(args, "sub");
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value - *(p_num *)atom_getindex(args, 1)->value));
}

MFUNC_PROTO(mul) {
  check_argc("mul", 2, args);
  check_types(args, "mul");
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value * *(p_num *)atom_getindex(args, 1)->value));
}

MFUNC_PROTO(div) {
  check_argc("div", 2, args);
  check_types(args, "div");
  return make_atom(P_NUM, "", atom_dupnum(
        *(p_num *)args->value / *(p_num *)atom_getindex(args, 1)->value));
}

MFUNC_PROTO(mod) {
  check_argc("mod", 2, args);
  check_types(args, "mod");
  return make_atom(P_NUM, "", atom_dupnum(
        fmod(*(p_num *)args->value, *(p_num *)atom_getindex(args, 1)->value)));
}

MFUNC_PROTO(pow) {
  check_argc("pow", 2, args);
  check_types(args, "pow");
  return make_atom(P_NUM, "", atom_dupnum(
        pow(*(p_num *)args->value, *(p_num *)atom_getindex(args, 1)->value)));
}

MFUNC_PROTO(abs) {
  check_argc("abs", 1, args);
  check_types(args, "abs");
  return make_atom(P_NUM, "", atom_dupnum(
        fabs(*(p_num *)args->value)));
}

MFUNC_PROTO(floor) {
  check_argc("floor", 1, args);
  check_types(args, "floor");
  return make_atom(P_NUM, "", atom_dupnum(
        floor(*(p_num *)args->value)));
}

MFUNC_PROTO(ceil) {
  check_argc("ceil", 1, args);
  check_types(args, "ceil");
  return make_atom(P_NUM, "", atom_dupnum(
        ceil(*(p_num *)args->value)));
}

