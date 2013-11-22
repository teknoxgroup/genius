/* GnomENIUS Calculator
 * Copyright (C) 1999 the Free Software Foundation.
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

#include <glib.h>
#include <string.h>
#include "structs.h"
#include "eval.h"
#include "matrix.h"

/*implement the inline functions*/
#undef G_INLINE_FUNC
#undef G_CAN_INLINE
#define	G_INLINE_FUNC extern
#define	G_CAN_INLINE 1
#include "matrixw.h"

/* we cast matrix to this structure to stuff it onto
   the free matrix list, we could just cast it to a
   pointer but this gives the impression of being
   cleaner*/
typedef struct _MatrixWFreeList MatrixWFreeList;
struct _MatrixWFreeList {
	MatrixWFreeList *next;
};

static MatrixWFreeList *free_matrices = NULL;

ETree *the_zero = NULL;

/*make new matrix*/
MatrixW *
matrixw_new(void)
{
	MatrixW *m;
	if(!free_matrices)
		m = g_new(MatrixW,1);
	else {
		m = (MatrixW *)free_matrices;
		free_matrices = free_matrices->next;
	}
	
	m->m = matrix_new();
	m->m->use = 1;
	
	m->tr = FALSE;
	m->region.x = 0;
	m->region.y = 0;
	m->region.w = m->m->width;
	m->region.h = m->m->height;
	
	if(!the_zero)
		the_zero = makenum_ui(0);
	
	return m;
}
MatrixW *
matrixw_new_with_matrix(Matrix *mat)
{
	MatrixW *m;
	if(!free_matrices)
		m = g_new(MatrixW,1);
	else {
		m = (MatrixW *)free_matrices;
		free_matrices = free_matrices->next;
	}
	
	m->m = mat;
	m->m->use++;
	
	m->tr = FALSE;
	m->region.x = 0;
	m->region.y = 0;
	m->region.w = m->m->width;
	m->region.h = m->m->height;
	
	if(!the_zero)
		the_zero = makenum_ui(0);
	
	return m;
}

/*neww and newh do not actually guarantee that size,
  they just prevent useless copying*/
static void
make_us_a_copy(MatrixW *m, int neww, int newh)
{
	Matrix *old;
	int i,j;
	int w,h;

	if(m->m->use==1)
		return;
	
	old = m->m;
	
	m->m = matrix_new();
	m->m->use = 1;
	matrix_set_size(m->m,neww,newh);
	w = MIN(neww,m->region.w);
	h = MIN(newh,m->region.h);
	for(i=0;i<w;i++) {
		for(j=0;j<h;j++) {
			ETree *t = matrix_index(old,i+m->region.x,j+m->region.y);
			matrix_index(m->m,i,j) = copynode(t);
		}
	}
	m->region.x = m->region.y = 0;
	m->region.w = w;
	m->region.h = h;
	old->use--;
}

/*set size of a matrix*/
void
matrixw_set_size(MatrixW *m, int width, int height)
{
	g_return_if_fail(m != NULL);
	g_return_if_fail(width>=0);
	g_return_if_fail(height>=0);

	if(m->tr) {
		int tmp = width;
		width = height;
		height = tmp;
	}
	
	if(m->region.w >= width ||
	   m->region.h >= height) {
		/*if we're the sole owner, we'll have to zero out some things*/
		if(m->m->use==1) {
			int i,j;
			for(i=m->region.x+width;i<m->region.w;i++) {
				for(j=m->region.y;j<m->region.h;j++) {
					if(matrix_index(m->m,i,j)) {
						freetree(matrix_index(m->m,i,j));
						matrix_index(m->m,i,j)=NULL;
					}
				}
			}
			for(i=m->region.x;i<width;i++) {
				for(j=m->region.y+height;j<m->region.h;j++) {
					if(matrix_index(m->m,i,j)) {
						freetree(matrix_index(m->m,i,j));
						matrix_index(m->m,i,j)=NULL;
					}
				}
			}
		}
		m->region.w = width;
		m->region.h = height;
		return;
	} else if(m->m->use>1) {
		int i,j;
		Matrix *old = m->m;
		m->m = matrix_new();
		m->m->use = 1;
		matrix_set_size(m->m,width,height);
		for(i=0;i<width;i++) {
			for(j=0;j<height;j++) {
				int oi,oj;
				oi = i+m->region.x;
				oj = j+m->region.y;
				if(oi<m->region.w &&
				   oj<m->region.h) {
					ETree *t = matrix_index(old,oi,oj);
					matrix_index(m->m,i,j) = copynode(t);
				}
			}
		}
		m->region.x = m->region.y = 0;
		m->region.w = width;
		m->region.h = height;
		old->use--;
	} else {
		int i,j;
		for(i=0;i<m->m->width;i++) {
			for(j=0;j<m->m->height;j++) {
				int ni,nj;
				ni = i-m->region.x;
				nj = j-m->region.y;
				if(ni<0 || ni>=width ||
				   nj<0 || nj>=height) {
					ETree *t = matrix_index(m->m,i,j);
					if(t) {
						freetree(t);
						matrix_index(m->m,i,j) = NULL;
					}
				}
			}
		}
		matrix_set_size(m->m,m->region.x+width,m->region.y+height);
		m->region.w = width - m->region.x;
		m->region.h = height - m->region.y;
	}
}

