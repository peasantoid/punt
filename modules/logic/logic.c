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

REPORT_MODULE("not",
              "and",
              "or",

              "yes",
              "no",
              
              "true",
              "false");

MFUNC_PROTO(not) {
  check_argc("not", 1, args);
  return atom_true(args) ?
    NIL_ATOM : TRUE_ATOM;
}

MFUNC_PROTO(and) {
  check_argc("and", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);
  return (atom_true(arg1) && atom_true(arg2)) ?
    TRUE_ATOM : NIL_ATOM;
}

MFUNC_PROTO(or) {
  check_argc("or", 2, args);
  p_atom *arg1 = args, *arg2 = atom_getindex(args, 1);
  return (atom_true(arg1) || atom_true(arg2)) ?
    TRUE_ATOM : NIL_ATOM;
}

MFUNC_PROTO(yes) {
  check_argc("yes", 0, args);
  return TRUE_ATOM;
}

MFUNC_PROTO(no) {
  check_argc("no", 0, args);
  return NIL_ATOM;
}

MFUNC_PROTO(true) {
  check_argc("true", 1, args);
  return atom_true(args) ? TRUE_ATOM : NIL_ATOM;
}

MFUNC_PROTO(false) {
  check_argc("false", 1, args);
  return atom_true(args) ? NIL_ATOM : TRUE_ATOM;
}

