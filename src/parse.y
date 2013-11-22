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
%{
#include <glib.h>
#include <string.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "calc.h"

/*functions defined in lexer*/
void push_notation(notation_t not);
void pop_notation(void);
	
extern evalstack_t evalstack;

extern int return_ret; /*should the lexer return on \n*/
extern calc_error_t error_num;

#define PUSH_ACT(ACT) stack_push(&evalstack, makefuncb(ACT,&evalstack));
#define PUSH_FUNC(FUNC) stack_push(&evalstack,makefuncd(FUNC,&evalstack));
#define PUSH_FAKEFUNC(FUNC) stack_push(&evalstack,makefuncd0(FUNC));

/*pops the last expression, pushes a marker
  entry and puts the last expression back*/
static int
push_marker(node_type_t markertype)
{
	tree_t * last_expr = stack_pop(&evalstack);
	tree_t * tree;
	
	if(!last_expr)
		return FALSE;
       
	tree = g_new(tree_t,1);
	tree->type = markertype;
	memset(tree->args,0,sizeof(void *)*8);
	stack_push(&evalstack,tree);
	stack_push(&evalstack,last_expr);
	return TRUE;
}
	
/*gather all expressions up until a row start marker and push the
  result as a MATRIX_ROW_NODE*/
static int
push_matrix_row(void)
{
	tree_t *tree;
	GList *row = NULL;
	GList *row_last = NULL;
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
		if(tree->type==MATRIX_ROW_START_NODE) {
			freetree(tree);
			break;
		}
		if(!row) {
			row = g_list_append(NULL,tree);
			row_last = row;
		} else {
			row_last = g_list_append(row_last,tree);
			row_last = g_list_next(row_last);
		}
		i++;
	}
	tree = g_new(tree_t,1);
	tree->type = MATRIX_ROW_NODE;
	tree->data.matrixrow.list = row;
	tree->data.matrixrow.size = i;
	memset(tree->args,0,sizeof(void *)*8);

	stack_push(&evalstack,tree);
}
	
/*gather all expressions up until a row start marker and push the
  result as a matrix*/
static int
push_matrix(void)
{
	/*FIXME: add matrix code here*/
	return TRUE;
}

/*pushes a NULL onto the stack, null cannot be evaluated, it will be
  read as ""*/
static void
push_null(void)
{
	tree_t *tree;
	tree = g_new(tree_t,1);
	tree->type = NULL_NODE;
	memset(tree->args,0,sizeof(void *)*8);

	stack_push(&evalstack,tree);
}

%}

%union {
	mpw_t val;
	func_t *func;
	char *id;
}

%token <val> POSNUM NEGNUM

/* functions have to have the number fo arguments figured out at parse time
   ... how would I put the type in here without bison barfing ... */
%token FUNCID0 FUNCID1 FUNCID2 FUNCID3 FUNCID4
%token FUNCID5 FUNCID6 FUNCID7 FUNCID8
%token FUNCDEF

%token INLINE_INFIX INLINE_PREFIX INLINE_POSTFIX

%token BEGINBLOCK ENDBLOCK

%token WHILE_CONS IF_CONS IFELSE_CONS

%token INFIX_EXPR POSTFIX_EXPR PREFIX_EXPR NEG

%token SEPAR EQUALS

%token EQ_CMP NE_CMP CMP_CMP LT_CMP GT_CMP LE_CMP GE_CMP

%token LOGICAL_XOR LOGICAL_OR LOGICAL_AND LOGICAL_NOT


%left SEPAR

%nonassoc WHILE_CONS IF_CONS IFELSE_CONS

%left LOGICAL_XOR LOGICAL_OR
%left LOGICAL_AND
%right LOGICAL_NOT

%nonassoc EQ_CMP NE_CMP CMP_CMP
%nonassoc LT_CMP GT_CMP LE_CMP GE_CMP

%right EQUALS
%left '+' '-'
%left '*' '/' '%'

