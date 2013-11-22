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

extern calc_error_t error_num;
extern calcstate_t calcstate;

extern void (*errorout)(char *);

extern ETree *free_trees;

int
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

void
value_matrix_multiply(MatrixW *res, MatrixW *m1, MatrixW *m2)
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
				mpw_mul(tmp,l->val.value,r->val.value);
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

static void
swap_rows(MatrixW *m, int row, int row2)
{
	int i;
	if(row==row2) return;
	
	matrixw_make_private(m);
	
	for(i=0;i<matrixw_width(m);i++) {
		ETree *t = matrixw_set_index(m,i,row);
		matrixw_set_index(m,i,row) = matrixw_set_index(m,i,row2);
		matrixw_set_index(m,i,row2) = t;
	}
}

static void
div_row(MatrixW *m, int row, mpw_t div)
{
	int i;
	
	if(mpw_cmp_ui(div,1)==0)
		return;

	matrixw_make_private(m);
	
	for(i=0;i<matrixw_width(m);i++) {
		ETree *t = matrixw_set_index(m,i,row);
		if(t)
			mpw_div(t->val.value,t->val.value,div);
	}
}

static void
mul_sub_row(MatrixW *m, int row, mpw_t mul, int row2)
{
	int i;
	mpw_t tmp;
	
	matrixw_make_private(m);
	
	mpw_init(tmp);
	for(i=0;i<matrixw_width(m);i++) {
		ETree *t = matrixw_set_index(m,i,row);
		if(t) {
			ETree *t2 = matrixw_set_index(m,i,row2);
			mpw_mul(tmp,t->val.value,mul);
			if(!t2) {
				mpw_neg(tmp,tmp);
				matrixw_set_index(m,i,row2) = makenum_use(tmp);
				mpw_init(tmp);
			} else {
				mpw_sub(t2->val.value,t2->val.value,tmp);
			}
		}
	}
	mpw_clear(tmp);
}

/*NOTE: if simul is passed then we assume that it's the same size as m*/
MatrixW *
value_matrix_gauss(MatrixW *m, int reduce, int uppertriang, int stopsing, mpw_ptr detop, MatrixW *simul)
{
	int i,j,d,ii;
	MatrixW *ret = matrixw_copy(m);
	ETree *piv;
	mpw_t tmp;
	
	if(detop)
		mpw_set_ui(detop,1);

	mpw_init(tmp);
	d = 0;
	for(i=0;i<matrixw_width(ret) && d<matrixw_height(ret);i++) {
		for(j=d;j<matrixw_height(ret);j++) {
			ETree *t = matrixw_set_index(ret,i,j);
			if(t && mpw_sgn(t->val.value)!=0)
				break;
		}
		if(j==matrixw_height(ret)) {
			if(stopsing) {
				matrixw_free(ret);
				mpw_clear(tmp);
				return NULL;
			}
			continue;
		} else if(j>d) {
			swap_rows(ret,j,d);
			if(simul)
				swap_rows(simul,j,d);
			if(detop)
				mpw_neg(detop,detop);
		}

		piv = matrixw_index(ret,i,d);
		matrixw_make_private(ret);
		
			
		for(j=d+1;j<matrixw_height(ret);j++) {
			ETree *t = matrixw_set_index(ret,i,j);
			if(t && mpw_sgn(t->val.value)!=0) {
				mpw_div(tmp,t->val.value,piv->val.value);
				mul_sub_row(ret,d,tmp,j);
				if(simul)
					mul_sub_row(simul,d,tmp,j);
			}
		}


		/*we just want to do an upper triangular matrix*/
		if(uppertriang) {
			d++;
			continue;
		}

		if(reduce) {
			for(j=0;j<d;j++) {
				ETree *t = matrixw_set_index(ret,i,j);
				if(t && mpw_sgn(t->val.value)!=0) {
					mpw_div(tmp,t->val.value,piv->val.value);
					mul_sub_row(ret,d,tmp,j);
					if(simul)
						mul_sub_row(simul,d,tmp,j);
				}
			}
		}

		for(ii=i+1;ii<matrixw_width(ret);ii++) {
			ETree *t = matrixw_set_index(ret,ii,d);
			if(t) {
				mpw_div(t->val.value,
					t->val.value,
					piv->val.value);
			}
		}
		if(detop)
			mpw_div(detop,detop,piv->val.value);
		if(simul)
			div_row(simul,d,piv->val.value);

		mpw_set_ui(piv->val.value,1);
		d++;
	}
	
	mpw_clear(tmp);
	return ret;
}


