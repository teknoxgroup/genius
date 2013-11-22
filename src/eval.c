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

extern void (*errorout)(char *);

static tree_t *free_trees = NULL;

/*returns 100 010 or 110 depending if the operation has left, right or both
branches, sometimes ??1 will mean a second right is used*/
int
branches(int op)
{
	switch(op) {
		case E_SEPAR: return 6;
		case E_EQUALS: return 6;
		case E_PLUS: return 6;
		case E_MINUS: return 6;
		case E_MUL: return 6;
		case E_DIV: return 6;
		case E_MOD: return 6;
		case E_NEG: return 2;
		case E_EXP: return 6;
		case E_FACT: return 4;
		case E_FUNCDEF: return 6;
		case E_IF_CONS: return 6;
		case E_IFELSE_CONS: return 7;
		case E_WHILE_CONS: return 6;
		case E_EQ_CMP: return 6;
		case E_NE_CMP: return 6;
		case E_CMP_CMP: return 6;
		case E_LT_CMP: return 6;
		case E_GT_CMP: return 6;
		case E_LE_CMP: return 6;
		case E_GE_CMP: return 6;
		case E_LOGICAL_AND: return 6;
		case E_LOGICAL_OR: return 6;
		case E_LOGICAL_XOR: return 6;
		case E_LOGICAL_NOT: return 2;
	}
	return 0;
}

/*similiar to the above, which branches should be evaluated*/
int
brancheseval(int op)
{
	switch(op) {
		case E_SEPAR: return 6;
		case E_EQUALS: return 2;
		case E_PLUS: return 6;
		case E_MINUS: return 6;
		case E_MUL: return 6;
		case E_DIV: return 6;
		case E_MOD: return 6;
		case E_NEG: return 2;
		case E_EXP: return 6;
		case E_FACT: return 4;
		case E_FUNCDEF: return 0;
		case E_IF_CONS: return 4;
		case E_IFELSE_CONS: return 4;
		case E_WHILE_CONS: return 0; /*this has to be done each
						iteration!*/
		case E_EQ_CMP: return 6;
		case E_NE_CMP: return 6;
		case E_CMP_CMP: return 6;
		case E_LT_CMP: return 6;
		case E_GT_CMP: return 6;
		case E_LE_CMP: return 6;
		case E_GE_CMP: return 6;
		case E_LOGICAL_AND: return 6;
		case E_LOGICAL_OR: return 6;
		case E_LOGICAL_XOR: return 6;
		case E_LOGICAL_NOT: return 2;
	}
	return 0;
}

tree_t *
makenum_ui(unsigned long num)
{
	tree_t *n;
	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=NUMBER_NODE;
	mpw_init(n->data.val);
	mpw_set_ui(n->data.val,num);
	memset(n->args,0,sizeof(void *)*8);
	return n;
}

tree_t *
makenum_si(long num)
{
	tree_t *n;
	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=NUMBER_NODE;
	mpw_init(n->data.val);
	mpw_set_si(n->data.val,num);
	memset(n->args,0,sizeof(void *)*8);
	return n;
}

tree_t *
makenum(mpw_t num)
{
	tree_t *n;
	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=NUMBER_NODE;
	mpw_init(n->data.val);
	mpw_set(n->data.val,num);
	memset(n->args,0,sizeof(void *)*8);
	return n;
}

tree_t *
makefuncb(int func,evalstack_t * stack)
{
	tree_t *n;
	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=ACTION_NODE;
	n->data.action.type=PRIMITIVE_TYPE;
	n->data.action.data.primitive=func;
	memset(n->args,0,sizeof(void *)*8);
	if(branches(func)&1)
		n->args[2]=stack_pop(stack);
	if(branches(func)&2)
		n->args[1]=stack_pop(stack);
	if(branches(func)&4)
		n->args[0]=stack_pop(stack);
	return n;
}

tree_t *
makefuncd(func_t * func,evalstack_t * stack)
{
	tree_t *n;
	int i;

	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=ACTION_NODE;
	n->data.action.type=FUNCTION_TYPE;
	n->data.action.data.func=func;
	memset(n->args,0,sizeof(void *)*8);
	for(i=func->nargs-1;i>=0;i--)
		n->args[i]=stack_pop(stack);
	return n;
}

/*same as above but used for declarations, it doesn'y pop any arguments of
  the stack and doesn't build an argument array*/
tree_t *
makefuncd0(func_t * func)
{
	tree_t *n;

	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}
	n->type=ACTION_NODE;
	n->data.action.type=FUNCTION_TYPE;
	n->data.action.data.func=func;
	memset(n->args,0,sizeof(void *)*8);
	return n;
}