%right '!'
%right '^'
%right NEG UMINUS

%right '@'

%%

expr:		INFIX_EXPR inexpr '\n' { YYACCEPT; }
	|	POSTFIX_EXPR poexprlist '\n' { YYACCEPT; }
	|	PREFIX_EXPR prexprlist '\n' { YYACCEPT; }
	|	INFIX_EXPR '\n' { YYACCEPT; }
	|	POSTFIX_EXPR '\n' { YYACCEPT; }
	|	PREFIX_EXPR '\n' { YYACCEPT; }
	|	INFIX_EXPR { error_num = EOF_ERROR; YYACCEPT; }
	|	POSTFIX_EXPR { error_num = EOF_ERROR; YYACCEPT; }
	|	PREFIX_EXPR { error_num = EOF_ERROR; YYACCEPT; }
	|	error '\n' { return_ret = TRUE; yyclearin; YYABORT; }
	|	error { return_ret = TRUE; }
	;

inexpr:		inexpr SEPAR inexpr	{ PUSH_ACT(E_SEPAR); }
	|	inexpr EQUALS inexpr	{ PUSH_ACT(E_EQUALS); }
	|	inexpr '+' inexpr	{ PUSH_ACT(E_PLUS); }
	|	inexpr '-' inexpr	{ PUSH_ACT(E_MINUS); }
	|	inexpr '*' inexpr	{ PUSH_ACT(E_MUL); }
	|	inexpr '/' inexpr	{ PUSH_ACT(E_DIV); }
	|	inexpr '%' inexpr	{ PUSH_ACT(E_MOD); }
	|	inexpr EQ_CMP inexpr	{ PUSH_ACT(E_EQ_CMP); }
	|	inexpr NE_CMP inexpr	{ PUSH_ACT(E_NE_CMP); }
	|	inexpr CMP_CMP inexpr	{ PUSH_ACT(E_CMP_CMP); }
	|	inexpr LT_CMP inexpr	{ PUSH_ACT(E_LT_CMP); }
	|	inexpr GT_CMP inexpr	{ PUSH_ACT(E_GT_CMP); }
	|	inexpr LE_CMP inexpr	{ PUSH_ACT(E_LE_CMP); }
	|	inexpr GE_CMP inexpr	{ PUSH_ACT(E_GE_CMP); }
	|	inexpr LOGICAL_AND inexpr	{ PUSH_ACT(E_LOGICAL_AND); }
	|	inexpr LOGICAL_OR inexpr	{ PUSH_ACT(E_LOGICAL_OR); }
	|	inexpr LOGICAL_XOR inexpr	{ PUSH_ACT(E_LOGICAL_XOR); }
	|	LOGICAL_NOT inexpr	{ PUSH_ACT(E_LOGICAL_NOT); }
	|	inexpr '!'		{ PUSH_ACT(E_FACT); }
	| 	NEG inexpr 		{ PUSH_ACT(E_NEG); }
	|	'-' inexpr %prec UMINUS	{ PUSH_ACT(E_NEG); }
	| 	inexpr '^' inexpr	{ PUSH_ACT(E_EXP); }
	|	'@' inexpr '(' inexpr ',' inexpr ')'
					{ PUSH_ACT(E_GET_ELEMENT); }
	|	'@' inexpr '(' ',' inexpr ')'
					{ PUSH_ACT(E_GET_ROW); }
	|	'@' inexpr '(' inexpr ',' ')'
					{ PUSH_ACT(E_GET_COLUMN); }
	|	'@' inexpr '(' ',' ')'
	|	infunc			{ PUSH_FUNC($<func>1); }
	|	funcdef inblock		{ PUSH_ACT(E_FUNCDEF); }
	|	WHILE_CONS inexpr inparenth	{ PUSH_ACT(E_WHILE_CONS); }
	|	IF_CONS inexpr inparenth	{ PUSH_ACT(E_IF_CONS); }
	|	IFELSE_CONS inexpr inparenth inparenth
			{ PUSH_ACT(E_IFELSE_CONS); }
	|	inparenth
	|	'[' inmatrixrows ']'	{ if(!push_matrix()) YYERROR; }
	|	posnum
	|	'.'			{ push_null(); }
	|	INLINE_INFIX '(' inexpr ')'	{ pop_notation(); }
	|	INLINE_PREFIX '(' prexpr ')'	{ pop_notation(); }
	|	INLINE_POSTFIX '(' poexpr ')'	{ pop_notation(); }
	;
	
