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

#include <glib.h>
#include "matrix.h"
#include <string.h>

static GList * free_matrices=NULL;

/*make new matrix*/
Matrix *
matrix_new(void)
{
	Matrix *m;
	if(!free_matrices)
		m = g_new(Matrix,1);
	else {
		m = free_matrices->data;
		free_matrices = g_list_remove(free_matrices,m);
	}

	m->width = 0;
	m->height = 0;
	
	m->data = NULL;
	
	m->realwidth = 0;
	m->fullsize = 0;
	
	return m;
}

/*set size of a matrix*/
void
matrix_set_size(Matrix *matrix, int width, int height)
{
	int newsize = width*height;

	g_return_if_fail(matrix != NULL);
	g_return_if_fail(width>0);
	g_return_if_fail(height>0);
	
	if(!matrix->data)
		matrix->data = g_ptr_array_new();
	
	g_ptr_array_set_size(matrix->data,newsize);
	
	if(matrix->fullsize<newsize)
		memset(matrix->data->pdata+(matrix->fullsize*sizeof(void *)),
		       0,
		       ((newsize-matrix->fullsize)*sizeof(void *)));
	matrix->fullsize = newsize;
}

/*set element*/
void
matrix_set_element(Matrix *matrix, int x, int y, gpointer data)
{
	g_return_if_fail(matrix != NULL);
	g_return_if_fail(x>=0);
	g_return_if_fail(y>=0);
	
	if(x>=matrix->width || y>=matrix->height)
		matrix_set_size(matrix,MAX(x+1,matrix->width),
				MAX(y+1,matrix->height));
	g_return_if_fail(matrix->data!=NULL);
	
	matrix->data->pdata[x+y*matrix->realwidth]=data;
}

/*copy a matrix*/
void
matrix_copy(Matrix *matrix, Matrix *source)
{
	g_return_if_fail(matrix != NULL);
	g_return_if_fail(source != NULL);
	
	/*FIXME: add this*/
}

/*free a matrix*/
void
matrix_free(Matrix *matrix)
{
	gpointer *a;
	
	g_return_if_fail(matrix != NULL);
	
	if(matrix->data)
		g_ptr_array_free(matrix->data,TRUE);
	matrix->data = NULL;
	free_matrices = g_list_prepend(free_matrices,matrix);
}