/*set the size of the matrix to be at least this*/
void
matrixw_set_at_least_size(MatrixW *m, int width, int height)
{
	g_return_if_fail(m != NULL);
	g_return_if_fail(width>=0);
	g_return_if_fail(height>=0);

	if(m->tr) {
		int tmp = width;
		width = height;
		height = tmp;
	}
	
	if(width>m->region.w || height>m->region.h) {
		make_us_a_copy(m,MAX(width,m->region.w),MAX(height,m->region.h));
		matrix_set_at_least_size(m->m,m->region.x+width,m->region.y+height);
	}
}

/*set element*/
void
matrixw_set_element(MatrixW *m, int x, int y, gpointer data)
{
	ETree *t;
	g_return_if_fail(m != NULL);
	g_return_if_fail(x>=0);
	g_return_if_fail(y>=0);
	
	if(m->tr) {
		int tmp = x;
		x = y;
		y = tmp;
	}

	make_us_a_copy(m,MAX(m->region.w,x+1),MAX(m->region.h,y+1));
	if(x+1>m->region.w || y+1>m->region.h) {
		matrix_set_at_least_size(m->m,m->region.x+x+1,m->region.y+y+1);
		m->region.w = MAX(m->region.w,x+1);
		m->region.h = MAX(m->region.h,y+1);
	}
	
	t = matrix_index(m->m,m->region.x+x,m->region.y+y);
	if(t) freetree(t);
	matrix_index(m->m,m->region.x+x,m->region.y+y) = data;
}

/*make sure it's in range first!*/
MatrixW *
matrixw_get_region(MatrixW *m, int x, int y, int w, int h)
{
	MatrixW *new;
	
	g_return_val_if_fail(m != NULL,NULL);
	g_return_val_if_fail(x>=0,NULL);
	g_return_val_if_fail(y>=0,NULL);
	g_return_val_if_fail(w>=0,NULL);
	g_return_val_if_fail(h>=0,NULL);

	if(m->tr) {
		int tmp = x;
		x = y;
		y = tmp;
		tmp = w;
		w = h;
		h = tmp;
	}
	
	new = matrixw_copy(m);
	if(x+w>m->region.w || y+h>m->region.h) {
		make_us_a_copy(m,MAX(x+w,m->region.w),MAX(y+h,m->region.h));
		matrix_set_at_least_size(m->m,m->region.x+x+w,m->region.y+y+h);
	}
	
	new->region.x += x;
	new->region.y += y;
	new->region.w = w;
	new->region.h = h;

	return new;
}