void
freenode(tree_t *n)
{
	if(!n)
		return;
	if(n->type==NUMBER_NODE)
		mpw_clear(n->data.val);
	/*put onto the free list*/
	n->args[0] = free_trees;
	free_trees = n;
}

/*free arguments to a dictionary function*/
void
freeargs(tree_t *n)
{
	int i;

	if(n->type!=ACTION_NODE)
		return;

	if(n->data.action.type==PRIMITIVE_TYPE) {
		for(i=0;i<3;i++)
			if(n->args[i])
				freetree(n->args[i]);
	} else { /*function*/
		for(i=0;i<n->data.action.data.func->nargs;i++)
			if(n->args[i])
				freetree(n->args[i]);
	}
}

void
freetree(tree_t *n)
{
	if(!n)
		return;
	freeargs(n);
	freenode(n);
}

tree_t *
copynode(tree_t *o)
{
	tree_t *n;
	int i;

	if(!o)
		return NULL;

	if(!free_trees)
		n = g_new(tree_t,1);
	else {
		n = free_trees;
		free_trees = free_trees->args[0];
	}

	memcpy(n,o,sizeof(tree_t));

	if(o->type==NUMBER_NODE) {
		mpw_init_set(n->data.val,o->data.val);
	} else { /*type==ACTION_NODE*/
		if(o->data.action.type==PRIMITIVE_TYPE) {
			for(i=0;i<3;i++)
				if(o->args[i])
					n->args[i]=copynode(o->args[i]);
		} else if(o->data.action.type==FUNCTION_TYPE) {
			for(i=0;i<n->data.action.data.func->nargs;i++)
				n->args[i]=copynode(o->args[i]);
		}
	}

	return n;
}

/*evaluate arguments of a function*/
tree_t *
evalargs(tree_t *n)
{
	int i;
	if(n->type!=ACTION_NODE || n->data.action.type!=FUNCTION_TYPE)
		return NULL;

	for(i=0;i<n->data.action.data.func->nargs;i++)
		if((n->args[i]=evalnode(n->args[i]))==NULL)
			return NULL;
	return n;
}

/*evaluate a user function*/
tree_t *
evaluserfunc(tree_t *n)
{
	tree_t *r;
	char tmp[5]="arg?";
	int i;
	func_t *f;

	if(n->type!=ACTION_NODE || n->data.action.type!=FUNCTION_TYPE ||
		n->data.action.data.func->type!=USER_FUNC)
		return NULL;

	if(n->data.action.data.func->data.user.value)
		r=copynode(n->data.action.data.func->
			data.user.value);
	else {
		/*this was a dummy function, find the real
		  one*/
		f=d_lookup(n->data.action.data.func->id,TRUE);
		if(!f || !f->data.user.value) {
			(*errorout)(_("Variable used uninitialized!"));
			return NULL;
		}

		r=copynode(f->data.user.value);
		/*FIXME: maybe some reference counting here!*/
		n->data.action.data.func=f;
	}

	/*only a variable*/
	if(r->type==NUMBER_NODE)
		return r;

	d_addcontext(NULL);
	/*push arguments on context stack*/
	for(i=0;i<n->data.action.data.func->nargs;i++){
		tmp[3]=(char)i+'1'; /*the arg var name*/
		d_addfunc(d_makeufunc(tmp,copynode( n->args[i]),
				      NULL,0,TRUE));
	}
	r=evalnode(r);
	freedict(d_popcontext(),TRUE);

	return r;
}

/*evaluate branches of a primitive, returns 0 if all ok, 1 if n should
  be returned or 2 on a serious error*/
int
evalbranches(tree_t *n)
{
	int noeval=FALSE;
	int i;

	if(n->type!=ACTION_NODE || n->data.action.type!=PRIMITIVE_TYPE)
		return 2;

	/*primitives have maximum of 3 arguments*/
	for(i=0;i<3;i++) {
		if(brancheseval(n->data.action.data.primitive)& (1<<(2-i)) ) {
			if((n->args[i]=evalnode(n->args[i]))==NULL) {
				freetree(n);
				return 2; /*an error occured*/
			}
			/*can't evaluate this!*/
			if(n->args[i]->type!=NUMBER_NODE)
				noeval=TRUE;
		}
	}
	/*if something didn't evaluate we can't calculate this
	  primitive except for SEPARATOR which will just copy
	  the right side and so it works even with unevaluated
	  arguments*/
	if(noeval && n->data.action.data.primitive!=E_SEPAR)
		return 1;
	return 0;
}