inmatrixrow:	inmatrixrow ',' inexpr
	|	inexpr { if(!push_marker(MATRIX_ROW_START_NODE)) YYERROR; }
	;

inmatrixrows:	inmatrixrows ':' inmatrixrow { if(!push_matrix_row()) YYERROR; }
	|	inmatrixrow { if(!push_marker(MATRIX_START_NODE)) YYERROR; }
	;

inparenth:	'(' inexpr ')'
	;

inblock:	BEGINBLOCK inexpr ENDBLOCK
	;
	
infunc:		FUNCID0
			{ $<func>$ = $<func>1 }
	|	FUNCID1 '(' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID2 '(' inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID3 '(' inexpr ',' inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID4 '(' inexpr ',' inexpr ',' inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID5 '(' inexpr ',' inexpr ',' inexpr ',' inexpr ','
			inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID6 '(' inexpr ',' inexpr ',' inexpr ',' inexpr ','
			inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID7 '(' inexpr ',' inexpr ',' inexpr ',' inexpr ','
			inexpr ',' inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	|	FUNCID8 '(' inexpr ',' inexpr ',' inexpr ',' inexpr ','
			inexpr ',' inexpr ',' inexpr ',' inexpr ')'
			{ $<func>$ = $<func>1 }
	;

poexprlist:	poexprlist SEPAR poexpr	{ PUSH_ACT(E_SEPAR); }
	|	poexpr
	;

poexpr:		poexpr poexpr EQUALS	{ PUSH_ACT(E_EQUALS); }
	|	poexpr poexpr '+'	{ PUSH_ACT(E_PLUS); }
	|	poexpr poexpr '-'	{ PUSH_ACT(E_MINUS); }
	|	poexpr poexpr '*'	{ PUSH_ACT(E_MUL); }
	|	poexpr poexpr '/'	{ PUSH_ACT(E_DIV); }
	|	poexpr poexpr '%'	{ PUSH_ACT(E_MOD); }
	|	poexpr poexpr EQ_CMP	{ PUSH_ACT(E_EQ_CMP); }
	|	poexpr poexpr NE_CMP	{ PUSH_ACT(E_NE_CMP); }
	|	poexpr poexpr CMP_CMP	{ PUSH_ACT(E_CMP_CMP); }
	|	poexpr poexpr LT_CMP	{ PUSH_ACT(E_LT_CMP); }
	|	poexpr poexpr GT_CMP	{ PUSH_ACT(E_GT_CMP); }
	|	poexpr poexpr LE_CMP	{ PUSH_ACT(E_LE_CMP); }
	|	poexpr poexpr GE_CMP	{ PUSH_ACT(E_GE_CMP); }
	|	poexpr poexpr LOGICAL_AND	{ PUSH_ACT(E_LOGICAL_AND); }
	|	poexpr poexpr LOGICAL_OR	{ PUSH_ACT(E_LOGICAL_OR); }
	|	poexpr poexpr LOGICAL_XOR	{ PUSH_ACT(E_LOGICAL_XOR); }
	|	poexpr LOGICAL_NOT	{ PUSH_ACT(E_LOGICAL_NOT); }
	|	poexpr NEG		{ PUSH_ACT(E_NEG); }
	|	poexpr '!'		{ PUSH_ACT(E_FACT); }
	|	poexpr poexpr '^'	{ PUSH_ACT(E_EXP); }
	|	'(' poexpr ',' poexpr ')' poexpr '@'
					{ PUSH_ACT(E_GET_ELEMENT); }
	|	'(' ',' poexpr ')' poexpr '@'
					{ PUSH_ACT(E_GET_ROW); }
	|	'(' poexpr ',' ')' poexpr '@'
					{ PUSH_ACT(E_GET_COLUMN); }
	|	'(' ',' ')' poexpr '@'
	|	pofunc			{ PUSH_FUNC($<func>1); }
	|	funcdef poblock		{ PUSH_ACT(E_FUNCDEF); }
	|	poexpr poexpr WHILE_CONS { PUSH_ACT(E_WHILE_CONS); }
	|	poexpr poexpr IF_CONS	{ PUSH_ACT(E_IF_CONS); }
	|	poexpr poexpr poexpr IFELSE_CONS { PUSH_ACT(E_IFELSE_CONS); }
	|	poparenth
	|	'[' pomatrixrows ']'	{ if(!push_matrix()) YYERROR; }
	|	num
	|	'.'			{ push_null(); }
	|	INLINE_INFIX '(' inexpr ')'	{ pop_notation(); }
	|	INLINE_PREFIX '(' prexpr ')'	{ pop_notation(); }
	|	INLINE_POSTFIX '(' poexpr ')'	{ pop_notation(); }
	;

