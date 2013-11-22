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
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "funclib.h"
#include "compil.h"

/*the context stack structure*/
typedef struct _context_t {
	GList *stack;
	int top;
} context_t;

static context_t context={NULL,-1};

static GHashTable *dictionary;

extern GHashTable *uncompiled;

EFunc *free_funcs = NULL;

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

/*make a variable function and return it*/
EFunc *
d_makevfunc(Token *id, ETree *value)
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
	n->nargs=0;
	n->named_args=NULL;
	n->context=context.top;
	n->type=VARIABLE_FUNC;

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

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(EFunc));
	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC) {
		if(!o->data.user) {
			g_assert(uncompiled);
			o->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,o->id));
			g_hash_table_remove(uncompiled,o->id);
			g_assert(o->data.user);
		}
		n->data.user=copynode(o->data.user);
	}
	n->named_args = g_list_copy(o->named_args);
	
	return n;
}

/*make a real function from a fake*/
EFunc *
d_makerealfunc(EFunc *o,Token *id, int use)
{
	EFunc *n;

	if(!free_funcs)
		n = g_new(EFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(EFunc));
	n->id = id;
	n->context = context.top;

	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC) {
		if(!o->data.user) {
			g_assert(uncompiled);
			o->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,o->id));
			g_hash_table_remove(uncompiled,o->id);
			g_assert(o->data.user);
		}
		if(use) {
			n->data.user = o->data.user;
			o->data.user = NULL;
		} else
			n->data.user = copynode(o->data.user);
	}
	if(use) {
		o->named_args = NULL;
		o->named_args = 0;
	} else
		n->named_args = g_list_copy(o->named_args);
	
	return n;
}

/*make real func and replace n with it, without changing n's context or id*/
/*if use is set, we USE the original function, NULLing approriately*/
void
d_setrealfunc(EFunc *n,EFunc *fake, int use)
{
	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC)
		freetree(n->data.user);
	
	n->type = fake->type;
	n->data = fake->data;
	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC) {
		if(!fake->data.user) {
			g_assert(uncompiled);
			fake->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,fake->id));
			g_hash_table_remove(uncompiled,fake->id);
			g_assert(fake->data.user);
		}
		if(use) {
			n->data.user = fake->data.user;
			fake->data.user = NULL;
		} else
			n->data.user = copynode(fake->data.user);
	}

	if(use) {
		n->named_args = fake->named_args;
		n->nargs = fake->nargs;
		fake->named_args = NULL;
		fake->nargs = 0;
	} else {
		n->named_args = g_list_copy(fake->named_args);
		n->nargs = fake->nargs;
	}
}


void
d_initcontext(void)
{
	Token *tok;
	context.top=0; /*0 means that element 0 exists!*/
	/*add an empty dictionary*/
	context.stack=g_list_prepend(NULL,NULL);

	dictionary = g_hash_table_new(g_str_hash,g_str_equal);

	/*add Ans and ans as the same token*/

	tok = g_new(Token,1);
	tok->token = g_strdup("Ans");
	tok->refs = NULL;
	g_hash_table_insert(dictionary,tok->token,tok);
	g_hash_table_insert(dictionary,g_strdup("ans"),tok);

	/*this is where the built in functions register into the global
	  dictionary*/
	funclib_addall();
}

/*compare two EFunc's by their context numbers*/
/*static int
compare_func_bycontext(gconstpointer p1, gconstpointer p2)
{
	EFunc *func1 = (EFunc *)p1;
	EFunc *func2 = (EFunc *)p2;
	
	return func1->context < func2->context;
}*/

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
	if(!f || (f->type!=USER_FUNC &&
		  f->type!=VARIABLE_FUNC))
		return FALSE;
	f->data.user=value;
	return TRUE;
}

/*dictionary functions*/

/*lookup a function in the dictionary in the current context*/
EFunc *
d_lookup_local(Token *id)
{
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

/*lookup a function in the dictionary NOT in the current context*/
EFunc *
d_lookup_global_up1(Token *id)
{
	EFunc *func;
	
	if(!id ||
	   !id->refs)
		return NULL;
	
	/*the first one must be the lowest context*/
	func = id->refs->data;
	
	if(func->context<context.top)
		return func;
	if(!id->refs->next)
		return NULL;
	
	return id->refs->next->data;
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
	if(!n)
		return;
	g_assert(!n->id || g_list_find(n->id->refs,n)==NULL);
	/*if(n->id)
		n->id->refs = g_list_remove(n->id->refs,n);*/
	if((n->type == USER_FUNC ||
	    n->type == VARIABLE_FUNC) &&
	   n->data.user)
		freetree(n->data.user);
	g_list_free(n->named_args);
	/*prepend to free list*/
	n->data.next = free_funcs;
	free_funcs = n;
}

/*replace old with stuff from new and free new,
  new has to have the same id, also new->id should
  not hold a reference to new*/
void
replacefunc(EFunc *old,EFunc *new)
{
	g_return_if_fail(old && new);
	g_return_if_fail(old->id == new->id);

	if(old->type == USER_FUNC ||
	   old->type == VARIABLE_FUNC)
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
	if(!n || !ref)
		return;
	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC)
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
	if(!n || !value)
		return;
	if(n->type == USER_FUNC ||
	   n->type == VARIABLE_FUNC)
		freetree(n->data.user);
	n->type = VARIABLE_FUNC;
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

/*gimme the last dictionary and pop the context stack*/
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

/*gimme the current dictinary*/
GList *
d_getcontext(void)
{
	if(context.top==-1)
		return NULL;
	else
		return context.stack->data;
}
