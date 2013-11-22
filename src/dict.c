/* GENIUS Calculator
 * Copyright (C) 1997-2002 George Lebl
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
	GSList *stack;
	GSList *subststack;
	int top;
} context_t;

static context_t context={NULL,NULL,-1};

static GHashTable *dictionary;

extern GHashTable *uncompiled;

GelEFunc *free_funcs = NULL;

/*return current context number (0 is global, -1 is uninitialized)*/
int
d_curcontext(void)
{
	return context.top;
}

/*make builtin function and return it*/
GelEFunc *
d_makebifunc(GelToken *id, dictfunc f, int nargs)
{
	GelEFunc *n;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id = id;
	n->data.func = f;
	n->nargs = nargs;
	n->vararg = 0;
	n->on_subst_list = 0;
	n->named_args = NULL;
	n->extra_dict = NULL;
	n->context = context.top;
	n->type = GEL_BUILTIN_FUNC;

	return n;
}

/*make a user function and return it*/
GelEFunc *
d_makeufunc (GelToken *id, GelETree *value, GSList *argnames, int nargs,
	     const GSList *extra_dict)
{
	GelEFunc *n;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=id;
	n->data.user=value;
	n->nargs=nargs;
	n->vararg = 0;
	n->on_subst_list = 0;
	n->named_args=argnames;
	n->context=context.top;
	n->type=GEL_USER_FUNC;

	if (extra_dict != NULL) {
		GSList *li;
		n->extra_dict = g_slist_copy ((GSList *)extra_dict);
		for (li = n->extra_dict; li != NULL; li = li->next)
			li->data = d_copyfunc (li->data);
	} else {
		n->extra_dict = NULL;
	}

	return n;
}

/*make a variable function and return it*/
GelEFunc *
d_makevfunc(GelToken *id, GelETree *value)
{
	GelEFunc *n;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id=id;
	n->data.user=value;
	n->nargs=0;
	n->vararg = 0;
	n->on_subst_list = 0;
	n->named_args=NULL;
	n->extra_dict = NULL;
	n->context=context.top;
	n->type=GEL_VARIABLE_FUNC;

	return n;
}

/*make a user function and return it*/
GelEFunc *
d_makereffunc(GelToken *id, GelEFunc *ref)
{
	GelEFunc *n;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	n->id = id;
	n->data.ref = ref;
	n->nargs = 0;
	n->vararg = 0;
	n->on_subst_list = 0;
	n->named_args = NULL;
	n->extra_dict = NULL;
	n->context = context.top;
	n->type = GEL_REFERENCE_FUNC;

	return n;
}

/*copy a function*/
GelEFunc *
d_copyfunc(GelEFunc *o)
{
	GelEFunc *n;
	GSList *li;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(GelEFunc));
	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC) {
		D_ENSURE_USER_BODY (o);
		n->data.user=copynode(o->data.user);
	}
	n->named_args = g_slist_copy (o->named_args);

	n->extra_dict = g_slist_copy (o->extra_dict);
	for (li = n->extra_dict; li != NULL; li = li->next)
		li->data = d_copyfunc (li->data);

	if (n->on_subst_list) {
		n->on_subst_list = 0;
		d_put_on_subst_list (n);
	}
	
	return n;
}

/*make a real function from a fake*/
GelEFunc *
d_makerealfunc(GelEFunc *o,GelToken *id, int use)
{
	GelEFunc *n;

	if(!free_funcs)
		n = g_new(GelEFunc,1);
	else {
		n = free_funcs;
		free_funcs = free_funcs->data.next;
	}
	memcpy(n,o,sizeof(GelEFunc));
	n->id = id;
	n->context = context.top;

	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC) {
		D_ENSURE_USER_BODY (o);
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
		n->named_args = g_slist_copy(o->named_args);

	if (use) {
		o->extra_dict = NULL;
	} else {
		GSList *li;
		n->extra_dict = g_slist_copy (o->extra_dict);
		for (li = n->extra_dict; li != NULL; li = li->next)
			li->data = d_copyfunc (li->data);
	}

	if (n->on_subst_list) {
		n->on_subst_list = 0;
		d_put_on_subst_list (n);
	}
	
	return n;
}

/*make real func and replace n with it, without changing n's context or id*/
/*if use is set, we USE the original function, NULLing approriately*/
void
d_setrealfunc(GelEFunc *n,GelEFunc *fake, int use)
{
	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC)
		gel_freetree(n->data.user);
	
	n->type = fake->type;
	n->data = fake->data;
	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC) {
		D_ENSURE_USER_BODY (fake);
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
		n->named_args = g_slist_copy(fake->named_args);
		n->nargs = fake->nargs;
	}

	if (use) {
		n->extra_dict = fake->extra_dict;
		fake->extra_dict = NULL;
	} else {
		GSList *li;
		n->extra_dict = g_slist_copy (fake->extra_dict);
		for (li = n->extra_dict; li != NULL; li = li->next)
			li->data = d_copyfunc (li->data);
	}

	if (fake->on_subst_list) {
		d_put_on_subst_list (n);
	}
}

