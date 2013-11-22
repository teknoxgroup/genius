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
#include "structs.h"
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "calc.h"

extern evalstack_t evalstack;

extern int return_ret; /*should the lexer return on \n*/
extern calc_error_t error_num;
extern int got_eof;
extern ETree *free_trees;
extern EFunc *free_funcs;

#define SYNTAX_ERROR {yyerror("syntax error"); YYERROR;}

#define PUSH_ACT(ACT) { \
	ETree *tree = makeoperator(ACT,&evalstack); \
	if(!tree) {SYNTAX_ERROR;} \
	stack_push(&evalstack,tree); \
}

#define PUSH_IDENTIFIER(ID) { \
	ETree * tree; \
	GET_NEW_NODE(tree); \
	tree->type = IDENTIFIER_NODE; \
	tree->data.id = d_intern(ID); \
	tree->args = NULL; \
	tree->nargs = 0; \
	stack_push(&evalstack,tree); \
}

#define PUSH_STRING(ID) { \
	ETree * tree; \
	GET_NEW_NODE(tree); \
	tree->type = STRING_NODE; \
	tree->data.str = g_strdup(ID); \
	tree->args = NULL; \
	tree->nargs = 0; \
	stack_push(&evalstack,tree); \
}

static int
push_func(void)
{
	ETree * tree;
	EFunc * func;
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
		list = g_list_prepend(list,tree->data.id);
		freetree(tree);
		i++;
	}
	
	if(!free_funcs)
		func = g_new0(EFunc,1);
	else {
		func = free_funcs;
		free_funcs = free_funcs->data.next;
		memset(func,0,sizeof(EFunc));
	}
	
	func->type = USER_FUNC;
	func->context = -1;
	func->nargs = i;
	func->named_args = list;
	func->data.user = val;

	GET_NEW_NODE(tree);

	tree->type = FUNCTION_NODE;
	tree->data.func = func;
	tree->args = NULL;
	tree->nargs = 0;

	stack_push(&evalstack,tree);

	return TRUE;
}

/*pops the last expression, pushes a marker
  entry and puts the last expression back*/
static int
push_marker(ETreeType markertype)
{
	ETree * last_expr = stack_pop(&evalstack);
	ETree * tree;
	
	if(!last_expr)
		return FALSE;
       
	GET_NEW_NODE(tree);
	tree->type = markertype;
	tree->args = NULL;
	tree->nargs = 0;
	stack_push(&evalstack,tree);
	stack_push(&evalstack,last_expr);
	return TRUE;
}

/*pushes a marker*/
static void
push_marker_simple(ETreeType markertype)
{
	ETree *tree;
	GET_NEW_NODE(tree);
	tree->type = markertype;
	tree->args = NULL;
	tree->nargs = 0;
	stack_push(&evalstack,tree);
}
	
/*gather all expressions up until a row start marker and push the
  result as a MATRIX_ROW_NODE*/
static int
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
	tree->args = row;
	tree->nargs = i;

	stack_push(&evalstack,tree);
}
	
/*gather all expressions up until a row start marker and push the
  result as a matrix*/
static int
push_matrix(void)
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
			if(tree->nargs>cols)
				cols = tree->nargs;
			rowl = g_list_prepend(rowl,tree->args);
			tree->args = NULL;
			tree->nargs = 0;
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
	tree->data.matrix = matrix;
	tree->args = NULL;
	tree->nargs = 0;
	
	stack_push(&evalstack,tree);
	return TRUE;
}

/*pushes a NULL onto the stack, null cannot be evaluated, it will be
  read as ""*/
static void
push_null(void)
{
	ETree *tree;
	GET_NEW_NODE(tree);
	tree->type = NULL_NODE;
	tree->args = NULL;
	tree->nargs = 0;

	stack_push(&evalstack,tree);
}

%}

%union {
	mpw_t val;
	char *id;
}

%token STARTTOK

%token <val> NUMBER
%token <id> STRING
%token <id> FUNCID

%token DEFINE CALL

%token RETURNTOK BAILOUT EXCEPTION

%token WHILE UNTIL DO IF THEN ELSE

%token AT

%token SEPAR EQUALS

%token EQ_CMP NE_CMP CMP_CMP LT_CMP GT_CMP LE_CMP GE_CMP

%token LOGICAL_XOR LOGICAL_OR LOGICAL_AND LOGICAL_NOT


%left SEPAR

%nonassoc LOWER_THEN_ELSE
%nonassoc WHILE UNTIL DO IF THEN ELSE DEFINE CALL RETURNTOK

%left LOGICAL_XOR LOGICAL_OR
%left LOGICAL_AND
%right LOGICAL_NOT

%nonassoc EQ_CMP NE_CMP CMP_CMP
%nonassoc LT_CMP GT_CMP LE_CMP GE_CMP

%right EQUALS
%left '+' '-'
%left '*' '/' '%'

%right '\''
%right '!'
%right '^'
%right UMINUS

%left AT

%%

fullexpr:	STARTTOK expr '\n' { YYACCEPT; }
	|	STARTTOK '\n' { YYACCEPT; }
	|	STARTTOK expr SEPAR '\n' { push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; }
	|	error '\n' { return_ret = TRUE; yyclearin; YYABORT; }
	|	error { return_ret = TRUE; }
	;

