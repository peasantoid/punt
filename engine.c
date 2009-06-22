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

/* parse basic tokens into more complex ones */
p_atom *parse_tokens(p_atom *tokens) {
  p_atom *code = NULL, *list = NULL;
  static unsigned int level;

  while(tokens) {
    if(tokens->type == P_NUM) {
      atom_append(&code, make_atom(P_NUM, "", atom_dupnum(*(p_num *)tokens->value)));
    } else if(tokens->type == P_STR) {
      atom_append(&code, make_atom(P_STR, "", (void *)strdup((char *)tokens->value)));
    } else if(tokens->type == P_SYM) {
      atom_append(&code, make_atom(P_SYM, "", tokens->value));
    /* it's an expression */
    } else if(tokens->type == PT_PARENL) {
      tokens = (p_atom *)tokens->next;
      if(!tokens ) { break; }

      list = NULL;
      level = 1;
      while(tokens) {
        if(tokens->type == PT_PARENL) { level++; }
        else if(tokens->type == PT_PARENR) { level--; }
        if(level <= 0) { break; }

        atom_append(&list, make_atom(tokens->type, "", tokens->value));
        tokens = (p_atom *)tokens->next;
      }
      atom_append(&code, make_atom(PT_EXP, "", parse_tokens(list)));
    /* it's a literal something-or-other */
    } else if(tokens->type == PT_QUOTE) {
      tokens = (p_atom *)tokens->next;
      if(!tokens) { break; }

      /* it's a block: .( ... ) */
      else if(tokens->type == PT_PARENL) {
        tokens = (p_atom *)tokens->next;
        if(!tokens) { break; }

        list = NULL;
        level = 1;
        while(tokens) {
          if(tokens->type == PT_PARENL) { level++; }
          else if(tokens->type == PT_PARENR) { level--; }
          if(level <= 0) { break; }

          atom_append(&list, make_atom(tokens->type, "", tokens->value));
          tokens = (p_atom *)tokens->next;
        }
        atom_append(&code, make_atom(P_BLOCK, "", parse_tokens(list)));

      /* it's a quoted symbol: .something */
      } else if(tokens->type == P_SYM) {
        atom_append(&code, make_atom(PT_LITSYM, "", tokens->value));
      }
    }
    if(tokens) { tokens = (p_atom *)tokens->next; }
  }

  return code;
}

void run_code(p_atom *code, p_atom **vars) {
  while(code) {
    run_exp(code, vars);
    code = (p_atom *)code->next;
  }
}

p_atom *resolve_symbol(p_atom *vars, const char *sym) {
  p_atom *atom = atom_getname(vars, sym);
  if(!atom) {
    fprintf(stderr, "%s: undefined symbol\n", sym);
    exit(1);
  }
  return atom;
}

p_atom *run_exp(p_atom *exp, p_atom **vars) {
  p_atom *rval = NULL, *_args, *args = NULL, *atom, *func,
         *funcvars, *orig;
  p_atom *(*funcptr)(p_atom *, p_atom **) = NULL;
  int i;
  
  /* it evaluates to a symbol */
  if(exp->type == PT_LITSYM) {
    return make_atom(P_SYM, "", exp->value);
  /* we can resolve it */
  } else if(exp->type == P_SYM) {
    return resolve_symbol(*vars, (char *)exp->value);
  /* otherwise, return it */
  } else if(exp->type != PT_EXP) {
    return exp;
  }
  _args = (p_atom *)exp->value;

  while(_args) {
    if(_args->type == PT_EXP) {
      atom_append(&args, atom = run_exp(_args, vars));
    } else if(_args->type == P_SYM) {
      atom = resolve_symbol(*vars, (char *)_args->value);
      atom_append(&args, make_atom(atom->type, "", atom->value));
    } else if(_args->type == PT_LITSYM) {
      atom_append(&args, make_atom(P_SYM, "", _args->value));
    } else {
      atom_append(&args, make_atom(_args->type, "", _args->value));
    }
    _args = (p_atom *)_args->next;
  }

  rval = NIL_ATOM;
  if(!atom_len(args)) { return rval; }
  func = args;
  args = (p_atom *)args->next;

  if(func->type == P_SYM) {
    if(atom_len(args) != 1) {
      fprintf(stderr, "exactly 1 arg required for assignment, %d given\n", atom_len(args));
      exit(1);
    }
    atom_setname(vars, make_atom(args->type, (char *)func->value, args->value));
    rval = args;
  } else if(func->type == P_FUNC) {
    funcvars = NULL;
    /* only pass down functions */
    orig = *vars;
    while(*vars) {
      if((*vars)->type == P_FUNC || (*vars)->type == P_MFUNC) {
        /* using make_atom() so we don't bring the rest of the scope along as well */
        atom_setname(&funcvars, make_atom((*vars)->type, (*vars)->name, (*vars)->value));
      }
      *vars = (p_atom *)(*vars)->next;
    }
    *vars = orig;

    /* set some special variables */
    atom_setname(&funcvars, make_atom(P_FUNC, "__func", func->value));
    atom_setname(&funcvars, make_atom(P_NUM, "__argc", atom_dupnum(atom_len(args))));
    
    /* arguments referring to variables */
    i = 1;
    orig = args;
    while(args) {
      atom_setname(&funcvars, make_atom(args->type, vafmt("_%d", i), args->value));
      args = (p_atom *)args->next;
      i++;
    }
    args = orig;

    /*
     * convert the func to an exp so the engine will know to run it
     * rather than return its value
     */
    rval = run_exp(make_atom(PT_EXP, "", func->value), &funcvars);
  } else if(func->type == P_MFUNC) {
    funcptr = func->value;
    rval = (*funcptr)(args, vars);
  } else {
    fprintf(stderr, "type not callable\n");
    exit(1);
  }

  return rval;
}

int load_module(const char *path, p_atom **vars) {
  void *module;
  char **(*namesptr)(void), **names;
  p_atom *(*funcptr)(p_atom *, p_atom **);
  
  module = dlopen(path, RTLD_LAZY); /* or RTLD_NOW? */
  if(!module) { return 0; }

  namesptr = dlsym(module, "_punt_report_funcs");
  if(!namesptr) { return 0; }

  names = (*namesptr)();
  while(*names) {
    funcptr = dlsym(module, vafmt("punt_%s", *names));
    if(!funcptr) { return 0; }
    atom_setname(vars, make_atom(P_MFUNC, *names, funcptr));

    names++;
  }

  return 1;
}

