/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998, 1999 the Free Software Foundation.
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

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#define _(x) x
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "mpwrap.h"
#include "eval.h"
#include "calc.h"
#include "dict.h"
#include "util.h"
#include "funclib.h"
#include "matrix.h"
#include "matrixw.h"

extern calc_error_t error_num;
extern calcstate_t calcstate;

/*if we "return" we jump out of the current function using an exception and
  set this, if we are at the top most context, this is what is "returned to
  the "screen"*/
ETree *returnval = NULL;
/*similiar case for bailout and exception*/
int inbailout = FALSE;
int inexception = FALSE;
enum {
	LOOPOUT_NOTHING=0,
	LOOPOUT_CONTINUE=1,
	LOOPOUT_BREAK=2
};
int loopout = 0;
GSList *inloop = NULL; /*on loop entry and function antry prepend 1 or 0
			 and on exit pop the first value*/

extern void (*errorout)(char *);

ETree *free_trees = NULL;

extern int interrupted;

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
		case E_TRANSPOSE: return 1;
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
		case E_REGION_SEP: return 2;
		case E_GET_ELEMENT: return 3;
		case E_GET_REGION: return 3;
		case E_GET_ROW_REGION: return 2;
		case E_GET_COL_REGION: return 2;
		case E_REFERENCE: return 1;
		case E_DEREFERENCE: return 1;
		case E_DIRECTCALL: return -2;
		case E_CALL: return -2;
		case E_RETURN: return 1;
		case E_BAILOUT: return 0;
		case E_EXCEPTION: return 0;
		case E_CONTINUE: return 0;
		case E_BREAK: return 0;
	}
	return 0;
}

ETree *
makenum_null(void)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type = NULL_NODE;
	n->args = NULL;
	n->nargs = 0;
	return n;
}

ETree *
makenum_ui(unsigned long num)
{
	ETree *n;
	GET_NEW_NODE(n);
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
	GET_NEW_NODE(n);
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
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	n->args = NULL;
	n->nargs = 0;
	mpw_init_set(n->data.value,num);
	return n;
}

/*don't create a new number*/
ETree *
makenum_use(mpw_t num)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	n->args = NULL;
	n->nargs = 0;
	memcpy(n->data.value,num,sizeof(struct _mpw_t));
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

	GET_NEW_NODE(n);
	n->type = OPERATOR_NODE;
	n->data.oper = oper;
	
	n->args = list;
	n->nargs = args;

	return n;
}

static void
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
	else if(n->type == MATRIX_NODE) {
		if(n->data.matrix)
			matrixw_free(n->data.matrix);
	} else if(n->type == IDENTIFIER_NODE) {
		/*was this a fake token, to an anonymous function*/
		if(!n->data.id->token) {
			/*XXX:where does the function go?*/
			g_list_free(n->data.id->refs);
			g_free(n->data.id);
		}
	}
	/*put onto the free list*/
	n->data.next = free_trees;
	free_trees = n;
}

/*free arguments to a dictionary function*/
static void
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

	GET_NEW_NODE(n);

	memcpy(n,o,sizeof(ETree));

	if(o->type==VALUE_NODE)
		mpw_init_set(n->data.value,o->data.value);
	else if(o->type == STRING_NODE)
		n->data.str = g_strdup(o->data.str);
	else if(o->type == FUNCTION_NODE)
		n->data.func = d_copyfunc(o->data.func);
	else if(o->type == MATRIX_NODE)
		n->data.matrix = matrixw_copy(o->data.matrix);
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

/*need_colwise will return if we need column wise expansion*/
static int
expand_row(Matrix *dest, MatrixW *src, int di, int si, int *need_col, int *need_colwise)
{
	GList *ev = NULL;
	GList *li;
	int i;
	int w = 1;
	int roww;
	
	roww = 0;
	for(i=0;i<matrixw_width(src);i++) {
		if(!matrixw_set_index(src,i,si)) continue;
		roww = i+1;
	}
	*need_col = roww;

	for(i=0;i<roww;i++) {
		ETree *et = matrixw_set_index(src,i,si);
		if(et)
			et = evalnode(et);
		ev = g_list_prepend(ev,et);
		if(!et || et->type != MATRIX_NODE)
			continue;
		if(matrixw_height(et->data.matrix)>w)
			w = matrixw_height(et->data.matrix);
	}
	
	matrix_set_at_least_size(dest,1,di+w);
	
	for(li=ev,i=roww-1;i>=0;li=g_list_next(li),i--) {
		int x;
		ETree *et = li->data;
		
		/*0 node*/
		if(!et) {
			for(x=0;x<w;x++)
				matrix_index(dest,i,di+x) = NULL;
		/*non-matrix node*/
		} else if(et->type!=MATRIX_NODE) {
			matrix_index(dest,i,di) = et;
			for(x=1;x<w;x++)
				matrix_index(dest,i,di+x) = copynode(et);
		/*single column matrix, convert to regular nodes*/
		} else if(matrixw_width(et->data.matrix) == 1) {
			int xx;
			matrixw_make_private(et->data.matrix);
			for(x=0;x<matrixw_height(et->data.matrix);x++) {
				matrix_index(dest,i,di+x) =
					matrixw_set_index(et->data.matrix,0,x);
				matrixw_set_index(et->data.matrix,0,x) = NULL;
			}
			xx = 0;
			for(x=matrixw_height(et->data.matrix);x<w;x++) {
				matrix_index(dest,i,di+x) =
					copynode(matrix_index(dest,i,di+xx));
				if((++xx)>=matrixw_height(et->data.matrix))
					xx=0;
			}
			freetree(et);
		/*non-trivial matrix*/
		} else {
			int xx;

			matrixw_make_private(et->data.matrix);
			
			*need_col += matrixw_width(et->data.matrix) - 1;

			for(x=0;x<matrixw_height(et->data.matrix);x++) {
				ETree *n;
				GET_NEW_NODE(n);
				n->type = MATRIX_ROW_NODE;
				
				n->args = NULL;
				for(xx=matrixw_width(et->data.matrix)-1;xx>=0;xx--) {
					n->args = g_list_prepend(n->args,
								 matrixw_set_index(et->data.matrix,xx,x));
					matrixw_set_index(et->data.matrix,xx,x) = NULL;
				}
				n->nargs = matrixw_width(et->data.matrix);
				
				matrix_index(dest,i,di+x) = n;

				*need_colwise = TRUE;
			}
			xx = 0;
			for(x=matrixw_height(et->data.matrix);x<w;x++) {
				matrix_index(dest,i,di+x) =
					copynode(matrix_index(dest,i,di+xx));
				if((++xx)>=matrixw_height(et->data.matrix))
					xx=0;
			}
			freetree(et);
		}
	}
	
	g_list_free(ev);
	
	return w;
}


