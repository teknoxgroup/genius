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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "mpwrap.h"

/*dictionary function structure*/
typedef enum {
	BUILTIN_FUNC=0, /*function internal to genius*/
	USER_FUNC, /*function that points to an ETree for evaluation*/
	VARIABLE_FUNC, /*function that points to an ETree result */
	REFERENCE_FUNC /*a function that points to some other EFunc*/
} EFuncType;

typedef struct _EFunc EFunc;
typedef struct _ETree ETree;
typedef struct _Token Token;

/*not defined here, but needed and we can't include matrixw.h, but
  matrixw.h includes structs.h*/
typedef struct _MatrixW MatrixW;

struct _Token {
	char *token;
	GList *refs;
};

struct _EFunc {
	Token *id;
	EFuncType type;
	int context; /*the context number this is used for .. if we pop this
		       context, we will destroy the function*/
	int nargs; /*number of arguments*/
	GList *named_args; /*names of arguments*/
	union {
		ETree *user;
		ETree *(*func)(ETree * *, int *); /*the integer is exception*/
		EFunc *ref;
		EFunc *next; /*this is for keeping a free list*/
	} data;
};

typedef enum {
	NULL_NODE,
	VALUE_NODE,
	MATRIX_NODE,
	OPERATOR_NODE,
	IDENTIFIER_NODE,
	STRING_NODE,
	FUNCTION_NODE, /*stores an anonymous function*/
	COMPARISON_NODE,
	
	/*marker nodes*/
	MATRIX_ROW_NODE,
	MATRIX_START_NODE,
	EXPRLIST_START_NODE,
	SPACER_NODE
} ETreeType;

struct _ETree {
	ETreeType type;
	union {
		mpw_t value;
		int oper;
		GList *comp;
		Token *id;
		char *str;
		EFunc *func; /*anon function*/
		MatrixW *matrix;
		ETree *next; /*this is for keeping a free list*/
	} data;
	int nargs;
	GList *args;
};

#endif
