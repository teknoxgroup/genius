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

#include "config.h"

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#include <libintl.h>
#define _(x) gettext(x)
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
#include "matop.h"
#include "compil.h"

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

extern GHashTable *uncompiled;

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
		case E_FOR_CONS: return 4;
		case E_FORBY_CONS: return 5;
		case E_FORIN_CONS: return 3;
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
		case E_GET_VELEMENT: return 2;
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
	return n;
}

ETree *
makenum_ui(unsigned long num)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	mpw_init(n->val.value);
	mpw_set_ui(n->val.value,num);
	return n;
}

ETree *
makenum_si(long num)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	mpw_init(n->val.value);
	mpw_set_si(n->val.value,num);
	return n;
}

ETree *
makenum(mpw_t num)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	mpw_init_set(n->val.value,num);
	return n;
}

/*don't create a new number*/
ETree *
makenum_use(mpw_t num)
{
	ETree *n;
	GET_NEW_NODE(n);
	n->type=VALUE_NODE;
	memcpy(n->val.value,num,sizeof(struct _mpw_t));
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
	n->op.oper = oper;
	
	n->op.args = list;
	n->op.nargs = args;

	return n;
}

static void
freetree_full(ETree *n, int freeargs)
{
	GList *li;
	if(!n)
		return;
	switch(n->type) {
	case NULL_NODE: break;
	case VALUE_NODE:
		mpw_clear(n->val.value);
		break;
	case MATRIX_NODE:
		if(n->mat.matrix)
			matrixw_free(n->mat.matrix);
		break;
	case OPERATOR_NODE:
		if(!freeargs) break;
		for(li=n->op.args;li;li=g_list_next(li)) {
			if(li->data)
				freetree_full(li->data,TRUE);
		}
		g_list_free(n->op.args); 
		break;
	case IDENTIFIER_NODE:
		/*was this a fake token, to an anonymous function*/
		if(!n->id.id->token) {
			/*XXX:where does the function go?*/
			g_list_free(n->id.id->refs);
			g_free(n->id.id);
		}
		break;
	case STRING_NODE:
		g_free(n->str.str);
		break;
	case FUNCTION_NODE:
		freefunc(n->func.func);
		break;
	case COMPARISON_NODE:
		for(li=n->comp.args;li;li=g_list_next(li)) {
			if(li->data)
				freetree_full(li->data,TRUE);
		}
		g_list_free(n->comp.args); 
		g_list_free(n->comp.comp);
		break;
	case MATRIX_ROW_NODE:
		for(li=n->row.args;li;li=g_list_next(li)) {
			if(li->data)
				freetree_full(li->data,TRUE);
		}
		g_list_free(n->row.args); 
		break;
	case SPACER_NODE:
		if(freeargs && n->sp.arg)
			freetree(n->sp.arg);
		break;
	default: break;
	}
	/*put onto the free list*/
	n->next = free_trees;
	free_trees = n;
}

void
freetree(ETree *n)
{
	freetree_full(n,TRUE);
}

static void
freenode(ETree *n)
{
	freetree_full(n,FALSE);
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

	switch(n->type) {
	case NULL_NODE: break;
	case VALUE_NODE:
		mpw_init_set(n->val.value,o->val.value);
		break;
	case MATRIX_NODE:
		n->mat.matrix = matrixw_copy(o->mat.matrix);
		break;
	case OPERATOR_NODE:
		n->op.args = NULL;
		for(li = o->op.args;li!=NULL;li=g_list_next(li))
			n->op.args = g_list_append(n->op.args,
						   copynode(li->data));
		break;
	case IDENTIFIER_NODE: break;
	case STRING_NODE:
		n->str.str = g_strdup(o->str.str);
		break;
	case FUNCTION_NODE:
		n->func.func = d_copyfunc(o->func.func);
		break;
	case COMPARISON_NODE:
		n->comp.args = NULL;
		for(li = o->comp.args;li!=NULL;li=g_list_next(li))
			n->comp.args = g_list_append(n->comp.args,
						     copynode(li->data));
		n->comp.comp = g_list_copy(o->comp.comp);
		break;
	case MATRIX_ROW_NODE:
		n->row.args = NULL;
		for(li = o->row.args;li!=NULL;li=g_list_next(li))
			n->row.args = g_list_append(n->row.args,
						    copynode(li->data));
		break;
	case SPACER_NODE:
		if(o->sp.arg)
			n->sp.arg = copynode(n->sp.arg);
	default: break;
	}

	return n;
}

/*evaluate arguments, will crash if the node doesn't have at least
  one arg or isn't and operator*/
