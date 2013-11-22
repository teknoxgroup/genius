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

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <glib.h>

typedef struct _Matrix Matrix;
struct _Matrix {
	int width;
	int height;

	GPtrArray *data;

	/*private data*/
	int realwidth;
	int fullsize;
};

/*make new matrix*/
Matrix * matrix_new(void);

/*set size of a matrix*/
void matrix_set_size(Matrix *matrix, int width, int height);

/*set element*/
void matrix_set_element(Matrix *matrix, int x, int y, gpointer data);

/*copy a matrix*/
void matrix_copy(Matrix *matrix, Matrix *source);

/*free a matrix*/
void matrix_free(Matrix *matrix);

/*get the value at*/
#define matrix_index(m,x,y) (g_str_array_index(m->data,x+y*m->realwidth))

#endif