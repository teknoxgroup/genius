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

static EFunc *free_funcs = NULL;

/*return current context number (0 is global, -1 is uninitialized)*/
int
d_curcontext(void)
{
	return context.top;
}

/*make builtin function and return it*/
EFunc *
d_makebifunc(Token *id, dictfunc f, int nargs)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=id;
	n->data.func=f;
	n->nargs=nargs;
	n->named_args = NULL;
	n->context=context.top;
	n->type=BUILTIN_FUNC;

	return n;
}

/*make a user function and return it*/
EFunc *
d_makeufunc(Token *id, ETree *value, GList *argnames, int nargs)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=id;
	n->data.user=value;
	n->nargs=nargs;
	n->named_args=argnames;
	n->context=context.top;
	n->type=USER_FUNC;

	return n;
}

/*make a user function and return it*/
EFunc *
d_makereffunc(Token *id, EFunc *ref)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=id;
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
	if(n->type == USER_FUNC)
		n->data.user=copynode(o->data.user);
	n->named_args = g_list_copy(o->named_args);
	
	return n;
}

/*make a real function from a fake*/
EFunc *
d_makerealfunc(EFunc *o,Token *id)
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
	n->id = id;
	n->context = context.top;

	if(n->type == USER_FUNC)
		n->data.user=copynode(o->data.user);
	n->named_args = g_list_copy(o->named_args);
	
	return n;
}

/*make real func and replace n with it, without changing n's context or id*/
void
d_setrealfunc(EFunc *n,EFunc *fake)
{
	GList *li;
	
	if(n->type == USER_FUNC)
		freenode(n->data.user);
	
	n->type = fake->type;
	n->data = fake->data;
	if(fake->type == USER_FUNC)
		n->data.user = copynode(fake->data.user);

	n->named_args = g_list_copy(fake->named_args);
	n->nargs = fake->nargs;
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
	
	g_return_val_if_fail(func->context == context.top,func);
	
	/*we already found it (in current context)*/
	n=d_lookup_local(func->id);
	if(n) {
		replacefunc(n,func);
		return n;
	}

	context.stack->data = g_list_prepend(context.stack->data,func);
	
	func->id->refs = g_list_prepend(func->id->refs,func);

	return func;
}

/*set value of an existing function (in local context), used for arguments
  WARNING, does not free the memory allocated by previous value!*/
int
d_setvalue(Token *id,ETree *value)
{
	EFunc *f;
	f=d_lookup_local(id);
	if(!f || f->type!=USER_FUNC)
		return FALSE;
	f->data.user=value;
	return TRUE;
}

/*dictionary functions*/

/*lookup a function in the dictionary in the current context*/
EFunc *
d_lookup_local(Token *id)
{
	GList *list;
	EFunc *func;
	
	if(!id ||
	   !id->refs)
		return NULL;
	
	/*the first one must be the lowest context*/
	func = id->refs->data;
	
	/*not in currect context and we only want the currect context ones*/
	if(func->context<context.top)
		return NULL;

	return func;
}

Token *
d_intern(char *id)
{
	Token * tok = g_hash_table_lookup(dictionary,id);
	if(!tok) {
		tok = g_new(Token,1);
		tok->token = g_strdup(id);
		tok->refs = NULL;
		g_hash_table_insert(dictionary,tok->token,tok);
	}
	return tok;
}

int
d_delete(Token *id)
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
	if(n->type == USER_FUNC && n->data.user) 
		freetree(n->data.user);
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
	if(old->type == USER_FUNC && old->data.user) 
		freetree(old->data.user);
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
	n->type = REFERENCE_FUNC;
	g_list_free(n->named_args);
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
	n->type = USER_FUNC;
	g_list_free(n->named_args);
	n->nargs = 0;
	n->named_args = NULL;
	
	n->data.user = value;
}

/*push a new dictionary onto the context stack*/
int
d_addcontext(void)
{
	context.stack = g_list_prepend(context.stack,NULL);
	context.top++;
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
			func->id->refs = g_list_remove(func->id->refs,func);
		}
		context.top--;
		li = context.stack;
		context.stack = g_list_remove_link(context.stack,li);
		g_list_free_1(li);
		return dict;
	}
}