void
d_initcontext(void)
{
	GelToken *tok;

	context.top = 0; /*0 means that element 0 exists!*/
	/*add an empty dictionary*/
	context.stack = g_slist_prepend (NULL,NULL);
	context.subststack = g_slist_prepend (NULL,NULL);

	dictionary = g_hash_table_new (g_str_hash, g_str_equal);

	/*add Ans and ans as the same token*/
	tok = g_new0 (GelToken, 1);
	tok->token = g_strdup ("Ans");
	g_hash_table_insert (dictionary, tok->token, tok);
	g_hash_table_insert (dictionary, g_strdup("ans"), tok);

	/*this is where the built in functions register into the global
	  dictionary*/
	gel_funclib_addall ();
}

/*compare two GelEFunc's by their context numbers*/
/*static int
compare_func_bycontext(gconstpointer p1, gconstpointer p2)
{
	GelEFunc *func1 = (GelEFunc *)p1;
	GelEFunc *func2 = (GelEFunc *)p2;
	
	return func1->context < func2->context;
}*/

/*add a function struct to the dict (in current context)*/
GelEFunc *
d_addfunc (GelEFunc *func)
{
	GelEFunc *n;
	
	g_return_val_if_fail(func->context == context.top,func);
	
	/*we already found it (in current context)*/
	n = d_lookup_local(func->id);
	if(n) {
		d_replacefunc(n,func);
		return n;
	}

	context.stack->data = g_slist_prepend(context.stack->data,func);
	
	func->id->refs = g_slist_prepend(func->id->refs,func);
	func->id->curref = func;

	return func;
}

/*add a function struct to the dict (in global context)*/
GelEFunc *
d_addfunc_global (GelEFunc *func)
{
	GelEFunc *n;
	GSList *last;
	
	g_return_val_if_fail (func->context == 0, func);

	/* get the function in the lowest context */
	last = g_slist_last (func->id->refs);
	n = last != NULL ? last->data : NULL;
	/* if this function is global */
	if (n != NULL && n->context == 0) {
		d_replacefunc (n, func);
		return n;
	}

	last = g_slist_last (context.stack);
	g_assert (last != NULL);

	last->data = g_slist_prepend (last->data, func);
	
	func->id->refs = g_slist_append (func->id->refs, func);
	if (func->id->curref == NULL)
		func->id->curref = func;

	return func;
}

/*set value of an existing function (in local context), used for arguments
  WARNING, does not free the memory allocated by previous value!*/
int
d_setvalue(GelToken *id,GelETree *value)
{
	GelEFunc *f;
	f=d_lookup_local(id);
	if(!f || (f->type!=GEL_USER_FUNC &&
		  f->type!=GEL_VARIABLE_FUNC))
		return FALSE;
	f->data.user=value;
	return TRUE;
}

/*dictionary functions*/

/*lookup a function in the dictionary in the current context*/
GelEFunc *
d_lookup_local(GelToken *id)
{
	GelEFunc *func;
	
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
GelEFunc *
d_lookup_global_up1(GelToken *id)
{
	GelEFunc *func;
	
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

GelToken *
d_intern (const char *id)
{
	GelToken * tok = g_hash_table_lookup(dictionary,id);
	if(!tok) {
		tok = g_new0(GelToken,1);
		tok->token = g_strdup(id);
		g_hash_table_insert(dictionary,tok->token,tok);
	}
	return tok;
}

int
d_delete(GelToken *id)
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
			d_popcontext ();
}

/*free all memory allocated by a dictionary*/
void
d_freedict (GSList *n)
{
	GSList *li;
	for (li = n; li != NULL; li = li->next)
		d_freefunc (li->data);
	g_slist_free (n);
}

static void
whack_from_lists (GelEFunc *func)
{
	GSList *li;
	for (li = context.subststack; li != NULL; li = li->next) {
		GSList *fl = g_slist_find (li->data, func);
		if (fl != NULL) {
			li->data = g_slist_delete_link (li->data, fl);
			return;
		}
	}
}

/* Put on subst local var list for this current stack */
void
d_put_on_subst_list (GelEFunc *func)
{
	if (func->on_subst_list) {
		/* On a lower stackframe? */
		/* weird but true.  So whack it and put it here,
		 * it will get to the lower one eventually */
		whack_from_lists (func);
	}
	context.subststack->data = 
		g_slist_prepend (context.subststack->data, func);
	func->on_subst_list = 1;
}

void
d_freefunc (GelEFunc *n)
{
	GSList *li;
	if(!n)
		return;
	g_assert(!n->id || g_slist_find(n->id->refs,n)==NULL);


	if (n->on_subst_list) {
		whack_from_lists (n);
		n->on_subst_list = 0;
	}

	/*if(n->id) {
		n->id->refs = g_slist_remove(n->id->refs,n);
		n->id->curref = n->id->refs?n->id->refs->data:NULL;
	} */
	if((n->type == GEL_USER_FUNC ||
	    n->type == GEL_VARIABLE_FUNC) &&
	   n->data.user)
		gel_freetree(n->data.user);
	g_slist_free(n->named_args);
	for (li = n->extra_dict; li != NULL; li = li->next)
		d_freefunc (li->data);
	g_slist_free (n->extra_dict);
	/*prepend to free list*/
	n->data.next = free_funcs;
	free_funcs = n;
}

/*replace old with stuff from new and free new,
  new has to have the same id, also new->id should
  not hold a reference to new*/
void
d_replacefunc(GelEFunc *old,GelEFunc *new)
{
	GSList *li;

	g_return_if_fail(old && new);
	g_return_if_fail(old->id == new->id);

	if(old->type == GEL_USER_FUNC ||
	   old->type == GEL_VARIABLE_FUNC)
		gel_freetree(old->data.user);

	g_slist_free(old->named_args);

	for (li = old->extra_dict; li != NULL; li = li->next)
		d_freefunc (li->data);
	g_slist_free (old->extra_dict);

	memcpy(old,new,sizeof(GelEFunc));

	/*prepend to free list*/
	new->data.next = free_funcs;
	free_funcs = new;
}

/*set_ref*/
void
d_set_ref(GelEFunc *n,GelEFunc *ref)
{
	if(!n || !ref)
		return;
	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC)
		gel_freetree(n->data.user);
	n->type = GEL_REFERENCE_FUNC;
	g_slist_free(n->named_args);
	n->nargs = 0;
	n->named_args = NULL;
	
	n->data.ref = ref;
}