expr:		expr SEPAR expr		{ PUSH_ACT(E_SEPAR); }
	|	expr EQUALS expr	{ PUSH_ACT(E_EQUALS); }
	|	'|' expr '|'		{ PUSH_ACT(E_ABS); }
	|	expr '+' expr		{ PUSH_ACT(E_PLUS); }
	|	expr '-' expr		{ PUSH_ACT(E_MINUS); }
	|	expr '*' expr		{ PUSH_ACT(E_MUL); }
	|	expr '/' expr		{ PUSH_ACT(E_DIV); }
	|	expr '%' expr		{ PUSH_ACT(E_MOD); }
	|	expr EQ_CMP expr	{ PUSH_ACT(E_EQ_CMP); }
	|	expr NE_CMP expr	{ PUSH_ACT(E_NE_CMP); }
	|	expr CMP_CMP expr	{ PUSH_ACT(E_CMP_CMP); }
	|	expr LT_CMP expr	{ PUSH_ACT(E_LT_CMP); }
	|	expr GT_CMP expr	{ PUSH_ACT(E_GT_CMP); }
	|	expr LE_CMP expr	{ PUSH_ACT(E_LE_CMP); }
	|	expr GE_CMP expr	{ PUSH_ACT(E_GE_CMP); }
	|	expr LOGICAL_AND expr	{ PUSH_ACT(E_LOGICAL_AND); }
	|	expr LOGICAL_OR expr	{ PUSH_ACT(E_LOGICAL_OR); }
	|	expr LOGICAL_XOR expr	{ PUSH_ACT(E_LOGICAL_XOR); }
	|	LOGICAL_NOT expr	{ PUSH_ACT(E_LOGICAL_NOT); }
	|	expr '!'		{ PUSH_ACT(E_FACT); }
	|	expr '\''		{ PUSH_ACT(E_TRANSPOSE); }
	|	'-' expr %prec UMINUS	{ PUSH_ACT(E_NEG); }
	| 	expr '^' expr		{ PUSH_ACT(E_EXP); }
	|	expr AT expr ',' expr ')'
					{ PUSH_ACT(E_GET_ELEMENT); }
	|	expr AT expr ',' ')'	{ PUSH_ACT(E_GET_ROW); }
	|	expr AT ',' expr ')'	{ PUSH_ACT(E_GET_COLUMN); }
	|	'[' matrixrows ']'	{ if(!push_matrix()) {SYNTAX_ERROR;} }
	|	WHILE expr DO expr	{ PUSH_ACT(E_WHILE_CONS); }
	|	UNTIL expr DO expr	{ PUSH_ACT(E_UNTIL_CONS); }
	|	DO expr WHILE expr	{ PUSH_ACT(E_DOWHILE_CONS); }
	|	DO expr UNTIL expr	{ PUSH_ACT(E_DOUNTIL_CONS); }
	|	IF expr THEN expr %prec LOWER_THEN_ELSE	{ PUSH_ACT(E_IF_CONS); }
	|	IF expr THEN expr ELSE expr { PUSH_ACT(E_IFELSE_CONS); }
	|	'(' expr ')'
	|	ident
	|	'&' ident		{ PUSH_ACT(E_REFERENCE); }
	|	deref
	|	ident '(' ')'		{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); }
	|	ident '(' exprlist ')'	{ PUSH_ACT(E_DIRECTCALL); }
	|	deref '(' ')'		{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); }
	|	deref '(' exprlist ')'	{ PUSH_ACT(E_CALL); }
	|	expr CALL '(' exprlist ')' { PUSH_ACT(E_CALL); }
	|	expr CALL '(' ')'	{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); }
	|	DEFINE ident funcdef	{ PUSH_ACT(E_EQUALS); }
	|	'`' funcdef
	|	RETURNTOK expr		{ PUSH_ACT(E_RETURN); }
	|	BAILOUT			{ PUSH_ACT(E_BAILOUT); }
	|	EXCEPTION		{ PUSH_ACT(E_EXCEPTION); }
	|	NUMBER			{ stack_push(&evalstack,makenum($1));
					  mpw_clear($1);
					}
	|	STRING			{ PUSH_STRING($<id>1); }
	|	'.'			{ push_null(); }
	;
	
deref:		'*' ident		{ PUSH_ACT(E_DEREFERENCE); }
	;
	
ident:		FUNCID			{ PUSH_IDENTIFIER($<id>1); }
	;
	
funcdef:	'(' identlist')' '{' expr '}'	{ if(!push_func()) {SYNTAX_ERROR;} }
	|	'(' ')' '{' expr '}'		{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;}
						  if(!push_func()) {SYNTAX_ERROR;} }
	;

identlist:	identlist ',' ident
	|	ident { if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
	;

exprlist:	exprlist ',' expr
	|	expr { if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
	;
	
matrixrows:	matrixrows ':' exprlist { if(!push_matrix_row()) {SYNTAX_ERROR;} }
	|	exprlist { if(!push_matrix_row()) {SYNTAX_ERROR;} if(!push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} }
	;
	
%%