static int
evalargs(GList *args, ETree *ret[], int eval_first)
{
	GList *li;
	int i;
	int r = 0;
	
	/*if the first shouldn't be evaluated just pass it*/
	if(!eval_first)
		ret[0] = copynode(args->data);
	else {
		ret[0] = evalnode(args->data);
		/*exception is signaled by NULL*/
		if(!ret[0])
			return 2;
			
	}
	
	for(i=1,li=args->next; li!=NULL; li=g_list_next(li),i++) {
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
		    ret[i]->op.oper != E_REFERENCE))
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
		if(matrixw_height(et->mat.matrix)>w)
			w = matrixw_height(et->mat.matrix);
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
		} else if(matrixw_width(et->mat.matrix) == 1) {
			int xx;
			matrixw_make_private(et->mat.matrix);
			for(x=0;x<matrixw_height(et->mat.matrix);x++) {
				matrix_index(dest,i,di+x) =
					matrixw_set_index(et->mat.matrix,0,x);
				matrixw_set_index(et->mat.matrix,0,x) = NULL;
			}
			xx = 0;
			for(x=matrixw_height(et->mat.matrix);x<w;x++) {
				matrix_index(dest,i,di+x) =
					copynode(matrix_index(dest,i,di+xx));
				if((++xx)>=matrixw_height(et->mat.matrix))
					xx=0;
			}
			freetree(et);
		/*non-trivial matrix*/
		} else {
			int xx;

			matrixw_make_private(et->mat.matrix);
			
			*need_col += matrixw_width(et->mat.matrix) - 1;

			for(x=0;x<matrixw_height(et->mat.matrix);x++) {
				ETree *n;
				GET_NEW_NODE(n);
				n->type = MATRIX_ROW_NODE;
				
				n->row.args = NULL;
				for(xx=matrixw_width(et->mat.matrix)-1;xx>=0;xx--) {
					n->row.args =
						g_list_prepend(n->row.args,
							       matrixw_set_index(et->mat.matrix,xx,x));
					matrixw_set_index(et->mat.matrix,xx,x) = NULL;
				}
				n->row.nargs = matrixw_width(et->mat.matrix);
				
				matrix_index(dest,i,di+x) = n;

				*need_colwise = TRUE;
			}
			xx = 0;
			for(x=matrixw_height(et->mat.matrix);x<w;x++) {
				matrix_index(dest,i,di+x) =
					copynode(matrix_index(dest,i,di+xx));
				if((++xx)>=matrixw_height(et->mat.matrix))
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
		if(et->row.nargs>w)
			w = et->row.nargs;
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
			for(li=et->row.args,x=0;li!=NULL;li=g_list_next(li),x++)
				matrix_index(dest,di[i]+x,i) = li->data;
			exp = MIN(w-1,cols-wid);
			roww->data = GINT_TO_POINTER(wid + exp);
			xx = 0;
			for(x=et->row.nargs;x<=exp;x++) {
				matrix_index(dest,di[i]+x,i) =
					copynode(matrix_index(dest,di[i]+xx,i));
				if((++xx)>=et->row.nargs)
					xx=0;
			}
			di[i] += x;
			g_list_free(et->row.args);
			et->row.args = NULL;
			freenode(et);
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
	MatrixW *nm;
	int h;

	nm = n->mat.matrix;

	h = matrixw_height(nm);
	matrix_set_size(m,matrixw_width(nm),h);
	
	cols = matrixw_width(nm);
	
	for(i=0,k=0;i<h;i++) {
		int c;
		int w = expand_row(m,nm,k,i,&c,&need_colwise);
		k += w;
		for(;w>0;w--)
			roww = g_list_prepend(roww,GINT_TO_POINTER(c));

		if(cols<c)
			cols = c;
	}
	
	roww = g_list_reverse(roww);
	
	if(need_colwise) {
		Matrix *tm = matrix_new();
		guint *di = g_new0(guint,k);
		int j;
		matrix_set_size(tm,cols,m->height);
		for(i=0;i<m->width;i++)
			expand_col(tm,m,di,i,roww);
		g_free(di);
		matrix_free(m);
		m = tm;
	}
	
	g_list_free(roww);

	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->mat.matrix = matrixw_new_with_matrix(m);
	nn->mat.quoted = 0;
	
	return nn;
}

/*evaluate a quoted matrix, so we just evaluate all nodes, but don't expand*/
static ETree *
qevalmatrix(ETree *n)
{
	int i;
	int j;
	ETree *nn;
	Matrix *m = matrix_new();
	MatrixW *nm;
	int w,h;
	
	nm = n->mat.matrix;

	w = matrixw_width(nm);
	h = matrixw_height(nm);
	matrix_set_size(m,w,h);
	
	for(i=0;i<w;i++) {
		for(j=0;j<h;j++) {
			ETree *e = matrixw_set_index(nm,i,j);
			if(e)
				matrix_index(m,i,j)=evalnode(e);
		}
	}

	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->mat.matrix = matrixw_new_with_matrix(m);
	nn->mat.quoted = 1;
	
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
	   !mpw_is_integer(num->val.value)) {
		(*errorout)(_("Wrong argument type as matrix index"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return -1;
	}

	i = mpw_get_long(num->val.value);
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
get_index_region(ETree *n, ETree *l, ETree *set, ETree *m,
		 ETree *ll, ETree *rr, 
		 ETree *num, ETree **ret, int *from, int *to)
{
	if(num->type == OPERATOR_NODE &&
	   num->op.oper == E_REGION_SEP) {
		g_assert(num->op.args);
		g_assert(num->op.args->data);
		g_assert(num->op.args->next->data);
		*from = get_matrix_index_num(n,l,set,m,ll,rr,
					     num->op.args->data,ret);
		if(*from == -1) return FALSE;
		*to = get_matrix_index_num(n,l,set,m,ll,rr,
					   num->op.args->next->data,ret);
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
get_matrix_index(ETree *n, ETree *l, ETree *set, ETree **m,
		 ETree **ll, ETree **rr, 
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
		f = d_lookup_local(m->id.id);
		if(!f) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->mat.matrix = matrixw_new();
			t->mat.quoted = 0;
			matrixw_set_size(t->mat.matrix,1,1);

			f = d_makevfunc(m->id.id,t);
			d_addfunc(f);
			if(new_matrix) *new_matrix = TRUE;
		} else if(f->type != USER_FUNC &&
			  f->type != VARIABLE_FUNC) {
			(*errorout)(_("Indexed Lvalue not user function"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}
		if(!f->data.user) {
			g_assert(uncompiled);
			f->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,f->id));
			g_hash_table_remove(uncompiled,f->id);
			g_assert(f->data.user);
		}
		if(f->data.user->type != MATRIX_NODE) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->mat.matrix = matrixw_new();
			t->mat.quoted = 0;
			matrixw_set_size(t->mat.matrix,1,1);

			d_set_value(f,t);
			if(new_matrix) *new_matrix = TRUE;
		}
		mat = f->data.user->mat.matrix;
	} else if(m->type == OPERATOR_NODE ||
		  m->op.oper == E_DEREFERENCE) {
		ETree *lll;
		EFunc *f;
		GET_L(m,lll);

		if(lll->type != IDENTIFIER_NODE) {
			(*errorout)(_("Dereference of non-identifier!"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}

		f = d_lookup_local(lll->id.id);
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

		if(f->data.ref->type != USER_FUNC &&
		   f->data.ref->type != VARIABLE_FUNC) {
			(*errorout)(_("Indexed Lvalue not user function"));
			*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
			return NULL;
		}
		if(!f->data.ref->data.user) {
			g_assert(uncompiled);
			f->data.ref->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,f->data.ref->id));
			g_hash_table_remove(uncompiled,f->id);
			g_assert(f->data.user);
		}
		if(f->data.ref->data.user->type != MATRIX_NODE) {
			ETree *t;
			GET_NEW_NODE(t);
			t->type = MATRIX_NODE;
			t->mat.matrix = matrixw_new();
			t->mat.quoted = 0;
			matrixw_set_size(t->mat.matrix,1,1);

			d_set_value(f->data.ref,t);
			if(new_matrix) *new_matrix = TRUE;
		}
		mat = f->data.ref->data.user->mat.matrix;
	} else {
		(*errorout)(_("Indexed Lvalue not an identifier or a dereference"));
		*ret = matrix_el_bailout_set(n, set, l, m, ll, rr);
		return NULL;
	}
	return mat;
}

static ETree *
equalsop(ETree *n, int do_ret)
{
	ETree *l;
	ETree *r;
	ETree *set;
	GET_LR(n,l,r);
	
	if(l->type != IDENTIFIER_NODE &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_GET_VELEMENT) &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_GET_ELEMENT) &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_GET_REGION) &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_GET_COL_REGION) &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_GET_ROW_REGION) &&
	   !(l->type == OPERATOR_NODE && l->op.oper == E_DEREFERENCE)) {
		(*errorout)(_("Lvalue not an identifier/dereference/matrix location!"));
		return copynode(n);
	}

	if(l->type == IDENTIFIER_NODE) {
		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;
		if(set->type == FUNCTION_NODE) {
			if(do_ret)
				d_addfunc(d_makerealfunc(set->func.func,l->id.id,FALSE));
			else {
				d_addfunc(d_makerealfunc(set->func.func,l->id.id,TRUE));
				freetree(set);
				set = EMPTY_RET;
			}
		} else if(set->type == OPERATOR_NODE &&
			set->op.oper == E_REFERENCE) {
			ETree *t = set->op.args->data;
			EFunc *rf = d_lookup_global(t->id.id);
			if(!rf) {
				ETree *args[2];
				(*errorout)(_("Referencing an undefined variable!"));
				args[0]=copynode(l);
				args[1]=set;
				return copynode_args(n,args);
			}
			d_addfunc(d_makereffunc(l->id.id,rf));
		} else {
			if(do_ret)
				d_addfunc(d_makevfunc(l->id.id,copynode(set)));
			else {
				d_addfunc(d_makevfunc(l->id.id,set));
				set = EMPTY_RET;
			}
		}
	} else if(l->op.oper == E_DEREFERENCE) {
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
		
		f = d_lookup_local(ll->id.id);
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
		
		if(set->type == FUNCTION_NODE) {
			if(do_ret)
				d_setrealfunc(f->data.ref,set->func.func,FALSE);
			else {
				d_setrealfunc(f->data.ref,set->func.func,TRUE);
				freetree(set);
				set = EMPTY_RET;
			}
		} else if(set->type == OPERATOR_NODE &&
			set->op.oper == E_REFERENCE) {
			ETree *t = set->op.args->data;
			EFunc *rf = d_lookup_global(t->id.id);
			if(!rf) {
				ETree *args[2];
				(*errorout)(_("Referencing an undefined variable!"));
				args[0]=copynode(l);
				args[1]=set;
				return copynode_args(n,args);
			}
			d_set_ref(f->data.ref,rf);
		} else {
			if(do_ret)
				d_set_value(f->data.ref,copynode(set));
			else {
				d_set_value(f->data.ref,set);
				set = EMPTY_RET;
			}
		}
	} else if(l->op.oper == E_GET_ELEMENT) {
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
		
		if(do_ret)
			matrixw_set_element(mat,ri-1,li-1,set);
		else {
			matrixw_set_element(mat,ri-1,li-1,copynode(set));
			set = EMPTY_RET;
		}
	} else if(l->op.oper == E_GET_VELEMENT) {
		MatrixW *mat;
		ETree *ll,*rr,*m;
		ETree *ret = NULL;
		int li; 

		set = evalnode(r);
		/*exception*/
		if(!set)
			return NULL;
		
		if(!get_matrix_index(n,l,set,&m,&ll,&rr,TRUE,FALSE,
				     &li,&li,NULL,NULL,&ret))
			return ret;

		if(!(mat = get_matrix_p(n,l,set,m,ll,rr,NULL,&ret)))
			return ret;

		freetree(ll);
		freetree(rr);
		
		if(matrixw_height(mat)==1) {
			if(do_ret)
				matrixw_set_element(mat,li-1,0,copynode(set));
			else {
				matrixw_set_element(mat,li-1,0,set);
				set = EMPTY_RET;
			}
		} else if(matrixw_width(mat)==1) {
			if(do_ret)
				matrixw_set_element(mat,0,li-1,copynode(set));
			else {
				matrixw_set_element(mat,0,li-1,set);
				set = EMPTY_RET;
			}
		} else {
			int x,y;
			x = (li-1)%matrixw_width(mat);
			y = (li-1)/matrixw_width(mat);
			if(do_ret)
				matrixw_set_element(mat,x,y,copynode(set));
			else {
				matrixw_set_element(mat,x,y,set);
				set = EMPTY_RET;
			}
		}
	} else /*l->data.oper == E_GET_REGION E_GET_COL_REGION E_GET_ROW_REGION*/ {
		MatrixW *mat;
		ETree *ll,*rr,*m;
		ETree *ret = NULL;
		int rowfrom,rowto,colfrom,colto; 
		int w,h;
		int new_matrix = FALSE;
		int do_col,do_row;
		
		if(l->op.oper == E_GET_REGION) {
			do_col = TRUE;
			do_row = TRUE;
		} else if(l->op.oper == E_GET_ROW_REGION) {
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
					rowto = matrixw_height(set->mat.matrix);
				else
					rowto = 1;
			} else
				rowto = matrixw_height(mat);
		}
		if(!do_col) {
			colfrom = 1;
			if(new_matrix) {
				if(set->type == MATRIX_NODE)
					colto = matrixw_width(set->mat.matrix);
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
		     (matrixw_width(set->mat.matrix) == w &&
		      matrixw_height(set->mat.matrix) == h))) {
			(*errorout)(_("Can't set a region to a region of a different size"));
			return matrix_el_bailout_set(n, set, l, m, ll, rr);
		}

		freetree(ll);
		freetree(rr);
		
		if(set->type == MATRIX_NODE) {
			matrixw_set_region(mat,set->mat.matrix,0,0,
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
	
	g_assert(o->type == OPERATOR_NODE ||
		 o->type == COMPARISON_NODE);

	GET_NEW_NODE(n);

	memcpy(n,o,sizeof(ETree));

	/*add the arguments*/
	if(n->op.type == OPERATOR_NODE) {
		n->op.args = NULL;
		for(i=0;i<n->op.nargs;i++)
			n->op.args = g_list_append(n->op.args,r[i]);
	} else /*COMPARISON_NODE*/ {
		n->comp.args = NULL;
		for(i=0;i<n->comp.nargs;i++)
			n->comp.args = g_list_append(n->comp.args,r[i]);
		n->comp.comp = g_list_copy(n->comp.comp);
	}

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
funccallop(ETree *n,int direct_call,int do_ret)
{
	ETree **r;
	ETree *ret = NULL;
	EFunc *f;
	ETree *l;
	
	GET_L(n,l);
	
	r = g_new(ETree *,n->op.nargs);
	if(evalargs(n->op.args,r,
		    (direct_call ||
		     (l->type == OPERATOR_NODE &&
		      l->op.oper == E_DEREFERENCE))?FALSE:TRUE
		   )==2) {
		g_free(r);
		return NULL;
	}
	
	if(l->type == IDENTIFIER_NODE) {
		f = d_lookup_global(l->id.id);
		if(!f) {
			char buf[256];
			g_snprintf(buf,256,_("Function '%s' used unintialized"),
				   l->id.id->token);
			(*errorout)(buf);
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		}
	} else if(l->type == FUNCTION_NODE) {
		f = l->func.func;
	} else if(l->type == OPERATOR_NODE &&
		l->op.oper == E_DEREFERENCE) {
		ETree *ll;
		GET_L(l,ll);
		f = d_lookup_global(ll->id.id);
		if(!f) {
			char buf[256];
			g_snprintf(buf,256,_("Variable '%s' used unintialized"),
				   ll->id.id->token);
			(*errorout)(buf);
			ret = copynode_args(n,r);
			g_free(r);
			return ret;
		} else if(f->type != REFERENCE_FUNC) {
			char buf[256];
			g_snprintf(buf,256,_("Can't dereference '%s'!"),
				   ll->id.id->token);
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
	
	if(f->nargs + 1 != n->op.nargs) {
		char buf[256];
		g_snprintf(buf,256,_("Call of '%s' with the wrong number of arguments!\n"
				     "(should be %d)"),f->id?f->id->token:"anonymous",f->nargs);
		(*errorout)(buf);
		ret = copynode_args(n,r);
		g_free(r);
		return ret;
	} else if(f->type == USER_FUNC ||
		  f->type == VARIABLE_FUNC) {
		int i;
		GList *li;
		GSList *sli;

		d_addcontext();
		/*push arguments on context stack*/
		for(i=1,li=f->named_args;i<n->op.nargs;i++,li=g_list_next(li)) {
			if(r[i]->type == FUNCTION_NODE) {
				d_addfunc(d_makerealfunc(r[i]->func.func,li->data,FALSE));
			} else if(r[i]->type == OPERATOR_NODE &&
			   r[i]->op.oper == E_REFERENCE) {
				ETree *t = r[i]->op.args->data;
				EFunc *rf = d_lookup_global_up1(t->id.id);
				if(!rf) {
					freedict(d_popcontext());
					(*errorout)(_("Referencing an undefined variable!"));
					ret = copynode_args(n,r);
					g_free(r);
					return ret;
				}
				d_addfunc(d_makereffunc(li->data,rf));
			} else
				d_addfunc(d_makevfunc(li->data,copynode(r[i])));
		}

		if(!f->data.user) {
			g_assert(uncompiled);
			f->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,f->id));
			g_hash_table_remove(uncompiled,f->id);
			g_assert(f->data.user);
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
		ret = evalnode_full(f->data.user,do_ret);
		sli=inloop;
		inloop=g_slist_remove_link(inloop,sli);
		g_slist_free_1(sli);
		
		if(!inexception && inbailout) {
			if(returnval) freetree(returnval);
			returnval = NULL;
			inbailout = FALSE;
			if(ret && ret!=EMPTY_RET) freetree(ret);
			ret = copynode_args(n,r);
		} else {
			for(i=0;i<n->op.nargs;i++)
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
			if(ret && ret!=EMPTY_RET) freetree(ret);
			ret = NULL;
		}
		return ret;
	} else if(f->type == BUILTIN_FUNC) {
		int exception = FALSE;
		ret = (*f->data.func)(&r[1],&exception);
		if(!ret && !exception)
			ret = copynode_args(n,r);
		else
			freeargarr(r,n->op.nargs);
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
		
		if(!do_ret) {
			freeargarr(r,n->op.nargs);
			g_free(r);
			return EMPTY_RET;
		}

		GET_NEW_NODE(i);
		i->type = IDENTIFIER_NODE;
		i->id.id = f->id; /*this WILL have an id*/

		GET_NEW_NODE(a);
		a->type = OPERATOR_NODE;
		a->op.oper = E_REFERENCE;

		a->op.args = g_list_append(NULL,i);
		a->op.nargs = 1;
		freeargarr(r,n->op.nargs);
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
	
	f = d_lookup_global(l->id.id);
	if(!f) {
		char buf[256];
		g_snprintf(buf,256,_("Variable '%s' used unintialized"),l->id.id->token);
		(*errorout)(buf);
	} else if(f->nargs != 0) {
		char buf[256];
		g_snprintf(buf,256,_("Call of '%s' with the wrong number of arguments!\n"
				     "(should be %d)"),f->id?f->id->token:"anonymous",f->nargs);
		(*errorout)(buf);
	} else if(f->type != REFERENCE_FUNC) {
		char buf[256];
		g_snprintf(buf,256,_("Trying to dereference '%s' which is not a reference!\n"),
				     f->id?f->id->token:"anonymous");
		(*errorout)(buf);
	} else /*if(f->type == REFERENCE_FUNC)*/ {
		f = f->data.ref;
		if(!f) {
			(*errorout)(_("NULL reference encountered!"));
		} else if(f->type == USER_FUNC ||
			  f->type == VARIABLE_FUNC) {
			if(!f->data.user) {
				g_assert(uncompiled);
				f->data.user =
					decompile_tree(g_hash_table_lookup(uncompiled,f->id));
				g_hash_table_remove(uncompiled,f->id);
				g_assert(f->data.user);
			}
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
			i->id.id = f->id; /*this WILL have an id*/

			GET_NEW_NODE(a);
			a->type = OPERATOR_NODE;
			a->op.oper = E_REFERENCE;
			
			a->op.args = g_list_append(NULL,i);
			a->op.nargs = 1;
			return a;
		} else
			(*errorout)(_("Unevaluatable function type encountered!"));
	}
	return copynode(n);
}

static ETree *
variableop(ETree *n)
{
	EFunc *f;
	
	f = d_lookup_global(n->id.id);
	if(!f) {
		char buf[256];
		g_snprintf(buf,256,_("Variable '%s' used unitialized"),n->id.id->token);
		(*errorout)(buf);
		return copynode(n);
	}
	
	if(f->type == VARIABLE_FUNC) {
		if(!f->data.user) {
			g_assert(uncompiled);
			f->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,f->id));
			g_hash_table_remove(uncompiled,f->id);
			g_assert(f->data.user);
		}
		return copynode(f->data.user);
	} else if(f->type == USER_FUNC) {
		ETree *nn;
		if(!f->data.user) {
			g_assert(uncompiled);
			f->data.user =
				decompile_tree(g_hash_table_lookup(uncompiled,f->id));
			g_hash_table_remove(uncompiled,f->id);
			g_assert(f->data.user);
		}
		GET_NEW_NODE(nn);
		nn->type = FUNCTION_NODE;
		nn->func.func = d_makeufunc(NULL,copynode(f->data.user),
					    g_list_copy(f->named_args),f->nargs);
		nn->func.func->context = -1;

		return nn;
	} else if(f->type == BUILTIN_FUNC) {
		ETree *ret;
		int exception = FALSE;

		if(f->nargs != 0) {
			ETree *nn;
			GET_NEW_NODE(nn);
			nn->type = FUNCTION_NODE;
			nn->func.func = d_makerealfunc(f,NULL,FALSE);
			nn->func.func->context = -1;

			return nn;
		}
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
			i->id.id = f->id;
		} else {
			/*make up a new fake id*/
			Token *tok = g_new(Token,1);
			tok->token = NULL;
			tok->refs = g_list_append(NULL,f);
			i->id.id = tok;
		}

		GET_NEW_NODE(a);
		a->type = OPERATOR_NODE;
		a->op.oper = E_REFERENCE;

		a->op.args = g_list_append(NULL,i);
		a->op.nargs = 1;
		return a;
	} else
		(*errorout)(_("Unevaluatable function type encountered!"));
	return copynode(n);
}

static ETree *
ifop(ETree *n, int do_ret)
{
	ETree *l,*r,*rr=NULL;
	ETree *errorret;
	int exception=FALSE;
	if(n->op.oper == E_IF_CONS) {
		GET_LR(n,l,r);
	} else { /*IF_ELSE*/
		GET_LRR(n,l,r,rr);
	}

	if(eval_isnodetrue(l,&exception,&errorret)) {
		return evalnode_full(r,do_ret);
	} else {
		/*if we got a non-critical error and just can't evaluate*/
		if(errorret) {
			/*copy the current node and replace the predicate
			  with the evaluated value*/
			ETree *a = copynode(n);
			freetree(a->op.args->data);
			a->op.args->data = errorret;
			return a;
		}
		if(exception) return NULL;
		if(n->op.oper == E_IF_CONS) {
			if(do_ret)
				return makenum_null();
			else
				return EMPTY_RET;
		} else /*IF_ELSE*/
			return evalnode_full(rr,do_ret);
	}
}

static ETree *
loopop(ETree *n,int do_ret)
{
	ETree *l,*r,*ret;
	ETree *t;
	ETree *errorret;
	int exception=FALSE;
	int until=FALSE;
	int first=FALSE;

	if(do_ret)
		ret = makenum_null();
	else
		ret = EMPTY_RET;

	GET_LR(n,l,r);
	
	switch (n->op.oper) {
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
		if(ret && ret!=EMPTY_RET) freetree(ret);
		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode_full(r,do_ret);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				if(do_ret)
					return makenum_null();
				else
					return EMPTY_RET;
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = NULL;
			} else
				return NULL;
		}
	}
	while(until?
	      !eval_isnodetrue(l,&exception,&errorret):
	      eval_isnodetrue(l,&exception,&errorret)) {
		GSList *li;
		if(ret && ret!=EMPTY_RET) freetree(ret);
		if(exception)
			return NULL;
		/*if we got a non-critical error and just can't evaluate*/
		else if(errorret) {
			/*copy the current node and replace the predicate with the
			  evaluated value*/
			ETree *a = copynode(n);
			freetree(a->op.args->data);
			a->op.args->data = errorret;
			return a;
		}
		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode_full(r,do_ret);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				if(do_ret)
					return makenum_null();
				else
					return EMPTY_RET;
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = NULL;
			} else
				return NULL;
		}
	}
	if(exception) {
		if(ret && ret!=EMPTY_RET) freetree(ret);
		return NULL;
	/*if we got a non-critical error and just can't evaluate*/
	} else if(errorret) {
		/*copy the current node and replace the predicate with the
		  evaluated value*/
		ETree *a = copynode(n);
		freetree(ret);
		freetree(a->op.args->data);
		a->op.args->data = errorret;
		return a;
	}
	if(!ret) {
		if(do_ret)
			return makenum_null();
		else
			return EMPTY_RET;
	} else
		return ret;
}