pomatrixrow:	pomatrixrow ',' poexpr
	|	poexpr { if(!push_marker(MATRIX_ROW_START_NODE)) YYERROR; }
	;

pomatrixrows:	pomatrixrows ':' pomatrixrow { if(!push_matrix_row()) YYERROR; }
	|	pomatrixrow { if(!push_marker(MATRIX_START_NODE)) YYERROR; }
	;


poparenth:	'(' poexprlist ')'
	;


poblock:	BEGINBLOCK poexprlist ENDBLOCK
	;

pofunc:		FUNCID0
			{ $<func>$ = $<func>1 }
	|	poexpr FUNCID1
			{ $<func>$ = $<func>2 }
	|	poexpr poexpr FUNCID2
			{ $<func>$ = $<func>3 }
	|	poexpr poexpr poexpr FUNCID3
			{ $<func>$ = $<func>4 }
	|	poexpr poexpr poexpr poexpr FUNCID4
			{ $<func>$ = $<func>5 }
	|	poexpr poexpr poexpr poexpr poexpr FUNCID5
			{ $<func>$ = $<func>6 }
	|	poexpr poexpr poexpr poexpr poexpr poexpr FUNCID6
			{ $<func>$ = $<func>7 }
	|	poexpr poexpr poexpr poexpr poexpr poexpr poexpr FUNCID7
			{ $<func>$ = $<func>8 }
	|	poexpr poexpr poexpr poexpr poexpr poexpr poexpr poexpr FUNCID8
			{ $<func>$ = $<func>9 }
	;

prexprlist:	prexprlist SEPAR prexpr	{ PUSH_ACT(E_SEPAR); }
	|	prexpr
	;

