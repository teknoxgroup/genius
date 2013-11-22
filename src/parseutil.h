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
#ifndef _PARSEUTIL_H_
#define _PARSEUTIL_H_

int push_func(void);

/*pops the last expression, pushes a marker
  entry and puts the last expression back*/
int push_marker(ETreeType markertype);

/*pushes a marker*/
void push_marker_simple(ETreeType markertype);

/*puts a spacer into the tree, spacers are just useless nodes to be removed
  before evaluation, they just signify where there were parenthesis*/
int push_spacer(void);

/*gather all expressions up until a row start marker and push the
  result as a MATRIX_ROW_NODE*/
int push_matrix_row(void);

/*gather all expressions up until a row start marker and push the
  result as a matrix*/
int push_matrix(int quoted);

/*pushes a NULL onto the stack, null cannot be evaluated, it will be
  read as ""*/
void push_null(void);

#define SYNTAX_ERROR {yyerror("syntax error"); YYERROR;}

#define PUSH_ACT(ACT) { \
	ETree *tree = makeoperator((ACT),&evalstack); \
	if(!tree) {SYNTAX_ERROR;} \
	stack_push(&evalstack,tree); \
}

#define PUSH_IDENTIFIER(ID) { \
	ETree * tree; \
	GET_NEW_NODE(tree); \
	tree->type = IDENTIFIER_NODE; \
	tree->id.id = d_intern(ID); \
	stack_push(&evalstack,tree); \
}

#define PUSH_STRING(ID) { \
	ETree * tree; \
	GET_NEW_NODE(tree); \
	tree->type = STRING_NODE; \
	tree->str.str = (ID); \
	stack_push(&evalstack,tree); \
}

#endif
