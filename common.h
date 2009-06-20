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
#include <dlfcn.h>
#include <libgen.h>

/* identifies the type of an item */
typedef enum {
  P_NIL, /* nil/false/null */
  P_NUM,
  P_STR,
  P_SYM, /* symbol: something */
  P_STRUCT,
  P_BLOCK, /* .( ... ) */
  P_LIST,
  P_MFUNC, /* module function */
  P_FUNC, /* user-defined function */
  P_FILE, /* file pointer/stream */
  P_RSRC, /* resource (anything else) */

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

  char *name;
  void *value;

  struct p_atom *next;
} p_atom;

typedef double p_num;

#define MFUNC_REPORT char **_punt_report_funcs(void)
#define MFUNC_PROTO(f) p_atom *punt_##f(p_atom *args, p_atom **vars)

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
#define NIL_ATOM make_atom(P_NIL, "", NULL)
#define TRUE_ATOM make_atom(P_NUM, "", atom_dupnum(1))
#define ATOM_NEXT(a) a = (p_atom *)a->next

/* tokenizer.c */
p_atom *tokenize_fp(FILE *);
p_atom *tokenize_str(const char *);

/* engine.c */
p_atom *parse_tokens(p_atom *);
void run_code(p_atom *, p_atom **);
p_atom *run_exp(p_atom *, p_atom **);
void check_argc(const char *, const int, p_atom *);
p_atom *resolve_symbol(p_atom *, const char *);
int load_module(const char *, p_atom **);

/* string.c */
char *vafmt(const char *, ...);
int str_pos(const char *, const char *, unsigned int);
char *str_replace(const char *, const char *, const char *, unsigned int);

/* builtins.c */
void register_builtins(p_atom **);

