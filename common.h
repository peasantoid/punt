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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

/* identifies the type of an item */
typedef enum {
  P_NUM,
  P_STR,
  P_UDEF /* user-defined */
} p_type;

/*
 * serves as: token, piece of data, variable
 */
typedef struct {
  p_type type;

  char *name;
  void *value;

  struct p_atom *next;
} p_atom;

/* atom.c */


/* tokenizer.c */
p_atom *tokenize_fp(const FILE *);
p_atom *tokenize_str(const char *);

/* engine.c */
p_atom *run_tokens(p_atom *, p_atom *);

