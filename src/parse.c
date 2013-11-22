
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
#define	XRETURN	264
#define	WHILE	265
#define	UNTIL	266
#define	DO	267
#define	IF	268
#define	THEN	269
#define	ELSE	270
#define	NEG	271
#define	AT	272
#define	SEPAR	273
#define	EQUALS	274
#define	EQ_CMP	275
#define	NE_CMP	276
#define	CMP_CMP	277
#define	LT_CMP	278
#define	GT_CMP	279
#define	LE_CMP	280
#define	GE_CMP	281
#define	LOGICAL_XOR	282
#define	LOGICAL_OR	283
#define	LOGICAL_AND	284
#define	LOGICAL_NOT	285
#define	LOWER_THEN_ELSE	286
#define	UMINUS	287

#line 21 "parse.y"

#include <config.h>

#ifndef WITHOUT_GNOME
#include <gnome.h>
#else
#ifndef _
#define _(x) x
#endif
#endif

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

#define SYNTAX_ERROR {yyerror("syntax error"); YYERROR;}

#define PUSH_ACT(ACT) { \
	ETree *tree = makeoperator(ACT,&evalstack); \
	if(!tree) {SYNTAX_ERROR;} \
	stack_push(&evalstack,tree); \
}

#define PUSH_IDENTIFIER(ID) { \
	ETree * tree; \
	tree = g_new0(ETree,1); \
	tree->type = IDENTIFIER_NODE; \
	tree->data.id = g_strdup(ID); \
	stack_push(&evalstack,tree); \
}

#define PUSH_STRING(ID) { \
	ETree * tree; \
	tree = g_new0(ETree,1); \
	tree->type = STRING_NODE; \
	tree->data.id = g_strdup(ID); \
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
		tree->data.id = NULL;
		freetree(tree);
		i++;
	}
	
	func = g_new0(EFunc,1);
	
	func->type = USER_FUNC;
	func->context = -1;
	func->nargs = i;
	func->named_args = list;
	func->data.user = val;

	tree = g_new0(ETree,1);

	tree->type = FUNCTION_NODE;
	tree->data.func = func;

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
       
	tree = g_new0(ETree,1);
	tree->type = markertype;
	stack_push(&evalstack,tree);
	stack_push(&evalstack,last_expr);
	return TRUE;
}

/*pushes a marker*/
static void
push_marker_simple(ETreeType markertype)
{
	ETree * tree = g_new0(ETree,1);
	tree->type = markertype;
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
	tree = g_new(ETree,1);
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
	/*FIXME: add matrix code here*/
	return TRUE;
}

/*pushes a NULL onto the stack, null cannot be evaluated, it will be
  read as ""*/
static void
push_null(void)
{
	ETree *tree;
	tree = g_new0(ETree,1);
	tree->type = NULL_NODE;

	stack_push(&evalstack,tree);
}


#line 205 "parse.y"
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



#define	YYFINAL		138
#define	YYFLAG		-32768
#define	YYNTBASE	52

#define YYTRANSLATE(x) ((unsigned)(x) <= 287 ? yytranslate[x] : 60)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    37,     2,     2,     2,    36,    46,     2,    44,
    45,    34,    32,    41,    33,    48,    35,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    51,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    43,     2,    42,    38,     2,    47,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    49,     2,    50,     2,     2,     2,     2,     2,
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
    26,    27,    28,    29,    30,    31,    39
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     7,    12,    15,    17,    21,    25,    29,    33,
    37,    41,    45,    49,    53,    57,    61,    65,    69,    73,
    77,    81,    85,    88,    91,    94,    97,   101,   108,   114,
   120,   124,   129,   134,   139,   144,   149,   156,   160,   162,
   165,   167,   171,   176,   180,   185,   191,   196,   200,   203,
   206,   208,   210,   212,   215,   217,   224,   230,   234,   236,
   240,   242,   246
};

