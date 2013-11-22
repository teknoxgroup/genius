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

#include <glib.h>
#include "matrix.h"
#include <string.h>

/* we cast matrix to this structure to stuff it onto
   the free matrix list, we could just cast it to a
   pointer but this gives the impression of being
   cleaner*/
typedef struct _MatrixFreeList MatrixFreeList;
struct _MatrixFreeList {
	MatrixFreeList *next;
};

static MatrixFreeList *free_matrices = NULL;

/*make new matrix*/
Matrix *
matrix_new(void)
{
	Matrix *m;
	if(!free_matrices)
		m = g_new(Matrix,1);
	else {
		m = (Matrix *)free_matrices;
		free_matrices = free_matrices->next;
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
	GPtrArray *na;
	int i;

	g_return_if_fail(matrix != NULL);
	g_return_if_fail(width>0);
	g_return_if_fail(height>0);
	
	if(!matrix->data) {
		matrix->width = width;
		matrix->realwidth = width+10;
		matrix->height = height;
		matrix->fullsize=(width+10)*(height+10);

		matrix->data = g_ptr_array_new();
		g_ptr_array_set_size(matrix->data,matrix->fullsize);

		memset(matrix->data->pdata, 0,
		       (matrix->fullsize*sizeof(void *)));
		return;
	}
	
	if(width<=matrix->realwidth) {
		int newsize = matrix->realwidth*height;
		if(newsize>matrix->fullsize) {
			matrix->fullsize = newsize;
			g_ptr_array_set_size(matrix->data,matrix->fullsize);
		}
		if(width<matrix->width) {
			for(i=0;i<matrix->height;i++)
				memset(matrix->data->pdata+((matrix->realwidth*i)+width)*sizeof(void *),0,(matrix->width-width)*sizeof(void *));
		}
		if(height<matrix->height) {
			memset(matrix->data->pdata+(matrix->realwidth*height)*sizeof(void *),0,((matrix->realwidth*matrix->height)-(matrix->realwidth*height))*sizeof(void *));
		}
		matrix->width = width;
		matrix->height = height;
		return;
	}
	
	matrix->fullsize = (width+10)*(height+10);
	na = g_ptr_array_new();
	g_ptr_array_set_size(na,matrix->fullsize);
	memset(matrix->data->pdata,0,matrix->fullsize*sizeof(void *));
	
	for(i=0;i<matrix->height;i++) {
		memcpy(na->pdata+((width+10)*i)*sizeof(void *),matrix->data->pdata+(matrix->realwidth*i)*sizeof(void *),matrix->width*sizeof(void *));
	}
	
	matrix->realwidth = width+10;
	matrix->width = width;
	matrix->height = height;

	g_ptr_array_free(matrix->data,TRUE);
	
	matrix->data = na;
}

/*set the size of the matrix to be at least this*/
void
matrix_set_at_least_size(Matrix *matrix, int width, int height)
{
	g_return_if_fail(matrix != NULL);
	g_return_if_fail(width>=0);
	g_return_if_fail(height>=0);
	
	if(width>matrix->width || height>matrix->height)
		matrix_set_size(matrix,MAX(width,matrix->width),
				MAX(height,matrix->height));
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
Matrix *
matrix_copy(Matrix *source, ElementCopyFunc el_copy, gpointer func_data)
{
	Matrix *matrix;
	int i,j;

	g_return_if_fail(source != NULL);
	
	matrix = matrix_new();
	
	/*copy over the structure*/
	*matrix = *source;
	
	matrix->data = NULL;
	
	if(source->data==NULL)
		return matrix;

	/*make us a new matrix data array*/
	matrix_set_size(matrix,source->width,source->height);
	
	/*copy the data*/
	if(el_copy) {
		for(i=0;i<source->width;i++)
			for(j=0;j<source->height;j++) {
				gpointer data = matrix_index(source,i,j);
				if(data)
					matrix_index(matrix,i,j) =
						(*el_copy)(data, func_data);
			}
	} else {
		for(i=0;i<source->width;i++)
			for(j=0;j<source->height;j++)
				matrix_index(matrix,i,j) =
					matrix_index(source,i,j);
	}
	return matrix;
}

/*transpose a matrix*/
Matrix *
matrix_transpose(Matrix *matrix)
{
	int i,j;
	Matrix *new;

	g_return_if_fail(matrix != NULL);
	
	new = matrix_new();

	if(!matrix->data)
		return new;

	matrix_set_size(new,matrix->height,matrix->width);
	
	for(i=0;i<matrix->width;i++)
		for(j=0;j<matrix->height;j++)
			matrix_index(new,j,i) = matrix_index(matrix,i,j);

	return new;
}

/*run a GFunc for each non-null element*/
void
matrix_foreach(Matrix *matrix, GFunc func, gpointer func_data)
{
	int i,j;

	g_return_if_fail(matrix != NULL);
	g_return_if_fail(func != NULL);
	
	if(matrix->data==NULL)
		return;

	for(i=0;i<matrix->width;i++)
		for(j=0;j<matrix->height;j++) {
			gpointer data = matrix_index(matrix,i,j);
			if(data)
				(*func)(data,func_data);
		}
}

/*free a matrix*/
void
matrix_free(Matrix *matrix)
{
	gpointer *a;
	MatrixFreeList *mf;
	
	g_return_if_fail(matrix != NULL);
	
	mf = (MatrixFreeList *)matrix;
	
	if(matrix->data)
		g_ptr_array_free(matrix->data,TRUE);
	matrix->data = NULL;
	
	mf->next = free_matrices;
	free_matrices = mf;
}