static ETree *
forloop(ETree *n, int do_ret)
{
	ETree *from,*to,*by=NULL,*body,*ident;
	ETree *ret = NULL;
	int init_cmp;

	if (n->op.oper==E_FOR_CONS) {
		GET_ABCD(n,ident,from,to,body);
	} else /*if (n->op.oper==E_FORBY_CONS)*/ {
		GET_ABCDE(n,ident,from,to,by,body);
	}
	
	from = evalnode(from);
	if(!from) return NULL;
	
	to = evalnode(to);
	if(!to) {
		freetree(from);
		return NULL;
	}

	if(by) {
		by = evalnode(by);
		if(!by) {
			freetree(to);
			freetree(from);
			return NULL;
		}
	}
	
	if((by && (by->type != VALUE_NODE ||
		   mpw_is_complex(by->val.value))) ||
	   from->type != VALUE_NODE || mpw_is_complex(from->val.value) ||
	   to->type != VALUE_NODE || mpw_is_complex(to->val.value)) {
		(*errorout)(_("Bad type for 'for' loop!"));
		if(!by) {
			ETree *args[4];
			args[0] = copynode(ident);
			args[1] = from;
			args[2] = to;
			args[3] = copynode(body);
			return copynode_args(n,args);
		} else {
			ETree *args[5];
			args[0] = copynode(ident);
			args[1] = from;
			args[2] = to;
			args[3] = by;
			args[4] = copynode(body);
			return copynode_args(n,args);
		}
	}
	if(by && mpw_sgn(by->val.value)==0) {
		(*errorout)(_("'for' loop increment can't be 0"));
		if(!by) {
			ETree *args[4];
			args[0] = copynode(ident);
			args[1] = from;
			args[2] = to;
			args[3] = body;
			return copynode_args(n,args);
		} else {
			ETree *args[5];
			args[0] = copynode(ident);
			args[1] = from;
			args[2] = to;
			args[3] = by;
			args[4] = body;
			return copynode_args(n,args);
		}
	}
	
	init_cmp = mpw_cmp(from->val.value,to->val.value);
	
	/*if no iterations*/
	if(!by) {
		if(init_cmp>0) {
			freetree(to);
			freetree(by);
			d_addfunc(d_makevfunc(ident->id.id,from));
			if(do_ret)
				return makenum_null();
			else
				return EMPTY_RET;
		} else if(init_cmp==0) {
			init_cmp = -1;
		}
	} else {
		int sgn = mpw_sgn(by->val.value);
		if((sgn>0 && init_cmp>0) || (sgn<0 && init_cmp<0)) {
			freetree(to);
			freetree(by);
			d_addfunc(d_makevfunc(ident->id.id,from));
			if(do_ret)
				return makenum_null();
			else
				return EMPTY_RET;
		}
		if(init_cmp == 0)
			init_cmp = -sgn;
	}


	do {
		GSList *li;
		if(ret && ret!=EMPTY_RET) freetree(ret);

		d_addfunc(d_makevfunc(ident->id.id,copynode(from)));

		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode_full(body,do_ret);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				freetree(from);
				freetree(to);
				freetree(by);
				if(do_ret)
					return makenum_null();
				else
					return EMPTY_RET;
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = NULL;
			} else {
				freetree(from);
				freetree(to);
				freetree(by);
				return NULL;
			}
		}
		if(by)
			mpw_add(from->val.value,from->val.value,by->val.value);
		else
			mpw_add_ui(from->val.value,from->val.value,1);

	} while(mpw_cmp(from->val.value,to->val.value)!=-init_cmp);
	freetree(from);
	freetree(to);
	freetree(by);
	if(!ret) {
		if(do_ret)
			return makenum_null();
		else
			return EMPTY_RET;
	} else
		return ret;
}

