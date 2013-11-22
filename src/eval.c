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
#include <gnome.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "mpwrap.h"
#include "eval.h"
#include "calc.h"
#include "dict.h"
#include "util.h"
#include "funclib.h"
#include "mymath.h"

extern calc_error_t error_num;
extern calcstate_t calcstate;

/*if we "return" we jump out of the current function using an exception and
  set this, if we are at the top most context, this is what is "returned to
  the "screen"*/
ETree *returnval = NULL;

extern void (*errorout)(char *);

static ETree *free_trees = NULL;

/*returns the number of args for an operator, or -1 if it takes up till
  exprlist marker or -2 if it takes one more for the first argument*/
int
branches(int op)
{
	switch(op) {
		case E_SEPAR: return 2;
		case E_EQUALS: return 2;
		case E_ABS: return 1;
		case E_PLUS: return 2;
		case E_MINUS: return 2;
		case E_MUL: return 2;
		case E_DIV: return 2;
		case E_MOD: return 2;
		case E_NEG: return 1;
		case E_EXP: return 2;
		case E_FACT: return 1;
		case E_IF_CONS: return 2;
		case E_IFELSE_CONS: return 3;
		case E_WHILE_CONS: return 2;
		case E_UNTIL_CONS: return 2;
		case E_DOWHILE_CONS: return 2;
		case E_DOUNTIL_CONS: return 2;
		case E_EQ_CMP: return 2;
		case E_NE_CMP: return 2;
		case E_CMP_CMP: return 2;
		case E_LT_CMP: return 2;
		case E_GT_CMP: return 2;
		case E_LE_CMP: return 2;
		case E_GE_CMP: return 2;
		case E_LOGICAL_AND: return 2;
		case E_LOGICAL_OR: return 2;
		case E_LOGICAL_XOR: return 2;
		case E_LOGICAL_NOT: return 1;
		case E_GET_ELEMENT: return 3;
		case E_GET_ROW: return 2;
		case E_GET_COLUMN: return 2;
		case E_REFERENCE: return 1;
		case E_DEREFERENCE: return 1;
		case E_DIRECTCALL: return -2;
		case E_CALL: return -2;
		case E_RETURN: return 1;
	}
	return 0;
}

ETree *
makenum_null(void)
{
	ETree *n;
	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}
	n->type = NULL_NODE;
	n->args = NULL;
	n->nargs = 0;
	return n;
}

ETree *
makenum_ui(unsigned long num)
{
	ETree *n;
	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}
	n->type=VALUE_NODE;
	n->args = NULL;
	n->nargs = 0;
	mpw_init(n->data.value);
	mpw_set_ui(n->data.value,num);
	return n;
}

ETree *
makenum_si(long num)
{
	ETree *n;
	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}
	n->type=VALUE_NODE;
	n->args = NULL;
	n->nargs = 0;
	mpw_init(n->data.value);
	mpw_set_si(n->data.value,num);
	return n;
}

ETree *
makenum(mpw_t num)
{
	ETree *n;
	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}
	n->type=VALUE_NODE;
	n->args = NULL;
	n->nargs = 0;
	mpw_init_set(n->data.value,num);
	return n;
}

ETree *
makeoperator(int oper, evalstack_t * stack)
{
	ETree *n;
	int args;
	GList *list = NULL;
	args = branches(oper);
	if(args>=0) {
		int i;
		for(i=0;i<args;i++) {
			ETree *tree = stack_pop(stack);
			if(!tree)  {
				g_list_foreach(list,(GFunc)freetree,NULL);
				g_list_free(list); 
				return NULL;
			}
			list = g_list_prepend(list,tree);
		}
	} else {
		int i=0;
		for(;;) {
			ETree *tree;
			tree = stack_pop(stack);
			/*we have gone all the way to the top and haven't
			  found a marker*/
			if(!tree) {
				g_list_foreach(list,(GFunc)freetree,NULL);
				g_list_free(list); 
				return NULL;
			}
			if(tree->type==EXPRLIST_START_NODE) {
				freetree(tree);
				/*pop one more in case of -2*/
				if(args==-2) {
					ETree *t;
					t = stack_pop(stack);
					/*we have gone all the way to the top
					  whoops!*/
					if(!t) {
						g_list_foreach(list,
							       (GFunc)freetree,
							       NULL);
						g_list_free(list); 
						return NULL;
					}
					list = g_list_prepend(list,t);
					i++;
				}
				break;
			}
			list = g_list_prepend(list,tree);
			i++;
		}
		args = i;
	}

	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}
	n->type = OPERATOR_NODE;
	n->data.oper = oper;
	
	n->args = list;
	n->nargs = args;

	return n;
}