/*return TRUE if node is true (a number node !=0), false otherwise*/
int
isnodetrue(tree_t *n)
{
	if(n->type!=NUMBER_NODE)
		return FALSE;
	if(mpw_sgn(n->data.val)!=0)
		return TRUE;
	else
		return FALSE;
}

int
eval_isnodetrue(tree_t *n)
{
	tree_t *m;
	int r=FALSE;
	int madecopy=FALSE;

	/*we don't have to make a copy and evaluate if the tree is a number
	  already*/
	if(n->type!=NUMBER_NODE) {
		madecopy=TRUE;
		m=evalnode(copynode(n));
		if(!m)
			return FALSE;
		if(m->type!=NUMBER_NODE) {
			freetree(m);
			return FALSE;
		}
	} else
		m=n;
	if(mpw_sgn(m->data.val)!=0)
		r = TRUE;
	else
		r = FALSE;
	if(madecopy)
		freetree(m);
	return r;
}

/*evaluate a treenode, the tree node will become a number node
  or at least all the calculatable parts will be calculated so
  it will be a reduced tree
  the tree will be freed*/
tree_t *
evalnode(tree_t *n)
{
	tree_t *r=NULL;

	if(!n || n->type!=ACTION_NODE)
		return n;

	if(n->data.action.type==FUNCTION_TYPE) {
		/*evaluate arguments*/
		if(!evalargs(n))
			return NULL;

		if(n->data.action.data.func->type==USER_FUNC)
			r=evaluserfunc(n);
		else
			r=(*n->data.action.data.func->data.func)
				(n->args);
	} else {
		/*evaluate the different primitive branches
		  note that some type of operations will not
		  allow certain branches to be evaluated at this
		  time*/
		switch(evalbranches(n)) {
			case 1: return n;
			case 2: return NULL;
		}
		switch(n->data.action.data.primitive) {
			case E_SEPAR: r=n->args[1]; n->args[1]=NULL; break;
			case E_EQUALS: r=equalsop(n->args[0],n->args[1]); break;
			case E_PLUS: r=plusop(n->args[0],n->args[1]); break;
			case E_MINUS: r=minusop(n->args[0],n->args[1]); break;
			case E_MUL: r=mulop(n->args[0],n->args[1]); break;
			case E_DIV: r=divop(n->args[0],n->args[1]); break;
			case E_MOD: r=modop(n->args[0],n->args[1]); break;
			case E_NEG: r=negop(n->args[1]); break;
			case E_EXP: r=expop(n->args[0],n->args[1]); break;
			case E_FACT: r=factop(n->args[0]); break;
			case E_FUNCDEF: r=funcdefop(n->args[0],n->args[1]); break;
			case E_IF_CONS:
				if(n->args[0]->type!=NUMBER_NODE) {
					(*errorout)(_("Left value is NOT a"
						" number!"
						" (ignoring if construct)"));
					return n;
				}
				if(isnodetrue(n->args[0])) {
					r=evalnode(n->args[1]);
					n->args[1]=NULL;
				} else
					r=makenum_ui(0);
				break;
			case E_IFELSE_CONS:
				if(n->args[0]->type!=NUMBER_NODE) {
					(*errorout)(_("Left value is NOT a"
						" number!"
						" (ignoring ifelse construct)"));
					return n;
				}
				if(isnodetrue(n->args[0])) {
					r=evalnode(n->args[1]);
					n->args[1]=NULL;
				} else {
					r=evalnode(n->args[2]);
					n->args[2]=NULL;
				}
				break;
			case E_WHILE_CONS:
				/*if there is NO iteration return 0*/
				if(eval_isnodetrue(n->args[0])) {
					r = evalnode(copynode(n->args[1]));
					while(eval_isnodetrue(n->args[0])) {
						/*get rid of previous results*/
						freetree(r);
						r=evalnode(copynode(n->args[1]));
					}
				} else
					r=makenum_ui(0);
				break;
			case E_EQ_CMP: r=eqcmpop(n->args[0],n->args[1]); break;
			case E_NE_CMP: r=necmpop(n->args[0],n->args[1]); break;
			case E_CMP_CMP: r=cmpcmpop(n->args[0],n->args[1]); break;
			case E_LT_CMP: r=ltcmpop(n->args[0],n->args[1]); break;
			case E_GT_CMP: r=gtcmpop(n->args[0],n->args[1]); break;
			case E_LE_CMP: r=lecmpop(n->args[0],n->args[1]); break;
			case E_GE_CMP: r=gecmpop(n->args[0],n->args[1]); break;
			case E_LOGICAL_AND:
				r=logicalandop(n->args[0],n->args[1]); break;
			case E_LOGICAL_OR:
				r=logicalorop(n->args[0],n->args[1]); break;
			case E_LOGICAL_XOR:
				r=logicalxorop(n->args[0],n->args[1]); break;
			case E_LOGICAL_NOT:
				r=logicalnotop(n->args[1]); break;
		}
	}
	if(r!=NULL) {
		freetree(n);
		return r;
	} else if(error_num!=NO_ERROR) {
		/*something errored out*/
		freetree(n);
		return NULL;
	} /*r==NULL && error_num==NO_ERROR*/
	/*this error is not too serious, just this action can't be computed*/
	return n;
}