void
matrixw_set_region(MatrixW *m, MatrixW *src,
		   int srcx, int srcy, int destx, int desty,
		   int w, int h)
{
	int i,j;

	g_return_if_fail(m != NULL);
	g_return_if_fail(src != NULL);
	g_return_if_fail(srcx>=0);
	g_return_if_fail(srcy>=0);
	g_return_if_fail(destx>=0);
	g_return_if_fail(desty>=0);
	g_return_if_fail(w>=0);
	g_return_if_fail(h>=0);

	if(m->tr) {
		int tmp = destx;
		destx = desty;
		desty = tmp;
		tmp = w;
		w = h;
		h = tmp;
	}
	if(src->tr) {
		int tmp = srcx;
		srcx = srcy;
		srcy = tmp;
	}
	make_us_a_copy(m,MAX(destx+w,m->region.w),MAX(desty+h,m->region.h));
	if(destx+w>m->region.w || desty+h>m->region.h) {
		matrix_set_at_least_size(m->m,m->region.x+destx+w,m->region.y+desty+h);
		m->region.w = MAX(m->region.w,destx+w);
		m->region.h = MAX(m->region.h,desty+h);
	}
	
	for(i=0;i<w;i++) {
		for(j=0;j<h;j++) {
			int si,sj;
			ETree *t = matrix_index(m->m,destx+m->region.x+i,desty+m->region.y+j);
			if(t) freetree(t);
			if(m->tr == src->tr) {
				si = i;
				sj = j;
			} else {
				si = j;
				sj = i;
			}

			if(si>=src->region.w ||
			   sj>=src->region.h) {
				matrix_index(m->m,destx+m->region.x+i,desty+m->region.y+j) = NULL;
			} else {
				matrix_index(m->m,destx+m->region.x+i,desty+m->region.y+j) = 
					copynode(matrix_index(src->m,srcx+src->region.x+si,srcy+src->region.y+sj));
			}
		}
	}
}

void
matrixw_set_region_etree(MatrixW *m, ETree *src,
			 int destx, int desty,
			 int w, int h)
{
	int i,j;

	g_return_if_fail(m != NULL);
	g_return_if_fail(src != NULL);
	g_return_if_fail(destx>=0);
	g_return_if_fail(desty>=0);
	g_return_if_fail(w>=0);
	g_return_if_fail(h>=0);

	if(m->tr) {
		int tmp = destx;
		destx = desty;
		desty = tmp;
		tmp = w;
		w = h;
		h = tmp;
	}
	make_us_a_copy(m,MAX(destx+w,m->region.w),MAX(desty+h,m->region.h));
	if(destx+w>m->region.w || desty+h>m->region.h) {
		matrix_set_at_least_size(m->m,m->region.x+destx+w,m->region.y+desty+h);
		m->region.w = MAX(m->region.w,destx+w);
		m->region.h = MAX(m->region.h,desty+h);
	}
	
	for(i=0;i<w;i++) {
		for(j=0;j<h;j++) {
			ETree *t = matrix_index(m->m,destx+m->region.x+i,desty+m->region.y+j);
			if(t) freetree(t);
			matrix_index(m->m,destx+m->region.x+i,desty+m->region.y+j) = copynode(src);
		}
	}
}

/*copy a matrix*/
MatrixW *
matrixw_copy(MatrixW *source)
{
	MatrixW *m;

	g_return_val_if_fail(source != NULL,NULL);

	if(!free_matrices)
		m = g_new(MatrixW,1);
	else {
		m = (MatrixW *)free_matrices;
		free_matrices = free_matrices->next;
	}
	memcpy(m,source,sizeof(MatrixW));
	m->m->use++;
	return m;
}

/*transpose a matrix*/
MatrixW *
matrixw_transpose(MatrixW *m)
{
	MatrixW *new;
	
	g_return_val_if_fail(m != NULL,NULL);
	
	new = matrixw_copy(m);
	new->tr = !new->tr;
	return new;
}

/*make private copy of the Matrix*/
void
matrixw_make_private(MatrixW *m)
{
	g_return_if_fail(m != NULL);

	if(m->m->use==1)
		return;

	make_us_a_copy(m,m->region.w,m->region.h);
}

/*free a matrix*/
void
matrixw_free(MatrixW *m)
{
	int i,j;
	MatrixWFreeList *mf;
	
	g_return_if_fail(m != NULL);

	if(m->m->use == 1) {
		for(i=0;i<m->m->width;i++) {
			for(j=0;j<m->m->height;j++) {
				ETree *t = matrix_index(m->m,i,j);
				if(t) freetree(t);
			}
		}
		matrix_free(m->m);
	} else
		m->m->use--;
	
	mf = (MatrixWFreeList *)m;
	
	mf->next = free_matrices;
	free_matrices = mf;
}
