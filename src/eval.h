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

#ifndef _EVAL_H_
#define _EVAL_H_

#include "mpwrap.h"

/*declarations of structures*/
#include "structs.h"

typedef GList * evalstack_t;

/* builtin primitives */
enum {
	E_SEPAR = 1,
	E_EQUALS,
	E_ABS,
	E_PLUS,
	E_MINUS,
	E_MUL,
	E_DIV,
	E_MOD,
	E_NEG,
	E_EXP,
	E_FACT,
	E_TRANSPOSE,
	E_IF_CONS,
	E_IFELSE_CONS,
	E_WHILE_CONS,
	E_UNTIL_CONS,
	E_DOWHILE_CONS,
	E_DOUNTIL_CONS,
	E_EQ_CMP,
	E_NE_CMP,
	E_CMP_CMP,
	E_LT_CMP,
	E_GT_CMP,
	E_LE_CMP,
	E_GE_CMP,
	E_LOGICAL_AND,
	E_LOGICAL_OR,
	E_LOGICAL_XOR,
	E_LOGICAL_NOT,
	E_GET_ELEMENT,
	E_GET_ROW,
	E_GET_COLUMN,
	E_REFERENCE,
	E_DEREFERENCE,
	E_DIRECTCALL,
	E_CALL,
	E_RETURN,
	E_BAILOUT,
	E_EXCEPTION,
	E_CONTINUE,
	E_BREAK,
};

/*functions for manipulating a tree*/
ETree * makenum(mpw_t num);
ETree * makenum_use(mpw_t num); /*don't create a new number*/
ETree * makenum_ui(unsigned long num);
ETree * makenum_si(long num);
ETree * makenum_null(void);
ETree * makeoperator(int oper, evalstack_t * stack);

/*returns the number of args for an operator, or -1 if it takes up till
  exprlist marker -2 if it takes 1 past the marker for the first argument*/
int branches(int op);

/*copy a node*/
ETree * copynode(ETree *o);

/*copy node but use the args from r*/
ETree * copynode_args(ETree *o, ETree *r[]);

/*functions for reclaiming memory*/
void freetree(ETree *n);

/*evaluate a treenode, the treenode will become a number node*/
/*returns a newly allocated tree, doesn't hurt n*/
ETree *evalnode(ETree *n);

/*return TRUE if node is true (a number node !=0, or nonempty string),
  false otherwise*/
int isnodetrue(ETree *n, int *bad_node);
int eval_isnodetrue(ETree *n, int *exception, ETree **errorret);

#define GET_LRR(n,l,r,rr) { l = n->args->data; \
	r = n->args->next->data; \
	rr = n->args->next->next->data; \
}
#define GET_LR(n,l,r) { l = n->args->data; r = n->args->next->data; }
#define GET_L(n,l) { l = n->args->data; }

#define GET_NEW_NODE(n) {				\
	if(!free_trees)					\
		n = g_new(ETree,1);			\
	else {						\
		n = free_trees;				\
		free_trees = free_trees->data.next;	\
	}						\
}


#endif