prexpr:		EQUALS prexpr prexpr	{ PUSH_ACT(E_EQUALS); }
	|	'+' prexpr prexpr	{ PUSH_ACT(E_PLUS); }
	|	'-' prexpr prexpr	{ PUSH_ACT(E_MINUS); }
	|	'*' prexpr prexpr	{ PUSH_ACT(E_MUL); }
	|	'/' prexpr prexpr	{ PUSH_ACT(E_DIV); }
	|	'%' prexpr prexpr	{ PUSH_ACT(E_MOD); }
	|	EQ_CMP prexpr prexpr	{ PUSH_ACT(E_EQ_CMP); }
	|	NE_CMP prexpr prexpr	{ PUSH_ACT(E_NE_CMP); }
	|	CMP_CMP prexpr prexpr	{ PUSH_ACT(E_CMP_CMP); }
	|	LT_CMP prexpr prexpr	{ PUSH_ACT(E_LT_CMP); }
	|	GT_CMP prexpr prexpr	{ PUSH_ACT(E_GT_CMP); }
	|	LE_CMP prexpr prexpr	{ PUSH_ACT(E_LE_CMP); }
	|	GE_CMP prexpr prexpr	{ PUSH_ACT(E_GE_CMP); }
	|	LOGICAL_AND prexpr prexpr	{ PUSH_ACT(E_LOGICAL_AND); }
	|	LOGICAL_OR prexpr prexpr	{ PUSH_ACT(E_LOGICAL_OR); }
	|	LOGICAL_XOR prexpr prexpr	{ PUSH_ACT(E_LOGICAL_XOR); }
	|	LOGICAL_NOT prexpr prexpr	{ PUSH_ACT(E_LOGICAL_NOT); }
	|	NEG prexpr 		{ PUSH_ACT(E_NEG); }
	|	'!' prexpr		{ PUSH_ACT(E_FACT); }
	|	'^' prexpr prexpr	{ PUSH_ACT(E_EXP); }
	|	'@' prexpr '(' prexpr ',' prexpr ')'
					{ PUSH_ACT(E_GET_ELEMENT); }
	|	'@' prexpr '(' ',' prexpr ')'
					{ PUSH_ACT(E_GET_ROW); }
	|	'@' prexpr '(' prexpr ',' ')'
					{ PUSH_ACT(E_GET_COLUMN); }
	|	'@' prexpr '(' ',' ')'
	|	prfunc			{ PUSH_FUNC($<func>1); }
	|	funcdef prblock		{ PUSH_ACT(E_FUNCDEF); }
	|	WHILE_CONS prexpr prparenth	{ PUSH_ACT(E_WHILE_CONS); }
	|	IF_CONS prexpr prparenth	{ PUSH_ACT(E_IF_CONS); }
	|	IFELSE_CONS prexpr prparenth prparenth
			{ PUSH_ACT(E_IFELSE_CONS); }
	|	prparenth
	|	'[' prmatrixrows ']'	{ if(!push_matrix()) YYERROR; }
	|	num
	|	'.'			{ push_null(); }
	|	INLINE_INFIX '(' inexpr ')'	{ pop_notation(); }
	|	INLINE_PREFIX '(' prexpr ')'	{ pop_notation(); }
	|	INLINE_POSTFIX '(' poexpr ')'	{ pop_notation(); }
	;

prmatrixrow:	prmatrixrow ',' prexpr
	|	prexpr { if(!push_marker(MATRIX_ROW_START_NODE)) YYERROR; }
	;

prmatrixrows:	prmatrixrows ':' prmatrixrow { if(!push_matrix_row()) YYERROR; }
	|	prmatrixrow { if(!push_marker(MATRIX_START_NODE)) YYERROR; }
	;

prparenth:	'(' prexprlist ')'
	;

prblock:	BEGINBLOCK prexprlist ENDBLOCK
	;

prfunc: 	FUNCID0
			{ $<func>$ = $<func>1 }
	|	FUNCID1 prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID2 prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID3 prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID4 prexpr prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID5 prexpr prexpr prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID6 prexpr prexpr prexpr prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID7 prexpr prexpr prexpr prexpr prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	|	FUNCID8 prexpr prexpr prexpr prexpr prexpr prexpr prexpr prexpr
			{ $<func>$ = $<func>1 }
	;

funcdef:	FUNCDEF			{ PUSH_FAKEFUNC($<func>1); }
	;
	
num:		posnum
	|	negnum
	;

posnum:		POSNUM	{
				stack_push(&evalstack,makenum($1));
				mpw_clear($1);
			}
	;

negnum:		NEGNUM	{
				stack_push(&evalstack,makenum($1));
				mpw_clear($1);
			}
	;

%%
