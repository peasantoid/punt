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
    switch(tokens->type) {
      case P_NUM:
        atom_append(&code, make_atom(P_NUM, "", atom_dupnum(*(p_num *)tokens->value)));
        break;
      case P_STR:
        atom_append(&code, make_atom(P_STR, "", (void *)strdup((char *)tokens->value)));
        break;
      case P_SYM:
        atom_append(&code, make_atom(P_SYM, "", tokens->value));
        break;
      /* it's an expression */
      case PT_PARENL:
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
        break;
      case PT_QUOTE:
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
        break;
      default:
        break;
    }
    if(tokens) { tokens = (p_atom *)tokens->next; }
  }

  return code;
}

void run_code(p_atom *code, p_atom **vars) {
  while(code) {
    /* it's an expression; execute it */
    if(code->type == PT_EXP) {
      run_exp(code, vars);
    }
    code = (p_atom *)code->next;
  }
}

p_atom *run_exp(p_atom *exp, p_atom **vars) {
  p_atom *rval = NULL, *_args, *args = NULL, *atom, *func,
         *funcvars, *orig;
  p_atom *(*funcptr)(p_atom *, p_atom **) = NULL;
  
  switch(exp->type) {
    /* we can run it */
    case PT_EXP:
      break;
    /* we can't run it, so return it */
    default:
      return exp;
      break;
  }
  _args = (p_atom *)exp->value;

  while(_args) {
    switch(_args->type) {
      case PT_EXP:
        atom_append(&args, run_exp(_args, vars));
        break;
      case P_SYM:
        atom = atom_getname(*vars, (char *)_args->value);
        if(!atom) {
          fprintf(stderr, "%s: undefined symbol\n", (char *)_args->value);
          exit(1);
        }
        atom_append(&args, make_atom(atom->type, "", atom->value));
        break;
      case PT_LITSYM:
        atom_append(&args, make_atom(P_SYM, "", _args->value));
        break;
      default:
        atom_append(&args, make_atom(_args->type, "", _args->value));
        break;
    }
    _args = (p_atom *)_args->next;
  }

  rval = make_atom(P_NIL, "", NULL);
  if(!atom_len(args)) { return rval; }
  func = args;
  args = (p_atom *)args->next;

  switch(func->type) {
    case P_SYM:
      if(atom_len(args) != 1) {
        fprintf(stderr, "exactly 1 arg required for assignment, %d given\n", atom_len(args));
        exit(1);
      }
      atom_setname(vars, make_atom(args->type, (char *)func->value, args->value));
      break;
    case P_FUNC:
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
      atom_setname(&funcvars, make_atom(P_FUNC, "__func", func));
      atom_setname(&funcvars, make_atom(P_NUM, "__argc", atom_dupnum(atom_len(args))));

      /*
       * convert the func to an exp so the engine will know to run it
       * rather than return its value
       */
      rval = run_exp(make_atom(PT_EXP, "", func->value), &funcvars);
      break;
    case P_MFUNC:
      funcptr = func->value;
      rval = (*funcptr)(args, vars);
      break;
    default:
      fprintf(stderr, "type not callable\n");
      exit(1);
      break;
  }

  return rval;
}

void check_argc(const char *func, const int minlen, p_atom *args) {
  /* must be exact */
  if(minlen <= 0) {
    if(atom_len(args) != abs(minlen)) {
      fprintf(stderr , "%s: exactly %d %s required, %d given\n", func, abs(minlen),
          minlen == -1 ? "arg" : "args", atom_len(args));
      exit(1);
    }
  } else {
    if(atom_len(args) < minlen) {
      fprintf(stderr, "%s: at least %d %s required %d given\n", func, minlen,
          minlen == 1 ? "arg" : "args", minlen);
      exit(1);
    }
  }
}

