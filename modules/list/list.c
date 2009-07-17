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

/*
 * This is ugly. Feel free to clean it up (or rewrite it)
 * and submit a patch.
 */

#include "../../common.h"

REPORT_MODULE("list",
              "llen",
              "lpos",
              "lkey");

MFUNC_PROTO(list) {
  static p_atom **rval;
    rval = (p_atom **)calloc(1, sizeof(p_atom *));
    *rval = NULL;

  while(args) {
    atom_append(rval, make_atom(args->type, "", args->value));
    ATOM_NEXT(args);
  }
  return make_atom(P_LIST, "", (void *)rval);
}

MFUNC_PROTO(llen) {
  check_argc("llen", 1, args);
  check_argt("llen", args, P_LIST, 0);
  return make_atom(P_NUM, "", atom_dupnum(atom_len(*(p_atom **)args->value)));
}

MFUNC_PROTO(lpos) {
  check_argc("lpos", 2, args);
  check_argt("lpos", args, P_LIST, P_NUM, 0);

  static p_atom *list, *rval;
    list = *(p_atom **)args->value;
  static size_t listlen;
    listlen = atom_len(list);
  static long pos;
    pos = *(p_num *)atom_getindex(args, 1)->value;

  if(pos < 0 || pos >= listlen) {
    func_err("lpos",
        vafmt("index %ld out of range in list with length of %ld", pos, listlen));
  }
  rval = atom_getindex(list, pos);
  return make_atom(rval->type, "", rval->value);
}

MFUNC_PROTO(lkey) {
  check_argc("lkey", 2, args);
  check_argt("lkey", args, P_LIST, P_STR, 0);

  static char *key;
    key = (char *)atom_getindex(args, 1)->value;
  static p_atom *rval;
    rval = atom_getname(*(p_atom **)args->value, key);

  if(!rval) {
    func_err("lkey", vafmt("key '%s' not in list", key));
  }
  return make_atom(rval->type, "", rval->value);
}