void
freenode(ETree *n)
{
	if(!n)
		return;
	if(n->type==VALUE_NODE)
		mpw_clear(n->data.value);
	else if(n->type == STRING_NODE)
		g_free(n->data.str);
	else if(n->type == FUNCTION_NODE)
		freefunc(n->data.func);
	/*put onto the free list*/
	n->data.next = free_trees;
	free_trees = n;
}

/*free arguments to a dictionary function*/
void
freeargs(ETree *n)
{
	int i;

	if(!n)
		return;

	g_list_foreach(n->args,(GFunc)freetree,NULL);
	g_list_free(n->args); 
}

void
freetree(ETree *n)
{
	if(!n)
		return;
	freeargs(n);
	freenode(n);
}

ETree *
copynode(ETree *o)
{
	ETree *n;
	GList *li;
	int i;

	if(!o)
		return NULL;

	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}

	memcpy(n,o,sizeof(ETree));

	if(o->type==VALUE_NODE)
		mpw_init_set(n->data.value,o->data.value);
	else if(o->type == STRING_NODE)
		n->data.str = g_strdup(o->data.str);
	else if(o->type == FUNCTION_NODE)
		n->data.func = d_copyfunc(o->data.func);
	/*FIXME: copy matrix as well*/
	
	/*copy the arguments*/
	n->args = NULL;
	for(li = o->args;li!=NULL;li=g_list_next(li))
		n->args = g_list_append(n->args,copynode(li->data));

	return n;
}

/*evaluate arguments, will crash if the node doesn't have at least
  one arg*/
static int
evalargs(ETree *n, ETree *ret[], int eval_first)
{
	GList *li;
	int i;
	int r = 0;
	
	/*if the first shouldn't be evaluated just pass it*/
	if(!eval_first)
		ret[0] = copynode(n->args->data);
	else {
		ret[0] = evalnode(n->args->data);
		/*exception is signaled by NULL*/
		if(!ret[0])
			return 2;
			
	}
	
	for(i=1,li=n->args->next; li!=NULL; li=g_list_next(li),i++) {
		ret[i] = evalnode(li->data);
		/*exception is signaled by NULL*/
		if(!ret[i]) {
			while(--i>=0)
				freetree(ret[i]);
			return 2;
		}
		/*unevaluated*/
		/*FIXME: this needs to be some sort of exception
		  or whatnot, with matrices, we won't be able to
		  distinguish*/
		if(ret[i]->type == IDENTIFIER_NODE ||
		   (ret[i]->type == OPERATOR_NODE &&
		    ret[i]->data.oper != E_REFERENCE))
			r = 1;
	}
	return r;
}

/*evaluate a matrix (or try to), this might error out if the result is not
  a matrix such as if b = [8,7]; a = [1,2:3,b] .. then evaluating the second
  matrix will fail with an error, however a = [1,2,4:3,b] will work just fine */
static ETree *
evalmatrix(ETree *n)
{
	/*FIXME:EVALUATE MATRIX HERE*/
	return copynode(n);
}

