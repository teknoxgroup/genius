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

#ifndef _MATRIXW_H_
#define _MATRIXW_H_

#include <glib.h>
#include "structs.h"
#include "matrix.h"

/*the MatrixW typedef is in structs.h*/
struct _MatrixW {
	Matrix *m;
	int tr; /*transposed*/
	struct {
		int x;
		int y;
		int w;
		int h;
	} region;
};

/*new matrix*/
MatrixW *matrixw_new(void);
MatrixW *matrixw_new_with_matrix(Matrix *mat);

/*set size of a matrix*/
void matrixw_set_size(MatrixW *m, int width, int height);
/*set the size of the matrix to be at least this*/
void matrixw_set_at_least_size(MatrixW *m, int width, int height);

/*set element*/
void matrixw_set_element(MatrixW *m, int x, int y, gpointer data);

/*copy a matrix*/
MatrixW * matrixw_copy(MatrixW *source);

MatrixW * matrixw_get_region(MatrixW *m, int x, int y, int w, int h);
void matrixw_set_region(MatrixW *m, MatrixW *src,
			int srcx, int srcy, int destx, int desty,
			int w, int h);
void matrixw_set_region_etree(MatrixW *m, ETree *src,
			      int destx, int desty,
			      int w, int h);

/*transpose a matrix*/
MatrixW * matrixw_transpose(MatrixW *m);

/*free a matrix*/
void matrixw_free(MatrixW *m);

/*make private copy of the Matrix*/
void matrixw_make_private(MatrixW *m);

extern ETree *the_zero;

/*get the value at, make sure it's in the range*/
G_INLINE_FUNC ETree *matrixw_index(MatrixW *m, int x, int y);
#ifdef G_CAN_INLINE
G_INLINE_FUNC ETree *
matrixw_index(MatrixW *m, int x, int y) {
	ETree *t = matrix_index(m->m,m->region.x+(m->tr?y:x),m->region.y+(m->tr?x:y));
	return t?t:the_zero;
}
#endif

/*sort of unsafe, and only for setting, since we can
  get a NULL from this*/
#define matrixw_set_index(a,i,j) (matrix_index((a)->m,(a)->region.x+((a)->tr?(j):(i)),(a)->region.y+((a)->tr?(i):(j))))

#define matrixw_width(a) ((a)->tr?(a)->region.h:(a)->region.w)
#define matrixw_height(a) ((a)->tr?(a)->region.w:(a)->region.h)

#endif
