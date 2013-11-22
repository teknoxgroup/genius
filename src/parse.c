
/*  A Bison parser, made from parse.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	STARTTOK	258
#define	LOADFILE	259
#define	LOADFILE_GLOB	260
#define	NUMBER	261
#define	STRING	262
#define	FUNCID	263
#define	DEFINE	264
#define	CALL	265
#define	RETURNTOK	266
#define	BAILOUT	267
#define	EXCEPTION	268
#define	CONTINUE	269
#define	BREAK	270
#define	WHILE	271
#define	UNTIL	272
#define	DO	273
#define	IF	274
#define	THEN	275
#define	ELSE	276
#define	AT	277
#define	REGION_SEP	278
#define	SEPAR	279
#define	EQUALS	280
#define	EQ_CMP	281
#define	NE_CMP	282
#define	CMP_CMP	283
#define	LT_CMP	284
#define	GT_CMP	285
#define	LE_CMP	286
#define	GE_CMP	287
#define	LOGICAL_XOR	288
#define	LOGICAL_OR	289
#define	LOGICAL_AND	290
#define	LOGICAL_NOT	291
#define	LOWER_THEN_ELSE	292
#define	UMINUS	293

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
extern char *loadfile;
extern char *loadfile_glob;

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
	tree->data.str = ID; \
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
	
	func->id = NULL;
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
	
	return TRUE;
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


#line 287 "parse.y"
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



#define	YYFINAL		161
#define	YYFLAG		-32768
#define	YYNTBASE	60

#define YYTRANSLATE(x) ((unsigned)(x) <= 293 ? yytranslate[x] : 69)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    47,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    44,     2,     2,     2,    42,    54,    43,    53,
    50,    40,    38,    49,    39,    56,    41,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    59,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    51,     2,    52,    45,     2,    55,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    57,    48,    58,     2,     2,     2,     2,     2,
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
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    46
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     8,    12,    15,    20,    23,    25,    29,    33,
    37,    41,    45,    49,    53,    57,    61,    65,    69,    73,
    77,    81,    85,    89,    93,    97,   100,   103,   106,   109,
   113,   120,   127,   134,   141,   147,   153,   159,   165,   169,
   174,   179,   184,   189,   194,   201,   205,   207,   210,   212,
   216,   221,   225,   230,   236,   241,   245,   248,   251,   253,
   255,   257,   259,   261,   263,   265,   268,   270,   277,   283,
   287,   289,   293,   295,   299,   301
};

static const short yyrhs[] = {     3,
    61,    47,     0,     3,     4,    47,     0,     3,     5,    47,
     0,     3,    47,     0,     3,    61,    24,    47,     0,     1,
    47,     0,     1,     0,    61,    24,    61,     0,    61,    25,
    61,     0,    48,    61,    48,     0,    61,    38,    61,     0,
    61,    39,    61,     0,    61,    40,    61,     0,    61,    41,
    61,     0,    61,    42,    61,     0,    61,    26,    61,     0,
    61,    27,    61,     0,    61,    28,    61,     0,    61,    29,
    61,     0,    61,    30,    61,     0,    61,    31,    61,     0,
    61,    32,    61,     0,    61,    35,    61,     0,    61,    34,
    61,     0,    61,    33,    61,     0,    36,    61,     0,    61,
    44,     0,    61,    43,     0,    39,    61,     0,    61,    45,
    61,     0,    61,    22,    61,    49,    61,    50,     0,    61,
    22,    68,    49,    61,    50,     0,    61,    22,    61,    49,
    68,    50,     0,    61,    22,    68,    49,    68,    50,     0,
    61,    22,    61,    49,    50,     0,    61,    22,    68,    49,
    50,     0,    61,    22,    49,    61,    50,     0,    61,    22,
    49,    68,    50,     0,    51,    67,    52,     0,    16,    61,
    18,    61,     0,    17,    61,    18,    61,     0,    18,    61,
    16,    61,     0,    18,    61,    17,    61,     0,    19,    61,
    20,    61,     0,    19,    61,    20,    61,    21,    61,     0,
    53,    61,    50,     0,    63,     0,    54,    63,     0,    62,
     0,    63,    53,    50,     0,    63,    53,    66,    50,     0,
    62,    53,    50,     0,    62,    53,    66,    50,     0,    61,
    10,    53,    66,    50,     0,    61,    10,    53,    50,     0,
     9,    63,    64,     0,    55,    64,     0,    11,    61,     0,
    12,     0,    13,     0,    14,     0,    15,     0,     6,     0,
     7,     0,    56,     0,    40,    63,     0,     8,     0,    53,
    65,    50,    57,    61,    58,     0,    53,    50,    57,    61,
    58,     0,    65,    49,    63,     0,    63,     0,    66,    49,
    61,     0,    61,     0,    67,    59,    66,     0,    66,     0,
    61,    23,    61,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   340,   341,   342,   343,   344,   345,   346,   349,   350,   351,
   352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
   362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
   372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
   382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
   393,   394,   396,   397,   398,   400,   401,   402,   403,   404,
   405,   406,   407,   409,   410,   413,   416,   419,   420,   424,
   425,   428,   429,   432,   433,   436
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STARTTOK",
"LOADFILE","LOADFILE_GLOB","NUMBER","STRING","FUNCID","DEFINE","CALL","RETURNTOK",
"BAILOUT","EXCEPTION","CONTINUE","BREAK","WHILE","UNTIL","DO","IF","THEN","ELSE",
"AT","REGION_SEP","SEPAR","EQUALS","EQ_CMP","NE_CMP","CMP_CMP","LT_CMP","GT_CMP",
"LE_CMP","GE_CMP","LOGICAL_XOR","LOGICAL_OR","LOGICAL_AND","LOGICAL_NOT","LOWER_THEN_ELSE",
"'+'","'-'","'*'","'/'","'%'","'''","'!'","'^'","UMINUS","'\\n'","'|'","','",
"')'","'['","']'","'('","'&'","'`'","'.'","'{'","'}'","':'","fullexpr","expr",
"deref","ident","funcdef","identlist","exprlist","matrixrows","reg", NULL
};
#endif

static const short yyr1[] = {     0,
    60,    60,    60,    60,    60,    60,    60,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    62,    63,    64,    64,    65,
    65,    66,    66,    67,    67,    68
};

static const short yyr2[] = {     0,
     3,     3,     3,     2,     4,     2,     1,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
     6,     6,     6,     6,     5,     5,     5,     5,     3,     4,
     4,     4,     4,     4,     6,     3,     1,     2,     1,     3,
     4,     3,     4,     5,     4,     3,     2,     2,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     6,     5,     3,
     1,     3,     1,     3,     1,     3
};

static const short yydefact[] = {     0,
     7,     0,     6,     0,     0,    63,    64,    67,     0,     0,
    59,    60,    61,    62,     0,     0,     0,     0,     0,     0,
     0,     4,     0,     0,     0,     0,     0,    65,     0,    49,
    47,     2,     3,     0,    58,     0,     0,     0,     0,    26,
    29,    66,     0,    73,    75,     0,     0,    48,     0,    57,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
    27,     0,     1,     0,     0,    56,     0,     0,     0,     0,
     0,     0,    10,     0,    39,     0,    46,     0,    71,     0,
     0,     0,     0,     0,     5,     8,     9,    16,    17,    18,
    19,    20,    21,    22,    25,    24,    23,    11,    12,    13,
    14,    15,    30,    52,     0,    50,     0,    40,    41,    42,
    43,    44,    72,    74,     0,     0,     0,    55,     0,     0,
     0,     0,     0,     0,    53,    51,     0,     0,    70,     0,
    54,    37,    38,    76,    35,     0,     0,    36,     0,     0,
    45,    69,     0,    31,    33,    32,    34,    68,     0,     0,
     0
};

static const short yydefgoto[] = {   159,
    44,    30,    31,    50,    90,    45,    46,    94
};

static const short yypact[] = {     6,
   -41,   100,-32768,   -37,   -25,-32768,-32768,-32768,    31,   508,
-32768,-32768,-32768,-32768,   508,   508,   508,   508,   508,   508,
    31,-32768,   508,   508,   508,    31,    -7,-32768,   805,    16,
    20,-32768,-32768,    -7,  1017,   877,   913,   841,   939,  1091,
    25,-32768,   778,   965,    -8,   -47,   716,-32768,    -6,-32768,
    21,   151,   202,   508,   508,   508,   508,   508,   508,   508,
   508,   508,   508,   508,   508,   508,   508,   508,   508,-32768,
-32768,   508,-32768,   253,   304,-32768,   508,   508,   508,   508,
   508,   508,-32768,   508,-32768,   508,-32768,    24,-32768,   -30,
   355,   508,   752,    35,-32768,  1043,  1150,  1115,  1115,  1115,
  1139,  1139,  1139,  1139,  1067,  1067,  1091,   -11,   -11,   -19,
   -19,   -19,   -18,-32768,   -14,-32768,   -12,  1017,  1017,  1017,
  1017,   991,   965,    -8,   508,    31,    28,-32768,    -1,   617,
    36,   508,   406,   457,-32768,-32768,   508,   543,-32768,   508,
-32768,-32768,-32768,   965,-32768,   653,    37,-32768,   689,    39,
  1017,-32768,   580,-32768,-32768,-32768,-32768,-32768,    91,    92,
-32768
};

static const short yypgoto[] = {-32768,
    -2,-32768,    19,    59,-32768,    -3,-32768,   -91
};


#define	YYLAST		1195


static const short yytable[] = {    29,
   131,     8,    52,    52,    85,     3,     1,    35,     2,    32,
    52,    86,    36,    37,    38,    39,    40,    41,   126,   127,
    43,    33,    47,    70,    71,    72,    72,    34,    67,    68,
    69,    70,    71,    72,    84,   135,    84,   136,     8,    42,
    84,   147,   150,    88,    48,    49,    52,    84,   141,    93,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   109,   110,   111,   112,    89,    74,   113,
   115,   117,    75,    91,    96,   118,   119,   120,   121,   122,
   125,   123,   124,   134,   140,   143,   155,   129,   157,   130,
   160,   161,    76,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     4,     5,     6,     7,     8,     9,     0,
    10,    11,    12,    13,    14,    15,    16,    17,    18,     0,
     0,     0,   138,     0,     0,     0,     0,     0,     0,   144,
   146,   149,     0,     0,   151,    19,     0,   153,    20,    21,
     0,     0,     0,     0,   139,     0,    22,    23,     0,     0,
    24,     0,    25,    26,    27,    28,     6,     7,     8,     9,
     0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    19,     0,     0,    20,
    21,     0,     0,     0,     0,     0,     0,     0,    23,    92,
     0,    24,     0,    25,    26,    27,    28,     6,     7,     8,
     9,     0,    10,    11,    12,    13,    14,    15,    16,    17,
    18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    19,     0,     0,
    20,    21,     0,     0,     0,     0,     0,     0,    95,    23,
     0,     0,    24,     0,    25,    26,    27,    28,     6,     7,
     8,     9,     0,    10,    11,    12,    13,    14,    15,    16,
    17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    19,     0,
     0,    20,    21,     0,     0,     0,     0,     0,     0,     0,
    23,     0,   114,    24,     0,    25,    26,    27,    28,     6,
     7,     8,     9,     0,    10,    11,    12,    13,    14,    15,
    16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
     0,     0,    20,    21,     0,     0,     0,     0,     0,     0,
     0,    23,     0,   116,    24,     0,    25,    26,    27,    28,
     6,     7,     8,     9,     0,    10,    11,    12,    13,    14,
    15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    19,     0,     0,    20,    21,     0,     0,     0,     0,     0,
     0,     0,    23,     0,   128,    24,     0,    25,    26,    27,
    28,     6,     7,     8,     9,     0,    10,    11,    12,    13,
    14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    19,     0,     0,    20,    21,     0,     0,     0,     0,
     0,     0,     0,    23,     0,   145,    24,     0,    25,    26,
    27,    28,     6,     7,     8,     9,     0,    10,    11,    12,
    13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    19,     0,     0,    20,    21,     0,     0,     0,
     0,     0,     0,     0,    23,     0,   148,    24,     0,    25,
    26,    27,    28,     6,     7,     8,     9,     0,    10,    11,
    12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    19,     0,     0,    20,    21,     0,     0,
     0,     0,    51,     0,     0,    23,     0,     0,    24,     0,
    25,    26,    27,    28,    52,     0,    77,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,     0,     0,
    65,    66,    67,    68,    69,    70,    71,    72,     0,    51,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   152,    52,     0,    77,    54,    55,    56,    57,    58,    59,
    60,    61,    62,    63,    64,     0,     0,    65,    66,    67,
    68,    69,    70,    71,    72,     0,    51,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   158,    52,   132,
    77,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,     0,     0,    65,    66,    67,    68,    69,    70,
    71,    72,    51,     0,     0,     0,   142,     0,     0,     0,
     0,     0,     0,     0,    52,   132,    77,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,     0,     0,
    65,    66,    67,    68,    69,    70,    71,    72,    51,     0,
     0,     0,   154,     0,     0,     0,     0,     0,     0,     0,
    52,   132,    77,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,     0,    51,    65,    66,    67,    68,
    69,    70,    71,    72,     0,     0,     0,    52,   156,    77,
    54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
    64,     0,     0,    65,    66,    67,    68,    69,    70,    71,
    72,    51,     0,     0,     0,    87,     0,     0,     0,     0,
     0,     0,     0,    52,   132,    77,    54,    55,    56,    57,
    58,    59,    60,    61,    62,    63,    64,    51,     0,    65,
    66,    67,    68,    69,    70,    71,    72,     0,     0,    52,
   133,    77,    54,    55,    56,    57,    58,    59,    60,    61,
    62,    63,    64,     0,    51,    65,    66,    67,    68,    69,
    70,    71,    72,     0,     0,    83,    52,     0,    53,    54,
    55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
     0,     0,    65,    66,    67,    68,    69,    70,    71,    72,
    51,    73,     0,     0,     0,     0,    80,    81,     0,     0,
     0,     0,    52,     0,    77,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,     0,     0,    65,    66,
    67,    68,    69,    70,    71,    72,    51,     0,     0,     0,
     0,     0,     0,     0,    78,     0,     0,     0,    52,     0,
    77,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,     0,     0,    65,    66,    67,    68,    69,    70,
    71,    72,    51,     0,     0,     0,     0,     0,     0,     0,
    79,     0,     0,     0,    52,     0,    77,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    51,     0,
    65,    66,    67,    68,    69,    70,    71,    72,    82,     0,
    52,     0,    77,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    51,     0,    65,    66,    67,    68,
    69,    70,    71,    72,     0,     0,    52,     0,    77,    54,
    55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
    51,     0,    65,    66,    67,    68,    69,    70,    71,    72,
     0,   137,    52,     0,     0,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,-32768,     0,    65,    66,
    67,    68,    69,    70,    71,    72,     0,     0,    52,     0,
     0,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    51,     0,    65,    66,    67,    68,    69,    70,
    71,    72,     0,     0,    52,     0,     0,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,     0,     0,
    65,    66,    67,    68,    69,    70,    71,    72,    52,     0,
     0,    54,    55,    56,    57,    58,    59,    60,    61,     0,
     0,    64,     0,     0,    65,    66,    67,    68,    69,    70,
    71,    72,    52,     0,     0,    54,    55,    56,    57,    58,
    59,    60,    61,     0,     0,     0,     0,     0,    65,    66,
    67,    68,    69,    70,    71,    72,    52,     0,     0,    54,
-32768,-32768,-32768,    58,    59,    60,    61,     0,     0,     0,
     0,     0,    65,    66,    67,    68,    69,    70,    71,    72,
    52,     0,     0,    54,     0,     0,     0,-32768,-32768,-32768,
-32768,    52,     0,     0,    54,     0,    65,    66,    67,    68,
    69,    70,    71,    72,     0,     0,     0,    65,    66,    67,
    68,    69,    70,    71,    72
};

static const short yycheck[] = {     2,
    92,     8,    22,    22,    52,    47,     1,    10,     3,    47,
    22,    59,    15,    16,    17,    18,    19,    20,    49,    50,
    23,    47,    25,    43,    44,    45,    45,     9,    40,    41,
    42,    43,    44,    45,    49,    50,    49,    50,     8,    21,
    49,   133,   134,    50,    26,    53,    22,    49,    50,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    49,    53,    72,
    74,    75,    53,    53,    77,    78,    79,    80,    81,    82,
    57,    84,    86,    49,    57,    50,    50,    91,    50,    92,
     0,     0,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     4,     5,     6,     7,     8,     9,    -1,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,   132,
   133,   134,    -1,    -1,   137,    36,    -1,   140,    39,    40,
    -1,    -1,    -1,    -1,   126,    -1,    47,    48,    -1,    -1,
    51,    -1,    53,    54,    55,    56,     6,     7,     8,     9,
    -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    39,
    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
    -1,    51,    -1,    53,    54,    55,    56,     6,     7,     8,
     9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
    -1,    -1,    51,    -1,    53,    54,    55,    56,     6,     7,
     8,     9,    -1,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    48,    -1,    50,    51,    -1,    53,    54,    55,    56,     6,
     7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    48,    -1,    50,    51,    -1,    53,    54,    55,    56,
     6,     7,     8,     9,    -1,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    48,    -1,    50,    51,    -1,    53,    54,    55,
    56,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    48,    -1,    50,    51,    -1,    53,    54,
    55,    56,     6,     7,     8,     9,    -1,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    48,    -1,    50,    51,    -1,    53,
    54,    55,    56,     6,     7,     8,     9,    -1,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    36,    -1,    -1,    39,    40,    -1,    -1,
    -1,    -1,    10,    -1,    -1,    48,    -1,    -1,    51,    -1,
    53,    54,    55,    56,    22,    -1,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
    38,    39,    40,    41,    42,    43,    44,    45,    -1,    10,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    58,    22,    -1,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    -1,    -1,    38,    39,    40,
    41,    42,    43,    44,    45,    -1,    10,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    -1,    -1,    38,    39,    40,    41,    42,    43,
    44,    45,    10,    -1,    -1,    -1,    50,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
    38,    39,    40,    41,    42,    43,    44,    45,    10,    -1,
    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    -1,    10,    38,    39,    40,    41,
    42,    43,    44,    45,    -1,    -1,    -1,    22,    50,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    -1,    -1,    38,    39,    40,    41,    42,    43,    44,
    45,    10,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    10,    -1,    38,
    39,    40,    41,    42,    43,    44,    45,    -1,    -1,    22,
    49,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    -1,    10,    38,    39,    40,    41,    42,
    43,    44,    45,    -1,    -1,    48,    22,    -1,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
    10,    47,    -1,    -1,    -1,    -1,    16,    17,    -1,    -1,
    -1,    -1,    22,    -1,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    -1,    -1,    38,    39,
    40,    41,    42,    43,    44,    45,    10,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    -1,    -1,    38,    39,    40,    41,    42,    43,
    44,    45,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    18,    -1,    -1,    -1,    22,    -1,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    10,    -1,
    38,    39,    40,    41,    42,    43,    44,    45,    20,    -1,
    22,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    10,    -1,    38,    39,    40,    41,
    42,    43,    44,    45,    -1,    -1,    22,    -1,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    10,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
    -1,    21,    22,    -1,    -1,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    10,    -1,    38,    39,
    40,    41,    42,    43,    44,    45,    -1,    -1,    22,    -1,
    -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    10,    -1,    38,    39,    40,    41,    42,    43,
    44,    45,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
    38,    39,    40,    41,    42,    43,    44,    45,    22,    -1,
    -1,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
    -1,    35,    -1,    -1,    38,    39,    40,    41,    42,    43,
    44,    45,    22,    -1,    -1,    25,    26,    27,    28,    29,
    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
    40,    41,    42,    43,    44,    45,    22,    -1,    -1,    25,
    26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
    -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
    22,    -1,    -1,    25,    -1,    -1,    -1,    29,    30,    31,
    32,    22,    -1,    -1,    25,    -1,    38,    39,    40,    41,
    42,    43,    44,    45,    -1,    -1,    -1,    38,    39,    40,
    41,    42,    43,    44,    45
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
#line 340 "parse.y"
{ YYACCEPT; ;
    break;}
case 2:
#line 341 "parse.y"
{ loadfile = yyvsp[-1].id; YYACCEPT; ;
    break;}
case 3:
#line 342 "parse.y"
{ loadfile_glob = yyvsp[-1].id; YYACCEPT; ;
    break;}
case 4:
#line 343 "parse.y"
{ YYACCEPT; ;
    break;}
case 5:
#line 344 "parse.y"
{ push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; ;
    break;}
case 6:
#line 345 "parse.y"
{ return_ret = TRUE; yyclearin; YYABORT; ;
    break;}
case 7:
#line 346 "parse.y"
{ return_ret = TRUE; ;
    break;}
case 8:
#line 349 "parse.y"
{ PUSH_ACT(E_SEPAR); ;
    break;}
case 9:
#line 350 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 10:
#line 351 "parse.y"
{ PUSH_ACT(E_ABS); ;
    break;}
case 11:
#line 352 "parse.y"
{ PUSH_ACT(E_PLUS); ;
    break;}
case 12:
#line 353 "parse.y"
{ PUSH_ACT(E_MINUS); ;
    break;}
case 13:
#line 354 "parse.y"
{ PUSH_ACT(E_MUL); ;
    break;}
case 14:
#line 355 "parse.y"
{ PUSH_ACT(E_DIV); ;
    break;}
case 15:
#line 356 "parse.y"
{ PUSH_ACT(E_MOD); ;
    break;}
case 16:
#line 357 "parse.y"
{ PUSH_ACT(E_EQ_CMP); ;
    break;}
case 17:
#line 358 "parse.y"
{ PUSH_ACT(E_NE_CMP); ;
    break;}
case 18:
#line 359 "parse.y"
{ PUSH_ACT(E_CMP_CMP); ;
    break;}
case 19:
#line 360 "parse.y"
{ PUSH_ACT(E_LT_CMP); ;
    break;}
case 20:
#line 361 "parse.y"
{ PUSH_ACT(E_GT_CMP); ;
    break;}
case 21:
#line 362 "parse.y"
{ PUSH_ACT(E_LE_CMP); ;
    break;}
case 22:
#line 363 "parse.y"
{ PUSH_ACT(E_GE_CMP); ;
    break;}
case 23:
#line 364 "parse.y"
{ PUSH_ACT(E_LOGICAL_AND); ;
    break;}
case 24:
#line 365 "parse.y"
{ PUSH_ACT(E_LOGICAL_OR); ;
    break;}
case 25:
#line 366 "parse.y"
{ PUSH_ACT(E_LOGICAL_XOR); ;
    break;}
case 26:
#line 367 "parse.y"
{ PUSH_ACT(E_LOGICAL_NOT); ;
    break;}
case 27:
#line 368 "parse.y"
{ PUSH_ACT(E_FACT); ;
    break;}
case 28:
#line 369 "parse.y"
{ PUSH_ACT(E_TRANSPOSE); ;
    break;}
case 29:
#line 370 "parse.y"
{ PUSH_ACT(E_NEG); ;
    break;}
case 30:
#line 371 "parse.y"
{ PUSH_ACT(E_EXP); ;
    break;}
case 31:
#line 372 "parse.y"
{ PUSH_ACT(E_GET_ELEMENT); ;
    break;}
case 32:
#line 373 "parse.y"
{ PUSH_ACT(-1/*FIXME:*/); ;
    break;}
