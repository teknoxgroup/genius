/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998 the Free Software Foundation.
 *
 * Author: George Lebl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

#ifndef _DICT_H_
#define _DICT_H_

/*declarations of structures*/
#include "structs.h"

typedef tree_t *(*dictfunc)(tree_t * *);

/*return current context number (0 is global, -1 is uninitialized)*/
int d_curcontext(void);

/*make builtin function and return it*/
func_t * d_makebifunc(char *id, dictfunc f, int nargs, int dynamic);

/*make a user function and return it*/
func_t * d_makeufunc(char *id, tree_t *value, GList *dict, int nargs, 
	int dynamic);

void d_initcontext(void);

/*add a functuion struct to the dict (in current context)*/
func_t * d_addfunc(func_t *func);

/*set value of an existing function (in local context), used for arguments
  WARNING, does not free the memory allocated by previous value!*/
int d_setvalue(char *id,tree_t *value);

/*dictionary functions*/

/*lookup a function in the dictionary, either the whole thing if global
  is TRUE, or just the current context otherwise
  a terribly inefficent linear search, it was just easy to code nothing
  else, a hash would be more appropriate*/
func_t * d_lookup(char *id,int global);

int d_delete(char *id);

/*clear all context dictionaries and pop out all the contexts except
  the global one
  also init the context stack if it hasn't been done*/
void d_singlecontext(void);

/*free all memory allocated by a dictionary (except for non-dynamic (static)
  functions if dynamiconly is TRUE)*/
void freedict(GList *n,int dynamiconly);

void freefunc(func_t *n);

/*replace old with stuff from new and free new*/
void replacefunc(func_t *old,func_t *new);

/*copy a dictionary, but not functions, they stay the same pointers, this
  should only be done with dictionaries with no dymanic entries as those
  would be easily cleared*/
GList * copydict(GList *n);

/*push a new dictionary onto the context stack*/
int d_addcontext(GList *n);

/*gimme the last dictinary*/
GList * d_popcontext(void);

#endif