static ETree *
forinloop(ETree *n, int do_ret)
{
	ETree *in,*body,*ident;
	ETree *ret = NULL;
	int i,j;

	GET_LRR(n,ident,in,body);
	
	in = evalnode(in);
	if(!in) return NULL;
	
	if(in->type != VALUE_NODE &&
	   in->type != MATRIX_NODE) {
		ETree *args[3];
		(*errorout)(_("Bad type for 'for' loop!"));
		args[0] = copynode(ident);
		args[1] = in;
		args[2] = copynode(body);
		return copynode_args(n,args);
	}

	j=0;
	i=0;
	for(;;) {
		GSList *li;
		if(ret && ret!=EMPTY_RET) freetree(ret);

		if(in->type == VALUE_NODE)
			d_addfunc(d_makevfunc(ident->id.id,copynode(in)));
		else
			d_addfunc(d_makevfunc(ident->id.id,
					      copynode(matrixw_index(in->mat.matrix,i,j))));
		loopout = LOOPOUT_NOTHING;
		inloop = g_slist_prepend(inloop,GINT_TO_POINTER(1));
		ret = evalnode_full(body,do_ret);
		li=inloop;
		inloop=g_slist_remove_link(inloop,li);
		g_slist_free_1(li);
		if(!ret) {
			if(loopout == LOOPOUT_BREAK) {
				loopout = LOOPOUT_NOTHING;
				freetree(in);
				if(do_ret)
					return makenum_null();
				else
					return EMPTY_RET;
			} else if(loopout == LOOPOUT_CONTINUE) {
				loopout = LOOPOUT_NOTHING;
				ret = NULL;
			} else {
				freetree(in);
				return NULL;
			}
		}

		if(in->type == VALUE_NODE)
			break;
		else {
			if((++i)>=matrixw_width(in->mat.matrix)) {
				i=0;
				if((++j)>=matrixw_height(in->mat.matrix))
					break;
			}
		}
	}
	freetree(in);
	if(!ret) {
		if(do_ret)
			return makenum_null();
		else
			return EMPTY_RET;
	} else
		return ret;
}

/*compare nodes, return TRUE if equal
  makes them the same type as a side effect*/
static int
eqlnodes(ETree *l, ETree *r)
{
	int n = mpw_eql(l->val.value,r->val.value);
	if(error_num) return 0;
	return n;
}