static ETree *
equalsop(ETree *n)
{
	ETree *l;
	ETree *r;
	ETree *set;
	GET_LR(n,l,r);
	
	if(l->type != IDENTIFIER_NODE &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_DEREFERENCE)) {
		(*errorout)(_("Lvalue not an identifier/dereference!"));
		return copynode(n);
	}

	if(l->type == IDENTIFIER_NODE) {
		set = evalnode(r);
		if(set->type == FUNCTION_NODE)
			d_addfunc(d_makerealfunc(set->data.func,l->data.id));
		else if(set->type == OPERATOR_NODE &&
			set->data.oper == E_REFERENCE) {
			ETree *t = set->args->data;
			EFunc *rf = d_lookup_global(t->data.id);
			d_addfunc(d_makereffunc(l->data.id,rf));
		} else
			d_addfunc(d_makeufunc(l->data.id,copynode(set),NULL,0));
	} else { /*dereference*/
		ETree *ll;
		EFunc *f;
		GET_L(l,ll);

		if(ll->type != IDENTIFIER_NODE) {
			(*errorout)(_("Dereference of non-identifier!"));
			return copynode(n);
		}
		
		f = d_lookup_global(ll->data.id);
		if(f->type!=REFERENCE_FUNC) {
			(*errorout)(_("Dereference of non-reference!"));
			return copynode(n);
		}
		
		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;

		if(set->type == FUNCTION_NODE)
			d_setrealfunc(f->data.ref,set->data.func);
		else if(set->type == OPERATOR_NODE &&
			set->data.oper == E_REFERENCE) {
			ETree *t = set->args->data;
			EFunc *rf = d_lookup_global(t->data.id);
			d_set_ref(f->data.ref,rf);
		} else
			d_set_value(f->data.ref,copynode(set));
	}
	return set;
}

/*copy node but use the args from r*/
ETree *
copynode_args(ETree *o, ETree *r[])
{
	ETree *n;
	GList *li;
	int i;

	if(!o || !r)
		return NULL;

	if(!free_trees)
		n = g_new(ETree,1);
	else {
		n = free_trees;
		free_trees = free_trees->data.next;
	}

	memcpy(n,o,sizeof(ETree));

	if(o->type==VALUE_NODE)
		mpw_init_set(n->data.value,o->data.value);
	else if(o->type == STRING_NODE)
		n->data.str = g_strdup(o->data.str);
	else if(o->type == FUNCTION_NODE)
		n->data.func = d_copyfunc(o->data.func);
	/*FIXME: copy matrix as well*/
	
	/*add the arguments*/
	n->args = NULL;
	for(i=0;i<n->nargs;i++)
		n->args = g_list_append(n->args,r[i]);

	return n;
}

static void
freeargarr(ETree *r[],int n)
{
	int i;
	for(i=0;i<n;i++)
		freetree(r[i]);
}