/*set_value*/
void
d_set_value(GelEFunc *n,GelETree *value)
{
	if(!n || !value)
		return;
	if(n->type == GEL_USER_FUNC ||
	   n->type == GEL_VARIABLE_FUNC)
		gel_freetree(n->data.user);
	n->type = GEL_VARIABLE_FUNC;
	g_slist_free(n->named_args);
	n->nargs = 0;
	n->named_args = NULL;
	
	n->data.user = value;
}

/*push a new dictionary onto the context stack*/
int
d_addcontext(void)
{
	context.stack = g_slist_prepend (context.stack, NULL);
	context.subststack = g_slist_prepend (context.subststack, NULL);
	context.top++;
	return TRUE;
}

/*pop the context stack*/
void
d_popcontext(void)
{
	if (context.top != -1) {
		GSList *li;
		GSList *dict = context.stack->data;
		GSList *subst = context.subststack->data;
		GSList *substlast = NULL;

		if (context.top != 0) {
			for (li = subst; li != NULL; li = li->next) {
				GelEFunc *func = li->data;
				if (context.top == 1)
					func->on_subst_list = 0;

				if (func->type == GEL_USER_FUNC ||
				    func->type == GEL_VARIABLE_FUNC) {
					D_ENSURE_USER_BODY (func);
					func->extra_dict =
						gel_subst_local_vars (func->extra_dict,
								      func->data.user);
				}

				substlast = li;
			}
		}

		for (li = dict; li != NULL; li = li->next) {
			GelEFunc *func = li->data;
			func->id->refs = g_slist_remove(func->id->refs,func);
			func->id->curref =
				func->id->refs?func->id->refs->data:NULL;
		}
		context.top--;

		context.stack = g_slist_delete_link (context.stack, context.stack);
		context.subststack = g_slist_delete_link (context.subststack, context.subststack);

		/* substitute lower variables unless we are on the toplevel */
		if (substlast != NULL && context.top > 0) {
			context.subststack->data = subst;
			substlast->next = context.subststack->data;
			subst = NULL;
		}

		d_freedict (dict);
		g_slist_free (subst);
	}
}

/*gimme the current dictinary*/
GSList *
d_getcontext(void)
{
	if(context.top==-1)
		return NULL;
	else
		return context.stack->data;
}

void
d_protect_all(void)
{
	GSList *funcs;
	GSList *li;
	funcs = d_getcontext();
	if(!funcs) return;
	for(li=funcs;li;li=g_slist_next(li)) {
		GelEFunc *func = li->data;
		if(!func->id || strcmp(func->id->token,"Ans")==0)
			continue;
		if ( ! func->id->parameter)
			func->id->protected = 1;
	}
}

void
d_add_named_args (GelEFunc *f, const char *args)
{
	int i;
	char **s;

	if (args == NULL || args[0] == '\0')
		return;
       
	s = g_strsplit (args, ",", -1);
	for (i = 0; s[i] != NULL; i++) {
		f->named_args = g_slist_append (f->named_args, d_intern (s[i]));
	}
	g_strfreev (s);
}