/*compare nodes, return -1 if first one is smaller, 0 if they are
  equal, 1 if the first one is greater
  makes them the same type as a side effect*/
static int
cmpnodes(ETree *l, ETree *r)
{
	int n=0;

	n=mpw_cmp(l->val.value,r->val.value);

	if(error_num) return 0;

	if(n>0) n=1;
	else if(n<0) n=-1;
	return n;
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
logicalandop(ETree *n, int argeval)
{
	ETree *r[2]; 
	ETree *ret;
	int bad_node = FALSE;
	
	g_assert(n->op.args);
	g_assert(n->op.args->data);
	g_assert(n->op.args->next->data);

	r[0] = n->op.args->data;
	r[1] = n->op.args->next->data;

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
	
	g_assert(n->op.args);
	g_assert(n->op.args->data);
	g_assert(n->op.args->next->data);

	r[0] = n->op.args->data;
	r[1] = n->op.args->next->data;

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
string_concat(ETree *n, ETree *arg[], gpointer f)
{
	ETree *ret;
	char *s=NULL;
	
	if(arg[0]->type == STRING_NODE &&
	   arg[1]->type == STRING_NODE) {
		s = g_strconcat(arg[0]->str.str,arg[1]->str.str,NULL);
	} else if(arg[0]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[1]);
		s = g_strconcat(arg[0]->str.str,gs->str,NULL);
		g_string_free(gs,TRUE);
	} else if(arg[1]->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,arg[0]);
		s = g_strconcat(gs->str,arg[1]->str.str,NULL);
		g_string_free(gs,TRUE);
	} else
		g_assert_not_reached();
	
	freeargarr(arg,2);

	GET_NEW_NODE(ret);
	ret->type = STRING_NODE;
	ret->str.str = s;
	
	return ret;
}

static int
eqstring(ETree *a, ETree *b)
{
	int r=0;
	if(a->type == STRING_NODE &&
	   b->type == STRING_NODE) {
		r=strcmp(a->str.str,b->str.str)==0;
	} else if(a->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,b);
		r = strcmp(a->str.str,gs->str)==0;
		g_string_free(gs,TRUE);
	} else if(b->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,a);
		r = strcmp(b->str.str,gs->str)==0;
		g_string_free(gs,TRUE);
	} else
		g_assert_not_reached();

	return r;
}

static int
eqmatrix(ETree *a, ETree *b, int *error)
{
	int r=FALSE;
	int i,j;
	if(a->type == MATRIX_NODE &&
	   b->type == MATRIX_NODE) {
		if(!is_matrix_value_only(a->mat.matrix) ||
		   !is_matrix_value_only(b->mat.matrix)) {
			(*errorout)(_("Cannot compare non value-only matrixes"));
			*error = TRUE;
			return 0;
		}
		if(matrixw_width(a->mat.matrix)!=
		   matrixw_width(b->mat.matrix) ||
		   matrixw_height(a->mat.matrix)!=
		   matrixw_height(b->mat.matrix))
			r = FALSE;
		else {
			MatrixW *m1 = a->mat.matrix;
			MatrixW *m2 = b->mat.matrix;
			
			r = TRUE;

			for(i=0;i<matrixw_width(m1);i++) {
				for(j=0;j<matrixw_height(m1);j++) {
					ETree *t1,*t2;
					t1 = matrixw_index(m1,i,j);
					t2 = matrixw_index(m2,i,j);
					if(mpw_cmp(t1->val.value,
						   t2->val.value)!=0) {
						r = FALSE;
						break;
					}
				}
				if(!r) break;
			}
		}
	} else if(a->type == MATRIX_NODE) {
		MatrixW *m = a->mat.matrix;
		if(matrixw_width(m)>1 ||
		   matrixw_height(m)>1) {
			r = FALSE;
		} else {
			ETree *t = matrixw_index(m,0,0);
			if(t->type != VALUE_NODE) {
				(*errorout)(_("Cannot compare non value-only matrixes"));
				*error = TRUE;
				return 0;
			}
			r = mpw_cmp(t->val.value,b->val.value)==0;
		}
	} else if(b->type == MATRIX_NODE) {
		MatrixW *m = b->mat.matrix;
		if(matrixw_width(m)>1 ||
		   matrixw_height(m)>1) {
			r = FALSE;
		} else {
			ETree *t = matrixw_index(m,0,0);
			if(t->type != VALUE_NODE) {
				(*errorout)(_("Cannot compare non value-only matrixes"));
				*error = TRUE;
				return 0;
			}
			r = mpw_cmp(t->val.value,a->val.value)==0;
		}
	} else
		g_assert_not_reached();

	return r;
}

static int
cmpstring(ETree *a, ETree *b)
{
	int r=0;
	if(a->type == STRING_NODE &&
	   b->type == STRING_NODE) {
		r=strcmp(a->str.str,b->str.str);
	} else if(a->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,b);
		r = strcmp(a->str.str,gs->str);
		g_string_free(gs,TRUE);
	} else if(b->type == STRING_NODE) {
		GString *gs = g_string_new("");
		print_etree(gs,NULL,a);
		r = strcmp(b->str.str,gs->str);
		g_string_free(gs,TRUE);
	} else
		g_assert_not_reached();

	return r;
}