case 33:
#line 374 "parse.y"
{ PUSH_ACT(-1/*FIXME:*/); ;
    break;}
case 34:
#line 375 "parse.y"
{ PUSH_ACT(-1/*FIXME:*/); ;
    break;}
case 35:
#line 376 "parse.y"
{ PUSH_ACT(E_GET_ROW); ;
    break;}
case 36:
#line 377 "parse.y"
{ PUSH_ACT(-1/*FIXME:*/); ;
    break;}
case 37:
#line 378 "parse.y"
{ PUSH_ACT(E_GET_COLUMN); ;
    break;}
case 38:
#line 379 "parse.y"
{ PUSH_ACT(-1/*FIXME:*/); ;
    break;}
case 39:
#line 380 "parse.y"
{ if(!push_matrix()) {SYNTAX_ERROR;} ;
    break;}
case 40:
#line 381 "parse.y"
{ PUSH_ACT(E_WHILE_CONS); ;
    break;}
case 41:
#line 382 "parse.y"
{ PUSH_ACT(E_UNTIL_CONS); ;
    break;}
case 42:
#line 383 "parse.y"
{ PUSH_ACT(E_DOWHILE_CONS); ;
    break;}
case 43:
#line 384 "parse.y"
{ PUSH_ACT(E_DOUNTIL_CONS); ;
    break;}
