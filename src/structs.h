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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "mpwrap.h"

struct _tree_t;

/*dictionary function structure*/
typedef enum {BUILTIN_FUNC,USER_FUNC} func_type_t;
typedef struct _func_t {
	func_type_t type;
	char *id;
	int nargs; /*number of args*/
	int context;
	int dynamic;
	union {
		struct {
			struct _tree_t *value;
			GList *dict;
		} user;
		struct _tree_t *(*func)(struct _tree_t * *);
		struct _func_t *next; /*this is for keeping a free list*/
	} data;
} func_t;

/*evaluation tree structure, also used in parsing stack*/

typedef enum {
	NULL_NODE,
	/*types that should be present after parse*/
	NUMBER_NODE,
	MATRIX_NODE,
	ACTION_NODE,
	/*intermediate types for parsing*/
	MATRIX_ROW_NODE,
	MATRIX_ROW_START_NODE,
	MATRIX_START_NODE
} node_type_t;
typedef enum {PRIMITIVE_TYPE,FUNCTION_TYPE} op_type_t;
typedef struct _tree_t {
	node_type_t type;
	union {
		mpw_t val;
		struct {
			op_type_t type;
			union {
				int primitive;
				func_t * func;
			} data;
		} action;
		struct {
			GList *list;
			int size;
		} matrixrow;
	} data;
	struct _tree_t *args[8];
	/*struct _tree_t *left;
	struct _tree_t *right;
	struct _tree_t *secondright;*/ /*rarely used, for example IFELSE*/
} tree_t;

#endif