static ETree *
cmpstringop(ETree *n, ETree *arg[], gpointer f)
{
	int r;
	r = cmpstring(arg[0],arg[1]);
	freeargarr(arg,2);
	if(r>0) return makenum_ui(1);
	else if(r<0) return makenum_si(-1);
	return makenum_ui(0);
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

static ETree * evaloper(ETree *n, int argeval, int do_ret);

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
		switch(oper) {
		case E_PLUS:
			mpw_add(res,arg[0]->val.value,arg[1]->val.value);
			break;
		case E_MINUS:
			mpw_sub(res,arg[0]->val.value,arg[1]->val.value);
			break;
		case E_MUL:
			mpw_mul(res,arg[0]->val.value,arg[1]->val.value);
			break;
		case E_DIV:
			mpw_div(res,arg[0]->val.value,arg[1]->val.value);
			break;
		case E_MOD:
			mpw_mod(res,arg[0]->val.value,arg[1]->val.value);
			break;
		default: g_assert_not_reached();
		}
		if(error_num==NUMERICAL_MPW_ERROR) {
			GET_NEW_NODE(n);
			n->type = OPERATOR_NODE;
			n->op.oper = oper;
			n->op.args = g_list_append(NULL,copynode(arg[0]));
			n->op.args = g_list_append(n->op.args,copynode(arg[1]));
			n->op.nargs = 2;
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
		n->op.oper = oper;

		n->op.args = g_list_append(NULL,arg[0]);
		n->op.args = g_list_append(n->op.args,arg[1]);
		n->op.nargs = 2;

		/*eval operator without re-evaluating arguments*/
		r = evaloper(n,FALSE,TRUE);

		/*make sure this doesn't free the arguments, so
		  this has to be freenode not freetree*/
		g_list_free(n->op.args);
		n->op.args = NULL;
		freenode(n);

		return r;
	default:
		(*errorout)(_("Primitives must get numeric/matrix/string arguments"));
		GET_NEW_NODE(n);
		n->type = OPERATOR_NODE;
		n->op.oper = oper;
		n->op.args = g_list_append(NULL,copynode(arg[0]));
		n->op.args = g_list_append(n->op.args,copynode(arg[1]));
		n->op.nargs = 2;
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
	int quote = 0;
	if(arg[0]->type == MATRIX_NODE) {
		m = arg[0]->mat.matrix;
		quote = arg[0]->mat.quoted;
		node = arg[1];
	} else {
		order = 1;
		m = arg[1]->mat.matrix;
		quote = arg[1]->mat.quoted;
		node = arg[0];
	}

	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->mat.matrix = matrixw_new();
	nn->mat.quoted = quote;
	matrixw_set_size(nn->mat.matrix,matrixw_width(m),matrixw_height(m));
	for(i=0;i<matrixw_width(m);i++) {
		for(j=0;j<matrixw_height(m);j++) {
			ETree *t;
			if(order == 0) {
				t = op_two_nodes(matrixw_index(m,i,j),
						 node, n->op.oper);
			} else {
				t = op_two_nodes(node,
						 matrixw_index(m,i,j),
						 n->op.oper);
			}
			/*exception*/
			if(!t) {
				freeargarr(arg,2);
				freetree(nn);
				return NULL;
			}
			matrixw_set_index(nn->mat.matrix,i,j) = t;
		}
	}
	freeargarr(arg,2);
	return nn;
}

static ETree *
matrix_addsub_op(ETree *n, ETree *arg[], gpointer f)
{
	if(arg[0]->type == MATRIX_NODE &&
	   arg[1]->type == MATRIX_NODE) {
		int i,j;
		ETree *nn;
		MatrixW *m1,*m2;
		m1 = arg[0]->mat.matrix;
		m2 = arg[1]->mat.matrix;
		if((matrixw_width(m1) != matrixw_width(m2)) ||
		   (matrixw_height(m1) != matrixw_height(m2))) {
			(*errorout)(_("Can't add/subtract two matricies of different sizes"));
			return copynode_args(n,arg);
		}
		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->mat.matrix = matrixw_new();
		nn->mat.quoted = arg[0]->mat.quoted || arg[1]->mat.quoted;
		matrixw_set_size(nn->mat.matrix,matrixw_width(m1),matrixw_height(m1));
		for(i=0;i<matrixw_width(m1);i++) {
			for(j=0;j<matrixw_height(m1);j++) {
				ETree *t = op_two_nodes(matrixw_index(m1,i,j),
							matrixw_index(m2,i,j),
							n->op.oper);
				/*exception*/
				if(!t) {
					freeargarr(arg,2);
					freetree(nn);
					return NULL;
				}
				matrixw_set_index(nn->mat.matrix,i,j) = t;
			}
		}
		freeargarr(arg,2);
		return nn;
	} else {
		return matrix_scalar_matrix_op(n,arg);
	}
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
matrix_mul_op(ETree *n, ETree *arg[],gpointer f)
{
	int i,j;
	ETree *nn;
	if(arg[0]->type == MATRIX_NODE &&
	   arg[1]->type == MATRIX_NODE) {
		int i,j;
		ETree *nn;
		MatrixW *m1,*m2;
		m1 = arg[0]->mat.matrix;
		m2 = arg[1]->mat.matrix;
		if((matrixw_width(m1) != matrixw_height(m2))) {
			(*errorout)(_("Can't multiply matricies of wrong sizes"));
			return copynode_args(n,arg);
		}
		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->mat.matrix = matrixw_new();
		nn->mat.quoted = arg[0]->mat.quoted || arg[1]->mat.quoted;
		matrixw_set_size(nn->mat.matrix,matrixw_width(m2),matrixw_height(m1));
		
		if(is_matrix_value_only(m1) &&
		   is_matrix_value_only(m2)) {
			value_matrix_multiply(nn->mat.matrix,m1,m2);
		} else {
			if(!expensive_matrix_multiply(nn->mat.matrix,m1,m2)) {
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

static ETree *
matrix_pow_op(ETree *n, ETree *arg[],gpointer f)
{
	int i,j;
	long power;
	ETree *nn;
	MatrixW *res = NULL;
	MatrixW *m;
	int free_m = FALSE;

	if(arg[1]->type != VALUE_NODE ||
	   mpw_is_complex(arg[1]->val.value) ||
	   !mpw_is_integer(arg[1]->val.value) ||
	   (matrixw_width(arg[0]->mat.matrix) !=
	    matrixw_height(arg[0]->mat.matrix)) ||
	   !is_matrix_value_only(arg[0]->mat.matrix)) {
		(*errorout)(_("Powers are defined on (square matrix)^(integer) only"));
		return copynode_args(n,arg);
	}
	
	m = arg[0]->mat.matrix;
	power = mpw_get_long(arg[1]->val.value);
	if(error_num) {
		error_num = 0;
		return copynode_args(n,arg);
	}
	
	if(power<=0) {
		MatrixW *mi;
		mi = matrixw_new();
		matrixw_set_size(mi,matrixw_width(m),
				 matrixw_height(m));
		for(i=0;i<matrixw_width(m);i++)
			for(j=0;j<matrixw_width(m);j++)
				if(i==j)
					matrixw_set_index(mi,i,j) =
						makenum_ui(1);
		if(power==0) {
			/*make us a new empty node*/
			GET_NEW_NODE(nn);
			nn->type = MATRIX_NODE;
			nn->mat.matrix = mi;
			nn->mat.quoted = arg[0]->mat.quoted;
			freeargarr(arg,2);
			return nn;
		}
		
		m = value_matrix_gauss(m,TRUE,FALSE,TRUE,NULL,mi);
		if(!m) {
			(*errorout)(_("Matrix appears singular and can't be inverted"));
			matrixw_free(mi);
			return copynode_args(n,arg);
		}
		matrixw_free(m);
		m = mi;
		free_m = TRUE;
	}
	
	power = power<0?-power:power;
	
	if(power==1) {
		/*make us a new empty node*/
		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		if(free_m)
			nn->mat.matrix = m;
		else
			nn->mat.matrix = matrixw_copy(m);
		nn->mat.quoted = arg[0]->mat.quoted;
		freeargarr(arg,2);
		return nn;
	}

	while(power>0) {
		/*if odd*/
		if(power & 0x1) {
			if(res) {
				MatrixW *ml = matrixw_new();
				matrixw_set_size(ml,matrixw_width(m),
						 matrixw_height(m));
				value_matrix_multiply(ml,res,m);
				matrixw_free(res);
				res = ml;
			} else
				res = matrixw_copy(m);
			power--;
		} else { /*even*/
			MatrixW *ml = matrixw_new();
			matrixw_set_size(ml,matrixw_width(m),
					 matrixw_height(m));
			value_matrix_multiply(ml,m,m);
			if(free_m)
				matrixw_free(m);
			m = ml;
			free_m = TRUE;

			power >>= 1; /*divide by two*/
		}
	}
	

	/*make us a new empty node*/
	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	if(!res) {
		if(free_m)
			nn->mat.matrix = m;
		else
			nn->mat.matrix = matrixw_copy(m);
	} else {
		nn->mat.matrix = res;
		if(free_m)
			matrixw_free(m);
	}
	nn->mat.quoted = arg[0]->mat.quoted;
	freeargarr(arg,2);
	return nn;
}

static ETree *
matrix_div_op(ETree *n, ETree *arg[],gpointer f)
{
	int i,j;
	ETree *nn;
	if(arg[0]->type == MATRIX_NODE &&
	   arg[1]->type == MATRIX_NODE) {
		int i,j;
		ETree *nn;
		MatrixW *m1,*m2;
		MatrixW *mi;
		MatrixW *res;

		m1 = arg[0]->mat.matrix;
		m2 = arg[1]->mat.matrix;

		if((matrixw_width(m1) !=
		    matrixw_height(m1)) ||
		   (matrixw_width(m2) !=
		    matrixw_height(m2)) ||
		   (matrixw_width(m1) !=
		    matrixw_width(m2)) ||
		   !is_matrix_value_only(m1) ||
		   !is_matrix_value_only(m2)) {
			(*errorout)(_("Can't divide matrices of different sizes or non-square matrices"));
			return copynode_args(n,arg);
		}

		mi = matrixw_new();
		matrixw_set_size(mi,matrixw_width(m1),
				 matrixw_height(m1));
		for(i=0;i<matrixw_width(m1);i++)
			for(j=0;j<matrixw_width(m1);j++)
				if(i==j)
					matrixw_set_index(mi,i,j) =
						makenum_ui(1);
		
		m2 = value_matrix_gauss(m2,TRUE,FALSE,TRUE,NULL,mi);
		if(!m2) {
			(*errorout)(_("Matrix appears singular and can't be inverted"));
			matrixw_free(mi);
			return copynode_args(n,arg);
		}
		matrixw_free(m2);
		m2 = mi;
		
		res = matrixw_new();
		matrixw_set_size(res,matrixw_width(m1),
				 matrixw_height(m1));
		value_matrix_multiply(res,m1,m2);
		matrixw_free(m2);

		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->mat.matrix = res;
		nn->mat.quoted = 0;
		freeargarr(arg,2);
		return nn;
	} else if(arg[0]->type == MATRIX_NODE) {
		return matrix_scalar_matrix_op(n,arg);
	} else /*2nd argument is matrix first must be value*/ {
		int i,j;
		ETree *nn;
		MatrixW *m;
		MatrixW *mi;
		MatrixW *res;

		m = arg[1]->mat.matrix;

		if((matrixw_width(m) !=
		    matrixw_height(m)) ||
		   !is_matrix_value_only(m)) {
			(*errorout)(_("Can't divide by a non-square matrix"));
			return copynode_args(n,arg);
		}

		mi = matrixw_new();
		matrixw_set_size(mi,matrixw_width(m),
				 matrixw_height(m));
		for(i=0;i<matrixw_width(m);i++)
			for(j=0;j<matrixw_width(m);j++)
				if(i==j)
					matrixw_set_index(mi,i,j) =
						makenum_ui(1);
		
		m = value_matrix_gauss(m,TRUE,FALSE,TRUE,NULL,mi);
		if(!m) {
			(*errorout)(_("Matrix appears singular and can't be inverted"));
			matrixw_free(mi);
			return copynode_args(n,arg);
		}
		matrixw_free(m);
		m = mi;
		
		for(i=0;i<matrixw_width(m);i++)
			for(j=0;j<matrixw_width(m);j++) {
				ETree *t = matrixw_set_index(m,i,j);
				if(t)
					mpw_mul(t->val.value,t->val.value,
						arg[0]->val.value);
			}

		GET_NEW_NODE(nn);
		nn->type = MATRIX_NODE;
		nn->mat.matrix = m;
		nn->mat.quoted = 0;
		freeargarr(arg,2);
		return nn;
	}
}

typedef void (*doubleopfunc)(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2);
typedef void (*singleopfunc)(mpw_ptr rop,mpw_ptr op);

static ETree *
matrix_1num_op(ETree *n, ETree *arg[], singleopfunc f)
{
	int i,j;
	ETree *nn;
	MatrixW *m;
	mpw_t tmp;
	m = arg[0]->mat.matrix;

	if(!is_matrix_value_only(m)) {
		(*errorout)(_("Can't operate on a non value-only matrix"));
		return copynode_args(n,arg);
	}
	GET_NEW_NODE(nn);
	nn->type = MATRIX_NODE;
	nn->mat.matrix = matrixw_new();
	nn->mat.quoted = 0;
	matrixw_set_size(nn->mat.matrix,matrixw_width(m),matrixw_height(m));
	mpw_init(tmp);
	for(i=0;i<matrixw_width(m);i++) {
		for(j=0;j<matrixw_height(m);j++) {
			(*f)(tmp,matrixw_index(m,i,j)->val.value);
			if(error_num==NUMERICAL_MPW_ERROR) {
				ETree *arg[1];
				error_num=NO_ERROR;
				arg[0] = copynode(matrixw_index(m,i,j));
				matrixw_set_index(nn->mat.matrix,i,j) =
					copynode_args(n,arg);
			} else if(mpw_sgn(tmp)!=0) {
				matrixw_set_index(nn->mat.matrix,i,j) =
					makenum_use(tmp);
				mpw_init(tmp);
			}
		}
	}
	mpw_clear(tmp);
	freeargarr(arg,1);
	return nn;
}

static ETree *
doubleop(ETree *n, ETree *arg[], doubleopfunc f)
{
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->val.value,arg[1]->val.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode_args(n,arg);
	}

	mpw_clear(arg[0]->val.value);
	memcpy(arg[0]->val.value,res,sizeof(struct _mpw_t));

	freetree(arg[1]);

	return arg[0];
}

static ETree *
singleop(ETree *n, ETree *arg[], singleopfunc f)
{
	mpw_t res;

	mpw_init(res);
	(*f)(res,arg[0]->val.value);
	if(error_num==NUMERICAL_MPW_ERROR) {
		mpw_clear(res);
		error_num=NO_ERROR;
		return copynode_args(n,arg);
	}
	mpw_clear(arg[0]->val.value);
	memcpy(arg[0]->val.value,res,sizeof(struct _mpw_t));

	return arg[0];
}


typedef ETree * (*primfunc_t)(ETree *,ETree **,gpointer);

#define EVAL_PRIMITIVE(n,numop,numfunc,matrixfunc,stringfunc,argeval) {\
	ETree *r[2]; 					\
	ETree *ret;					\
	if(argeval) {					\
		switch(evalargs(n->op.args,r,TRUE)) {	\
		case 1:					\
			return copynode_args(n,r);	\
		case 2:					\
			return NULL;			\
		}					\
	} else {					\
		r[0]=copynode(n->op.args->data);	\
		r[1]=n->op.args->next->data;		\
		if(r[1]) r[1]=copynode(r[1]);		\
	}						\
	switch(arglevel(r,n->op.nargs)) {		\
	case 0: ret = numop(n,r,numfunc);		\
		return ret;				\
	case 1: if(matrixfunc) {			\
			primfunc_t m = matrixfunc;	\
			ret = (*m)(n,r,numfunc);	\
			return ret;			\
		} else {				\
			(*errorout)(_("Primitive on matrixes undefined")); \
			return copynode_args(n,r);	\
		}					\
	case 2: if(stringfunc) {			\
			primfunc_t m = stringfunc;	\
			ret = (*m)(n,r,NULL);		\
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
	switch(evalargs(n->op.args,r,TRUE)) {
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
	
	r[0]->mat.matrix->tr = !(r[0]->mat.matrix->tr);
	
	return r[0];
}

static ETree *
get_element(ETree *n)
{
	ETree *ll,*rr,*m;
	ETree *ret = NULL;
	int li,ri; 

	if(n->op.oper==E_GET_ELEMENT) {
		if(!get_matrix_index(NULL,n,NULL,&m,&ll,&rr,TRUE,TRUE,
				     &li,&li,&ri,&ri,&ret))
			return ret;
	} else /*E_GET_VELEMENT*/ {
		if(!get_matrix_index(NULL,n,NULL,&m,&ll,&rr,TRUE,FALSE,
				     &li,&li,NULL,NULL,&ret))
			return ret;
	}

	if(m->type != MATRIX_NODE) {
		ETree *arg[3];
		(*errorout)(_("Index works only on matricies"));
		arg[0]=m;
		arg[1]=ll;
		arg[2]=rr;
		return copynode_args(n,arg);
	}

	if(n->op.oper == E_GET_VELEMENT) {
		ri = ((li-1)%matrixw_width(m->mat.matrix))+1;
		li = ((li-1)/matrixw_width(m->mat.matrix))+1;
	}
	
	if(matrixw_width(m->mat.matrix) < ri ||
	   matrixw_height(m->mat.matrix) < li) {
		ETree *arg[3];
		(*errorout)(_("Matrix index out of range"));
		arg[0]=m;
		arg[1]=ll;
		arg[2]=rr;
		return copynode_args(n,arg);
	}
	
	freetree(ll);
	freetree(rr);
	
	ret = copynode(matrixw_index(m->mat.matrix,ri-1,li-1));

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

	if(n->op.oper == E_GET_REGION) {
		do_col = TRUE;
		do_row = TRUE;
	} else if(n->op.oper == E_GET_ROW_REGION) {
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
		rowto = matrixw_height(m->mat.matrix);
	}
	if(!do_col) {
		colfrom = 1;
		colto = matrixw_width(m->mat.matrix);
	}

	w = colto-colfrom+1;
	h = rowto-rowfrom+1;

	if(colto > matrixw_width(m->mat.matrix) ||
	   rowto > matrixw_height(m->mat.matrix)) {
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

	ret->mat.matrix = matrixw_get_region(m->mat.matrix,
					     colfrom-1,rowfrom-1,w,h);
	ret->mat.quoted = m->mat.quoted;

	freetree(m);
	
	return ret;
}


/*note: eval args only applies to primitives*/
static ETree *
evaloper(ETree *n, int argeval, int do_ret)
{
	ETree *nn;
	switch(n->op.oper) {
		case E_SEPAR:
			if(!(nn=evalnode_full(n->op.args->data,FALSE)))
				return NULL; /*exception*/
			if(nn && nn!=EMPTY_RET) freetree(nn);
			return evalnode_full(n->op.args->next->data,do_ret);

		case E_EQUALS:
			return equalsop(n,do_ret);

		case E_ABS: EVAL_PRIMITIVE(n,singleop,mpw_abs,(primfunc_t)matrix_1num_op,NULL,argeval);
		case E_PLUS: EVAL_PRIMITIVE(n,doubleop,mpw_add,matrix_addsub_op,string_concat,argeval);
		case E_MINUS: EVAL_PRIMITIVE(n,doubleop,mpw_sub,matrix_addsub_op,NULL,argeval);
		case E_MUL: EVAL_PRIMITIVE(n,doubleop,mpw_mul,matrix_mul_op,NULL,argeval);
		case E_DIV: EVAL_PRIMITIVE(n,doubleop,mpw_div,matrix_div_op,NULL,argeval);
		case E_MOD: EVAL_PRIMITIVE(n,doubleop,mpw_mod,(primfunc_t)matrix_scalar_matrix_op,NULL,argeval);
		case E_NEG: EVAL_PRIMITIVE(n,singleop,mpw_neg,(primfunc_t)matrix_1num_op,NULL,argeval);
		case E_EXP: EVAL_PRIMITIVE(n,doubleop,mpw_pow,matrix_pow_op,NULL,argeval);
		case E_FACT: EVAL_PRIMITIVE(n,singleop,mpw_fac,(primfunc_t)matrix_1num_op,NULL,argeval);
		case E_TRANSPOSE: return transpose_matrix(n);

		/*these should have been translated to COMPARE_NODEs*/
		case E_EQ_CMP:
		case E_NE_CMP: g_assert_not_reached();

		case E_CMP_CMP: EVAL_PRIMITIVE(n,cmpcmpop,NULL,NULL,cmpstringop,argeval);
		/*these should have been translated to COMPARE_NODEs*/
		case E_LT_CMP:
		case E_GT_CMP:
		case E_LE_CMP: 
		case E_GE_CMP: g_assert_not_reached();

		case E_LOGICAL_AND: return logicalandop(n,argeval);
		case E_LOGICAL_OR: return logicalorop(n,argeval);
		case E_LOGICAL_XOR: EVAL_PRIMITIVE(n,logicalxorop,NULL,NULL,(primfunc_t)logicalxorop,argeval);
		case E_LOGICAL_NOT: EVAL_PRIMITIVE(n,logicalnotop,NULL,NULL,(primfunc_t)logicalnotop,argeval);

		case E_REGION_SEP:
			{
				ETree *arg[2];
				g_assert(n->op.args);
				g_assert(n->op.args->data);
				g_assert(n->op.args->next->data);
				arg[0] = evalnode(n->op.args->data);
				if(!arg[0]) return NULL;
				arg[1] = evalnode(n->op.args->next->data);
				if(!arg[1]) {
					freetree(arg[0]);
					return NULL;
				}
				return copynode_args(n,arg);
			}

		case E_GET_VELEMENT:
		case E_GET_ELEMENT:
			return get_element(n);

		case E_GET_REGION:
		case E_GET_ROW_REGION:
		case E_GET_COL_REGION:
			return get_region(n);

		case E_REFERENCE:
			if(do_ret)
				return copynode(n);
			else
				return EMPTY_RET;
		case E_DEREFERENCE:
			return derefvarop(n);

		case E_IF_CONS:
		case E_IFELSE_CONS:
			return ifop(n,do_ret);
		case E_WHILE_CONS:
		case E_UNTIL_CONS:
		case E_DOWHILE_CONS:
		case E_DOUNTIL_CONS:
			return loopop(n,do_ret);

		case E_FOR_CONS:
		case E_FORBY_CONS:
			return forloop(n,do_ret);

		case E_FORIN_CONS:
			return forinloop(n,do_ret);

		case E_DIRECTCALL:
			return funccallop(n,TRUE,do_ret);
		case E_CALL:
			return funccallop(n,FALSE,do_ret);
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

#define RET_ERR	\
	ETree *ret = copynode_args(n,r);	\
	error_num = 0;				\
	g_free(r);				\
	return ret;

#define RET_RES(x) \
	freeargarr(r,n->comp.nargs);	\
	g_free(r);			\
	return makenum_ui((x));

static ETree *
evalcomp(ETree *n)
{
	GList *li;
	ETree **r;
	ETree **ri;
	int oper;
	r = g_new(ETree *,n->comp.nargs);

	if(evalargs(n->comp.args,r,TRUE)==2)
		return NULL;
	
	for(ri=r,li=n->comp.comp;li;li=g_list_next(li),ri++) {
		int lev;
		oper = GPOINTER_TO_INT(li->data);
		lev = arglevel(ri,2);
		if(lev==0) {
			switch(oper) {
			case E_EQ_CMP:
				if(!eqlnodes(ri[0],ri[1])) {
					if(error_num) {
						RET_ERR
					}
					RET_RES(0)
				}
				break;
			case E_NE_CMP:
				if(eqlnodes(ri[0],ri[1])) {
					RET_RES(0)
				} else if(error_num) {
					RET_ERR
				}
				break;
			case E_LT_CMP:
				if(cmpnodes(ri[0],ri[1])>=0) {
					if(error_num) {
						RET_ERR
					}
					RET_RES(0)
				}
				break;
			case E_GT_CMP:
				if(cmpnodes(ri[0],ri[1])<=0) {
					if(error_num) {
						RET_ERR
					}
					RET_RES(0)
				}
				break;
			case E_LE_CMP:
				if(cmpnodes(ri[0],ri[1])>0) {
					RET_RES(0)
				} else if(error_num) {
					RET_ERR
				}
				break;
			case E_GE_CMP:
				if(cmpnodes(ri[0],ri[1])<0) {
					RET_RES(0)
				} else if(error_num) {
					RET_ERR
				}
				break;
			default:
				g_assert_not_reached();
			}
		} else if (lev==1) {
			int err = FALSE;
			switch(oper) {
			case E_EQ_CMP:
				if(!eqmatrix(ri[0],ri[1],&err)) {
					if(err) {
						RET_ERR
					}
					RET_RES(0)
				}
				break;
			case E_NE_CMP:
				if(eqmatrix(ri[0],ri[1],&err)) {
					RET_RES(0)
				} else if(err) {
					RET_ERR
				}
				break;
			default:
				(*errorout)(_("Cannot compare matrixes"));
				{
					RET_ERR
				}
			}
		} else if (lev==2) {
			switch(oper) {
			case E_EQ_CMP:
				if(!eqstring(ri[0],ri[1])) {
					RET_RES(0)
				}
				break;
			case E_NE_CMP:
				if(eqstring(ri[0],ri[1])) {
					RET_RES(0)
				}
				break;
			case E_LT_CMP:
				if(cmpstring(ri[0],ri[1])>=0) {
					RET_RES(0)
				}
				break;
			case E_GT_CMP:
				if(cmpstring(ri[0],ri[1])<=0) {
					RET_RES(0)
				}
				break;
			case E_LE_CMP:
				if(cmpstring(ri[0],ri[1])>0) {
					RET_RES(0)
				}
				break;
			case E_GE_CMP:
				if(cmpstring(ri[0],ri[1])<0) {
					RET_RES(0)
				}
				break;
			default:
				g_assert_not_reached();
			}
		} else {
			(*errorout)(_("Primitives must get numeric/matrix/string arguments"));
			{
				RET_ERR
			}
		}
	}
	RET_RES(1)
}

#undef RET_ERR
#undef RET_RES

ETree *
evalnode_full(ETree *n, int do_ret)
{
	if(evalnode_hook) {
		static int i = 0;
		if(i++>run_hook_every) {
			(*evalnode_hook)();
			i=0;
		}
	}
	if(interrupted)
		return NULL;
	if(!n)
		return NULL;
	
	switch(n->type) {
	case NULL_NODE:
	case VALUE_NODE:
		if(do_ret)
			return copynode(n);
		else
			return EMPTY_RET;
	case MATRIX_NODE:
		if(n->mat.quoted)
			return qevalmatrix(n);
		else
			return evalmatrix(n);
	case OPERATOR_NODE:
		return evaloper(n,TRUE,do_ret);
	case IDENTIFIER_NODE:
		return variableop(n);
	case STRING_NODE:
	case FUNCTION_NODE:
		if(do_ret)
			return copynode(n);
		else
			return EMPTY_RET;
	case COMPARISON_NODE:
		return evalcomp(n);
	default:
		(*errorout)(_("Unexpected node!"));
		if(do_ret)
			return copynode(n);
		else
			return EMPTY_RET;
	}
}

/*return TRUE if node is true (a number node !=0), false otherwise*/
int
isnodetrue(ETree *n, int *bad_node)
{
	if(n->type==STRING_NODE) {
		if(n->str.str && *n->str.str)
			return TRUE;
		else 
			return FALSE;
	}
	if(n->type!=VALUE_NODE) {
		if(bad_node) *bad_node = TRUE;
		return FALSE;
	}
	if(mpw_sgn(n->val.value)!=0)
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
		if(n->str.str && *n->str.str)
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
			if(n->str.str && *n->str.str)
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
		if(mpw_sgn(m->val.value)!=0)
			r = TRUE;
		else
			r = FALSE;
		freetree(m);
		return r;
	}
	if(mpw_sgn(n->val.value)!=0)
		return TRUE;
	else
		return FALSE;
}

ETree *
gather_comparisons(ETree *n)
{
	if(!n) return NULL;
	if(n->type == SPACER_NODE) {
		ETree *t = n->sp.arg;
		n->sp.arg = NULL;
		freenode(n);
		return gather_comparisons(t);
	} else if(n->type==OPERATOR_NODE) {
		ETree *nn;
		GList *li;
		switch(n->op.oper) {
		case E_EQ_CMP:
		case E_NE_CMP:
		case E_LT_CMP:
		case E_GT_CMP:
		case E_LE_CMP:
		case E_GE_CMP:
			GET_NEW_NODE(nn);
			nn->type = COMPARISON_NODE;
			nn->comp.nargs = 0;
			nn->comp.args = NULL;
			nn->comp.comp = NULL;
			
			for(;;) {
				ETree *t;
				nn->comp.args = g_list_append(nn->comp.args,gather_comparisons(n->op.args->data));
				nn->comp.nargs++;
				nn->comp.comp =
					g_list_append(nn->comp.comp,
						      GINT_TO_POINTER(n->op.oper));

				t = n->op.args->next->data;
				g_list_free(n->op.args);
				freenode(n);
				n = t;
				if(n->type != OPERATOR_NODE ||
				   (n->op.oper != E_EQ_CMP &&
				    n->op.oper != E_NE_CMP &&
				    n->op.oper != E_LT_CMP &&
				    n->op.oper != E_GT_CMP &&
				    n->op.oper != E_LE_CMP &&
				    n->op.oper != E_GE_CMP)) {
					nn->comp.args = g_list_append(nn->comp.args,gather_comparisons(n));
					nn->comp.nargs++;
					break;
				}
			}
			return nn;
		default:
			for(li=n->op.args;li;li=g_list_next(li))
				li->data = gather_comparisons(li->data);
			return n;
		}
	} else if(n->type==MATRIX_NODE) {
		int i,j;
		if(!n->mat.matrix) return n;
		for(i=0;i<matrixw_width(n->mat.matrix);i++) {
			for(j=0;j<matrixw_height(n->mat.matrix);j++) {
				ETree *t = matrixw_set_index(n->mat.matrix,i,j);
				if(t) {
					matrixw_set_index(n->mat.matrix,i,j) =
						gather_comparisons(t);
				}
			}
		}
		return n;
	} else if(n->type==FUNCTION_NODE) {
		if(n->func.func->type!=USER_FUNC ||
		   !n->func.func->data.user)
			return n;
		n->func.func->data.user =
			gather_comparisons(n->func.func->data.user);
		return n;
	} else
		return n;
}