static ETree *
funccallop(ETree *n,int direct_call)
{
	ETree **r;
	ETree *ret;
	EFunc *f;
	ETree *l;
	
	GET_L(n,l);
	
	r = g_new(ETree *,n->nargs);
	switch(evalargs(n,r,
			(direct_call ||
			 (l->type == OPERATOR_NODE &&
			  l->data.oper == E_DEREFERENCE))?FALSE:TRUE
		       )) {
	case 1:
		ret = copynode_args(n,r);
		g_free(r);
		return ret;
	case 2:
		g_free(r);
		return NULL;
	}
	
	if(l->type == IDENTIFIER_NODE) {
		f = d_lookup_global(l->data.id);
		if(!f) {
			char buf[256];
			g_snprintf(buf,256,_("Function '%s' used unintialized"),
				   l->data.id->token);
			(*errorout)(buf);
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		}
	} else if(l->type == FUNCTION_NODE) {
		f = l->data.func;
	} else if(l->type == OPERATOR_NODE &&
		l->data.oper == E_DEREFERENCE) {
		ETree *ll;
		GET_L(l,ll);
		f = d_lookup_global(ll->data.id);
		if(!f) {
			char buf[256];
			g_snprintf(buf,256,_("Variable '%s' used unintialized"),
				   ll->data.id->token);
			(*errorout)(buf);
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		} else if(f->type != REFERENCE_FUNC) {
			char buf[256];
			g_snprintf(buf,256,_("Can't dereference '%s'!"),
				   ll->data.id->token);
			(*errorout)(buf);
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		}
		f = f->data.ref;
	} else {
		(*errorout)(_("Can't call a non-function!"));
		ret = copynode_args(n,r);
		g_free(r);
		return ret;
	}
	
	g_assert(f);
	
	if(f->nargs + 1 != n->nargs) {
		char buf[256];
		g_snprintf(buf,256,_("Call of '%s' with the wrong number of arguments!\n"
				     "(should be %d)"),f->id?f->id->token:"anonymous",f->nargs);
		(*errorout)(buf);
		ret = copynode_args(n,r);
		g_free(r);
		return ret;
	} else if(f->type == USER_FUNC) {
		int i;
		GList *li;
		GList *tofree=NULL;
		d_addcontext();
		/*push arguments on context stack*/
		for(i=1,li=f->named_args;i<n->nargs;i++,li=g_list_next(li)) {
			if(r[i]->type == FUNCTION_NODE) {
				d_addfunc(d_makereffunc(li->data,
							r[i]->data.func));
				tofree = g_list_append(tofree,r[i]);
			} else if(r[i]->type == OPERATOR_NODE &&
			   r[i]->data.oper == E_REFERENCE) {
				ETree *t = r[i]->args->data;
				EFunc *rf = d_lookup_global(t->data.id);
				d_addfunc(d_makereffunc(li->data,rf));
				freetree(r[i]);
			} else
				d_addfunc(d_makeufunc(li->data,r[i],NULL,0));
		}
		g_free(r);
		if(returnval) {
			(*errorout)(_("Extraneous return value!"));
			freetree(returnval);
		}
		returnval = NULL;
		ret = evalnode(f->data.user);
		/*free trees that are schedhuled to be freed after execution
		  of body*/
		if(tofree){
			freetree(tofree->data);
			for(li=g_list_next(tofree);li!=NULL;li=g_list_next(li))
				freetree(li->data);
			g_list_free(tofree);
		}
		freedict(d_popcontext());
		if(!ret && returnval)
			ret = returnval;
		else if(returnval)
			freetree(returnval);
		returnval = NULL;
		return ret;
	} else if(f->type == BUILTIN_FUNC) {
		int exception = FALSE;
		ret = (*f->data.func)(&r[1],&exception);
		if(!ret && !exception)
			ret = copynode_args(n,r);
		else
			freeargarr(r,n->nargs);
		g_free(r);
		if(exception) {
			if(ret)
				freetree(ret);
			return NULL;
		}
		return ret;
	} else if(f->type == REFERENCE_FUNC) {
		ETree *a;
		ETree *i;
		if(f->nargs>0) {
			(*errorout)(_("Reference function with arguments encountered!"));
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		}
		f = f->data.ref;
		if(!f->id) {
			(*errorout)(_("Unnamed reference function encountered!"));
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		}
		if(!free_trees)
			i = g_new(ETree,1);
		else {
			i = free_trees;
			free_trees = free_trees->data.next;
		}
		i->type = IDENTIFIER_NODE;
		i->data.id = f->id; /*this WILL have an id*/
		i->args = NULL;
		i->nargs = 0;

		if(!free_trees)
			a = g_new(ETree,1);
		else {
			a = free_trees;
			free_trees = free_trees->data.next;
		}
		a->type = OPERATOR_NODE;
		a->data.oper = E_REFERENCE;

		a->args = g_list_append(NULL,i);
		a->nargs = 1;
		freeargarr(r,n->nargs);
		g_free(r);
		return a;
	} else {
		(*errorout)(_("Unevaluatable function type encountered!"));
		ret = copynode_args(n,r);
		g_free(r);
		return ret;
	}
}

