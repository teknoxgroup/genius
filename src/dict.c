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
#include <config.h>

#ifndef WITHOUT_GNOME
#include <gnome.h>
#else
#ifndef _
#define _(x) x
#endif
#endif

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

static EFunc *free_funcs = NULL;

/*return current context number (0 is global, -1 is uninitialized)*/
int
d_curcontext(void)
{
	return context.top;
}

/*make builtin function and return it*/
EFunc *
d_makebifunc(char *id, dictfunc f, int nargs)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=g_strdup(id);
	n->data.func=f;
	n->nargs=nargs;
	n->named_args = NULL;
	n->context=context.top;
	n->type=BUILTIN_FUNC;

	return n;
}

/*make a user function and return it*/
EFunc *
d_makeufunc(char *id, ETree *value, GList *argnames, int nargs)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=g_strdup(id);
	n->data.user=value;
	n->nargs=nargs;
	n->named_args=argnames;
	n->context=context.top;
	n->type=USER_FUNC;

	return n;
}

/*make a user function and return it*/
EFunc *
d_makereffunc(char *id, EFunc *ref)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=g_strdup(id);
	n->data.ref=ref;
	n->nargs=0;
	n->named_args=NULL;
	n->context=context.top;
	n->type=REFERENCE_FUNC;

	return n;
}

/*copy a function*/
EFunc *
d_copyfunc(EFunc *o)
{
	EFunc *n;
	GList *li;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(EFunc));
	if(o->id)
		n->id=g_strdup(o->id);
	if(n->type == USER_FUNC)
		n->data.user=copynode(o->data.user);
	n->named_args=NULL;
	for(li=o->named_args;li!=NULL;li=g_list_next(li))
		n->named_args = g_list_append(n->named_args,g_strdup(li->data));
	
	return n;
}

/*make a real function from a fake*/
EFunc *
d_makerealfunc(EFunc *o,char *id)
{
	EFunc *n;
	GList *li;
	int i;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(EFunc));
	if(id)
		n->id=g_strdup(id);
	n->context = context.top;

	if(n->type == USER_FUNC)
		n->data.user=copynode(o->data.user);
	n->named_args=NULL;
	for(i=0,li=o->named_args;li!=NULL;li=g_list_next(li),i++)
		n->named_args = g_list_append(n->named_args,g_strdup(li->data));
	n->nargs = i;
	
	return n;
}

/*make real func and replace n with it, without changing n's context or id*/
void
d_setrealfunc(EFunc *n,EFunc *fake)
{
	GList *li;
	int i;
	
	if(n->type == USER_FUNC)
		freenode(n->data.user);
	
	n->type = fake->type;
	n->data = fake->data;
	if(fake->type == USER_FUNC)
		n->data.user = copynode(fake->data.user);

	g_list_foreach(n->named_args,(GFunc)g_free,NULL);
	n->named_args=NULL;
	for(i=0,li=fake->named_args;li!=NULL;li=g_list_next(li),i++)
		n->named_args = g_list_append(n->named_args,g_strdup(li->data));
	n->nargs = i;
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

/*compare two EFunc's by their context numbers*/
static int
compare_func_bycontext(gconstpointer p1, gconstpointer p2)
{
	EFunc *func1 = (EFunc *)p1;
	EFunc *func2 = (EFunc *)p2;
	
	return func1->context < func2->context;
}

/*add a function struct to the dict (in current context)*/
EFunc *
d_addfunc(EFunc *func)
{
	EFunc *n;
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
d_setvalue(char *id,ETree *value)
{
	EFunc *f;
	f=d_lookup(id,FALSE);
	if(!f || f->type!=USER_FUNC)
		return FALSE;
	f->data.user=value;
	return TRUE;
}

/*dictionary functions*/

/*lookup a function in the dictionary, either the whole thing if global
  is TRUE, or just the current context otherwise*/
EFunc *
d_lookup(char *id,int global)
{
	GList *list;
	EFunc *func;
	
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
			freedict(d_popcontext());
}

/*free all memory allocated by a dictionary*/
void
freedict(GList *n)
{
	GList *li;
	for(li=n;li!=NULL;li=g_list_next(li))
		freefunc(li->data);
	g_list_free(n);
}

void
freefunc(EFunc *n)
{
	GList *li;
	if(!n)
		return;
	if(n->id)
		g_free(n->id);
	if(n->type == USER_FUNC && n->data.user) 
		freetree(n->data.user);
	for(li=n->named_args;li!=NULL;li=g_list_next(li))
		g_free(li->data);
	g_list_free(n->named_args);
	/*prepend to free list*/
	n->data.next = free_funcs;
	free_funcs = n;
}

/*replace old with stuff from new and free new*/
void
replacefunc(EFunc *old,EFunc *new)
{
	GList *li;
	if(!old || !new)
		return;
	if(old->id)
		g_free(old->id);
	if(old->type == USER_FUNC && old->data.user) 
		freetree(old->data.user);
	for(li=old->named_args;li!=NULL;li=g_list_next(li))
		g_free(li->data);
	g_list_free(old->named_args);
	memcpy(old,new,sizeof(EFunc));
	/*prepend to free list*/
	new->data.next = free_funcs;
	free_funcs = new;
}

/*set_ref*/
void
d_set_ref(EFunc *n,EFunc *ref)
{
	GList *li;
	if(!n || !ref)
		return;
	if(n->type == USER_FUNC && n->data.user) 
		freetree(n->data.user);
	for(li=n->named_args;li!=NULL;li=g_list_next(li))
		g_free(li->data);
	n->nargs = 0;
	n->named_args = NULL;
	
	n->data.ref = ref;
}

/*set_value*/
void
d_set_value(EFunc *n,ETree *value)
{
	GList *li;
	if(!n || !value)
		return;
	if(n->type == USER_FUNC && n->data.user) 
		freetree(n->data.user);
	for(li=n->named_args;li!=NULL;li=g_list_next(li))
		g_free(li->data);
	n->nargs = 0;
	n->named_args = NULL;
	
	n->data.user = value;
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
		EFunc *func = n->data;
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
			EFunc *func = li->data;
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
