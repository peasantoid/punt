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
              "lkey",
              "lcat",
              "lsetpos",
              "lsetkey",
              "lappend",
              "lwalk",
              "lcopy",
              "ldelpos",
              "ldelkey");

char *outofrange(const size_t pos, p_atom *list) {
  return vafmt("index %ld out of range in list with length of %ld", pos, atom_len(list));
}

MFUNC_PROTO(list) {
  static p_atom **rval;
    rval = (p_atom **)calloc(1, sizeof(p_atom *));
    *rval = NULL;

  while(args) {
    atom_append(rval, make_atom(args->type, NULL, args->value));
    ATOM_NEXT(args);
  }
  return make_atom(P_LIST, NULL, (void *)rval);
}

MFUNC_PROTO(llen) {
  check_argc("llen", 1, args);
  check_argt("llen", args, P_LIST, 0);
  return make_atom(P_NUM, NULL, atom_dupnum(atom_len(*(p_atom **)args->value)));
}

MFUNC_PROTO(lpos) {
  check_argc("lpos", 2, args);
  check_argt("lpos", args, P_LIST, P_NUM, 0);

  static p_atom *list, *rval;
    list = *(p_atom **)args->value;
  static size_t pos;
    pos = (size_t)*(p_num *)atom_getindex(args, 1)->value;

  if(pos < 0 || pos >= atom_len(list)) {
    func_err("lpos", outofrange(pos, list));
  }
  rval = atom_getindex(list, pos);
  return make_atom(rval->type, NULL, rval->value);
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
  return make_atom(rval->type, NULL, rval->value);
}

MFUNC_PROTO(lcat) {
  static p_atom *_args;
    _args = args;
  while(_args) {
    if(_args->type != P_LIST) {
      func_err("lcat", "can't concatenate non-list to list");
    }
    ATOM_NEXT(_args);
  }

  static p_atom **dest_list, *src_list;
    dest_list = (p_atom **)calloc(1, sizeof(p_atom *));
    *dest_list = NULL;
    
  _args = args;
  while(_args) {
    src_list = *(p_atom **)_args->value;
    while(src_list) {
      atom_append(dest_list, make_atom(src_list->type,
            src_list->name, /* preserve name? */
            src_list->value));
      ATOM_NEXT(src_list);
    }
    ATOM_NEXT(_args);
  }

  return make_atom(P_LIST, NULL, (void *)dest_list);
}

MFUNC_PROTO(lsetpos) {
  check_argc("lsetpos", 3, args);
  check_argt("lsetpos", args, P_LIST, P_NUM, P_ANY, 0);

  static p_atom *list, *target, *new;
    list = *(p_atom **)args->value;
    new = atom_getindex(args, 2);
  static size_t pos;
    pos = (size_t)*(p_num *)atom_getindex(args, 1)->value;

  if(pos < 0 || pos >= atom_len(list)) {
    func_err("lsetpos", outofrange(pos, list));
  }
  target = atom_getindex(list, pos);
  target->type = new->type;
  target->name = NULL; /* preserve name? */
  target->value = new->value;

  return NIL_ATOM;
}

MFUNC_PROTO(lsetkey) {
  check_argc("lsetkey", 3, args);
  check_argt("lsetkey", args, P_LIST, P_STR, P_ANY, 0);

  static char *key;
    key = (char *)atom_getindex(args, 1)->value;
  static p_atom **list, *new;
    list = (p_atom **)args->value;
    new = atom_getindex(args, 2);

  atom_setname(list, make_atom(new->type, key, new->value));

  return NIL_ATOM;
}

MFUNC_PROTO(lappend) {
  check_argc("lappend", -1, args);
  check_argt("lappend", args, P_LIST, 0);

  static p_atom **list;
    list = (p_atom **)args->value;
  for(ATOM_NEXT(args);
      args;
      ATOM_NEXT(args)) {
    atom_append(list, make_atom(args->type, NULL, args->value));
  }

  return NIL_ATOM;
}

MFUNC_PROTO(lwalk) {
  check_argc("lwalk", 3, args);
  check_argt("lwalk", args, P_LIST, P_SYM, P_BLOCK, 0);

  static p_atom *list, *exp, *rval;
    list = *(p_atom **)args->value;
    exp = make_atom(PT_EXP, NULL, atom_getindex(args, 2)->value);
    rval = NIL_ATOM;
  static char *name;
    name = (char *)atom_getindex(args, 1)->value;

  while(list) {
    atom_setname(vars, make_atom(list->type, name, list->value));
    rval = run_exp(exp, vars);
    ATOM_NEXT(list);
  }

  return rval;
}

MFUNC_PROTO(lcopy) {
  check_argc("lcopy", 1, args);
  check_argt("lcopy", args, P_LIST, 0);

  static p_atom **new;
    new = ALLOC(1, p_atom *);
    *new = atom_dup(*(p_atom **)args->value);

  return make_atom(P_LIST, NULL, new);
}

MFUNC_PROTO(ldelpos) {
  check_argc("ldelpos", 2, args);
  check_argt("ldelpos", args, P_LIST, P_NUM, 0);

  static p_atom **list;
    list = (p_atom **)args->value;
  static size_t pos;
    pos = (size_t)*(p_num *)atom_getindex(args, 1)->value;

  if(pos < 0 || pos >= atom_len(*list)) {
    func_err("ldelpos", outofrange(pos, *list));
  } else if(pos == 0) {
    /* advance list */
    ATOM_NEXT(*list);
  } else {
    /* bypass target element */
    atom_getindex(*list, pos - 1)->next = atom_getindex(*list, pos)->next;
  }

  return NIL_ATOM;
}

MFUNC_PROTO(ldelkey) {
  return NIL_ATOM;
}