static const short yyrhs[] = {     3,
    53,    40,     0,     3,    40,     0,     3,    53,    18,    40,
     0,     1,    40,     0,     1,     0,    53,    18,    53,     0,
    53,    19,    53,     0,    53,    32,    53,     0,    53,    33,
    53,     0,    53,    34,    53,     0,    53,    35,    53,     0,
    53,    36,    53,     0,    53,    20,    53,     0,    53,    21,
    53,     0,    53,    22,    53,     0,    53,    23,    53,     0,
    53,    24,    53,     0,    53,    25,    53,     0,    53,    26,
    53,     0,    53,    29,    53,     0,    53,    28,    53,     0,
    53,    27,    53,     0,    30,    53,     0,    53,    37,     0,
    16,    53,     0,    33,    53,     0,    53,    38,    53,     0,
    53,    17,    53,    41,    53,    42,     0,    53,    17,    53,
    41,    42,     0,    53,    17,    41,    53,    42,     0,    43,
    59,    42,     0,    10,    53,    12,    53,     0,    11,    53,
    12,    53,     0,    12,    53,    10,    53,     0,    12,    53,
    11,    53,     0,    13,    53,    14,    53,     0,    13,    53,
    14,    53,    15,    53,     0,    44,    53,    45,     0,    55,
     0,    46,    55,     0,    54,     0,    55,    44,    45,     0,
    55,    44,    58,    45,     0,    54,    44,    45,     0,    54,
    44,    58,    45,     0,    53,     8,    44,    58,    45,     0,
    53,     8,    44,    45,     0,     7,    55,    56,     0,    47,
    56,     0,     9,    53,     0,     4,     0,     5,     0,    48,
     0,    34,    55,     0,     6,     0,    44,    57,    45,    49,
    53,    50,     0,    44,    45,    49,    53,    50,     0,    57,
    41,    55,     0,    55,     0,    58,    41,    53,     0,    53,
     0,    59,    51,    58,     0,    58,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   255,   256,   257,   258,   259,   262,   263,   264,   265,   266,
   267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
   277,   278,   279,   280,   281,   282,   283,   284,   286,   287,
   288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
   298,   299,   301,   302,   304,   305,   306,   308,   309,   310,
   311,   314,   315,   318,   321,   324,   325,   329,   330,   333,
   334,   337,   338
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STARTTOK",
"NUMBER","STRING","FUNCID","DEFINE","CALL","XRETURN","WHILE","UNTIL","DO","IF",
"THEN","ELSE","NEG","AT","SEPAR","EQUALS","EQ_CMP","NE_CMP","CMP_CMP","LT_CMP",
"GT_CMP","LE_CMP","GE_CMP","LOGICAL_XOR","LOGICAL_OR","LOGICAL_AND","LOGICAL_NOT",
"LOWER_THEN_ELSE","'+'","'-'","'*'","'/'","'%'","'!'","'^'","UMINUS","'\\n'",
"','","']'","'['","'('","')'","'&'","'`'","'.'","'{'","'}'","':'","fullexpr",
"expr","deref","ident","funcdef","identlist","exprlist","matrixrows", NULL
};
#endif

static const short yyr1[] = {     0,
    52,    52,    52,    52,    52,    53,    53,    53,    53,    53,
    53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
    53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
    53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
    53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
    53,    53,    53,    54,    55,    56,    56,    57,    57,    58,
    58,    59,    59
};

static const short yyr2[] = {     0,
     3,     2,     4,     2,     1,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     2,     2,     2,     2,     3,     6,     5,     5,
     3,     4,     4,     4,     4,     4,     6,     3,     1,     2,
     1,     3,     4,     3,     4,     5,     4,     3,     2,     2,
     1,     1,     1,     2,     1,     6,     5,     3,     1,     3,
     1,     3,     1
};

static const short yydefact[] = {     0,
     5,     0,     4,    51,    52,    55,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     2,     0,     0,     0,
     0,    53,     0,    41,    39,     0,    50,     0,     0,     0,
     0,    25,    23,    26,    54,    61,    63,     0,     0,    40,
     0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    24,     0,     1,     0,     0,    48,     0,     0,     0,
     0,     0,     0,     0,    31,     0,    38,     0,    59,     0,
     0,     0,     0,     3,     6,     7,    13,    14,    15,    16,
    17,    18,    19,    22,    21,    20,     8,     9,    10,    11,
    12,    27,    44,     0,    42,     0,    32,    33,    34,    35,
    36,    60,    62,     0,     0,     0,    47,     0,     0,     0,
    45,    43,     0,     0,    58,     0,    46,    30,    29,     0,
    37,    57,     0,    28,    56,     0,     0,     0
};

