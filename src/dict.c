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
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "funclib.h"

/*the context stack structure*/
typedef struct _context_t {
	GList *stack;
	int top;
} context_t;

static context_t context={NULL,-1};

static GHashTable *dictionary;

static func_t *free_funcs = NULL;

/*return current context number (0 is global, -1 is uninitialized)*/
int
d_curcontext(void)
{
	return context.top;
}

/*make builtin function and return it*/
func_t *
d_makebifunc(char *id, dictfunc f, int nargs, int dynamic)
{
	func_t *n;

	if(!free_funcs)
		n = g_new(func_t,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=g_strdup(id);
	n->data.func=f;
	n->nargs=nargs;
	n->context=context.top;
	n->dynamic=dynamic;
	n->type=BUILTIN_FUNC;

	return n;
}

/*make a user function and return it*/
func_t *
d_makeufunc(char *id, tree_t *value, GList *dict, int nargs, int dynamic)
{
	func_t *n;

	if(!free_funcs)
		n = g_new(func_t,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=g_strdup(id);
	n->data.user.value=value;
	n->data.user.dict=dict;
	n->nargs=nargs;
	n->context=context.top;
	n->dynamic=dynamic;
	n->type=USER_FUNC;

	return n;
}


void
d_initcontext(void)
{
	context.top=0; /*0 means that element 0 exists!*/
	/*add an empty dictionary*/
	context.stack=g_list_prepend(NULL,NULL);

	dictionary = g_hash_table_new(g_str_hash,g_str_equal);

	/*this is where the built in functions register into the global
	  dictionary*/
	funclib_addall();
}

/*compare two func_t's by their context numbers*/
static int
compare_func_bycontext(gconstpointer p1, gconstpointer p2)
{
	func_t *func1 = (func_t *)p1;
	func_t *func2 = (func_t *)p2;
	
	return func1->context < func2->context;
}

/*add a function struct to the dict (in current context)*/
func_t *
d_addfunc(func_t *func)
{
	func_t *n;
	GList *list = NULL;
	char *origkey = NULL;
	
	/*we already found it (in current context)*/
	n=d_lookup(func->id,FALSE);
	if(n) {
		replacefunc(n,func);
		return n;
	}

	context.stack->data = g_list_prepend(context.stack->data,func);

	g_hash_table_lookup_extended(dictionary,func->id,(gpointer *)&origkey,
				     (gpointer *)&list);
	list = g_list_insert_sorted(list,func,compare_func_bycontext);
	if(!origkey)
		origkey = g_strdup(func->id);
	g_hash_table_insert(dictionary,origkey,list);

	return func;
}

/*set value of an existing function (in local context), used for arguments
  WARNING, does not free the memory allocated by previous value!*/
int
d_setvalue(char *id,tree_t *value)
{
	func_t *f;
	f=d_lookup(id,FALSE);
	if(!f || f->type!=USER_FUNC)
		return FALSE;
	f->data.user.value=value;
	return TRUE;
}

/*dictionary functions*/

/*lookup a function in the dictionary, either the whole thing if global
  is TRUE, or just the current context otherwise*/
func_t *
d_lookup(char *id,int global)
{
	GList *list;
	func_t *func;
	
	list = g_hash_table_lookup(dictionary,id);
	
	if(!list)
		return NULL;
	
	/*the first one must be the lowest context*/
	func = list->data;
	
	/*not in currect context and we only want the currect context ones*/
	if(!global && func->context<context.top)
		return NULL;

	return func;
}

int
d_delete(char *id)
{
	/*FIXME: Delete function!*/
	return FALSE;
}

/*clear all context dictionaries and pop out all the contexts except
  the global one
  also init the context stack if it hasn't been done*/
void
d_singlecontext(void)
{
	if(context.stack==NULL)
		d_initcontext();
	else
		while(context.top>0)
			freedict(d_popcontext(),TRUE);
}

/*free all memory allocated by a dictionary (except for non-dynamic (static)
  functions if dynamiconly is TRUE)*/
void
freedict(GList *n,int dynamiconly)
{
	GList *li;
	for(li=n;li!=NULL;li=g_list_next(li)) {
		func_t * func = li->data;
		if(!dynamiconly || func->dynamic)
			freefunc(func);
	}
	g_list_free(n);
}

void
freefunc(func_t *n)
{
	if(!n)
		return;
	if(n->id)
		g_free(n->id);
	if(n->type==USER_FUNC) {
		if(n->data.user.value) 
			freetree(n->data.user.value);
		if(n->data.user.dict) 
			freedict(n->data.user.dict,FALSE);
	}
	/*prepend to free list*/
	n->data.next = free_funcs;
	free_funcs = n;
}

/*replace old with stuff from new and free new*/
void
replacefunc(func_t *old,func_t *new)
{
	if(!old || !new)
		return;
	if(old->id)
		g_free(old->id);
	if(old->type==USER_FUNC) {
		if(old->data.user.value) 
			freetree(old->data.user.value);
		if(old->data.user.dict) 
			freedict(old->data.user.dict,FALSE);
	}
	memcpy(old,new,sizeof(func_t));
	/*prepend to free list*/
	new->data.next = free_funcs;
	free_funcs = new;
}

/*copy a dictionary, but not functions, they stay the same pointers, this
  should only be done with dictionaries with no dymanic entries as those
  would be easily cleared*/
GList *
copydict(GList *n)
{
	GList *r = NULL;

	while(n) {
		r = g_list_prepend(r,n);
		n = g_list_next(n);
	}
	
	return r;
}

/*push a new dictionary onto the context stack*/
int
d_addcontext(GList *n)
{
	context.stack = g_list_prepend(context.stack,n);
	context.top++;
	
	while(n) {
		func_t *func = n->data;
		GList *list = NULL;
		char *origkey = NULL;
		
		/*can this ever happen? if it's possible we need to not
		  make it assert*/
		g_assert(func->context!=context.top);
	
		g_hash_table_lookup_extended(dictionary,func->id,
					     (gpointer *)&origkey,
				             (gpointer *)&list);
		list = g_list_insert_sorted(list,func,compare_func_bycontext);
		if(!origkey)
			origkey = g_strdup(func->id);
		g_hash_table_insert(dictionary,origkey,list);
		n=g_list_next(n);
	}
	return TRUE;
}

/*gimme the last dictionary, (removes the items from the global hash too)*/
GList *
d_popcontext(void)
{
	if(context.top==-1)
		return NULL;
	else {
		GList *li;
		GList *dict = context.stack->data;

		for(li=dict;li!=NULL;li=g_list_next(li)) {
			func_t *func = li->data;
			GList *list = NULL;
			char *origkey = NULL;

			/*we need to take this one out of the dictionary*/
			g_hash_table_lookup_extended(dictionary,func->id,
					             (gpointer *)&origkey,
						     (gpointer *)&list);
			list = g_list_remove(list,func);
			if(!origkey)
				origkey = g_strdup(func->id);
			g_hash_table_insert(dictionary,origkey,list);
		}
		context.top--;
		li = context.stack;
		context.stack = g_list_remove_link(context.stack,li);
		g_list_free_1(li);
		return dict;
	}
}