static ETree *
derefvarop(ETree *n)
{
	EFunc *f;
	ETree *l;
	
	GET_L(n,l);
	
	
	f = d_lookup_global(l->data.id);
	if(!f) {
		char buf[256];
		g_snprintf(buf,256,_("Variable '%s' used unintialized"),l->data.id->token);
		(*errorout)(buf);
	} else if(f->nargs != 0) {
		char buf[256];
		g_snprintf(buf,256,_("Call of '%s' with the wrong number of arguments!\n"
				     "(should be %d)"),f->id?f->id->token:"anonymous",f->nargs);
		(*errorout)(buf);
	} else if(f->type == USER_FUNC || f->type == BUILTIN_FUNC) {
		char buf[256];
		g_snprintf(buf,256,_("Trying to dereference '%s' which is not a reference!\n"),
				     f->id?f->id->token:"anonymous");
		(*errorout)(buf);
	} else if(f->type == REFERENCE_FUNC) {
		f = f->data.ref;
		if(!f) {
			(*errorout)(_("NULL reference encountered!"));
		} else if(f->type == USER_FUNC) {
			ETree *ret;
			d_addcontext();
			if(returnval) {
				(*errorout)(_("Extraneous return value!"));
				freetree(returnval);
			}
			returnval = NULL;
			ret = evalnode(f->data.user);
			freedict(d_popcontext());
			if(!ret && returnval)
				ret = returnval;
			else if(returnval)
				freetree(returnval);
			returnval = NULL;
			return ret;
		} else if(f->type == BUILTIN_FUNC) {
			ETree *ret;
			int exception = FALSE;
			ret = (*f->data.func)(NULL,&exception);
			if(!ret && !exception) ret = copynode(n);
			if(exception) {
				if(ret)
					freetree(ret);
				return NULL;
			}
			return ret;
		} else if(f->type == REFERENCE_FUNC) {
			ETree *a;
			ETree *i;
			if(!f->id) {
				(*errorout)(_("Unnamed reference function encountered"));
				return copynode(n);
			}
			if(!free_trees)
				i = g_new(ETree,1);
			else {
				i = free_trees;
				free_trees = free_trees->data.next;
			}
			i->type = IDENTIFIER_NODE;
			i->data.id = f->id; /*this WILL have an id*/
			i->args = NULL;
			i->nargs = 0;

			if(!free_trees)
				a = g_new(ETree,1);
			else {
				a = free_trees;
				free_trees = free_trees->data.next;
			}
			a->type = OPERATOR_NODE;
			a->data.oper = E_REFERENCE;
			
			a->args = g_list_append(NULL,i);
			a->nargs = 1;
			return a;
		} else
			(*errorout)(_("Unevaluatable function type encountered!"));
	} else
		(*errorout)(_("Unevaluatable function type encountered!"));
	return copynode(n);
}

static ETree *
variableop(ETree *n)
{
	EFunc *f;
	
	f = d_lookup_global(n->data.id);
	if(!f) {
		char buf[256];
		g_snprintf(buf,256,_("Variable '%s' used unitialized"),n->data.id->token);
		(*errorout)(buf);
		return copynode(n);
	}
	
	if(f->nargs != 0) {
		char buf[256];
		g_snprintf(buf,256,_("Call of '%s' with the wrong number of arguments!\n"
				     "(should be %d)"),f->id?f->id->token:"anonymous",f->nargs);
		(*errorout)(buf);
	} else if(f->type == USER_FUNC) {
		ETree *ret;
		d_addcontext();
		if(returnval) {
			(*errorout)(_("Extraneous return value!"));
			freetree(returnval);
		}
		returnval = NULL;
		ret = evalnode(f->data.user);
		freedict(d_popcontext());
		if(!ret && returnval)
			ret = returnval;
		else if(returnval)
			freetree(returnval);
		returnval = NULL;
		return ret;
	} else if(f->type == BUILTIN_FUNC) {
		ETree *ret;
		int exception = FALSE;
		ret = (*f->data.func)(NULL,&exception);
		if(!ret && !exception) ret = copynode(n);
		if(exception) {
			if(ret)
				freetree(ret);
			return NULL;
		}
		return ret;
	} else if(f->type == REFERENCE_FUNC) {
		ETree *a;
		ETree *i;
		f = f->data.ref;
		if(!free_trees)
			i = g_new(ETree,1);
		else {
			i = free_trees;
			free_trees = free_trees->data.next;
		}
		i->type = IDENTIFIER_NODE;
		i->data.id = f->id; /*this WILL have an id*/
		i->args = NULL;
		i->nargs = 0;

		if(!free_trees)
			a = g_new(ETree,1);
		else {
			a = free_trees;
			free_trees = free_trees->data.next;
		}
		a->type = OPERATOR_NODE;
		a->data.oper = E_REFERENCE;

		a->args = g_list_append(NULL,i);
		a->nargs = 1;
		return a;
	} else
		(*errorout)(_("Unevaluatable function type encountered!"));
	return copynode(n);
}

static ETree *
ifop(ETree *n)
{
	ETree *l,*r,*rr;
	ETree *errorret;
	int exception=FALSE;
	if(n->data.oper == E_IF_CONS) {
		GET_LR(n,l,r);
	} else { /*IF_ELSE*/
		GET_LRR(n,l,r,rr);
	}

	if(eval_isnodetrue(l,&exception,&errorret)) {
		return evalnode(r);
	} else {
		/*if we got a non-critical error and just can't evaluate*/
		if(errorret) {
			/*copy the current node and replace the predicate
			  with the evaluated value*/
			ETree *a = copynode(n);
			freetree(a->args->data);
			a->args->data = errorret;
			return a;
		}
		if(exception) return NULL;
		if(n->data.oper == E_IF_CONS)
			return makenum_null();
		else /*IF_ELSE*/
			return evalnode(rr);
	}
}