static const short yydefgoto[] = {   136,
    36,    24,    25,    42,    80,    37,    38
};

static const short yypact[] = {    73,
   -38,    83,-32768,-32768,-32768,-32768,     8,   401,   401,   401,
   401,   401,   401,   401,   401,     8,-32768,   401,   401,     8,
   -29,-32768,   625,   -17,   -16,   -29,   824,   687,   718,   656,
   749,    14,   269,    14,-32768,   771,    24,   -10,   501,-32768,
    -5,-32768,    -6,   131,   176,   401,   401,   401,   401,   401,
   401,   401,   401,   401,   401,   401,   401,   401,   401,   401,
   401,-32768,   401,-32768,   221,   266,-32768,   401,   401,   401,
   401,   401,   401,   401,-32768,   401,-32768,    28,-32768,   -12,
   311,   401,   594,-32768,   846,   -14,   314,   314,   314,   359,
   359,   359,   359,   224,   224,   269,    47,    47,    -1,    -1,
    -1,   -13,-32768,   -11,-32768,    34,   824,   824,   824,   824,
   802,   771,    24,   401,     8,    37,-32768,    56,   532,   356,
-32768,-32768,   401,   433,-32768,   401,-32768,-32768,-32768,   563,
   824,-32768,   467,-32768,-32768,    91,    98,-32768
};

static const short yypgoto[] = {-32768,
    -2,-32768,    19,    74,-32768,    -3,-32768
};


#define	YYLAST		884


