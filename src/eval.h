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
	E_PLUS,
	E_MINUS,
	E_MUL,
	E_DIV,
	E_MOD,
	E_NEG,
	E_EXP,
	E_FACT,
	E_FUNCDEF,
	E_IF_CONS,
	E_IFELSE_CONS,
	E_WHILE_CONS,
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
	E_GET_COLUMN
};

/*functions for manipulating a tree*/
tree_t * makenum(mpw_t num);
tree_t * makenum_ui(unsigned long num);
tree_t * makenum_si(long num);
tree_t * makefuncb(int func, evalstack_t * stack);
tree_t * makefuncd(func_t * func, evalstack_t * stack);
/*same as above but used for declarations, it doesn'y pop any arguments of
  the stack and doesn't build an argument array*/
tree_t * makefuncd0(func_t * func);

/*returns 1 or 2 depending if the operation has one or two branches*/
int branches(int op);

/*similiar to the above, which branches should be evaluated*/
int brancheseval(int op);

/*functions for reclaiming memory*/
void freenode(tree_t *n);
void freetree(tree_t *n);
/*free arguments to a dictionary function*/
void freeargs(tree_t *n);

/*copy a node*/
tree_t * copynode(tree_t *o);

/*evaluate arguments of a function*/
tree_t * evalargs(tree_t *n);

/*evaluate a user function*/
tree_t * evaluserfunc(tree_t *n);

/*evaluate branches of a primitive, returns 0 if all ok, 1 if n should
  be returned or 2 on a serious error*/
int evalbranches(tree_t *n);

/*return TRUE if node is true (a number node !=0), false otherwise*/
int isnodetrue(tree_t *n);

/*evaluate a treenode, the treenode will become a number node*/
/*the tree will be freed*/
tree_t *evalnode(tree_t *n);

/*operations take two/one number nodes and return a number node*/
tree_t *plusop(tree_t *l,tree_t *r);
tree_t *minusop(tree_t *l,tree_t *r);
tree_t *mulop(tree_t *l,tree_t *r);
tree_t *divop(tree_t *l,tree_t *r);
tree_t *modop(tree_t *l,tree_t *r);
tree_t *negop(tree_t *l);
tree_t *expop(tree_t *l, tree_t *r);
tree_t *factop(tree_t *l);
tree_t *equalsop(tree_t *l, tree_t *r);
tree_t *funcdefop(tree_t *l, tree_t *r);
/*tree_t *ifconsop(tree_t *l, tree_t *r);
tree_t *ifelseconsop(tree_t *l, tree_t *r, tree_t *sr);
tree_t *whileconsop(tree_t *l, tree_t *r);*/
tree_t *eqcmpop(tree_t *l, tree_t *r);
tree_t *necmpop(tree_t *l, tree_t *r);
tree_t *cmpcmpop(tree_t *l, tree_t *r);
tree_t *ltcmpop(tree_t *l, tree_t *r);
tree_t *gtcmpop(tree_t *l, tree_t *r);
tree_t *lecmpop(tree_t *l, tree_t *r);
tree_t *gecmpop(tree_t *l, tree_t *r);
tree_t *logicalandop(tree_t *l, tree_t *r);
tree_t *logicalorop(tree_t *l, tree_t *r);
tree_t *logicalxorop(tree_t *l, tree_t *r);
tree_t *logicalnotop(tree_t *r);

/*compare nodes, return -1 if first one is smaller, 0 if they are
  equal, 1 if the first one is greater
  makes them the same type as a side effect*/
int cmpnodes(tree_t *l, tree_t *r);

#endif
