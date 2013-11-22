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

#include "config.h"

#include <glib.h>
#include <string.h>
#include "structs.h"
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "calc.h"
#include "matrix.h"
#include "matrixw.h"

#include "parseutil.h"

extern evalstack_t evalstack;
extern ETree *free_trees;

int
push_func(void)
{
	ETree * tree;
	ETree * val;
	GList * list = NULL;
	int i = 0;
	
	val = stack_pop(&evalstack);
	if(!val)
		return FALSE;


	for(;;) {
		tree = stack_pop(&evalstack);
		if(tree && tree->type==EXPRLIST_START_NODE) {
			freetree(tree);
			break;
		}
		/*we have gone all the way to the top and haven't found a
		  marker or tree is not an ident node*/
		if(!tree || tree->type != IDENTIFIER_NODE) {
			if(tree) freetree(tree);
			g_list_foreach(list,(GFunc)freetree,NULL);
			g_list_free(list); 
			return FALSE;
		}
		list = g_list_prepend(list,tree->id.id);
		freetree(tree);
		i++;
	}
	
	GET_NEW_NODE(tree);

	tree->type = FUNCTION_NODE;
	tree->func.func = d_makeufunc(NULL,val,list,i);
	tree->func.func->context = -1;

	stack_push(&evalstack,tree);

	return TRUE;
}

/*pops the last expression, pushes a marker
  entry and puts the last expression back*/
int
push_marker(ETreeType markertype)
{
	ETree * last_expr = stack_pop(&evalstack);
	ETree * tree;
	
	if(!last_expr)
		return FALSE;
       
	GET_NEW_NODE(tree);
	tree->type = markertype;
	stack_push(&evalstack,tree);
	stack_push(&evalstack,last_expr);
	return TRUE;
}

/*pushes a marker*/
void
push_marker_simple(ETreeType markertype)
{
	ETree *tree;
	GET_NEW_NODE(tree);
	tree->type = markertype;
	stack_push(&evalstack,tree);
}

/*puts a spacer into the tree, spacers are just useless nodes to be removed
  before evaluation, they just signify where there were parenthesis*/
int
push_spacer(void)
{
	ETree * last_expr = stack_pop(&evalstack);
	
	if(!last_expr)
		return FALSE;
	else if(last_expr->type == SPACER_NODE)
		stack_push(&evalstack,last_expr);
	else {
		ETree * tree;
		GET_NEW_NODE(tree);
		tree->type = SPACER_NODE;
		tree->sp.arg = last_expr;
		stack_push(&evalstack,tree);
	}
	return TRUE;
}
	
/*gather all expressions up until a row start marker and push the
  result as a MATRIX_ROW_NODE*/
int
push_matrix_row(void)
{
	ETree *tree;
	GList *row = NULL;
	int i=0;
	for(;;) {
		tree = stack_pop(&evalstack);
		/*we have gone all the way to the top and haven't found a
		  marker*/
		if(!tree) {
			g_list_foreach(row,(GFunc)freetree,NULL);
			g_list_free(row); 
			return FALSE;
		}
		if(tree->type==EXPRLIST_START_NODE) {
			freetree(tree);
			break;
		}
		row = g_list_prepend(row,tree);
		i++;
	}
	GET_NEW_NODE(tree);
	tree->type = MATRIX_ROW_NODE;
	tree->row.args = row;
	tree->row.nargs = i;

	stack_push(&evalstack,tree);
	
	return TRUE;
}
	
/*gather all expressions up until a row start marker and push the
  result as a matrix*/
int
push_matrix(int quoted)
{
	ETree *tree;
	int i,j;
	int cols,rows;
	GList *rowl = NULL;
	GList *lix,*liy;
	
	Matrix *matrix;
	
	rows=0;
	cols=0;
	for(;;) {
		tree = stack_pop(&evalstack);
		/*we have gone all the way to the top and haven't found a
		  marker*/
		if(!tree) {
			GList *li;
			for(li=rowl;li;li=g_list_next(li)) {
				g_list_foreach(li->data,(GFunc)freetree,
					       NULL);
				g_list_free(li->data); 
			}
			g_list_free(rowl);
			/**/g_warning("BAD MATRIX, NO START MARKER");
			return FALSE;
		} else if(tree->type==MATRIX_START_NODE) {
			freetree(tree);
			break;
		} else if(tree->type==MATRIX_ROW_NODE) {
			if(tree->row.nargs>cols)
				cols = tree->row.nargs;
			rowl = g_list_prepend(rowl,tree->row.args);
			tree->row.args = NULL;
			tree->row.nargs = 0;
			freetree(tree);
			rows++;
			continue;
		} else {
			GList *li;
			freetree(tree);
			for(li=rowl;li;li=g_list_next(li)) {
				g_list_foreach(li->data,(GFunc)freetree,
					       NULL);
				g_list_free(li->data); 
			}
			g_list_free(rowl);
			/**/g_warning("BAD MATRIX, A NON ROW ELEMENT FOUND");
			return FALSE;
		}
	}

	matrix = matrix_new();
	matrix_set_size(matrix, cols, rows);
	
	for(j=0,liy=rowl;liy;j++,liy=g_list_next(liy)) {
		for(i=0,lix=liy->data;lix;i++,lix=g_list_next(lix)) {
			matrix_index(matrix,i,j) = lix->data;
		}
		g_list_free(liy->data);
	}
	g_list_free(rowl);
	
	GET_NEW_NODE(tree);
	tree->type = MATRIX_NODE;
	tree->mat.matrix = matrixw_new_with_matrix(matrix);
	tree->mat.quoted = quoted?1:0;
	
	stack_push(&evalstack,tree);
	return TRUE;
}

/*pushes a NULL onto the stack, null cannot be evaluated, it will be
  read as ""*/
void
push_null(void)
{
	ETree *tree;
	GET_NEW_NODE(tree);
	tree->type = NULL_NODE;

	stack_push(&evalstack,tree);
}
