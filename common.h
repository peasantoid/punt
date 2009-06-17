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

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <ctype.h>

/* identifies the type of an item */
typedef enum {
  P_NIL,
  P_NUM,
  P_STR,
  P_SYM,
  P_STRUCT,
  P_BLOCK,
  P_LIST,
  P_MFUNC,
  P_FUNC,
  P_UDEF, /* user-defined */

  /* token types */
  PT_PARENL,
  PT_PARENR,
  PT_LITSYM, /* a quoted symbol: .something */
  PT_QUOTE,
  PT_EXP
} p_type;

typedef struct {

} p_udef;

/* serves as just about everything */
typedef struct {
  p_type type;
  char *utype; /* name of user-defined type */

  char *name;
  void *value;

  struct p_atom *next;
} p_atom;

typedef double p_num;

/* atom.c */
p_atom *make_atom(unsigned int, const char *, void *);
p_atom *atom_tail(p_atom *);
void atom_append(p_atom **, p_atom *);
p_atom *atom_getindex(p_atom *, unsigned int);
unsigned int atom_len(p_atom *);
void *atom_dupnum(p_num);
p_atom *atom_getname(p_atom *, const char *);
p_atom *atom_dup(p_atom *);
void atom_setname(p_atom **, p_atom *);
int atom_true(p_atom *);

/* tokenizer.c */
p_atom *tokenize_fp(FILE *);
p_atom *tokenize_str(const char *);

/* engine.c */
p_atom *parse_tokens(p_atom *);
void run_code(p_atom *, p_atom **);
p_atom *run_exp(p_atom *, p_atom **);
void check_argc(const char *, const int, p_atom *);

/* string.c */
char *vafmt(const char *, ...);
int str_pos(const char *, const char *, unsigned int);
char *str_replace(const char *, const char *, const char *, unsigned int);

/* builtins.c */
p_atom *register_builtins(void);
p_atom *blt_func(p_atom *, p_atom **);
p_atom *blt_cond(p_atom *, p_atom **);
p_atom *blt_yes(p_atom *, p_atom **);
p_atom *blt_no(p_atom *, p_atom **);

