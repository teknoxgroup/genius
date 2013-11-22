
/*  A Bison parser, made from parse.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	STARTTOK	258
#define	NUMBER	259
#define	STRING	260
#define	FUNCID	261
#define	DEFINE	262
#define	CALL	263
#define	RETURNTOK	264
#define	BAILOUT	265
#define	EXCEPTION	266
#define	WHILE	267
#define	UNTIL	268
#define	DO	269
#define	IF	270
#define	THEN	271
#define	ELSE	272
#define	AT	273
#define	SEPAR	274
#define	EQUALS	275
#define	EQ_CMP	276
#define	NE_CMP	277
#define	CMP_CMP	278
#define	LT_CMP	279
#define	GT_CMP	280
#define	LE_CMP	281
#define	GE_CMP	282
#define	LOGICAL_XOR	283
#define	LOGICAL_OR	284
#define	LOGICAL_AND	285
#define	LOGICAL_NOT	286
#define	LOWER_THEN_ELSE	287
#define	UMINUS	288

#line 21 "parse.y"

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


#line 282 "parse.y"
typedef union {
	mpw_t val;
	char *id;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		142
#define	YYFLAG		-32768
#define	YYNTBASE	55

#define YYTRANSLATE(x) ((unsigned)(x) <= 288 ? yytranslate[x] : 63)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    39,     2,     2,     2,    37,    49,    38,    48,
    45,    35,    33,    44,    34,    51,    36,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    54,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    46,     2,    47,    40,     2,    50,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    52,    43,    53,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    41
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     7,    12,    15,    17,    21,    25,    29,    33,
    37,    41,    45,    49,    53,    57,    61,    65,    69,    73,
    77,    81,    85,    89,    92,    95,    98,   101,   105,   112,
   118,   124,   128,   133,   138,   143,   148,   153,   160,   164,
   166,   169,   171,   175,   180,   184,   189,   195,   200,   204,
   207,   210,   212,   214,   216,   218,   220,   223,   225,   232,
   238,   242,   244,   248,   250,   254
};

static const short yyrhs[] = {     3,
    56,    42,     0,     3,    42,     0,     3,    56,    19,    42,
     0,     1,    42,     0,     1,     0,    56,    19,    56,     0,
    56,    20,    56,     0,    43,    56,    43,     0,    56,    33,
    56,     0,    56,    34,    56,     0,    56,    35,    56,     0,
    56,    36,    56,     0,    56,    37,    56,     0,    56,    21,
    56,     0,    56,    22,    56,     0,    56,    23,    56,     0,
    56,    24,    56,     0,    56,    25,    56,     0,    56,    26,
    56,     0,    56,    27,    56,     0,    56,    30,    56,     0,
    56,    29,    56,     0,    56,    28,    56,     0,    31,    56,
     0,    56,    39,     0,    56,    38,     0,    34,    56,     0,
    56,    40,    56,     0,    56,    18,    56,    44,    56,    45,
     0,    56,    18,    56,    44,    45,     0,    56,    18,    44,
    56,    45,     0,    46,    62,    47,     0,    12,    56,    14,
    56,     0,    13,    56,    14,    56,     0,    14,    56,    12,
    56,     0,    14,    56,    13,    56,     0,    15,    56,    16,
    56,     0,    15,    56,    16,    56,    17,    56,     0,    48,
    56,    45,     0,    58,     0,    49,    58,     0,    57,     0,
    58,    48,    45,     0,    58,    48,    61,    45,     0,    57,
    48,    45,     0,    57,    48,    61,    45,     0,    56,     8,
    48,    61,    45,     0,    56,     8,    48,    45,     0,     7,
    58,    59,     0,    50,    59,     0,     9,    56,     0,    10,
     0,    11,     0,     4,     0,     5,     0,    51,     0,    35,
    58,     0,     6,     0,    48,    60,    45,    52,    56,    53,
     0,    48,    45,    52,    56,    53,     0,    60,    44,    58,
     0,    58,     0,    61,    44,    56,     0,    56,     0,    62,
    54,    61,     0,    61,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   333,   334,   335,   336,   337,   340,   341,   342,   343,   344,
   345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
   355,   356,   357,   358,   359,   360,   361,   362,   363,   365,
   366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
   376,   377,   378,   380,   381,   383,   384,   385,   387,   388,
   389,   390,   391,   392,   395,   396,   399,   402,   405,   406,
   410,   411,   414,   415,   418,   419
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STARTTOK",
"NUMBER","STRING","FUNCID","DEFINE","CALL","RETURNTOK","BAILOUT","EXCEPTION",
"WHILE","UNTIL","DO","IF","THEN","ELSE","AT","SEPAR","EQUALS","EQ_CMP","NE_CMP",
"CMP_CMP","LT_CMP","GT_CMP","LE_CMP","GE_CMP","LOGICAL_XOR","LOGICAL_OR","LOGICAL_AND",
"LOGICAL_NOT","LOWER_THEN_ELSE","'+'","'-'","'*'","'/'","'%'","'''","'!'","'^'",
"UMINUS","'\\n'","'|'","','","')'","'['","']'","'('","'&'","'`'","'.'","'{'",
"'}'","':'","fullexpr","expr","deref","ident","funcdef","identlist","exprlist",
"matrixrows", NULL
};
#endif

static const short yyr1[] = {     0,
    55,    55,    55,    55,    55,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    57,    58,    59,    59,
    60,    60,    61,    61,    62,    62
};

static const short yyr2[] = {     0,
     3,     2,     4,     2,     1,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     2,     2,     2,     2,     3,     6,     5,
     5,     3,     4,     4,     4,     4,     4,     6,     3,     1,
     2,     1,     3,     4,     3,     4,     5,     4,     3,     2,
     2,     1,     1,     1,     1,     1,     2,     1,     6,     5,
     3,     1,     3,     1,     3,     1
};

static const short yydefact[] = {     0,
     5,     0,     4,    54,    55,    58,     0,     0,    52,    53,
     0,     0,     0,     0,     0,     0,     0,     2,     0,     0,
     0,     0,     0,    56,     0,    42,    40,     0,    51,     0,
     0,     0,     0,    24,    27,    57,     0,    64,    66,     0,
     0,    41,     0,    50,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    26,    25,     0,     1,     0,     0,    49,
     0,     0,     0,     0,     0,     0,     8,     0,    32,     0,
    39,     0,    62,     0,     0,     0,     0,     3,     6,     7,
    14,    15,    16,    17,    18,    19,    20,    23,    22,    21,
     9,    10,    11,    12,    13,    28,    45,     0,    43,     0,
    33,    34,    35,    36,    37,    63,    65,     0,     0,     0,
    48,     0,     0,     0,    46,    44,     0,     0,    61,     0,
    47,    31,    30,     0,    38,    60,     0,    29,    59,     0,
     0,     0
};

static const short yydefgoto[] = {   140,
    38,    26,    27,    44,    84,    39,    40
};

static const short yypact[] = {     1,
   -37,    84,-32768,-32768,-32768,-32768,    12,   423,-32768,-32768,
   423,   423,   423,   423,   423,   423,    12,-32768,   423,   423,
   423,    12,   -14,-32768,   694,   -13,    -9,   -14,   896,   760,
   793,   727,   826,   965,    24,-32768,   661,   849,    -7,   -16,
   529,-32768,    -5,-32768,    15,   135,   183,   423,   423,   423,
   423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
   423,   423,   423,-32768,-32768,   423,-32768,   231,   279,-32768,
   423,   423,   423,   423,   423,   423,-32768,   423,-32768,   423,
-32768,    28,-32768,   -29,   327,   423,   628,-32768,   919,    67,
   988,   988,   988,  1011,  1011,  1011,  1011,   942,   942,   965,
   -15,   -15,   -11,   -11,   -11,   -10,-32768,   -12,-32768,    23,
   896,   896,   896,   896,   873,   849,    -7,   423,    12,    29,
-32768,    34,   562,   375,-32768,-32768,   423,   457,-32768,   423,
-32768,-32768,-32768,   595,   896,-32768,   493,-32768,-32768,    43,
    75,-32768
};

static const short yypgoto[] = {-32768,
    -2,-32768,    19,    55,-32768,    -3,-32768
};


#define	YYLAST		1051


static const short yytable[] = {    25,
     6,     1,    46,     2,     3,    29,    46,    46,    30,    31,
    32,    33,    34,    35,   119,   120,    37,     6,    41,    61,
    62,    63,    64,    65,    66,    28,    64,    65,    66,    66,
    79,    78,   125,    43,    68,    36,    78,    80,    69,    82,
    42,    46,   141,    87,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
   105,    83,    85,   106,   108,   110,    78,   126,    89,   111,
   112,   113,   114,   115,   142,   116,   117,    78,   131,   118,
   130,   122,    70,   123,    46,     0,    48,     4,     5,     6,
     7,     0,     8,     9,    10,    11,    12,    13,    14,    59,
    60,    61,    62,    63,    64,    65,    66,     0,     0,     0,
     0,     0,     0,     0,    15,   128,     0,    16,    17,     0,
     0,   134,     0,     0,   135,    18,    19,   137,     0,    20,
     0,    21,    22,    23,    24,     0,     0,   129,     4,     5,
     6,     7,     0,     8,     9,    10,    11,    12,    13,    14,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    15,     0,     0,    16,    17,
     0,     0,     0,     0,     0,     0,     0,    19,    86,     0,
    20,     0,    21,    22,    23,    24,     4,     5,     6,     7,
     0,     8,     9,    10,    11,    12,    13,    14,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    15,     0,     0,    16,    17,     0,     0,
     0,     0,     0,     0,    88,    19,     0,     0,    20,     0,
    21,    22,    23,    24,     4,     5,     6,     7,     0,     8,
     9,    10,    11,    12,    13,    14,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    15,     0,     0,    16,    17,     0,     0,     0,     0,
     0,     0,     0,    19,     0,   107,    20,     0,    21,    22,
    23,    24,     4,     5,     6,     7,     0,     8,     9,    10,
    11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
     0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
     0,    19,     0,   109,    20,     0,    21,    22,    23,    24,
     4,     5,     6,     7,     0,     8,     9,    10,    11,    12,
    13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
    16,    17,     0,     0,     0,     0,     0,     0,     0,    19,
     0,   121,    20,     0,    21,    22,    23,    24,     4,     5,
     6,     7,     0,     8,     9,    10,    11,    12,    13,    14,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    15,     0,     0,    16,    17,
     0,     0,     0,     0,     0,     0,     0,    19,     0,   133,
    20,     0,    21,    22,    23,    24,     4,     5,     6,     7,
     0,     8,     9,    10,    11,    12,    13,    14,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    15,     0,     0,    16,    17,     0,     0,
     0,     0,     0,     0,    45,    19,     0,     0,    20,     0,
    21,    22,    23,    24,    46,    71,    48,    49,    50,    51,
    52,    53,    54,    55,    56,    57,    58,     0,     0,    59,
    60,    61,    62,    63,    64,    65,    66,     0,     0,     0,
    45,     0,     0,     0,     0,     0,     0,     0,     0,   136,
    46,    71,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,     0,     0,    59,    60,    61,    62,    63,
    64,    65,    66,     0,     0,     0,    45,     0,     0,     0,
     0,     0,     0,     0,     0,   139,    46,    71,    48,    49,
    50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
     0,    59,    60,    61,    62,    63,    64,    65,    66,    45,
     0,     0,     0,    81,     0,     0,     0,     0,     0,    46,
    71,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,     0,     0,    59,    60,    61,    62,    63,    64,
    65,    66,    45,     0,     0,     0,   132,     0,     0,     0,
     0,     0,    46,    71,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    57,    58,     0,     0,    59,    60,    61,
    62,    63,    64,    65,    66,    45,     0,     0,     0,   138,
     0,     0,     0,     0,     0,    46,    71,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,     0,     0,
    59,    60,    61,    62,    63,    64,    65,    66,    45,     0,
     0,   124,     0,     0,     0,     0,     0,     0,    46,    71,
    48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
    58,     0,     0,    59,    60,    61,    62,    63,    64,    65,
    66,    45,     0,    77,     0,     0,     0,     0,     0,     0,
     0,    46,    47,    48,    49,    50,    51,    52,    53,    54,
    55,    56,    57,    58,     0,     0,    59,    60,    61,    62,
    63,    64,    65,    66,    45,    67,     0,     0,    74,    75,
     0,     0,     0,     0,    46,    71,    48,    49,    50,    51,
    52,    53,    54,    55,    56,    57,    58,     0,     0,    59,
    60,    61,    62,    63,    64,    65,    66,    45,     0,     0,
     0,     0,     0,    72,     0,     0,     0,    46,    71,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     0,     0,    59,    60,    61,    62,    63,    64,    65,    66,
    45,     0,     0,     0,     0,     0,    73,     0,     0,     0,
    46,    71,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,     0,     0,    59,    60,    61,    62,    63,
    64,    65,    66,    45,     0,     0,     0,     0,     0,     0,
     0,    76,     0,    46,    71,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    45,     0,    59,    60,
    61,    62,    63,    64,    65,    66,    46,    71,    48,    49,
    50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
    45,    59,    60,    61,    62,    63,    64,    65,    66,   127,
    46,     0,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,-32768,     0,    59,    60,    61,    62,    63,
    64,    65,    66,    46,     0,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    45,     0,    59,    60,
    61,    62,    63,    64,    65,    66,    46,     0,    48,    49,
    50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
     0,    59,    60,    61,    62,    63,    64,    65,    66,    46,
     0,    48,    49,    50,    51,    52,    53,    54,    55,     0,
     0,    58,     0,     0,    59,    60,    61,    62,    63,    64,
    65,    66,    46,     0,    48,    49,    50,    51,    52,    53,
    54,    55,     0,     0,     0,     0,     0,    59,    60,    61,
    62,    63,    64,    65,    66,    46,     0,    48,-32768,-32768,
-32768,    52,    53,    54,    55,     0,     0,     0,     0,     0,
    59,    60,    61,    62,    63,    64,    65,    66,    46,     0,
    48,     0,     0,     0,-32768,-32768,-32768,-32768,     0,     0,
     0,     0,     0,    59,    60,    61,    62,    63,    64,    65,
    66
};

static const short yycheck[] = {     2,
     6,     1,    18,     3,    42,     8,    18,    18,    11,    12,
    13,    14,    15,    16,    44,    45,    19,     6,    21,    35,
    36,    37,    38,    39,    40,     7,    38,    39,    40,    40,
    47,    44,    45,    48,    48,    17,    44,    54,    48,    45,
    22,    18,     0,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    43,    48,    66,    68,    69,    44,    45,    71,    72,
    73,    74,    75,    76,     0,    78,    80,    44,    45,    52,
    52,    85,    28,    86,    18,    -1,    20,     4,     5,     6,
     7,    -1,     9,    10,    11,    12,    13,    14,    15,    33,
    34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    31,   118,    -1,    34,    35,    -1,
    -1,   124,    -1,    -1,   127,    42,    43,   130,    -1,    46,
    -1,    48,    49,    50,    51,    -1,    -1,   119,     4,     5,
     6,     7,    -1,     9,    10,    11,    12,    13,    14,    15,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,
    46,    -1,    48,    49,    50,    51,     4,     5,     6,     7,
    -1,     9,    10,    11,    12,    13,    14,    15,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    31,    -1,    -1,    34,    35,    -1,    -1,
    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    46,    -1,
    48,    49,    50,    51,     4,     5,     6,     7,    -1,     9,
    10,    11,    12,    13,    14,    15,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    31,    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    43,    -1,    45,    46,    -1,    48,    49,
    50,    51,     4,     5,     6,     7,    -1,     9,    10,    11,
    12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
    -1,    -1,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    43,    -1,    45,    46,    -1,    48,    49,    50,    51,
     4,     5,     6,     7,    -1,     9,    10,    11,    12,    13,
    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
    -1,    45,    46,    -1,    48,    49,    50,    51,     4,     5,
     6,     7,    -1,     9,    10,    11,    12,    13,    14,    15,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,
    46,    -1,    48,    49,    50,    51,     4,     5,     6,     7,
    -1,     9,    10,    11,    12,    13,    14,    15,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    31,    -1,    -1,    34,    35,    -1,    -1,
    -1,    -1,    -1,    -1,     8,    43,    -1,    -1,    46,    -1,
    48,    49,    50,    51,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    -1,    -1,    33,
    34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    -1,    -1,    33,    34,    35,    36,    37,
    38,    39,    40,    -1,    -1,    -1,     8,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    53,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
    -1,    33,    34,    35,    36,    37,    38,    39,    40,     8,
    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    -1,    -1,    33,    34,    35,    36,    37,    38,
    39,    40,     8,    -1,    -1,    -1,    45,    -1,    -1,    -1,
    -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    -1,    -1,    33,    34,    35,
    36,    37,    38,    39,    40,     8,    -1,    -1,    -1,    45,
    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
    33,    34,    35,    36,    37,    38,    39,    40,     8,    -1,
    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    -1,    -1,    33,    34,    35,    36,    37,    38,    39,
    40,     8,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    -1,    -1,    33,    34,    35,    36,
    37,    38,    39,    40,     8,    42,    -1,    -1,    12,    13,
    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    -1,    -1,    33,
    34,    35,    36,    37,    38,    39,    40,     8,    -1,    -1,
    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    -1,    -1,    33,    34,    35,    36,    37,
    38,    39,    40,     8,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,     8,    -1,    33,    34,
    35,    36,    37,    38,    39,    40,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
     8,    33,    34,    35,    36,    37,    38,    39,    40,    17,
    18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,     8,    -1,    33,    34,    35,    36,    37,
    38,    39,    40,    18,    -1,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,     8,    -1,    33,    34,
    35,    36,    37,    38,    39,    40,    18,    -1,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
    -1,    33,    34,    35,    36,    37,    38,    39,    40,    18,
    -1,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
    -1,    30,    -1,    -1,    33,    34,    35,    36,    37,    38,
    39,    40,    18,    -1,    20,    21,    22,    23,    24,    25,
    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,
    36,    37,    38,    39,    40,    18,    -1,    20,    21,    22,
    23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
    33,    34,    35,    36,    37,    38,    39,    40,    18,    -1,
    20,    -1,    -1,    -1,    24,    25,    26,    27,    -1,    -1,
    -1,    -1,    -1,    33,    34,    35,    36,    37,    38,    39,
    40
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 333 "parse.y"
{ YYACCEPT; ;
    break;}
case 2:
#line 334 "parse.y"
{ YYACCEPT; ;
    break;}
case 3:
#line 335 "parse.y"
{ push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; ;
    break;}
case 4:
#line 336 "parse.y"
{ return_ret = TRUE; yyclearin; YYABORT; ;
    break;}
case 5:
#line 337 "parse.y"
{ return_ret = TRUE; ;
    break;}
case 6:
#line 340 "parse.y"
{ PUSH_ACT(E_SEPAR); ;
    break;}
case 7:
#line 341 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 8:
#line 342 "parse.y"
{ PUSH_ACT(E_ABS); ;
    break;}
case 9:
#line 343 "parse.y"
{ PUSH_ACT(E_PLUS); ;
    break;}
case 10:
#line 344 "parse.y"
{ PUSH_ACT(E_MINUS); ;
    break;}
case 11:
#line 345 "parse.y"
{ PUSH_ACT(E_MUL); ;
    break;}
case 12:
#line 346 "parse.y"
{ PUSH_ACT(E_DIV); ;
    break;}
case 13:
#line 347 "parse.y"
{ PUSH_ACT(E_MOD); ;
    break;}
case 14:
#line 348 "parse.y"
{ PUSH_ACT(E_EQ_CMP); ;
    break;}
case 15:
#line 349 "parse.y"
{ PUSH_ACT(E_NE_CMP); ;
    break;}
case 16:
#line 350 "parse.y"
{ PUSH_ACT(E_CMP_CMP); ;
    break;}
case 17:
#line 351 "parse.y"
{ PUSH_ACT(E_LT_CMP); ;
    break;}
case 18:
#line 352 "parse.y"
{ PUSH_ACT(E_GT_CMP); ;
    break;}
case 19:
#line 353 "parse.y"
{ PUSH_ACT(E_LE_CMP); ;
    break;}
case 20:
#line 354 "parse.y"
{ PUSH_ACT(E_GE_CMP); ;
    break;}
case 21:
#line 355 "parse.y"
{ PUSH_ACT(E_LOGICAL_AND); ;
    break;}
case 22:
#line 356 "parse.y"
{ PUSH_ACT(E_LOGICAL_OR); ;
    break;}
case 23:
#line 357 "parse.y"
{ PUSH_ACT(E_LOGICAL_XOR); ;
    break;}
case 24:
#line 358 "parse.y"
{ PUSH_ACT(E_LOGICAL_NOT); ;
    break;}
case 25:
#line 359 "parse.y"
{ PUSH_ACT(E_FACT); ;
    break;}
case 26:
#line 360 "parse.y"
{ PUSH_ACT(E_TRANSPOSE); ;
    break;}
case 27:
#line 361 "parse.y"
{ PUSH_ACT(E_NEG); ;
    break;}
case 28:
#line 362 "parse.y"
{ PUSH_ACT(E_EXP); ;
    break;}
case 29:
#line 364 "parse.y"
{ PUSH_ACT(E_GET_ELEMENT); ;
    break;}
case 30:
#line 365 "parse.y"
{ PUSH_ACT(E_GET_ROW); ;
    break;}
case 31:
#line 366 "parse.y"
{ PUSH_ACT(E_GET_COLUMN); ;
    break;}
case 32:
#line 367 "parse.y"
{ if(!push_matrix()) {SYNTAX_ERROR;} ;
    break;}
case 33:
#line 368 "parse.y"
{ PUSH_ACT(E_WHILE_CONS); ;
    break;}
case 34:
#line 369 "parse.y"
{ PUSH_ACT(E_UNTIL_CONS); ;
    break;}
case 35:
#line 370 "parse.y"
{ PUSH_ACT(E_DOWHILE_CONS); ;
    break;}
case 36:
#line 371 "parse.y"
{ PUSH_ACT(E_DOUNTIL_CONS); ;
    break;}
case 37:
#line 372 "parse.y"
{ PUSH_ACT(E_IF_CONS); ;
    break;}
case 38:
#line 373 "parse.y"
{ PUSH_ACT(E_IFELSE_CONS); ;
    break;}
case 41:
#line 376 "parse.y"
{ PUSH_ACT(E_REFERENCE); ;
    break;}
case 43:
#line 378 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 44:
#line 380 "parse.y"
{ PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 45:
#line 381 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 46:
#line 383 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 47:
#line 384 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 48:
#line 385 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 49:
#line 387 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 51:
#line 389 "parse.y"
{ PUSH_ACT(E_RETURN); ;
    break;}
case 52:
#line 390 "parse.y"
{ PUSH_ACT(E_BAILOUT); ;
    break;}
case 53:
#line 391 "parse.y"
{ PUSH_ACT(E_EXCEPTION); ;
    break;}
case 54:
#line 392 "parse.y"
{ stack_push(&evalstack,makenum(yyvsp[0].val));
					  mpw_clear(yyvsp[0].val);
					;
    break;}
case 55:
#line 395 "parse.y"
{ PUSH_STRING(yyvsp[0].id); ;
    break;}
case 56:
#line 396 "parse.y"
{ push_null(); ;
    break;}
case 57:
#line 399 "parse.y"
{ PUSH_ACT(E_DEREFERENCE); ;
    break;}
case 58:
#line 402 "parse.y"
{ PUSH_IDENTIFIER(yyvsp[0].id); ;
    break;}
case 59:
#line 405 "parse.y"
{ if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 60:
#line 406 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;}
						  if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 62:
#line 411 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 64:
#line 415 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 65:
#line 418 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} ;
    break;}
case 66:
#line 419 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} if(!push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 422 "parse.y"