tree_t *
plusop(tree_t *l,tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_add(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
minusop(tree_t *l,tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_sub(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
mulop(tree_t *l,tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_mul(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
divop(tree_t *l,tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_div(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
modop(tree_t *l,tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_mod(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
negop(tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_neg(res,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
expop(tree_t *l, tree_t *r)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_pow(res,l->data.val,r->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
factop(tree_t *l)
{
	tree_t *n=NULL;
	mpw_t res;

	mpw_init(res);
	mpw_fac(res,l->data.val);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return NULL;
	}
	mpw_make_int(res,calcstate.make_floats_ints);
	n=makenum(res);
	mpw_clear(res);

	return n;
}

tree_t *
equalsop(tree_t *l, tree_t *r)
{
	if(!l || !r)
		g_error(_("'=' can't function without proper nodes!"));
	if(l->type!=ACTION_NODE ||
		l->data.action.type!=FUNCTION_TYPE ||
		l->data.action.data.func->nargs!=0) {
		(*errorout)(_("Left value is NOT a 0\\function! (ignoring '=')"));
		return NULL;
	}
	if(r->type!=NUMBER_NODE) {
		(*errorout)(_("Right value is NOT computed! (ignoring '=')"));
		return NULL;
	}
	d_addfunc(d_makeufunc(l->data.action.data.func->id,
			      copynode(r),NULL,0,TRUE));

	return copynode(r);
}

tree_t *
funcdefop(tree_t *l, tree_t *r)
{
	if(!l || !r)
		g_error(_("funcdefop can't function without proper nodes!"));
	if(l->type!=ACTION_NODE ||
		l->data.action.type!=FUNCTION_TYPE) {
		(*errorout)(_("Left value is NOT a function!"
			" (ignoring function definition)"));
		return NULL;
	}
	d_addfunc(d_makeufunc(l->data.action.data.func->id,
			      copynode(r),NULL,
			      l->data.action.data.func->nargs, TRUE));

	return makenum_ui(1);
}

/*tree_t *
ifconsop(tree_t *l, tree_t *r)
{
}

tree_t *
ifelseconsop(tree_t *l, tree_t *r, tree_t *sr)
{
}

tree_t *
whileconsop(tree_t *l, tree_t *r)
{
}*/

/*compare nodes, return TRUE if equal
  makes them the same type as a side effect*/
int
eqlnodes(tree_t *l, tree_t *r)
{
	return mpw_eql(l->data.val,r->data.val);
}

/*compare nodes, return -1 if first one is smaller, 0 if they are
  equal, 1 if the first one is greater
  makes them the same type as a side effect*/
int
cmpnodes(tree_t *l, tree_t *r)
{
	int n=0;

	n=mpw_cmp(l->data.val,r->data.val);

	if(n>0) n=1;
	else if(n<0) n=-1;
	return n;
}

tree_t *
eqcmpop(tree_t *l, tree_t *r)
{
	if(eqlnodes(l,r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
necmpop(tree_t *l, tree_t *r)
{
	if(!eqlnodes(l,r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
cmpcmpop(tree_t *l, tree_t *r)
{
	return makenum_si(cmpnodes(l,r));
}

tree_t *
ltcmpop(tree_t *l, tree_t *r)
{
	if(cmpnodes(l,r)<0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
gtcmpop(tree_t *l, tree_t *r)
{
	if(cmpnodes(l,r)>0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
lecmpop(tree_t *l, tree_t *r)
{
	if(cmpnodes(l,r)<=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
gecmpop(tree_t *l, tree_t *r)
{
	if(cmpnodes(l,r)>=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
logicalandop(tree_t *l, tree_t *r)
{
	if(isnodetrue(l) && isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
logicalorop(tree_t *l, tree_t *r)
{
	if(isnodetrue(l) || isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
logicalxorop(tree_t *l, tree_t *r)
{
	if(isnodetrue(l) != isnodetrue(r))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

tree_t *
logicalnotop(tree_t *r)
{
	if(isnodetrue(r))
		return makenum_ui(0);
	else
		return makenum_ui(1);
}