static int
expand_col(Matrix *dest, Matrix *src, int di[], int si, GList *roww)
{
	int i;
	int w;
	int cols;
	
	cols = dest->width;

	w = 1;
	for(i=0;i<src->height;i++) {
		ETree *et = matrix_index(src,si,i);
		if(!et || et->type != MATRIX_ROW_NODE)
			continue;
		if(et->nargs>w)
			w = et->nargs;
	}
	
	for(i=0;i<src->height;roww=g_list_next(roww),i++) {
		ETree *et = matrix_index(src,si,i);
		int wid = GPOINTER_TO_INT(roww->data);
		if(!et) continue;
		else if(et->type != MATRIX_ROW_NODE) {
			int x;
			int exp;
			matrix_index(dest,di[i],i) = et;
			
			exp = MIN(w-1,cols-wid);
			
			roww->data = GINT_TO_POINTER(wid + exp);

			for(x=1;x<=exp;x++)
				matrix_index(dest,di[i]+x,i) =
					copynode(et);
			di[i] += x;
		} else {
			int x;
			int xx;
			int exp;
			GList *li;
			for(li=et->args,x=0;li!=NULL;li=g_list_next(li),x++)
				matrix_index(dest,di[i]+x,i) = li->data;
			exp = MIN(w-1,cols-wid);
			roww->data = GINT_TO_POINTER(wid + exp);
			xx = 0;
			for(x=et->nargs;x<=exp;x++) {
				matrix_index(dest,di[i]+x,i) =
					copynode(matrix_index(dest,di[i]+xx,i));
				if((++xx)>=et->nargs)
					xx=0;
			}
			di[i] += x;
			g_list_free(et->args);
			et->args = NULL;
			et->nargs = 0;
			freetree(et);
		}
	}
	
	return w;
}

/*evaluate a matrix (or try to), it will try to expand the matrix and
  put 0's into the empty, undefined, spots. For example, a matrix such
  as if b = [8,7]; a = [1,2:3,b]  should expand to, [1,2,2:3,8,7] */
static ETree *
evalmatrix(ETree *n)
{
	int i;
	int k;
	int cols;
	ETree *nn;
	Matrix *m = matrix_new();
	int need_colwise = FALSE;
	GList *roww = NULL;

	matrix_set_size(m,matrixw_width(n->data.matrix),matrixw_height(n->data.matrix));
	
	cols = matrixw_width(n->data.matrix);
	
	for(i=0,k=0;i<matrixw_height(n->data.matrix);i++) {
		int c;
		int w = expand_row(m,n->data.matrix,k,i,&c,&need_colwise);
		k += w;
		for(;w>0;w--)
			roww = g_list_prepend(roww,GINT_TO_POINTER(c));

		if(cols<c)
			cols = c;
	}
	
	roww = g_list_reverse(roww);
	
	if(need_colwise) {
		Matrix *nm = matrix_new();
		guint *di = g_new0(guint,k);
		int j;
		matrix_set_size(nm,cols,m->height);
		for(i=0;i<m->width;i++)
			expand_col(nm,m,di,i,roww);
		g_free(di);
		matrix_free(m);
		m = nm;
	}
	
	g_list_free(roww);

	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->data.matrix = matrixw_new_with_matrix(m);
	nn->args = NULL;
	nn->nargs = 0;
	
	return nn;
}

static ETree *
matrix_el_bailout_set(ETree *n, ETree *set, ETree *l, ETree *m, ETree *ll, ETree *rr)
{
	if(n) {
		ETree *args1[3];
		ETree *args2[2];
		args1[0]=copynode(m);
		if(ll) {
			args1[1]=ll;
			args1[2]=rr;
		} else {
			args1[1]=rr;
			args1[2]=NULL;
		}
		args2[0]=copynode_args(l,args1);
		args2[1]=set;
		return copynode_args(n,args2);
	} else {
		ETree *args1[3];
		ETree *args2[2];
		args1[0]=m;
		if(ll) {
			args1[1]=ll;
			args1[2]=rr;
		} else {
			args1[1]=rr;
			args1[2]=NULL;
		}
		return copynode_args(l,args1);
	}
}

