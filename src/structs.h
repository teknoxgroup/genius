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
typedef union _ETree ETree;
/*typedef struct _ETreeNull ETreeNull;*/
typedef struct _ETreeValue ETreeValue;
typedef struct _ETreeMatrix ETreeMatrix;
typedef struct _ETreeOperator ETreeOperator;
typedef struct _ETreeIdentifier ETreeIdentifier;
typedef struct _ETreeString ETreeString;
typedef struct _ETreeFunction ETreeFunction;
typedef struct _ETreeComparison ETreeComparison;
typedef struct _ETreeMatrixRow ETreeMatrixRow;
/*typedef struct _ETreeMatrixStart ETreeMatrixStart;*/
/*typedef struct _ETreeExprlistStart ETreeExprlistStart;*/
typedef struct _ETreeSpacer ETreeSpacer;
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
	/*SET_NODE,*/
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

/*struct _ETreeNull {
	ETreeType type;
};*/

struct _ETreeValue {
	ETreeType type;
	mpw_t value;
};

struct _ETreeMatrix {
	ETreeType type;
	MatrixW *matrix;
	guint quoted:1;
};

/*struct _ETreeSet {
	ETreeType type;
	GList *set;
	guint multiset:1;
};*/

struct _ETreeOperator {
	ETreeType type;
	int oper;
	int nargs;
	GList *args;
};

struct _ETreeIdentifier {
	ETreeType type;
	Token *id;
};

struct _ETreeString {
	ETreeType type;
	char *str;
};

struct _ETreeFunction {
	ETreeType type;
	EFunc *func; /*anon function*/
};

struct _ETreeComparison {
	ETreeType type;
	int nargs;
	GList *args;
	GList *comp;
};

struct _ETreeMatrixRow {
	ETreeType type;
	int nargs;
	GList *args;
};

/*struct _ETreeMatrixStart {
	ETreeType type;
};*/

/*struct _ETreeExprlistStart {
	ETreeType type;
};*/

struct _ETreeSpacer {
	ETreeType type;
	ETree *arg;
};

union _ETree {
	ETreeType type;
	ETree *next; /*for allocation purposes only*/
	/*ETreeNull null;*/
	ETreeValue val;
	ETreeMatrix mat;
	ETreeOperator op;
	ETreeIdentifier id;
	ETreeString str;
	ETreeFunction func;
	ETreeComparison comp;
	ETreeMatrixRow row;
	/*ETreeMatrixStart mats;*/
	/*ETreeExprlistStart exps;*/
	ETreeSpacer sp;
};


#endif