static void
det2(mpw_t rop, MatrixW *m)
{
	mpw_t tmp;
	mpw_init(tmp);
	mpw_mul(rop,matrixw_index(m,0,0)->val.value,
		matrixw_index(m,1,1)->val.value);
	mpw_mul(tmp,matrixw_index(m,1,0)->val.value,
		matrixw_index(m,0,1)->val.value);
	mpw_sub(rop,rop,tmp);
	mpw_clear(tmp);
}

static void
det3(mpw_t rop, MatrixW *m)
{
	mpw_t tmp;
	mpw_init(tmp);

	mpw_mul(rop,matrixw_index(m,0,0)->val.value,
		matrixw_index(m,1,1)->val.value);
	mpw_mul(rop,rop,
		matrixw_index(m,2,2)->val.value);

	mpw_mul(tmp,matrixw_index(m,1,0)->val.value,
		matrixw_index(m,2,1)->val.value);
	mpw_mul(tmp,tmp,
		matrixw_index(m,0,2)->val.value);
	mpw_add(rop,rop,tmp);

	mpw_mul(tmp,matrixw_index(m,2,0)->val.value,
		matrixw_index(m,0,1)->val.value);
	mpw_mul(tmp,tmp,
		matrixw_index(m,1,2)->val.value);
	mpw_add(rop,rop,tmp);

	mpw_mul(tmp,matrixw_index(m,2,0)->val.value,
		matrixw_index(m,1,1)->val.value);
	mpw_mul(tmp,tmp,
		matrixw_index(m,0,2)->val.value);
	mpw_sub(rop,rop,tmp);

	mpw_mul(tmp,matrixw_index(m,1,0)->val.value,
		matrixw_index(m,0,1)->val.value);
	mpw_mul(tmp,tmp,
		matrixw_index(m,2,2)->val.value);
	mpw_sub(rop,rop,tmp);

	mpw_mul(tmp,matrixw_index(m,0,0)->val.value,
		matrixw_index(m,2,1)->val.value);
	mpw_mul(tmp,tmp,
		matrixw_index(m,1,2)->val.value);
	mpw_sub(rop,rop,tmp);

	mpw_clear(tmp);
}

int
value_matrix_det(mpw_t rop, MatrixW *m)
{
	int w = matrixw_width(m);
        int h = matrixw_height(m);
	MatrixW *mm;
	mpw_t tmp;
	int i,j;

	if(w != h) {
		(*errorout)(_("Determinant of a non-square matrix is undefined"));
		return FALSE;
	}
	switch(w) {
	case 1:
		mpw_set(rop,matrixw_index(m,0,0)->val.value);
		break;
	case 2:
		det2(rop,m);
		break;
	case 3:
		det3(rop,m);
		break;
	default:
		mpw_init(tmp);
		mm = value_matrix_gauss(m,FALSE,TRUE,FALSE,tmp,NULL);
		mpw_mul(rop,tmp,matrixw_index(mm,0,0)->val.value);
		mpw_clear(tmp);
		for(i=1;i<matrixw_width(mm);i++) {
			ETree *t = matrixw_set_index(mm,i,i);
			if(!t) {
				matrixw_free(mm);
				mpw_set_ui(rop,0);
				return TRUE;
			}
			mpw_mul(rop,rop,t->val.value);
		}
		matrixw_free(mm);
		break;
	}
	return TRUE;
}
