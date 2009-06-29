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

#ifndef COMMON_H
#define COMMON_H

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

/* primitive types */
typedef enum {
  /* 0 is used to mark end of check_argt() list */
  P_NIL = 1,
  P_NUM,
  P_STR,
  P_LIST,
  P_SYM,
  P_BLOCK,
  P_MFUNC,
  P_FUNC,
  P_UTYPE,

  PT_PARENL,
  PT_PARENR,
  PT_LITSYM,
  PT_QUOTE,
  PT_EXP
} p_type;

/* serves as just about everything */
typedef struct {
  p_type type;

  char *name;
  void *value;

  struct p_atom *next;
} p_atom;

/* non-primitive type */
typedef struct {
  char *type;
  void *value;
} p_utype;

typedef double p_num;

#define MODULE_INIT void _punt_init(void)
#define MFUNC_REPORT char **_punt_report_funcs(void)
#define MFUNC_PROTO(f) p_atom *punt_##f(p_atom *args, p_atom **vars)

/* atom.c */
p_atom *make_atom(p_type, const char *, void *);
p_atom *atom_tail(p_atom *);
void atom_append(p_atom **, p_atom *);
p_atom *atom_getindex(p_atom *, unsigned int);
unsigned int atom_len(p_atom *);
void *atom_dupnum(p_num);
p_atom *atom_getname(p_atom *, const char *);
p_atom *atom_dup(p_atom *);
void atom_setname(p_atom **, p_atom *);
int atom_true(p_atom *);
void *make_utype(const char *, void *);
#define NIL_ATOM make_atom(P_NIL, "", NULL)
#define TRUE_ATOM make_atom(P_NUM, "", atom_dupnum(1))
#define ATOM_NEXT(a) ((a) = (p_atom *)(a)->next)
#define UTYPE(a) ((p_utype *)((a)->value))

/* tokenizer.c */
p_atom *tokenize_fp(FILE *);
p_atom *tokenize_str(const char *);

/* engine.c */
p_atom *parse_tokens(p_atom *);
void run_code(p_atom *, p_atom **);
p_atom *run_exp(p_atom *, p_atom **);
p_atom *resolve_symbol(p_atom *, const char *);
int load_module(const char *, p_atom **);

/* string.c */
char *vafmt(const char *, ...);
int str_pos(const char *, const char *, unsigned int);
char *str_replace(const char *, const char *, const char *, unsigned int);

/* builtins.c */
void register_builtins(p_atom **);

/* helpers.c */
#define REPORT_MODULE(...) char **_punt_report_funcs(void) { \
    return _report_module(NULL, __VA_ARGS__, NULL); \
  }
/*
 * we need a fixed argument because stdarg.h doesn't support functions
 * without them
 */
char **_report_module(const void *, ...);
void func_err(const char *, const char *);
void check_argc(const char *, const int, p_atom *);
void check_argt(const char *, p_atom *, ...);
void check_argu(const char *, p_atom *, ...);
char *prim_type_name(p_type);

#endif