static const short yytable[] = {    23,
     6,     3,    44,    44,    46,    27,    28,    29,    30,    31,
    32,    33,    34,     6,    41,    44,    39,    57,    58,    59,
    60,    61,    62,    63,    63,    26,    65,    66,   115,    74,
    44,    75,   116,   121,    35,    62,    63,    81,    40,    78,
    76,    83,    85,    86,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    99,   100,   101,    79,
   102,   104,   106,    44,    74,    85,   107,   108,   109,   110,
   111,   112,   113,     1,    74,     2,   114,   118,   122,   119,
    59,    60,    61,    62,    63,   126,     4,     5,     6,     7,
   137,     8,     9,    10,    11,    12,    74,   138,    13,    67,
   127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   124,    14,     0,     0,    15,    16,   130,     0,     0,
   131,     0,    17,   133,     0,    18,    19,     0,    20,    21,
    22,     0,     0,   125,     4,     5,     6,     7,     0,     8,
     9,    10,    11,    12,     0,     0,    13,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    14,     0,     0,    15,    16,     0,     0,     0,     0,     0,
     0,    82,     0,    18,    19,     0,    20,    21,    22,     4,
     5,     6,     7,     0,     8,     9,    10,    11,    12,     0,
     0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    14,     0,     0,    15,    16,
     0,     0,     0,     0,     0,    84,     0,     0,    18,    19,
     0,    20,    21,    22,     4,     5,     6,     7,     0,     8,
     9,    10,    11,    12,     0,     0,    13,     0,     0,     0,
    44,     0,    46,    47,    48,    49,    50,    51,    52,    53,
    14,     0,    56,    15,    16,    57,    58,    59,    60,    61,
    62,    63,     0,    18,    19,   103,    20,    21,    22,     4,
     5,     6,     7,     0,     8,     9,    10,    11,    12,     0,
     0,    13,     0,     0,     0,    44,     0,    46,    47,    48,
    49,    50,    51,    52,    53,    14,     0,     0,    15,    16,
    57,    58,    59,    60,    61,    62,    63,     0,    18,    19,
   105,    20,    21,    22,     4,     5,     6,     7,     0,     8,
     9,    10,    11,    12,     0,     0,    13,     0,     0,     0,
    44,     0,    46,-32768,-32768,-32768,    50,    51,    52,    53,
    14,     0,     0,    15,    16,    57,    58,    59,    60,    61,
    62,    63,     0,    18,    19,   117,    20,    21,    22,     4,
     5,     6,     7,     0,     8,     9,    10,    11,    12,     0,
     0,    13,     0,     0,     0,    44,     0,    46,     0,     0,
     0,-32768,-32768,-32768,-32768,    14,     0,     0,    15,    16,
    57,    58,    59,    60,    61,    62,    63,   129,    18,    19,
     0,    20,    21,    22,     4,     5,     6,     7,     0,     8,
     9,    10,    11,    12,     0,     0,    13,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    14,     0,     0,    15,    16,     0,     0,     0,     0,     0,
    43,     0,     0,    18,    19,     0,    20,    21,    22,    44,
    68,    46,    47,    48,    49,    50,    51,    52,    53,    54,
    55,    56,     0,     0,    57,    58,    59,    60,    61,    62,
    63,     0,     0,     0,    43,     0,     0,     0,     0,     0,
     0,     0,   132,    44,    68,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,     0,     0,    57,    58,
    59,    60,    61,    62,    63,     0,     0,     0,    43,     0,
     0,     0,     0,     0,     0,     0,   135,    44,    68,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
     0,     0,    57,    58,    59,    60,    61,    62,    63,    43,
     0,     0,     0,     0,     0,    77,     0,     0,    44,    68,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    43,     0,     0,   128,     0,     0,     0,     0,     0,    44,
    68,    46,    47,    48,    49,    50,    51,    52,    53,    54,
    55,    56,     0,     0,    57,    58,    59,    60,    61,    62,
    63,    43,     0,     0,   134,     0,     0,     0,     0,     0,
    44,    68,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    56,     0,     0,    57,    58,    59,    60,    61,
    62,    63,    43,     0,   120,     0,     0,     0,     0,     0,
     0,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,     0,     0,    57,    58,    59,    60,
    61,    62,    63,    43,    64,    71,    72,     0,     0,     0,
     0,     0,    44,    68,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,    56,     0,     0,    57,    58,    59,
    60,    61,    62,    63,    43,     0,     0,     0,    69,     0,
     0,     0,     0,    44,    68,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,     0,     0,    57,    58,
    59,    60,    61,    62,    63,    43,     0,     0,     0,    70,
     0,     0,     0,     0,    44,    68,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,    56,     0,     0,    57,
    58,    59,    60,    61,    62,    63,    43,     0,     0,     0,
     0,     0,    73,     0,     0,    44,    68,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    43,     0,
    57,    58,    59,    60,    61,    62,    63,    44,    68,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
     0,     0,    57,    58,    59,    60,    61,    62,    63,    43,
     0,     0,     0,     0,     0,     0,   123,     0,    44,     0,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,-32768,     0,    57,    58,    59,    60,    61,    62,    63,
    44,     0,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    43,     0,    57,    58,    59,    60,    61,
    62,    63,    44,     0,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,    56,     0,     0,    57,    58,    59,
    60,    61,    62,    63
};

