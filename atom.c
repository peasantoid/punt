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

/* create an atom */
p_atom *make_atom(p_type type, const char *name, void *value) {
  p_atom *atom = (p_atom *)calloc(1, sizeof(p_atom));
    atom->type = type;
    atom->name = name ? strdup(name) : NULL;
    atom->value = value;
    atom->next = NULL;
  return atom;
}

/* duplicate a p_num */
void *atom_dupnum(p_num num) {
  p_num *rval = (p_num *)calloc(1, sizeof(p_num));
  *rval = num;
  return (void *)rval;
}

/* length of a linked list */
unsigned int atom_len(p_atom *atom) {
  static int i;
  for(i = 0; atom; i++) {
    ATOM_NEXT(atom);
  }
  return i;
}

/* return tail end of a list of atoms */
p_atom *atom_tail(p_atom *atom) {
  if(!atom) { return NIL_ATOM; }
  while(atom->next) {
    /*
     * FIXME: w/o cast, gcc throws 'assignment from incompatible pointer type'
     * This is due to ->next being 'struct p_atom *' and atom being 'p_atom *'.
     */
    atom = (p_atom *)atom->next;
  }
  return atom;
}

/* append an atom [list] to another atom [list] */
void atom_append(p_atom **target, p_atom *new) {
  if(!*target) {
    *target = new;
  } else {
    atom_tail(*target)->next = (struct p_atom *)new;
  }
}

/* get the atom corresponding to an index */
p_atom *atom_getindex(p_atom *atom, unsigned int index) {
  static unsigned int i;
  i = 0;
  for(i = 0;
      i < index;
      i++) {
    ATOM_NEXT(atom);
  }
  return atom;
}

/* get an atom by name */
p_atom *atom_getname(p_atom *atom, const char *name) {
  if(!name) {
    return NULL;
  }
  while(atom) {
    if(atom->name && !strcmp(atom->name, name)) {
      return atom;
    }
    ATOM_NEXT(atom);
  }
  return NULL;
}

/* duplicate atom list */
p_atom *atom_dup(p_atom *atom) {
  static p_atom **list;
    list = (p_atom **)calloc(1, sizeof(p_atom *));
    *list = NULL;

  while(atom) {
    atom_append(list, make_atom(atom->type, atom->name, atom->value));
    atom = (p_atom *)atom->next;
  }

  return *list;
}

void atom_setindex(p_atom *list, size_t pos, p_atom *new) {
  static size_t len, i;
    len = atom_len(list);
  for(i = 0;
      list != NULL;
      i++, ATOM_NEXT(list)) {
    if(i == pos) {
      list->type = new->type;
      list->name = NULL; /* preserve name? */
      list->value = new->value;
    }
  }
}

/* set atom by name */
void atom_setname(p_atom **list, p_atom *new) {
  p_atom *atom = atom_getname(*list, new->name);
  if(!atom) {
    atom_append(list, new);
  } else {
    atom->type = new->type;
    atom->value = new->value;
  }
}

/* test for true value */
int atom_true(p_atom *atom) {
  return (int)(atom->type != P_NIL);
}

/* create a p_utype* and return as void* */
void *make_utype(const char *type, void *value) {
  p_utype *rval = (p_utype *)calloc(1, sizeof(p_utype));
    rval->type = strdup(type);
    rval->value = value;
  return (void *)rval;
}