static ETree *
loopop(ETree *n)
{
	ETree *l,*r,*ret = makenum_null();
	ETree *t;
	ETree *errorret;
	int exception=FALSE;
	int until=FALSE;
	int first=FALSE;

	GET_LR(n,l,r);
	
	switch (n->data.oper) {
	case E_WHILE_CONS:
		until = FALSE;
		first = FALSE;
		break;
	case E_UNTIL_CONS:
		until = TRUE;
		first = FALSE;
		break;
	case E_DOWHILE_CONS:
		/*swap left/right*/
		t = l; l = r; r = t;
		until = FALSE;
		first = TRUE;
		break;
	case E_DOUNTIL_CONS:
		/*swap left/right*/
		t = l; l = r; r = t;
		until = TRUE;
		first = TRUE;
		break;
	}
	if(first) {
		freetree(ret);
		ret = evalnode(r);
		if(!ret) return NULL;
	}
	while(until?
	      !eval_isnodetrue(l,&exception,&errorret):
	      eval_isnodetrue(l,&exception,&errorret)) {
		freetree(ret);
		if(exception)
			return NULL;
		/*if we got a non-critical error and just can't evaluate*/
		else if(errorret) {
			/*copy the current node and replace the predicate with the
			  evaluated value*/
			ETree *a = copynode(n);
			freetree(a->args->data);
			a->args->data = errorret;
			return a;
		}
		ret = evalnode(r);
		if(!ret) return NULL;
	}
	if(exception) {
		freetree(ret);
		return NULL;
	/*if we got a non-critical error and just can't evaluate*/
	} else if(errorret) {
		/*copy the current node and replace the predicate with the
		  evaluated value*/
		ETree *a = copynode(n);
		freetree(ret);
		freetree(a->args->data);
		a->args->data = errorret;
		return a;
	}
	return ret;
}

/*compare nodes, return TRUE if equal
  makes them the same type as a side effect*/
static int
eqlnodes(ETree *l, ETree *r)
{
	return mpw_eql(l->data.value,r->data.value);
}

/*compare nodes, return -1 if first one is smaller, 0 if they are
  equal, 1 if the first one is greater
  makes them the same type as a side effect*/
static int
cmpnodes(ETree *l, ETree *r)
{
	int n=0;

	n=mpw_cmp(l->data.value,r->data.value);

	if(n>0) n=1;
	else if(n<0) n=-1;
	return n;
}