case 44:
#line 385 "parse.y"
{ PUSH_ACT(E_IF_CONS); ;
    break;}
case 45:
#line 386 "parse.y"
{ PUSH_ACT(E_IFELSE_CONS); ;
    break;}
case 48:
#line 389 "parse.y"
{ PUSH_ACT(E_REFERENCE); ;
    break;}
case 50:
#line 391 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 51:
#line 393 "parse.y"
{ PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 52:
#line 394 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 53:
#line 396 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 54:
#line 397 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 55:
#line 398 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 56:
#line 400 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 58:
#line 402 "parse.y"
{ PUSH_ACT(E_RETURN); ;
    break;}
case 59:
#line 403 "parse.y"
{ PUSH_ACT(E_BAILOUT); ;
    break;}
case 60:
#line 404 "parse.y"
{ PUSH_ACT(E_EXCEPTION); ;
    break;}
case 61:
#line 405 "parse.y"
{ PUSH_ACT(E_CONTINUE); ;
    break;}
case 62:
#line 406 "parse.y"
{ PUSH_ACT(E_BREAK); ;
    break;}
case 63:
#line 407 "parse.y"
{ stack_push(&evalstack,
						     makenum_use(yyvsp[0].val)); ;
    break;}
case 64:
#line 409 "parse.y"
{ PUSH_STRING(yyvsp[0].id); ;
    break;}
case 65:
#line 410 "parse.y"
{ push_null(); ;
    break;}
case 66:
#line 413 "parse.y"
{ PUSH_ACT(E_DEREFERENCE); ;
    break;}
case 67:
#line 416 "parse.y"
{ PUSH_IDENTIFIER(yyvsp[0].id); g_free(yyvsp[0].id); ;
    break;}
case 68:
#line 419 "parse.y"
{ if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 69:
#line 420 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;}
						  if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 71:
#line 425 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 73:
#line 429 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 74:
#line 432 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} ;
    break;}
case 75:
#line 433 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} if(!push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 76:
#line 436 "parse.y"
{/*FIXME:*/;;
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
#line 439 "parse.y"