static const short yycheck[] = {     2,
     6,    40,    17,    17,    19,     8,     9,    10,    11,    12,
    13,    14,    15,     6,    44,    17,    19,    32,    33,    34,
    35,    36,    37,    38,    38,     7,    44,    44,    41,    41,
    17,    42,    45,    45,    16,    37,    38,    44,    20,    45,
    51,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    41,
    63,    65,    66,    17,    41,    68,    69,    70,    71,    72,
    73,    74,    76,     1,    41,     3,    49,    81,    45,    82,
    34,    35,    36,    37,    38,    49,     4,     5,     6,     7,
     0,     9,    10,    11,    12,    13,    41,     0,    16,    26,
    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   114,    30,    -1,    -1,    33,    34,   120,    -1,    -1,
   123,    -1,    40,   126,    -1,    43,    44,    -1,    46,    47,
    48,    -1,    -1,   115,     4,     5,     6,     7,    -1,     9,
    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
    -1,    41,    -1,    43,    44,    -1,    46,    47,    48,     4,
     5,     6,     7,    -1,     9,    10,    11,    12,    13,    -1,
    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,
    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    43,    44,
    -1,    46,    47,    48,     4,     5,     6,     7,    -1,     9,
    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
    17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
    30,    -1,    29,    33,    34,    32,    33,    34,    35,    36,
    37,    38,    -1,    43,    44,    45,    46,    47,    48,     4,
     5,     6,     7,    -1,     9,    10,    11,    12,    13,    -1,
    -1,    16,    -1,    -1,    -1,    17,    -1,    19,    20,    21,
    22,    23,    24,    25,    26,    30,    -1,    -1,    33,    34,
    32,    33,    34,    35,    36,    37,    38,    -1,    43,    44,
    45,    46,    47,    48,     4,     5,     6,     7,    -1,     9,
    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
    17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
    30,    -1,    -1,    33,    34,    32,    33,    34,    35,    36,
    37,    38,    -1,    43,    44,    45,    46,    47,    48,     4,
     5,     6,     7,    -1,     9,    10,    11,    12,    13,    -1,
    -1,    16,    -1,    -1,    -1,    17,    -1,    19,    -1,    -1,
    -1,    23,    24,    25,    26,    30,    -1,    -1,    33,    34,
    32,    33,    34,    35,    36,    37,    38,    42,    43,    44,
    -1,    46,    47,    48,     4,     5,     6,     7,    -1,     9,
    10,    11,    12,    13,    -1,    -1,    16,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
     8,    -1,    -1,    43,    44,    -1,    46,    47,    48,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    -1,    -1,    32,    33,    34,    35,    36,    37,
    38,    -1,    -1,    -1,     8,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    50,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
    34,    35,    36,    37,    38,    -1,    -1,    -1,     8,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    50,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    -1,    -1,    32,    33,    34,    35,    36,    37,    38,     8,
    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
     8,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    -1,    -1,    32,    33,    34,    35,    36,    37,
    38,     8,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    -1,    -1,    32,    33,    34,    35,    36,
    37,    38,     8,    -1,    41,    -1,    -1,    -1,    -1,    -1,
    -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
    36,    37,    38,     8,    40,    10,    11,    -1,    -1,    -1,
    -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
    35,    36,    37,    38,     8,    -1,    -1,    -1,    12,    -1,
    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
    34,    35,    36,    37,    38,     8,    -1,    -1,    -1,    12,
    -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
    33,    34,    35,    36,    37,    38,     8,    -1,    -1,    -1,
    -1,    -1,    14,    -1,    -1,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,     8,    -1,
    32,    33,    34,    35,    36,    37,    38,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    -1,    -1,    32,    33,    34,    35,    36,    37,    38,     8,
    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,    -1,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,     8,    -1,    32,    33,    34,    35,    36,    37,    38,
    17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,     8,    -1,    32,    33,    34,    35,    36,
    37,    38,    17,    -1,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
    35,    36,    37,    38
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
#line 255 "parse.y"
{ YYACCEPT; ;
    break;}
case 2:
#line 256 "parse.y"
{ YYACCEPT; ;
    break;}
case 3:
#line 257 "parse.y"
{ push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; ;
    break;}
case 4:
#line 258 "parse.y"
{ return_ret = TRUE; yyclearin; YYABORT; ;
    break;}
case 5:
#line 259 "parse.y"
{ return_ret = TRUE; ;
    break;}
case 6:
#line 262 "parse.y"
{ PUSH_ACT(E_SEPAR); ;
    break;}
case 7:
#line 263 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 8:
#line 264 "parse.y"
{ PUSH_ACT(E_PLUS); ;
    break;}
case 9:
#line 265 "parse.y"
{ PUSH_ACT(E_MINUS); ;
    break;}
case 10:
#line 266 "parse.y"
{ PUSH_ACT(E_MUL); ;
    break;}
case 11:
#line 267 "parse.y"
{ PUSH_ACT(E_DIV); ;
    break;}
case 12:
#line 268 "parse.y"
{ PUSH_ACT(E_MOD); ;
    break;}
case 13:
#line 269 "parse.y"
{ PUSH_ACT(E_EQ_CMP); ;
    break;}
case 14:
#line 270 "parse.y"
{ PUSH_ACT(E_NE_CMP); ;
    break;}
case 15:
#line 271 "parse.y"
{ PUSH_ACT(E_CMP_CMP); ;
    break;}
case 16:
#line 272 "parse.y"
{ PUSH_ACT(E_LT_CMP); ;
    break;}
case 17:
#line 273 "parse.y"
{ PUSH_ACT(E_GT_CMP); ;
    break;}
case 18:
#line 274 "parse.y"
{ PUSH_ACT(E_LE_CMP); ;
    break;}
case 19:
#line 275 "parse.y"
{ PUSH_ACT(E_GE_CMP); ;
    break;}
case 20:
#line 276 "parse.y"
{ PUSH_ACT(E_LOGICAL_AND); ;
    break;}
case 21:
#line 277 "parse.y"
{ PUSH_ACT(E_LOGICAL_OR); ;
    break;}
case 22:
#line 278 "parse.y"
{ PUSH_ACT(E_LOGICAL_XOR); ;
    break;}
case 23:
#line 279 "parse.y"
{ PUSH_ACT(E_LOGICAL_NOT); ;
    break;}
case 24:
#line 280 "parse.y"
{ PUSH_ACT(E_FACT); ;
    break;}
case 25:
#line 281 "parse.y"
{ PUSH_ACT(E_NEG); ;
    break;}
case 26:
#line 282 "parse.y"
{ PUSH_ACT(E_NEG); ;
    break;}
case 27:
#line 283 "parse.y"
{ PUSH_ACT(E_EXP); ;
    break;}
case 28:
#line 285 "parse.y"
{ PUSH_ACT(E_GET_ELEMENT); ;
    break;}
case 29:
#line 286 "parse.y"
{ PUSH_ACT(E_GET_ROW); ;
    break;}
case 30:
#line 287 "parse.y"
{ PUSH_ACT(E_GET_COLUMN); ;
    break;}
case 31:
#line 288 "parse.y"
{ if(!push_matrix()) {SYNTAX_ERROR;} ;
    break;}
case 32:
#line 289 "parse.y"
{ PUSH_ACT(E_WHILE_CONS); ;
    break;}
case 33:
#line 290 "parse.y"
{ PUSH_ACT(E_UNTIL_CONS); ;
    break;}
case 34:
#line 291 "parse.y"
{ PUSH_ACT(E_DOWHILE_CONS); ;
    break;}
case 35:
#line 292 "parse.y"
{ PUSH_ACT(E_DOUNTIL_CONS); ;
    break;}
case 36:
#line 293 "parse.y"
{ PUSH_ACT(E_IF_CONS); ;
    break;}
case 37:
#line 294 "parse.y"
{ PUSH_ACT(E_IFELSE_CONS); ;
    break;}
case 40:
#line 297 "parse.y"
{ PUSH_ACT(E_REFERENCE); ;
    break;}
case 42:
#line 299 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 43:
#line 301 "parse.y"
{ PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 44:
#line 302 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 45:
#line 304 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 46:
#line 305 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 47:
#line 306 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 48:
#line 308 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 50:
#line 310 "parse.y"
{ PUSH_ACT(E_RETURN); ;
    break;}
case 51:
#line 311 "parse.y"
{ stack_push(&evalstack,makenum(yyvsp[0].val));
					  mpw_clear(yyvsp[0].val);
					;
    break;}
case 52:
#line 314 "parse.y"
{ PUSH_STRING(yyvsp[0].id); ;
    break;}
case 53:
#line 315 "parse.y"
{ push_null(); ;
    break;}
case 54:
#line 318 "parse.y"
{ PUSH_ACT(E_DEREFERENCE); ;
    break;}
case 55:
#line 321 "parse.y"
{ PUSH_IDENTIFIER(yyvsp[0].id); ;
    break;}
case 56:
#line 324 "parse.y"
{ if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 57:
#line 325 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;}
						  if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 59:
#line 330 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 61:
#line 334 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 62:
#line 337 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} ;
    break;}
case 63:
#line 338 "parse.y"
{ if(!push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} ;
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
#line 341 "parse.y"

