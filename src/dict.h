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

typedef ETree *(*dictfunc)(ETree * * /*arguments*/,int * /*exception*/);

/*return current context number (0 is global, -1 is uninitialized)*/
int d_curcontext(void);

/*make builtin function and return it*/
EFunc * d_makebifunc(Token *id, dictfunc f, int nargs);

/*make a user function and return it*/
EFunc * d_makeufunc(Token *id, ETree *value, GList *argnames, int nargs);

/*make a variable function and return in*/
EFunc * d_makevfunc(Token *id, ETree *value);

/*make a user function and return it*/
EFunc * d_makereffunc(Token *id, EFunc *ref);

/*copy a function*/
EFunc *d_copyfunc(EFunc *o);

/*make a real function from a fake*/
EFunc * d_makerealfunc(EFunc *o,Token *id, int use);

/*make real func and replace o with it, without changing o's context or id*/
/*if use is set, we USE the original function, NULLing approriately*/
void d_setrealfunc(EFunc *n,EFunc *fake, int use);

void d_initcontext(void);

/*add a functuion struct to the dict (in current context)*/
EFunc * d_addfunc(EFunc *func);

/*set value of an existing function (in local context), used for arguments
  WARNING, does not free the memory allocated by previous value!*/
int d_setvalue(Token *id,ETree *value);

/*this will work right in all situations*/
void d_set_value(EFunc *n,ETree *value);
void d_set_ref(EFunc *n,EFunc *ref);

/*dictionary functions*/

/*lookup a function in the dictionary, either the whole thing, or just the
  current context otherwise*/
/*lookup a function in the dictionary in the current context*/
EFunc * d_lookup_local(Token *id);
EFunc * d_lookup_global_up1(Token *id);
/*lookup a function in the dictionary, if there are more return the one in the
  highest context*/
#define d_lookup_global(id) ((id&&id->refs)?id->refs->data:NULL)

Token * d_intern(char *id);

int d_delete(Token *id);

/*clear all context dictionaries and pop out all the contexts except
  the global one
  also init the context stack if it hasn't been done*/
void d_singlecontext(void);

/*free all memory allocated by a dictionary*/
void freedict(GList *n);

void freefunc(EFunc *n);

/*replace old with stuff from new and free new*/
void replacefunc(EFunc *old,EFunc *new);

/*push a new dictionary onto the context stack*/
int d_addcontext(void);

/*gimme the last dictinary and pop the context stack*/
GList * d_popcontext(void);

/*gimme the current dictinary*/
GList * d_getcontext(void);

#endif