static ETree *
eqcmpop(ETree *l, ETree *r)
{
	if(eqlnodes(l,r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
necmpop(ETree *l, ETree *r)
{
	if(!eqlnodes(l,r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
cmpcmpop(ETree *l, ETree *r)
{
	return makenum_si(cmpnodes(l,r));
}

static ETree *
ltcmpop(ETree *l, ETree *r)
{
	if(cmpnodes(l,r)<0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
gtcmpop(ETree *l, ETree *r)
{
	if(cmpnodes(l,r)>0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
lecmpop(ETree *l, ETree *r)
{
	if(cmpnodes(l,r)<=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
gecmpop(ETree *l, ETree *r)
{
	if(cmpnodes(l,r)>=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
logicalandop(ETree *l, ETree *r)
{
	if(isnodetrue(l) && isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
logicalorop(ETree *l, ETree *r)
{
	if(isnodetrue(l) || isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
logicalxorop(ETree *l, ETree *r)
{
	if(isnodetrue(l) != isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
logicalnotop(ETree *r)
{
	if(isnodetrue(r))
		return makenum_ui(0);
	else
		return makenum_ui(1);
}


typedef void (*doubleopfunc)(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2);
typedef void (*singleopfunc)(mpw_ptr rop,mpw_ptr op);

static ETree *
doubleop(ETree *n, ETree *arg[], doubleopfunc f)
{
	ETree *r=NULL;
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->data.value,arg[1]->data.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode(n);
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	r=makenum(res);
	mpw_clear(res);

	return r;
}

static ETree *
singleop(ETree *n, ETree *arg[], singleopfunc f)
{
	ETree *r=NULL;
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->data.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode(n);
	}
	r=makenum(res);
	mpw_clear(res);

	return r;
}

/*returns 0 if all numeric, 1 if numeric/matrix, 2 otherwise*/
int
arglevel(ETree *r[], int cnt)
{
	int i;
	int level = 0;
	for(i=0;i<cnt;i++) {
		if(r[i]->type!=VALUE_NODE) {
			if(r[i]->type==MATRIX_NODE)
				level = 1;
			else
				return 2;
		}
	}
	return level;
}

typedef ETree * (*matrixpfunc_t)(ETree *,ETree **);

#define EVAL_PRIMITIVE(n,numop,numfunc,matrixfunc) {	\
	ETree *r[2]; 					\
	ETree *ret;					\
	switch(evalargs(n,r,TRUE)) {			\
	case 1:						\
		return copynode_args(n,r);		\
	case 2:						\
		return NULL;				\
	}						\
	switch(arglevel(r,n->nargs)) {			\
	case 0: ret = numop(n,r,numfunc);		\
		freeargarr(r,n->nargs);			\
		return ret;				\
	case 1: if(matrixfunc) {			\
			matrixpfunc_t m = matrixfunc;	\
			ret = (*m)(n,r);		\
			freeargarr(r,n->nargs);		\
			return ret;			\
		} else {				\
			(*errorout)(_("Primitive on matrixes undefined")); \
			return copynode_args(n,r);	\
		}					\
	default:					\
		(*errorout)(_("Primitives must get numeric/matrix arguments")); \
		return copynode_args(n,r);		\
	}						\
}

typedef ETree * (*matrixlfunc_t)(ETree *,ETree *);

#define EVAL_LOGIC(n,numfunc,matrixfunc) {		\
	ETree *r[2]; 					\
	ETree *ret;					\
	switch(evalargs(n,r,TRUE)) {			\
	case 1:						\
		return copynode_args(n,r);		\
	case 2:						\
		return NULL;				\
	}						\
	switch(arglevel(r,2)) {				\
	case 0: ret = numfunc(r[0],r[1]);		\
		freeargarr(r,2);			\
		return ret;				\
	case 1: if(matrixfunc) {			\
			matrixlfunc_t m = matrixfunc;	\
			ret = (*m)(r[0],r[1]);		\
			freeargarr(r,2);		\
			return ret;			\
		} else {				\
			(*errorout)(_("Logical operation on matrixes undefined")); \
			return copynode_args(n,r);	\
		}					\
	default:					\
		(*errorout)(_("Logical ops must get numeric/matrix arguments")); \
		return copynode_args(n,r);		\
	}						\
}

static ETree *
evaloper(ETree *n)
{
	switch(n->data.oper) {
		case E_SEPAR:
			if(!evalnode(n->args->data))
				return NULL; /*exception*/
			return evalnode(n->args->next->data);

		case E_EQUALS:
			return equalsop(n);

		case E_ABS: EVAL_PRIMITIVE(n,singleop,mpw_abs,NULL);
		case E_PLUS: EVAL_PRIMITIVE(n,doubleop,mpw_add,NULL);
		case E_MINUS: EVAL_PRIMITIVE(n,doubleop,mpw_sub,NULL);
		case E_MUL: EVAL_PRIMITIVE(n,doubleop,mpw_mul,NULL);
		case E_DIV: EVAL_PRIMITIVE(n,doubleop,mpw_div,NULL);
		case E_MOD: EVAL_PRIMITIVE(n,doubleop,mpw_mod,NULL);
		case E_NEG: EVAL_PRIMITIVE(n,singleop,mpw_neg,NULL);
		case E_EXP: EVAL_PRIMITIVE(n,doubleop,mpw_pow,NULL);
		case E_FACT: EVAL_PRIMITIVE(n,singleop,mpw_fac,NULL);
		case E_EQ_CMP: EVAL_LOGIC(n,eqcmpop,NULL);
		case E_NE_CMP: EVAL_LOGIC(n,necmpop,NULL);
		case E_CMP_CMP: EVAL_LOGIC(n,cmpcmpop,NULL);
		case E_LT_CMP: EVAL_LOGIC(n,ltcmpop,NULL);
		case E_GT_CMP: EVAL_LOGIC(n,gtcmpop,NULL);
		case E_LE_CMP: EVAL_LOGIC(n,lecmpop,NULL);
		case E_GE_CMP: EVAL_LOGIC(n,gecmpop,NULL);
		case E_LOGICAL_AND: EVAL_LOGIC(n,logicalandop,NULL);
		case E_LOGICAL_OR: EVAL_LOGIC(n,logicalorop,NULL);
		case E_LOGICAL_XOR: EVAL_LOGIC(n,logicalxorop,NULL);
		case E_LOGICAL_NOT: {
			ETree *r;
			ETree *ret;
			r = evalnode(n->args->data);
			if(!r)
				return NULL;
			if(r->type == VALUE_NODE) {
				ret = logicalnotop(r);
				freetree(r);
				return ret;
			} else {
				(*errorout)(_("Logical not only define on numbers"));
				ret = copynode(n);
				freetree(ret->args->data);
				ret->args->data = r;
				return ret;
			}
		}

		/*FIXME:*/
		case E_GET_ELEMENT:
		case E_GET_ROW:
		case E_GET_COLUMN:
			return copynode(n);/*!!!*/

		case E_REFERENCE:
			return copynode(n);
		case E_DEREFERENCE:
			return derefvarop(n);

		case E_IF_CONS:
		case E_IFELSE_CONS:
			return ifop(n);
		case E_WHILE_CONS:
		case E_UNTIL_CONS:
		case E_DOWHILE_CONS:
		case E_DOUNTIL_CONS:
			return loopop(n);

		case E_DIRECTCALL:
			return funccallop(n,TRUE);
		case E_CALL:
			return funccallop(n,FALSE);
		case E_RETURN:
			{
				ETree *l;
				ETree *r;
				GET_L(n,l);
				if(returnval) {
					(*errorout)(_("Extraneous return value!"));
					freetree(returnval);
				}
				r = evalnode(l);
				if(returnval) {
					if(r) freetree(returnval);
				} else if(r) {
					returnval = r;
				} else {
					(*errorout)(_("Can't evaluate return value!"));
					returnval = copynode(l);
				}

				/*use exception to jump out of the current
				  context (that is until a caller realizes
				  that returnval is set)*/
				return NULL;
			}

		default:
			(*errorout)(_("Unexpected operator!"));
			return copynode(n);
	}
}

ETree *
evalnode(ETree *n)
{
	if(!n)
		return NULL;
	else if(n->type == VALUE_NODE ||
		n->type == NULL_NODE ||
		n->type == FUNCTION_NODE ||
		n->type == STRING_NODE)
		return copynode(n);
	else if(n->type == MATRIX_NODE)
		return evalmatrix(n);
	else if(n->type == OPERATOR_NODE)
		return evaloper(n);
	else if(n->type == IDENTIFIER_NODE)
		return variableop(n);
	else {
		(*errorout)(_("Unexpected node!"));
		return copynode(n);
	}
}

/*return TRUE if node is true (a number node !=0), false otherwise*/
int
isnodetrue(ETree *n)
{
	if(n->type!=VALUE_NODE)
		return FALSE;
	if(mpw_sgn(n->data.value)!=0)
		return TRUE;
	else
		return FALSE;
}

int
eval_isnodetrue(ETree *n, int *exception, ETree **errorret)
{
	/*sane return values*/
	if(exception) *exception = FALSE;
	if(errorret) *errorret = NULL;
	/*we don't have to make a copy and evaluate if the tree is a number
	  already*/
	if(n->type!=VALUE_NODE) {
		int r=FALSE;
		ETree *m;

		m=evalnode(n);
		if(!m) {
			/*raise exception*/
			if(exception) *exception = TRUE;
			return FALSE;
		}
		if(m->type!=VALUE_NODE) {
			(*errorout)(_("Could not evaluate predicate into a numeric boolean value!"));
			if(errorret)
				*errorret = m;
			else
				freetree(m);
			return FALSE;
		}
		if(mpw_sgn(m->data.value)!=0)
			r = TRUE;
		else
			r = FALSE;
		freetree(m);
		return r;
	}
	if(mpw_sgn(n->data.value)!=0)
		return TRUE;
	else
		return FALSE;
}