static int
get_matrix_index_num(ETree *n, ETree *l, ETree *set, ETree *m, ETree *ll, ETree *rr, 
		     ETree *num, ETree **ret)
{
	long i;
	if(num->type != VALUE_NODE ||
	   !mpw_is_integer(num->data.value)) {
		(*errorout)(_("Wrong argument type as matrix index"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return -1;
	}

	i = mpw_get_long(num->data.value);
	if(error_num) {
		error_num = 0;
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return -1;
	}
	if(i>INT_MAX) {
		(*errorout)(_("Matrix index too large"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return -1;
	} else if(i<=0) {
		(*errorout)(_("Matrix index less then 1"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return -1;
	}
	return i;
}

static int
get_index_region(ETree *n, ETree *l, ETree *set, ETree *m, ETree *ll, ETree *rr, 
		 ETree *num, ETree **ret, int *from, int *to)
{
	if(num->type == OPERATOR_NODE &&
	   num->data.oper == E_REGION_SEP) {
		g_assert(num->args);
		g_assert(num->args->data);
		g_assert(num->args->next->data);
		*from = get_matrix_index_num(n,l,set,m,ll,rr,
					     num->args->data,ret);
		if(*from == -1) return FALSE;
		*to = get_matrix_index_num(n,l,set,m,ll,rr,
					   num->args->next->data,ret);
		if(*to == -1) return FALSE;
		if(*from>*to) {
			(*errorout)(_("Matrix 'to' index less then 'from' index"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return FALSE;
		}
	} else {
		*from = *to = get_matrix_index_num(n,l,set,m,ll,rr,num,ret);
		if(*from == -1) return FALSE;
	}
	return TRUE;
}

static int
get_matrix_index(ETree *n, ETree *l, ETree *set, ETree **m, ETree **ll, ETree **rr, 
		 int do_row, int do_column,
		 int *rowfrom, int *rowto,
		 int *colfrom, int *colto,
		 ETree **ret)
{
	if(do_row && do_column) {
		GET_LRR(l,(*m),(*ll),(*rr));
	} else if(do_row) {
		GET_LR(l,(*m),(*ll));
		*rr = NULL;
	} else /*if(do_column)*/ {
		GET_LR(l,(*m),(*rr));
		*ll = NULL;
	}

	if(do_row) {
		*ll = evalnode(*ll);
		/*exception*/
		if(!*ll) {
			freetree(set);
			return FALSE;
		}
	}
	if(do_column) {
		*rr = evalnode(*rr);
		/*exception*/
		if(!*rr) {
			if(*ll) freetree(*ll);
			freetree(set);
			return FALSE;
		}
	}

	/*if this is from the GET and not from SET, then evaluate m*/
	if(!n) {
		*m = evalnode(*m);
		/*exception*/
		if(!*m) {
			if(*ll) freetree(*ll);
			if(*rr) freetree(*rr);
			freetree(set);
			return FALSE;
		}
	}

	if(do_row) {
		if(!get_index_region(n,l,set,*m,*ll,*rr, 
				    *ll,ret,rowfrom,rowto))
			return FALSE;
	}
	if(do_column) {
		if(!get_index_region(n,l,set,*m,*ll,*rr, 
				    *rr,ret,colfrom,colto))
			return FALSE;
	}
	return TRUE;
}

static MatrixW *
get_matrix_p(ETree *n, ETree *l, ETree *set, ETree *m, ETree *ll, ETree *rr, 
	     int *new_matrix, ETree **ret)
{
	MatrixW *mat = NULL;

	if(m->type == IDENTIFIER_NODE) {
		EFunc *f;
		f = d_lookup_local(m->data.id);
		if(!f) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->args = NULL;
			t->nargs = 0;
			t->data.matrix = matrixw_new();
			matrixw_set_size(t->data.matrix,1,1);

			f = d_makeufunc(m->data.id,t,NULL,0);
			d_addfunc(f);
			if(new_matrix) *new_matrix = TRUE;
		} else if(f->type != USER_FUNC) {
			(*errorout)(_("Indexed Lvalue not user function"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		} else if(f->data.user->type != MATRIX_NODE) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->args = NULL;
			t->nargs = 0;
			t->data.matrix = matrixw_new();
			matrixw_set_size(t->data.matrix,1,1);

			d_set_value(f,t);
			if(new_matrix) *new_matrix = TRUE;
		}
		mat = f->data.user->data.matrix;
	} else if(m->type == OPERATOR_NODE ||
		  m->data.oper == E_DEREFERENCE) {
		ETree *lll;
		EFunc *f;
		GET_L(m,lll);

		if(lll->type != IDENTIFIER_NODE) {
			(*errorout)(_("Dereference of non-identifier!"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}

		f = d_lookup_local(lll->data.id);
		if(!f) {
			(*errorout)(_("Dereference of undefined variable!"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}
		if(f->type!=REFERENCE_FUNC) {
			(*errorout)(_("Dereference of non-reference!"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}

		if(f->data.ref->type != USER_FUNC) {
			(*errorout)(_("Indexed Lvalue not user function"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}
		if(f->data.ref->data.user->type != MATRIX_NODE) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->args = NULL;
			t->nargs = 0;
			t->data.matrix = matrixw_new();
			matrixw_set_size(t->data.matrix,1,1);

			d_set_value(f->data.ref,t);
			if(new_matrix) *new_matrix = TRUE;
		}
		mat = f->data.ref->data.user->data.matrix;
	} else {
		(*errorout)(_("Indexed Lvalue not an identifier or a dereference"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return NULL;
	}
	return mat;
}

static ETree *
equalsop(ETree *n)
{
	ETree *l;
	ETree *r;
	ETree *set;
	GET_LR(n,l,r);
	
	if(l->type != IDENTIFIER_NODE &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_GET_ELEMENT) &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_GET_REGION) &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_GET_COL_REGION) &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_GET_ROW_REGION) &&
	   !(l->type == OPERATOR_NODE && l->data.oper == E_DEREFERENCE)) {
		(*errorout)(_("Lvalue not an identifier/dereference/matrix location!"));
		return copynode(n);
	}

	if(l->type == IDENTIFIER_NODE) {
		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;
		if(set->type == FUNCTION_NODE)
			d_addfunc(d_makerealfunc(set->data.func,l->data.id));
		else if(set->type == OPERATOR_NODE &&
			set->data.oper == E_REFERENCE) {
			ETree *t = set->args->data;
			EFunc *rf = d_lookup_global(t->data.id);
			if(!rf) {
				ETree *args[2];
				(*errorout)(_("Referencing an undefined variable!"));
				args[0]=copynode(l);
				args[1]=set;
				return copynode_args(n,args);
			}
			d_addfunc(d_makereffunc(l->data.id,rf));
		} else
			d_addfunc(d_makeufunc(l->data.id,copynode(set),NULL,0));
	} else if(l->data.oper == E_DEREFERENCE) {
		ETree *ll;
		EFunc *f;
		GET_L(l,ll);

		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;

		if(ll->type != IDENTIFIER_NODE) {
			ETree *args[2];
			(*errorout)(_("Dereference of non-identifier!"));
			args[0]=copynode(l);
			args[1]=set;
			return copynode_args(n,args);
		}
		
		f = d_lookup_local(ll->data.id);
		if(!f) {
			ETree *args[2];
			(*errorout)(_("Dereference of undefined variable!"));
			args[0]=copynode(l);
			args[1]=set;
			return copynode_args(n,args);
		}
		if(f->type!=REFERENCE_FUNC) {
			ETree *args[2];
			(*errorout)(_("Dereference of non-reference!"));
			args[0]=copynode(l);
			args[1]=set;
			return copynode_args(n,args);
		}
		
		if(set->type == FUNCTION_NODE)
			d_setrealfunc(f->data.ref,set->data.func);
		else if(set->type == OPERATOR_NODE &&
			set->data.oper == E_REFERENCE) {
			ETree *t = set->args->data;
			EFunc *rf = d_lookup_global(t->data.id);
			if(!rf) {
				ETree *args[2];
				(*errorout)(_("Referencing an undefined variable!"));
				args[0]=copynode(l);
				args[1]=set;
				return copynode_args(n,args);
			}
			d_set_ref(f->data.ref,rf);
		} else
			d_set_value(f->data.ref,copynode(set));
	} else if(l->data.oper == E_GET_ELEMENT) {
		MatrixW *mat;
		ETree *ll,*rr,*m;
		ETree *ret = NULL;
		int li,ri; 

		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;
		
		if(!get_matrix_index(n,l,set,&m,&ll,&rr,TRUE,TRUE,
				     &li,&li,&ri,&ri,&ret))
			return ret;

		if(!(mat = get_matrix_p(n,l,set,m,ll,rr,NULL,&ret)))
			return ret;

		freetree(ll);
		freetree(rr);
		
		matrixw_set_element(mat,ri-1,li-1,copynode(set));
	} else /*l->data.oper == E_GET_REGION E_GET_COL_REGION E_GET_ROW_REGION*/ {
		MatrixW *mat;
		ETree *ll,*rr,*m;
		ETree *ret = NULL;
		int rowfrom,rowto,colfrom,colto; 
		int w,h;
		int new_matrix = FALSE;
		int do_col,do_row;
		
		if(l->data.oper == E_GET_REGION) {
			do_col = TRUE;
			do_row = TRUE;
		} else if(l->data.oper == E_GET_ROW_REGION) {
			do_col = FALSE;
			do_row = TRUE;
		} else /*E_GET_COL_REGION*/ {
			do_col = TRUE;
			do_row = FALSE;
		}

		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;
		
		if(!get_matrix_index(n,l,set,&m,&ll,&rr,do_row,do_col,
				     &rowfrom,&rowto,&colfrom,&colto,&ret))
			return ret;
		
		if(!(mat = get_matrix_p(n,l,set,m,ll,rr,&new_matrix,&ret)))
			return ret;
		
		if(!do_row) {
			rowfrom = 1;
			if(new_matrix) {
				if(set->type == MATRIX_NODE)
					rowto = matrixw_height(set->data.matrix);
				else
					rowto = 1;
			} else
				rowto = matrixw_height(mat);
		}
		if(!do_col) {
			colfrom = 1;
			if(new_matrix) {
				if(set->type == MATRIX_NODE)
					colto = matrixw_width(set->data.matrix);
				else
					colto = 1;
			} else
				colto = matrixw_width(mat);
		}
		
		w = colto-colfrom+1;
		h = rowto-rowfrom+1;

		/*weirdly written boolean expression, it's if these
		  conditions AREN'T met then get out, it was just
		  easier to write it this way*/
		if(!(set->type != MATRIX_NODE ||
		     (matrixw_width(set->data.matrix) == w &&
		      matrixw_height(set->data.matrix) == h))) {
			(*errorout)(_("Can't set a region to a region of a different size"));
			return matrix_el_bailout_set(n, set, l, m, ll, rr);
		}

		freetree(ll);
		freetree(rr);
		
		if(set->type == MATRIX_NODE) {
			matrixw_set_region(mat,set->data.matrix,0,0,
					   colfrom-1,rowfrom-1,w,h);
		} else {
			matrixw_set_region_etree(mat,set,
						 colfrom-1,rowfrom-1,w,h);
		}
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

	GET_NEW_NODE(n);

	memcpy(n,o,sizeof(ETree));

	if(o->type==VALUE_NODE)
		mpw_init_set(n->data.value,o->data.value);
	else if(o->type == STRING_NODE)
		n->data.str = g_strdup(o->data.str);
	else if(o->type == FUNCTION_NODE)
		n->data.func = d_copyfunc(o->data.func);
	else if(o->type == MATRIX_NODE)
		n->data.matrix = matrixw_copy(o->data.matrix);
	
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
		GSList *sli;

		d_addcontext();
		/*push arguments on context stack*/
		for(i=1,li=f->named_args;i<n->nargs;i++,li=g_list_next(li)) {
			if(r[i]->type == FUNCTION_NODE) {
				d_addfunc(d_makereffunc(li->data,
							r[i]->data.func));
			} else if(r[i]->type == OPERATOR_NODE &&
			   r[i]->data.oper == E_REFERENCE) {
				ETree *t = r[i]->args->data;
				EFunc *rf = d_lookup_global(t->data.id);
				if(!rf) {
					freedict(d_popcontext());
					(*errorout)(_("Referencing an undefined variable!"));
					ret = copynode_args(n,r);
					g_free(r);
					return ret;
				}
				d_addfunc(d_makereffunc(li->data,rf));
			} else
				d_addfunc(d_makeufunc(li->data,copynode(r[i]),NULL,0));
		}
		if(returnval) {
			(*errorout)(_("Extraneous return value!"));
			freetree(returnval);
		}
		returnval = NULL;
		inexception = FALSE;
		inbailout = FALSE;

		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(0));
		ret = evalnode(f->data.user);
		sli=inloop;
		inloop=g_slist_remove_link(inloop,sli);
		g_slist_free_1(sli);
		
		if(!inexception && inbailout) {
			if(returnval) freetree(returnval);
			returnval = NULL;
			inbailout = FALSE;
			if(ret) freetree(ret);
			ret = copynode_args(n,r);
		} else {
			for(i=0;i<n->nargs;i++)
				freetree(r[i]);
		}
		g_free(r);
		freedict(d_popcontext());
		if(!ret && returnval)
			ret = returnval;
		else if(returnval)
			freetree(returnval);
		returnval = NULL;
		if(inexception) {
			inbailout = FALSE;
			inexception = FALSE;
			if(ret) freetree(ret);
			ret = NULL;
		}
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
		GET_NEW_NODE(i);
		i->type = IDENTIFIER_NODE;
		i->data.id = f->id; /*this WILL have an id*/
		i->args = NULL;
		i->nargs = 0;

		GET_NEW_NODE(a);
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
			return copynode(f->data.user);
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
			GET_NEW_NODE(i);
			i->type = IDENTIFIER_NODE;
			i->data.id = f->id; /*this WILL have an id*/
			i->args = NULL;
			i->nargs = 0;

			GET_NEW_NODE(a);
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
		return copynode(f->data.user);
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
		
		GET_NEW_NODE(i);
		i->type = IDENTIFIER_NODE;
		if(f->id) {
			i->data.id = f->id;
		} else {
			/*make up a new fake id*/
			Token *tok = g_new(Token,1);
			tok->token = NULL;
			tok->refs = g_list_append(NULL,f);
			i->data.id = tok;
		}
		i->args = NULL;
		i->nargs = 0;

		GET_NEW_NODE(a);
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
	ETree *l,*r,*rr=NULL;
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
		GSList *li;
		freetree(ret);
		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode(r);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				return makenum_null();
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = makenum_null();
			} else
				return NULL;
		}
	}
	while(until?
	      !eval_isnodetrue(l,&exception,&errorret):
	      eval_isnodetrue(l,&exception,&errorret)) {
		GSList *li;
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
		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode(r);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				return makenum_null();
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = makenum_null();
			} else
				return NULL;
		}
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
eqcmpop(ETree *n, ETree *arg[], gpointer f)
{
	int r = eqlnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(r) return makenum_ui(1);
	else return makenum_ui(0);
}

static ETree *
necmpop(ETree *n, ETree *arg[], gpointer f)
{
	int r = eqlnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(r) return makenum_ui(0);
	else return makenum_ui(1);
}

static ETree *
cmpcmpop(ETree *n, ETree *arg[], gpointer f)
{
	int ret = cmpnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	return makenum_si(ret);
}

static ETree *
ltcmpop(ETree *n, ETree *arg[], gpointer f)
{
	int ret = cmpnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(ret<0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
gtcmpop(ETree *n, ETree *arg[], gpointer f)
{
	int ret = cmpnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(ret>0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
lecmpop(ETree *n, ETree *arg[], gpointer f)
{
	int ret = cmpnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(ret<=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
gecmpop(ETree *n, ETree *arg[], gpointer f)
{
	int ret = cmpnodes(arg[0],arg[1]);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(ret>=0)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
logicalandop(ETree *n, int argeval)
{
	ETree *r[2]; 
	ETree *ret;
	int bad_node = FALSE;
	
	g_assert(n->args);
	g_assert(n->args->data);
	g_assert(n->args->next->data);

	r[0] = n->args->data;
	r[1] = n->args->next->data;

	if(argeval) {
		r[0] = evalnode(r[0]);
		if(!r[0])
			return NULL;
	}
	
	if(isnodetrue(r[0],&bad_node)) {
		if(argeval) {
			r[1] = evalnode(r[1]);
			if(!r[1]) {
				freetree(r[1]);
				return NULL;
			}
		}
		if(isnodetrue(r[1],&bad_node)) {
			if(argeval) {
				freetree(r[0]);
				freetree(r[1]);
			}
			return makenum_ui(1);
		} else if(bad_node) {
			if(!argeval) {
				r[0]=copynode(r[0]);
				r[1]=copynode(r[1]);
			}
			(*errorout)(_("Logical and can only operate on numeric/string data"));
			return copynode_args(n,r);
		}
		if(argeval) {
			freetree(r[0]);
			freetree(r[1]);
		}
		return makenum_ui(0);
	} else if(bad_node) {
		if(!argeval)
			r[0]=copynode(r[0]);
		r[1]=copynode(r[1]);
		(*errorout)(_("Logical and can only operate on numeric/string data"));
		return copynode_args(n,r);
	}
	if(argeval)
		freetree(r[0]);
	return makenum_ui(0);
}

static ETree *
logicalorop(ETree *n, int argeval)
{
	ETree *r[2]; 
	ETree *ret;
	int bad_node = FALSE;
	
	g_assert(n->args);
	g_assert(n->args->data);
	g_assert(n->args->next->data);

	r[0] = n->args->data;
	r[1] = n->args->next->data;

	if(argeval) {
		r[0] = evalnode(r[0]);
		if(!r[0])
			return NULL;
	}
	
	if(!isnodetrue(r[0],&bad_node)) {
		if(bad_node) {
			if(!argeval)
				r[0]=copynode(r[0]);
			r[1]=copynode(r[1]);
			(*errorout)(_("Logical and can only operate on numeric/string data"));
			return copynode_args(n,r);
		}
		if(argeval) {
			r[1] = evalnode(r[1]);
			if(!r[1]) {
				freetree(r[1]);
				return NULL;
			}
		}
		if(!isnodetrue(r[1],&bad_node)) {
			if(bad_node) {
				if(!argeval) {
					r[0]=copynode(r[0]);
					r[1]=copynode(r[1]);
				}
				(*errorout)(_("Logical and can only operate on numeric/string data"));
				return copynode_args(n,r);
			}
			if(argeval) {
				freetree(r[0]);
				freetree(r[1]);
			}
			return makenum_ui(0);
		}
		if(argeval) {
			freetree(r[0]);
			freetree(r[1]);
		}
		return makenum_ui(1);
	}
	if(argeval)
		freetree(r[0]);
	return makenum_ui(1);
}

static ETree *
logicalxorop(ETree *n, ETree *arg[], gpointer f)
{
	int bad_node = FALSE;
	int ret = isnodetrue(arg[0],&bad_node) != isnodetrue(arg[1],&bad_node);
	if(bad_node || error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,2);
	if(ret) return makenum_ui(1);
	else return makenum_ui(0);
}

static ETree *
logicalnotop(ETree *n, ETree *arg[], gpointer f)
{
	int bad_node = FALSE;
	int ret = !isnodetrue(arg[0],&bad_node);
	if(bad_node || error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	freeargarr(arg,1);
	if(ret) return makenum_ui(1);
	else return makenum_ui(0);
}

static ETree *
string_concat(ETree *n, ETree *arg[])
{
	ETree *ret;
	char *s=NULL;
	
	if(arg[0]->type == STRING_NODE &&
	   arg[1]->type == STRING_NODE) {
		s = g_strconcat(arg[0]->data.str,arg[1]->data.str,NULL);
	} else if(arg[0]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[1]);
		s = g_strconcat(arg[0]->data.str,gs->str,NULL);
		g_string_free(gs,TRUE);
	} else if(arg[1]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[0]);
		s = g_strconcat(gs->str,arg[1]->data.str,NULL);
		g_string_free(gs,TRUE);
	} else
		g_assert_not_reached();
	
	freeargarr(arg,2);

	GET_NEW_NODE(ret);
	ret->type = STRING_NODE;
	ret->args = NULL;
	ret->nargs = 0;
	ret->data.str = s;
	
	return ret;
}

static ETree *
eqstringop(ETree *n, ETree *arg[])
{
	int r=0;
	if(arg[0]->type == STRING_NODE &&
	   arg[1]->type == STRING_NODE) {
		r=strcmp(arg[0]->data.str,arg[1]->data.str)==0;
	} else if(arg[0]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[1]);
		r = strcmp(arg[0]->data.str,gs->str)==0;
		g_string_free(gs,TRUE);
	} else if(arg[1]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[0]);
		r = strcmp(arg[1]->data.str,gs->str)==0;
		g_string_free(gs,TRUE);
	} else
		g_assert_not_reached();

	freeargarr(arg,2);
	if(r) return makenum_ui(1);
	else return makenum_ui(0);
}

/*returns 0 if all numeric, 1 if numeric/matrix, 2 otherwise*/
static int
arglevel(ETree *r[], int cnt)
{
	int i;
	int level = 0;
	for(i=0;i<cnt;i++) {
		if(r[i]->type!=VALUE_NODE) {
			if(r[i]->type==MATRIX_NODE)
				level = level<1?1:level;
			else if(r[i]->type==STRING_NODE)
				level = 2;
			else
				return 3;
		}
	}
	return level;
}

static ETree * evaloper(ETree *n, int argeval);

static ETree *
op_two_nodes(ETree *rr, ETree *ll, int oper)
{
	ETree *r=NULL;
	ETree *n;
	mpw_t res;
	ETree *arg[2];
	
	arg[0] = rr;
	arg[1] = ll;

	switch(arglevel(arg,2)) {
	case 0: 
		mpw_init(res);
		if(oper==E_PLUS)
			mpw_add(res,arg[0]->data.value,arg[1]->data.value);
		else if(oper==E_MINUS)
			mpw_sub(res,arg[0]->data.value,arg[1]->data.value);
		else if(oper==E_MUL)
			mpw_mul(res,arg[0]->data.value,arg[1]->data.value);
		else if(oper==E_EXP)
			mpw_pow(res,arg[0]->data.value,arg[1]->data.value);
		if(error_num==NUMERICAL_MPW_ERROR) {
			GET_NEW_NODE(n);
			n->type = OPERATOR_NODE;
			n->data.oper = oper;
			n->args = g_list_append(NULL,copynode(arg[0]));
			n->args = g_list_append(n->args,copynode(arg[1]));
			n->nargs = 2;
			mpw_clear(res);
			error_num=NO_ERROR;
			return n;
		}
		return makenum_use(res);
	/*this is the less common case so we can get around with a wierd
	  thing, we'll just make a new fake node and pretend we want to 
	  evaluate that*/
	case 1:
	case 2:
		GET_NEW_NODE(n);
		n->type = OPERATOR_NODE;
		n->data.oper = oper;

		n->args = g_list_append(NULL,arg[0]);
		n->args = g_list_append(n->args,arg[1]);
		n->nargs = 2;

		/*eval operator without re-evaluating arguments*/
		r = evaloper(n,FALSE);

		/*make sure this doesn't free the arguments, so
		  this has to be freenode not freetree*/
		g_list_free(n->args);
		n->args = NULL;
		freenode(n);

		return r;
	default:
		(*errorout)(_("Primitives must get numeric/matrix/string arguments"));
		GET_NEW_NODE(n);
		n->type = OPERATOR_NODE;
		n->data.oper = oper;
		n->args = g_list_append(NULL,copynode(arg[0]));
		n->args = g_list_append(n->args,copynode(arg[1]));
		n->nargs = 2;
		mpw_clear(res);
		return n;
	}
}


static ETree *
matrix_scalar_matrix_op(ETree *n, ETree *arg[])
{
	int i,j;
	ETree *nn;
	MatrixW *m;
	ETree *node;
	int order = 0;
	if(arg[0]->type == MATRIX_NODE) {
		m = arg[0]->data.matrix;
		node = arg[1];
	} else {
		order = 1;
		m = arg[1]->data.matrix;
		node = arg[0];
	}

	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->args = NULL;
	nn->nargs = 0;
	nn->data.matrix = matrixw_new();
	matrixw_set_size(nn->data.matrix,matrixw_width(m),matrixw_height(m));
	for(i=0;i<matrixw_width(m);i++) {
		for(j=0;j<matrixw_height(m);j++) {
			ETree *t;
			if(order == 0) {
				t = op_two_nodes(matrixw_index(m,i,j),
						 node, n->data.oper);
			} else {
				t = op_two_nodes(node,
						 matrixw_index(m,i,j),
						 n->data.oper);
			}
			/*exception*/
			if(!t) {
				freeargarr(arg,2);
				freetree(nn);
				return NULL;
			}
			matrixw_set_index(nn->data.matrix,i,j) = t;
		}
	}
	freeargarr(arg,2);
	return nn;
}

static ETree *
matrix_addsub_op(ETree *n, ETree *arg[])
{
	if(arg[0]->type == MATRIX_NODE &&
	   arg[1]->type == MATRIX_NODE) {
		int i,j;
		ETree *nn;
		MatrixW *m1,*m2;
		m1 = arg[0]->data.matrix;
		m2 = arg[1]->data.matrix;
		if((matrixw_width(m1) != matrixw_width(m2)) ||
		   (matrixw_height(m1) != matrixw_height(m2))) {
			(*errorout)(_("Can't add/subtract two matricies of different sizes"));
			return copynode_args(n,arg);
		}
		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->args = NULL;
		nn->nargs = 0;
		nn->data.matrix = matrixw_new();
		matrixw_set_size(nn->data.matrix,matrixw_width(m1),matrixw_height(m1));
		for(i=0;i<matrixw_width(m1);i++) {
			for(j=0;j<matrixw_height(m1);j++) {
				ETree *t = op_two_nodes(matrixw_index(m1,i,j),
							matrixw_index(m2,i,j),
							n->data.oper);
				/*exception*/
				if(!t) {
					freeargarr(arg,2);
					freetree(nn);
					return NULL;
				}
				matrixw_set_index(nn->data.matrix,i,j) = t;
			}
		}
		freeargarr(arg,2);
		return nn;
	} else {
		return matrix_scalar_matrix_op(n,arg);
	}
}

static int
is_matrix_value_only(MatrixW *m)
{
	int i,j;
	for(i=0;i<matrixw_width(m);i++) {
		for(j=0;j<matrixw_height(m);j++) {
			ETree *n = matrixw_set_index(m,i,j);
			if(n && n->type != VALUE_NODE)
				return FALSE;
		}
	}
	return TRUE;
}

static void
simple_matrix_multiply(MatrixW *res, MatrixW *m1, MatrixW *m2)
{
	int i,j,k;
	mpw_t tmp;
	mpw_init(tmp);
	for(i=0;i<matrixw_width(res);i++) {
		for(j=0;j<matrixw_height(res);j++) {
			mpw_t accu;
			mpw_init(accu);
			for(k=0;k<matrixw_width(m1);k++) {
				ETree *l = matrixw_index(m1,k,j);
				ETree *r = matrixw_index(m2,i,k);
				mpw_mul(tmp,l->data.value,r->data.value);
				mpw_add(accu,accu,tmp);
				/*XXX: are there any problems that could occur
				  here? ... I don't seem to see any, if there
				  are catch them here*/
			}
			matrixw_set_index(res,i,j) = makenum_use(accu);
		}
	}
	mpw_clear(tmp);
}

static int
expensive_matrix_multiply(MatrixW *res, MatrixW *m1, MatrixW *m2)
{
	int i,j,k;
	for(i=0;i<matrixw_width(res);i++) {
		for(j=0;j<matrixw_height(res);j++) {
			ETree *a = NULL;
			for(k=0;k<matrixw_width(m1);k++) {
				ETree *t;
				ETree *t2;
				t = op_two_nodes(matrixw_index(m1,j,k),
						 matrixw_index(m2,k,i),
						 E_MUL);
				/*exception*/
				if(!t) {
					if(a) freetree(a);
					return FALSE;
				}
				if(!a) {
					a=t;
				} else {
					t2 = op_two_nodes(a,t,E_PLUS);
					freetree(t);
					freetree(a);
					if(!t2)
						return FALSE;
					a = t2;
				}
			}
			matrixw_set_index(res,i,j) = a;
		}
	}
	return TRUE;
}

static ETree *
matrix_mul_op(ETree *n, ETree *arg[])
{
	int i,j;
	ETree *nn;
	if(arg[0]->type == MATRIX_NODE &&
	   arg[1]->type == MATRIX_NODE) {
		int i,j;
		ETree *nn;
		MatrixW *m1,*m2;
		m1 = arg[0]->data.matrix;
		m2 = arg[1]->data.matrix;
		if((matrixw_width(m1) != matrixw_height(m2)) ||
		   (matrixw_height(m1) != matrixw_width(m2))) {
			(*errorout)(_("Can't multiply matricies of wrong sizes"));
			return copynode_args(n,arg);
		}
		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->args = NULL;
		nn->nargs = 0;
		nn->data.matrix = matrixw_new();
		matrixw_set_size(nn->data.matrix,matrixw_width(m2),matrixw_height(m1));
		
		if(is_matrix_value_only(m1) &&
		   is_matrix_value_only(m2)) {
			simple_matrix_multiply(nn->data.matrix,m1,m2);
		} else {
			if(!expensive_matrix_multiply(nn->data.matrix,m1,m2)) {
				freeargarr(arg,2);
				freetree(nn);
				return NULL;
			}
		}
		freeargarr(arg,2);
		return nn;
	} else {
		return matrix_scalar_matrix_op(n,arg);
	}
}

typedef void (*doubleopfunc)(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2);
typedef void (*singleopfunc)(mpw_ptr rop,mpw_ptr op);

static ETree *
doubleop(ETree *n, ETree *arg[], doubleopfunc f)
{
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->data.value,arg[1]->data.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode_args(n,arg);
	}

	mpw_clear(arg[0]->data.value);
	memcpy(arg[0]->data.value,res,sizeof(struct _mpw_t));

	freetree(arg[1]);

	return arg[0];
}

static ETree *
singleop(ETree *n, ETree *arg[], singleopfunc f)
{
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->data.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode_args(n,arg);
	}
	mpw_clear(arg[0]->data.value);
	memcpy(arg[0]->data.value,res,sizeof(struct _mpw_t));

	return arg[0];
}


typedef ETree * (*primfunc_t)(ETree *,ETree **);

#define EVAL_PRIMITIVE(n,numop,numfunc,matrixfunc,stringfunc,argeval) {\
	ETree *r[2]; 					\
	ETree *ret;					\
	if(argeval) {					\
		switch(evalargs(n,r,TRUE)) {		\
		case 1:					\
			return copynode_args(n,r);	\
		case 2:					\
			return NULL;			\
		}					\
	} else {					\
		r[0]=copynode(n->args->data);		\
		r[1]=n->args->next->data;		\
		if(r[1]) r[1]=copynode(r[1]);		\
	}						\
	switch(arglevel(r,n->nargs)) {			\
	case 0: ret = numop(n,r,numfunc);		\
		return ret;				\
	case 1: if(matrixfunc) {			\
			primfunc_t m = matrixfunc;	\
			ret = (*m)(n,r);		\
			return ret;			\
		} else {				\
			(*errorout)(_("Primitive on matrixes undefined")); \
			return copynode_args(n,r);	\
		}					\
	case 2: if(stringfunc) {			\
			primfunc_t m = stringfunc;	\
			ret = (*m)(n,r);		\
			return ret;			\
		} else {				\
			(*errorout)(_("Primitive on strings undefined")); \
			return copynode_args(n,r);	\
		}					\
	default:					\
		(*errorout)(_("Primitives must get numeric/matrix/string arguments")); \
		return copynode_args(n,r);		\
	}						\
}

static ETree *
transpose_matrix(ETree *n)
{
	ETree *r[1];
	switch(evalargs(n,r,TRUE)) {
	case 1:
		return copynode_args(n,r);
	case 2:
		return NULL;
	default: ;
	}
	
	if(r[0]->type != MATRIX_NODE) {
		(*errorout)(_("Can't transpose non-matrix"));
		return copynode_args(n,r);
	}
	
	r[0]->data.matrix->tr = !(r[0]->data.matrix->tr);
	
	return r[0];
}

static ETree *
get_element(ETree *n)
{
	ETree *ll,*rr,*m;
	ETree *ret = NULL;
	int li,ri; 

	if(!get_matrix_index(NULL,n,NULL,&m,&ll,&rr,TRUE,TRUE,
			     &li,&li,&ri,&ri,&ret))
		return ret;

	if(m->type != MATRIX_NODE) {
		ETree *arg[3];
		(*errorout)(_("Index works only on matricies"));
		arg[0]=m;
		arg[1]=ll;
		arg[2]=rr;
		return copynode_args(n,arg);
	}
	
	if(matrixw_width(m->data.matrix) < ri ||
	   matrixw_height(m->data.matrix) < li) {
		ETree *arg[3];
		(*errorout)(_("Matrix index out of range"));
		arg[0]=m;
		arg[1]=ll;
		arg[2]=rr;
		return copynode_args(n,arg);
	}
	
	freetree(ll);
	freetree(rr);
	
	ret = copynode(matrixw_index(m->data.matrix,ri-1,li-1));

	freetree(m);
	
	return ret;
}

static ETree *
get_region(ETree *n)
{
	ETree *ll,*rr,*m;
	ETree *ret = NULL;
	int rowfrom,rowto,colfrom,colto; 
	int w,h;
	int new_matrix = FALSE;
	int do_col,do_row;

	if(n->data.oper == E_GET_REGION) {
		do_col = TRUE;
		do_row = TRUE;
	} else if(n->data.oper == E_GET_ROW_REGION) {
		do_col = FALSE;
		do_row = TRUE;
	} else /*E_GET_COL_REGION*/ {
		do_col = TRUE;
		do_row = FALSE;
	}

	if(!get_matrix_index(NULL,n,NULL,&m,&ll,&rr,do_row,do_col,
			     &rowfrom,&rowto,&colfrom,&colto,&ret))
		return ret;

	if(m->type != MATRIX_NODE) {
		ETree *arg[3];
		(*errorout)(_("Index works only on matricies"));
		arg[0]=m;
		if(ll) {
			arg[1]=ll;
			arg[2]=rr;
		} else {
			arg[1]=rr;
			arg[2]=ll;
		}
		return copynode_args(n,arg);
	}
	
	if(!do_row) {
		rowfrom = 1;
		rowto = matrixw_height(m->data.matrix);
	}
	if(!do_col) {
		colfrom = 1;
		colto = matrixw_width(m->data.matrix);
	}

	w = colto-colfrom+1;
	h = rowto-rowfrom+1;

	if(colto > matrixw_width(m->data.matrix) ||
	   rowto > matrixw_height(m->data.matrix)) {
		ETree *arg[3];
		(*errorout)(_("Index out of range"));
		arg[0]=m;
		if(ll) {
			arg[1]=ll;
			arg[2]=rr;
		} else {
			arg[1]=rr;
			arg[2]=ll;
		}
		return copynode_args(n,arg);
	}

	freetree(ll);
	freetree(rr);
	
	GET_NEW_NODE(ret);
	ret->type = MATRIX_NODE;
	ret->args = NULL;
	ret->nargs = 0;

	ret->data.matrix = matrixw_get_region(m->data.matrix,
					      colfrom-1,rowfrom-1,w,h);

	freetree(m);
	
	return ret;
}


/*note: eval args only applies to primitives*/
static ETree *
evaloper(ETree *n, int argeval)
{
	ETree *nn;
	switch(n->data.oper) {
		case E_SEPAR:
			if(!(nn=evalnode(n->args->data)))
				return NULL; /*exception*/
			freetree(nn);
			return evalnode(n->args->next->data);

		case E_EQUALS:
			return equalsop(n);

		case E_ABS: EVAL_PRIMITIVE(n,singleop,mpw_abs,NULL,NULL,argeval);
		case E_PLUS: EVAL_PRIMITIVE(n,doubleop,mpw_add,matrix_addsub_op,string_concat,argeval);
		case E_MINUS: EVAL_PRIMITIVE(n,doubleop,mpw_sub,matrix_addsub_op,NULL,argeval);
		case E_MUL: EVAL_PRIMITIVE(n,doubleop,mpw_mul,matrix_mul_op,NULL,argeval);
		case E_DIV: EVAL_PRIMITIVE(n,doubleop,mpw_div,NULL,NULL,argeval);
		case E_MOD: EVAL_PRIMITIVE(n,doubleop,mpw_mod,NULL,NULL,argeval);
		case E_NEG: EVAL_PRIMITIVE(n,singleop,mpw_neg,NULL,NULL,argeval);
		case E_EXP: EVAL_PRIMITIVE(n,doubleop,mpw_pow,NULL,NULL,argeval);
		case E_FACT: EVAL_PRIMITIVE(n,singleop,mpw_fac,NULL,NULL,argeval);
		case E_TRANSPOSE: return transpose_matrix(n);
		case E_EQ_CMP: EVAL_PRIMITIVE(n,eqcmpop,NULL,NULL,eqstringop,argeval);
		case E_NE_CMP: EVAL_PRIMITIVE(n,necmpop,NULL,NULL,NULL,argeval);
		case E_CMP_CMP: EVAL_PRIMITIVE(n,cmpcmpop,NULL,NULL,NULL,argeval);
		case E_LT_CMP: EVAL_PRIMITIVE(n,ltcmpop,NULL,NULL,NULL,argeval);
		case E_GT_CMP: EVAL_PRIMITIVE(n,gtcmpop,NULL,NULL,NULL,argeval);
		case E_LE_CMP: EVAL_PRIMITIVE(n,lecmpop,NULL,NULL,NULL,argeval);
		case E_GE_CMP: EVAL_PRIMITIVE(n,gecmpop,NULL,NULL,NULL,argeval);
		case E_LOGICAL_AND: return logicalandop(n,argeval);
		case E_LOGICAL_OR: return logicalorop(n,argeval);
		case E_LOGICAL_XOR: EVAL_PRIMITIVE(n,logicalxorop,NULL,NULL,(primfunc_t)logicalxorop,argeval);
		case E_LOGICAL_NOT: EVAL_PRIMITIVE(n,logicalnotop,NULL,NULL,(primfunc_t)logicalnotop,argeval);

		case E_REGION_SEP:
			{
				ETree *arg[2];
				g_assert(n->args);
				g_assert(n->args->data);
				g_assert(n->args->next->data);
				arg[0] = evalnode(n->args->data);
				if(!arg[0]) return NULL;
				arg[1] = evalnode(n->args->next->data);
				if(!arg[1]) {
					freetree(arg[0]);
					return NULL;
				}
				return copynode_args(n,arg);
			}

		case E_GET_ELEMENT:
			return get_element(n);

		case E_GET_REGION:
		case E_GET_ROW_REGION:
		case E_GET_COL_REGION:
			return get_region(n);

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
		case E_BAILOUT:
			{
				inbailout = TRUE;

				/*use exception to jump out of the current
				  context (that is until a caller realizes
				  that inbailout is set)*/
				return NULL;
			}
		case E_EXCEPTION:
			{
				inexception = TRUE;

				/*use exception to jump out of the current
				  context (that is until a caller realizes
				  that inexception is set)*/
				return NULL;
			}
		case E_CONTINUE:
			{
				g_assert(inloop);
				if(!inloop->data) {
					(*errorout)(_("Called 'continue' outside of a loop"));
					return copynode(n);
				}
				loopout = LOOPOUT_CONTINUE;

				/*use exception to jump out of the current
				  context (that is until a caller realizes
				  that loopout is set)*/
				return NULL;
			}
		case E_BREAK:
			{
				g_assert(inloop);
				if(!inloop->data) {
					(*errorout)(_("Called 'break' outside of a loop"));
					return copynode(n);
				}
				loopout = LOOPOUT_BREAK;

				/*use exception to jump out of the current
				  context (that is until a caller realizes
				  that loopout is set)*/
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
	if(interrupted)
		return NULL;
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
		return evaloper(n,TRUE);
	else if(n->type == IDENTIFIER_NODE)
		return variableop(n);
	else {
		(*errorout)(_("Unexpected node!"));
		return copynode(n);
	}
}

/*return TRUE if node is true (a number node !=0), false otherwise*/
int
isnodetrue(ETree *n, int *bad_node)
{
	if(n->type==STRING_NODE) {
		if(n->data.str && *n->data.str)
			return TRUE;
		else 
			return FALSE;
	}
	if(n->type!=VALUE_NODE) {
		if(bad_node) *bad_node = TRUE;
		return FALSE;
	}
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
	if(n->type==STRING_NODE) {
		if(n->data.str && *n->data.str)
			return TRUE;
		else 
			return FALSE;
	}
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
		if(n->type==STRING_NODE) {
			if(n->data.str && *n->data.str)
				return TRUE;
			else 
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
