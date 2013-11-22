
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
#define	FUNCTION	264
#define	CALL	265
#define	RETURNTOK	266
#define	BAILOUT	267
#define	EXCEPTION	268
#define	CONTINUE	269
#define	BREAK	270
#define	WHILE	271
#define	UNTIL	272
#define	FOR	273
#define	DO	274
#define	IF	275
#define	THEN	276
#define	ELSE	277
#define	TO	278
#define	BY	279
#define	IN	280
#define	AT	281
#define	REGION_SEP	282
#define	SEPAR	283
#define	EQUALS	284
#define	EQ_CMP	285
#define	NE_CMP	286
#define	CMP_CMP	287
#define	LT_CMP	288
#define	GT_CMP	289
#define	LE_CMP	290
#define	GE_CMP	291
#define	LOGICAL_XOR	292
#define	LOGICAL_OR	293
#define	LOGICAL_AND	294
#define	LOGICAL_NOT	295
#define	LOWER_THEN_ELSE	296
#define	UMINUS	297
#define	UPLUS	298

#line 21 "parse.y"

#include <glib.h>
#include <string.h>
#include "structs.h"
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "util.h"
#include "calc.h"
#include "matrix.h"
#include "matrixw.h"
	
#include "parseutil.h"

extern evalstack_t evalstack;
extern ETree *free_trees;

extern int return_ret; /*should the lexer return on \n*/
extern char *loadfile;
extern char *loadfile_glob;


#line 44 "parse.y"
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



#define	YYFINAL		193
#define	YYFLAG		-32768
#define	YYNTBASE	65

#define YYTRANSLATE(x) ((unsigned)(x) <= 298 ? yytranslate[x] : 74)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    52,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    48,     2,     2,     2,    46,    62,    47,    53,
    54,    44,    42,    56,    43,    63,    45,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    64,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    57,     2,    58,    49,     2,    59,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    60,    55,    61,     2,     2,     2,     2,     2,
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
    36,    37,    38,    39,    40,    41,    50,    51
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     8,    12,    15,    20,    23,    25,    29,    34,
    38,    42,    46,    50,    54,    58,    62,    66,    70,    74,
    78,    82,    86,    90,    94,    98,   102,   106,   109,   112,
   115,   118,   121,   125,   130,   137,   144,   151,   158,   164,
   170,   176,   182,   186,   191,   197,   201,   206,   211,   216,
   221,   226,   235,   246,   253,   258,   265,   267,   270,   272,
   276,   281,   285,   290,   296,   301,   305,   308,   311,   314,
   316,   318,   320,   322,   324,   326,   328,   331,   333,   339,
   344,   348,   350,   354,   356,   360,   362
};

static const short yyrhs[] = {     3,
    66,    52,     0,     3,     4,    52,     0,     3,     5,    52,
     0,     3,    52,     0,     3,    66,    28,    52,     0,     1,
    52,     0,     1,     0,    66,    28,    66,     0,    53,    66,
    28,    54,     0,    53,    66,    54,     0,    66,    29,    66,
     0,    55,    66,    55,     0,    66,    42,    66,     0,    66,
    43,    66,     0,    66,    44,    66,     0,    66,    45,    66,
     0,    66,    46,    66,     0,    66,    32,    66,     0,    66,
    30,    66,     0,    66,    31,    66,     0,    66,    33,    66,
     0,    66,    34,    66,     0,    66,    35,    66,     0,    66,
    36,    66,     0,    66,    39,    66,     0,    66,    38,    66,
     0,    66,    37,    66,     0,    40,    66,     0,    66,    48,
     0,    66,    47,     0,    43,    66,     0,    42,    66,     0,
    66,    49,    66,     0,    66,    26,    66,    54,     0,    66,
    26,    66,    56,    66,    54,     0,    66,    26,    73,    56,
    66,    54,     0,    66,    26,    66,    56,    73,    54,     0,
    66,    26,    73,    56,    73,    54,     0,    66,    26,    66,
    56,    54,     0,    66,    26,    73,    56,    54,     0,    66,
    26,    56,    66,    54,     0,    66,    26,    56,    73,    54,
     0,    57,    72,    58,     0,    59,    57,    72,    58,     0,
    53,    66,    56,    71,    54,     0,    60,    71,    61,     0,
    59,    60,    71,    61,     0,    16,    66,    19,    66,     0,
    17,    66,    19,    66,     0,    19,    66,    16,    66,     0,
    19,    66,    17,    66,     0,    18,    68,    29,    66,    23,
    66,    19,    66,     0,    18,    68,    29,    66,    23,    66,
    24,    66,    19,    66,     0,    18,    68,    25,    66,    19,
    66,     0,    20,    66,    21,    66,     0,    20,    66,    21,
    66,    22,    66,     0,    68,     0,    62,    68,     0,    67,
     0,    68,    53,    54,     0,    68,    53,    71,    54,     0,
    67,    53,    54,     0,    67,    53,    71,    54,     0,    66,
    10,    53,    71,    54,     0,    66,    10,    53,    54,     0,
     9,    68,    69,     0,     9,    69,     0,    59,    69,     0,
    11,    66,     0,    12,     0,    13,     0,    14,     0,    15,
     0,     6,     0,     7,     0,    63,     0,    44,    68,     0,
     8,     0,    53,    70,    54,    29,    66,     0,    53,    54,
    29,    66,     0,    70,    56,    68,     0,    68,     0,    71,
    56,    66,     0,    66,     0,    72,    64,    71,     0,    71,
     0,    66,    27,    66,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    98,    99,   100,   101,   102,   103,   104,   107,   108,   110,
   111,   112,   113,   114,   115,   116,   117,   118,   120,   121,
   122,   123,   124,   125,   127,   128,   129,   130,   131,   132,
   133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
   143,   144,   145,   146,   153,   154,   155,   156,   157,   158,
   159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
   170,   171,   173,   174,   175,   177,   178,   179,   180,   181,
   182,   183,   184,   185,   187,   188,   191,   194,   197,   198,
   204,   205,   208,   209,   212,   213,   216
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STARTTOK",
"LOADFILE","LOADFILE_GLOB","NUMBER","STRING","FUNCID","FUNCTION","CALL","RETURNTOK",
"BAILOUT","EXCEPTION","CONTINUE","BREAK","WHILE","UNTIL","FOR","DO","IF","THEN",
"ELSE","TO","BY","IN","AT","REGION_SEP","SEPAR","EQUALS","EQ_CMP","NE_CMP","CMP_CMP",
"LT_CMP","GT_CMP","LE_CMP","GE_CMP","LOGICAL_XOR","LOGICAL_OR","LOGICAL_AND",
"LOGICAL_NOT","LOWER_THEN_ELSE","'+'","'-'","'*'","'/'","'%'","'''","'!'","'^'",
"UMINUS","UPLUS","'\\n'","'('","')'","'|'","','","'['","']'","'`'","'{'","'}'",
"'&'","'.'","':'","fullexpr","expr","deref","ident","funcdef","identlist","exprlist",
"matrixrows","reg", NULL
};
#endif

static const short yyr1[] = {     0,
    65,    65,    65,    65,    65,    65,    65,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
    66,    66,    66,    66,    66,    66,    67,    68,    69,    69,
    70,    70,    71,    71,    72,    72,    73
};

static const short yyr2[] = {     0,
     3,     3,     3,     2,     4,     2,     1,     3,     4,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
     2,     2,     3,     4,     6,     6,     6,     6,     5,     5,
     5,     5,     3,     4,     5,     3,     4,     4,     4,     4,
     4,     8,    10,     6,     4,     6,     1,     2,     1,     3,
     4,     3,     4,     5,     4,     3,     2,     2,     2,     1,
     1,     1,     1,     1,     1,     1,     2,     1,     5,     4,
     3,     1,     3,     1,     3,     1,     3
};

static const short yydefact[] = {     0,
     7,     0,     6,     0,     0,    74,    75,    78,     0,     0,
    70,    71,    72,    73,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     4,     0,     0,     0,     0,     0,     0,
    76,     0,    59,    57,     2,     3,     0,     0,    67,    69,
     0,     0,     0,     0,     0,    28,    32,    31,    77,     0,
     0,    84,    86,     0,     0,     0,    68,     0,    58,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    30,    29,
     0,     1,     0,     0,     0,    82,     0,    66,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    10,     0,    12,
     0,    43,     0,     0,     0,    46,     0,     0,     0,     0,
     5,     8,    11,    19,    20,    18,    21,    22,    23,    24,
    27,    26,    25,    13,    14,    15,    16,    17,    33,    62,
     0,    60,     0,     0,     0,     0,    48,    49,     0,     0,
    50,    51,    55,     9,     0,    83,    85,    44,    47,    65,
     0,     0,     0,     0,    34,     0,     0,    63,    61,    80,
     0,    81,     0,     0,     0,    45,    64,    41,    42,    87,
    39,     0,     0,    40,     0,     0,    79,    54,     0,    56,
    35,    37,    36,    38,     0,     0,    52,     0,     0,    53,
     0,     0,     0
};

static const short yydefgoto[] = {   191,
    52,    33,    34,    39,    87,    53,    54,   110
};

static const short yypact[] = {    53,
   -46,   160,-32768,   -27,   -24,-32768,-32768,-32768,    -1,   682,
-32768,-32768,-32768,-32768,   682,   682,    39,   682,   682,   682,
   682,   682,    39,-32768,   682,   682,   682,   -16,   682,    39,
-32768,   902,     5,    24,-32768,-32768,    -5,    25,-32768,  1247,
  1004,  1035,    13,   942,  1065,   221,    10,    10,-32768,   751,
   782,  1187,    41,   -19,   682,   682,-32768,   -51,-32768,    45,
   218,   276,   682,   682,   682,   682,   682,   682,   682,   682,
   682,   682,   682,   682,   682,   682,   682,   682,-32768,-32768,
   682,-32768,   334,   392,    73,-32768,     1,-32768,   682,   682,
   682,   682,   682,   682,   682,   682,   450,-32768,   682,-32768,
   682,-32768,   682,   -18,   -13,-32768,   508,   682,   720,    49,
-32768,  1277,   221,   395,   395,   337,   395,   395,   395,   395,
    91,    91,   221,   -14,   -14,    60,    60,    60,   -22,-32768,
    28,-32768,    29,   682,    82,    39,  1247,  1247,  1096,  1126,
  1247,  1247,  1217,-32768,    47,  1187,    41,-32768,-32768,-32768,
    56,   812,    59,   682,-32768,   566,   624,-32768,-32768,   221,
   682,-32768,   682,   682,   682,-32768,-32768,-32768,-32768,  1187,
-32768,   842,    61,-32768,   872,    62,   221,  1247,   973,  1247,
-32768,-32768,-32768,-32768,   682,   682,  1247,  1157,   682,  1247,
   114,   118,-32768
};

static const short yypgoto[] = {-32768,
    -2,-32768,    -8,   -17,-32768,    -3,    64,  -106
};


#define	YYLAST		1326


static const short yytable[] = {    32,
    38,   153,     8,    61,   101,     3,     8,    40,    43,   106,
    57,    61,    41,    42,    49,    44,    45,    46,    47,    48,
    88,    59,    50,    51,    35,    58,    81,    36,    86,    76,
    77,    78,    79,    80,    81,    61,    37,    92,   102,   148,
    55,    93,   101,    56,   103,   103,     8,   149,    85,   173,
   176,    37,   105,     1,   135,     2,   136,    83,   109,   112,
   113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,    84,    37,   129,   131,
   133,   158,   159,   101,   101,    61,   112,   137,   138,   139,
   140,   141,   142,   143,   112,   145,   101,   107,   146,   147,
   166,   134,   101,   151,   157,   152,    79,    80,    81,   167,
   161,   101,   169,   192,   182,   184,    61,   193,   104,    63,
    64,    65,    66,    67,    68,    69,    70,   162,     0,    73,
     0,   160,    74,    75,    76,    77,    78,    79,    80,    81,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   170,     0,   172,   175,     0,     0,     0,   177,     0,
   178,   179,   180,     4,     5,     6,     7,     8,     9,     0,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     0,     0,   187,   188,     0,     0,   190,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    20,
     0,    21,    22,    23,     0,     0,     0,     0,     0,     0,
     0,    24,    25,     0,    26,     0,    27,     0,    28,    29,
     0,    30,    31,     6,     7,     8,     9,     0,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
     0,     0,     0,     0,     0,     0,    61,     0,     0,    63,
    64,    65,    66,    67,    68,    69,    70,    20,     0,    21,
    22,    23,    74,    75,    76,    77,    78,    79,    80,    81,
    25,     0,    26,   108,    27,     0,    28,    29,     0,    30,
    31,     6,     7,     8,     9,     0,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    20,     0,    21,    22,    23,
     0,     0,     0,     0,     0,     0,     0,   111,    25,     0,
    26,     0,    27,     0,    28,    29,     0,    30,    31,     6,
     7,     8,     9,     0,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
     0,     0,    61,     0,     0,     0,    64,    65,-32768,    67,
    68,    69,    70,    20,     0,    21,    22,    23,    74,    75,
    76,    77,    78,    79,    80,    81,    25,   130,    26,     0,
    27,     0,    28,    29,     0,    30,    31,     6,     7,     8,
     9,     0,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
    61,     0,     0,     0,    64,    65,     0,    67,    68,    69,
    70,    20,     0,    21,    22,    23,    74,    75,    76,    77,
    78,    79,    80,    81,    25,   132,    26,     0,    27,     0,
    28,    29,     0,    30,    31,     6,     7,     8,     9,     0,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    20,
     0,    21,    22,    23,     0,     0,     0,     0,     0,     0,
     0,     0,    25,   144,    26,     0,    27,     0,    28,    29,
     0,    30,    31,     6,     7,     8,     9,     0,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    20,     0,    21,
    22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
    25,   150,    26,     0,    27,     0,    28,    29,     0,    30,
    31,     6,     7,     8,     9,     0,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    20,     0,    21,    22,    23,
     0,     0,     0,     0,     0,     0,     0,     0,    25,   171,
    26,     0,    27,     0,    28,    29,     0,    30,    31,     6,
     7,     8,     9,     0,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    20,     0,    21,    22,    23,     0,     0,
     0,     0,     0,     0,     0,     0,    25,   174,    26,     0,
    27,     0,    28,    29,     0,    30,    31,     6,     7,     8,
     9,     0,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    20,     0,    21,    22,    23,     0,     0,     0,    60,
     0,     0,     0,     0,    25,     0,    26,     0,    27,     0,
    28,    29,     0,    30,    31,    61,   154,    89,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
    60,    74,    75,    76,    77,    78,    79,    80,    81,     0,
     0,     0,     0,   155,     0,   156,    61,     0,    97,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
     0,    60,    74,    75,    76,    77,    78,    79,    80,    81,
     0,     0,     0,     0,    98,     0,    99,    61,     0,    89,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    60,     0,    74,    75,    76,    77,    78,    79,    80,
    81,     0,     0,     0,     0,     0,   100,    61,   154,    89,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    60,     0,    74,    75,    76,    77,    78,    79,    80,
    81,     0,     0,     0,     0,   168,     0,    61,   154,    89,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    60,     0,    74,    75,    76,    77,    78,    79,    80,
    81,     0,     0,     0,     0,   181,     0,    61,   154,    89,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    60,     0,    74,    75,    76,    77,    78,    79,    80,
    81,     0,     0,     0,     0,   183,     0,    61,     0,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,     0,     0,    74,    75,    76,    77,    78,    79,    80,
    81,    60,     0,    82,     0,     0,     0,    94,    95,     0,
     0,     0,     0,     0,     0,     0,     0,    61,     0,    89,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,     0,    60,    74,    75,    76,    77,    78,    79,    80,
    81,   185,     0,     0,     0,     0,   186,     0,    61,     0,
    89,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,     0,    60,    74,    75,    76,    77,    78,    79,
    80,    81,    90,     0,     0,     0,     0,     0,     0,    61,
     0,    89,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,     0,    60,    74,    75,    76,    77,    78,
    79,    80,    81,    91,     0,     0,     0,     0,     0,     0,
    61,     0,    89,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    60,     0,    74,    75,    76,    77,
    78,    79,    80,    81,     0,    96,     0,     0,     0,     0,
    61,     0,    89,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,     0,    60,    74,    75,    76,    77,
    78,    79,    80,    81,   163,     0,     0,     0,     0,     0,
     0,    61,     0,    89,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    60,     0,    74,    75,    76,
    77,    78,    79,    80,    81,     0,     0,     0,   164,     0,
     0,    61,     0,    89,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,     0,    60,    74,    75,    76,
    77,    78,    79,    80,    81,   189,     0,     0,     0,     0,
     0,     0,    61,     0,    89,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    60,     0,    74,    75,
    76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
     0,     0,    61,     0,    89,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    60,     0,    74,    75,
    76,    77,    78,    79,    80,    81,     0,     0,   165,     0,
     0,     0,    61,     0,     0,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,-32768,     0,    74,    75,
    76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
     0,     0,    61,     0,     0,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    60,     0,    74,    75,
    76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
     0,     0,    61,     0,     0,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,     0,     0,    74,    75,
    76,    77,    78,    79,    80,    81
};

static const short yycheck[] = {     2,
     9,   108,     8,    26,    56,    52,     8,    10,    17,    61,
    28,    26,    15,    16,    23,    18,    19,    20,    21,    22,
    38,    30,    25,    26,    52,    29,    49,    52,    37,    44,
    45,    46,    47,    48,    49,    26,    53,    25,    58,    58,
    57,    29,    56,    60,    64,    64,     8,    61,    54,   156,
   157,    53,    56,     1,    54,     3,    56,    53,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    53,    53,    81,    83,
    84,    54,    54,    56,    56,    26,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    99,    56,    53,   101,   103,
    54,    29,    56,   107,    56,   108,    47,    48,    49,    54,
    29,    56,    54,     0,    54,    54,    26,     0,    55,    29,
    30,    31,    32,    33,    34,    35,    36,   136,    -1,    39,
    -1,   134,    42,    43,    44,    45,    46,    47,    48,    49,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   154,    -1,   156,   157,    -1,    -1,    -1,   161,    -1,
   163,   164,   165,     4,     5,     6,     7,     8,     9,    -1,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    -1,    -1,   185,   186,    -1,    -1,   189,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    52,    53,    -1,    55,    -1,    57,    -1,    59,    60,
    -1,    62,    63,     6,     7,     8,     9,    -1,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    29,
    30,    31,    32,    33,    34,    35,    36,    40,    -1,    42,
    43,    44,    42,    43,    44,    45,    46,    47,    48,    49,
    53,    -1,    55,    56,    57,    -1,    59,    60,    -1,    62,
    63,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    43,    44,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,
    55,    -1,    57,    -1,    59,    60,    -1,    62,    63,     6,
     7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    26,    -1,    -1,    -1,    30,    31,    32,    33,
    34,    35,    36,    40,    -1,    42,    43,    44,    42,    43,
    44,    45,    46,    47,    48,    49,    53,    54,    55,    -1,
    57,    -1,    59,    60,    -1,    62,    63,     6,     7,     8,
     9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    26,    -1,    -1,    -1,    30,    31,    -1,    33,    34,    35,
    36,    40,    -1,    42,    43,    44,    42,    43,    44,    45,
    46,    47,    48,    49,    53,    54,    55,    -1,    57,    -1,
    59,    60,    -1,    62,    63,     6,     7,     8,     9,    -1,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    53,    54,    55,    -1,    57,    -1,    59,    60,
    -1,    62,    63,     6,     7,     8,     9,    -1,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,
    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    53,    54,    55,    -1,    57,    -1,    59,    60,    -1,    62,
    63,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    43,    44,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,
    55,    -1,    57,    -1,    59,    60,    -1,    62,    63,     6,
     7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    40,    -1,    42,    43,    44,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,
    57,    -1,    59,    60,    -1,    62,    63,     6,     7,     8,
     9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    40,    -1,    42,    43,    44,    -1,    -1,    -1,    10,
    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
    59,    60,    -1,    62,    63,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
    10,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
    -1,    -1,    -1,    54,    -1,    56,    26,    -1,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    -1,    10,    42,    43,    44,    45,    46,    47,    48,    49,
    -1,    -1,    -1,    -1,    54,    -1,    56,    26,    -1,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    10,    -1,    42,    43,    44,    45,    46,    47,    48,
    49,    -1,    -1,    -1,    -1,    -1,    55,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    10,    -1,    42,    43,    44,    45,    46,    47,    48,
    49,    -1,    -1,    -1,    -1,    54,    -1,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    10,    -1,    42,    43,    44,    45,    46,    47,    48,
    49,    -1,    -1,    -1,    -1,    54,    -1,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    10,    -1,    42,    43,    44,    45,    46,    47,    48,
    49,    -1,    -1,    -1,    -1,    54,    -1,    26,    -1,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
    49,    10,    -1,    52,    -1,    -1,    -1,    16,    17,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    -1,    10,    42,    43,    44,    45,    46,    47,    48,
    49,    19,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    -1,    10,    42,    43,    44,    45,    46,    47,
    48,    49,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
    -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    -1,    10,    42,    43,    44,    45,    46,
    47,    48,    49,    19,    -1,    -1,    -1,    -1,    -1,    -1,
    26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    10,    -1,    42,    43,    44,    45,
    46,    47,    48,    49,    -1,    21,    -1,    -1,    -1,    -1,
    26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    -1,    10,    42,    43,    44,    45,
    46,    47,    48,    49,    19,    -1,    -1,    -1,    -1,    -1,
    -1,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    10,    -1,    42,    43,    44,
    45,    46,    47,    48,    49,    -1,    -1,    -1,    23,    -1,
    -1,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    -1,    10,    42,    43,    44,
    45,    46,    47,    48,    49,    19,    -1,    -1,    -1,    -1,
    -1,    -1,    26,    -1,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    10,    -1,    42,    43,
    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,    26,    -1,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    10,    -1,    42,    43,
    44,    45,    46,    47,    48,    49,    -1,    -1,    22,    -1,
    -1,    -1,    26,    -1,    -1,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    10,    -1,    42,    43,
    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,    26,    -1,    -1,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    10,    -1,    42,    43,
    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,
    -1,    -1,    26,    -1,    -1,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    -1,    -1,    42,    43,
    44,    45,    46,    47,    48,    49
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
#line 98 "parse.y"
{ YYACCEPT; ;
    break;}
case 2:
#line 99 "parse.y"
{ loadfile = yyvsp[-1].id; YYACCEPT; ;
    break;}
case 3:
#line 100 "parse.y"
{ loadfile_glob = yyvsp[-1].id; YYACCEPT; ;
    break;}
case 4:
#line 101 "parse.y"
{ YYACCEPT; ;
    break;}
case 5:
#line 102 "parse.y"
{ push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; ;
    break;}
case 6:
#line 103 "parse.y"
{ return_ret = TRUE; yyclearin; YYABORT; ;
    break;}
case 7:
#line 104 "parse.y"
{ return_ret = TRUE; ;
    break;}
case 8:
#line 107 "parse.y"
{ PUSH_ACT(E_SEPAR); ;
    break;}
case 9:
#line 108 "parse.y"
{ push_null(); PUSH_ACT(E_SEPAR);
					  push_spacer(); ;
    break;}
case 10:
#line 110 "parse.y"
{ push_spacer(); ;
    break;}
case 11:
#line 111 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 12:
#line 112 "parse.y"
{ PUSH_ACT(E_ABS); ;
    break;}
case 13:
#line 113 "parse.y"
{ PUSH_ACT(E_PLUS); ;
    break;}
case 14:
#line 114 "parse.y"
{ PUSH_ACT(E_MINUS); ;
    break;}
case 15:
#line 115 "parse.y"
{ PUSH_ACT(E_MUL); ;
    break;}
case 16:
#line 116 "parse.y"
{ PUSH_ACT(E_DIV); ;
    break;}
case 17:
#line 117 "parse.y"
{ PUSH_ACT(E_MOD); ;
    break;}
case 18:
#line 118 "parse.y"
{ PUSH_ACT(E_CMP_CMP); ;
    break;}
case 19:
#line 120 "parse.y"
{ PUSH_ACT(E_EQ_CMP); ;
    break;}
case 20:
#line 121 "parse.y"
{ PUSH_ACT(E_NE_CMP); ;
    break;}
case 21:
#line 122 "parse.y"
{ PUSH_ACT(E_LT_CMP); ;
    break;}
case 22:
#line 123 "parse.y"
{ PUSH_ACT(E_GT_CMP); ;
    break;}
case 23:
#line 124 "parse.y"
{ PUSH_ACT(E_LE_CMP); ;
    break;}
case 24:
#line 125 "parse.y"
{ PUSH_ACT(E_GE_CMP); ;
    break;}
case 25:
#line 127 "parse.y"
{ PUSH_ACT(E_LOGICAL_AND); ;
    break;}
case 26:
#line 128 "parse.y"
{ PUSH_ACT(E_LOGICAL_OR); ;
    break;}
case 27:
#line 129 "parse.y"
{ PUSH_ACT(E_LOGICAL_XOR); ;
    break;}
case 28:
#line 130 "parse.y"
{ PUSH_ACT(E_LOGICAL_NOT); ;
    break;}
case 29:
#line 131 "parse.y"
{ PUSH_ACT(E_FACT); ;
    break;}
case 30:
#line 132 "parse.y"
{ PUSH_ACT(E_TRANSPOSE); ;
    break;}
case 31:
#line 133 "parse.y"
{ PUSH_ACT(E_NEG); ;
    break;}
case 33:
#line 135 "parse.y"
{ PUSH_ACT(E_EXP); ;
    break;}
case 34:
#line 136 "parse.y"
{ PUSH_ACT(E_GET_VELEMENT); ;
    break;}
case 35:
#line 137 "parse.y"
{ PUSH_ACT(E_GET_ELEMENT); ;
    break;}
case 36:
#line 138 "parse.y"
{ PUSH_ACT(E_GET_REGION); ;
    break;}
case 37:
#line 139 "parse.y"
{ PUSH_ACT(E_GET_REGION); ;
    break;}
case 38:
#line 140 "parse.y"
{ PUSH_ACT(E_GET_REGION); ;
    break;}
case 39:
#line 141 "parse.y"
{ PUSH_ACT(E_GET_ROW_REGION); ;
    break;}
case 40:
#line 142 "parse.y"
{ PUSH_ACT(E_GET_ROW_REGION); ;
    break;}
case 41:
#line 143 "parse.y"
{ PUSH_ACT(E_GET_COL_REGION); ;
    break;}
case 42:
#line 144 "parse.y"
{ PUSH_ACT(E_GET_COL_REGION); ;
    break;}
case 43:
#line 145 "parse.y"
{ if(!push_matrix(FALSE)) {SYNTAX_ERROR;} ;
    break;}
case 44:
#line 146 "parse.y"
{ if(!push_matrix(TRUE)) {SYNTAX_ERROR;} ;
    break;}
case 45:
#line 153 "parse.y"
{SYNTAX_ERROR;;
    break;}
case 46:
#line 154 "parse.y"
{SYNTAX_ERROR;;
    break;}
case 47:
#line 155 "parse.y"
{SYNTAX_ERROR;;
    break;}
case 48:
#line 156 "parse.y"
{ PUSH_ACT(E_WHILE_CONS); ;
    break;}
case 49:
#line 157 "parse.y"
{ PUSH_ACT(E_UNTIL_CONS); ;
    break;}
case 50:
#line 158 "parse.y"
{ PUSH_ACT(E_DOWHILE_CONS); ;
    break;}
case 51:
#line 159 "parse.y"
{ PUSH_ACT(E_DOUNTIL_CONS); ;
    break;}
case 52:
#line 160 "parse.y"
{ PUSH_ACT(E_FOR_CONS); ;
    break;}
case 53:
#line 161 "parse.y"
{ PUSH_ACT(E_FORBY_CONS); ;
    break;}
case 54:
#line 162 "parse.y"
{ PUSH_ACT(E_FORIN_CONS); ;
    break;}
case 55:
#line 163 "parse.y"
{ PUSH_ACT(E_IF_CONS); ;
    break;}
case 56:
#line 164 "parse.y"
{ PUSH_ACT(E_IFELSE_CONS); ;
    break;}
case 58:
#line 166 "parse.y"
{ PUSH_ACT(E_REFERENCE); ;
    break;}
case 60:
#line 168 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 61:
#line 170 "parse.y"
{ PUSH_ACT(E_DIRECTCALL); ;
    break;}
case 62:
#line 171 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 63:
#line 173 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 64:
#line 174 "parse.y"
{ PUSH_ACT(E_CALL); ;
    break;}
case 65:
#line 175 "parse.y"
{ push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); ;
    break;}
case 66:
#line 177 "parse.y"
{ PUSH_ACT(E_EQUALS); ;
    break;}
case 69:
#line 180 "parse.y"
{ PUSH_ACT(E_RETURN); ;
    break;}
case 70:
#line 181 "parse.y"
{ PUSH_ACT(E_BAILOUT); ;
    break;}
case 71:
#line 182 "parse.y"
{ PUSH_ACT(E_EXCEPTION); ;
    break;}
case 72:
#line 183 "parse.y"
{ PUSH_ACT(E_CONTINUE); ;
    break;}
case 73:
#line 184 "parse.y"
{ PUSH_ACT(E_BREAK); ;
    break;}
case 74:
#line 185 "parse.y"
{ stack_push(&evalstack,
						     makenum_use(yyvsp[0].val)); ;
    break;}
case 75:
#line 187 "parse.y"
{ PUSH_STRING(yyvsp[0].id); ;
    break;}
case 76:
#line 188 "parse.y"
{ push_null(); ;
    break;}
case 77:
#line 191 "parse.y"
{ PUSH_ACT(E_DEREFERENCE); ;
    break;}
case 78:
#line 194 "parse.y"
{ PUSH_IDENTIFIER(yyvsp[0].id); g_free(yyvsp[0].id); ;
    break;}
case 79:
#line 197 "parse.y"
{ if(!push_func()) {SYNTAX_ERROR;} ;
    break;}
case 80:
#line 198 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE))
						{SYNTAX_ERROR;}
					  if(!push_func())
					  	{SYNTAX_ERROR;} ;
    break;}
case 82:
#line 205 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 84:
#line 209 "parse.y"
{ if(!push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 85:
#line 212 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} ;
    break;}
case 86:
#line 213 "parse.y"
{ if(!push_matrix_row()) {SYNTAX_ERROR;} if(!push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} ;
    break;}
case 87:
#line 216 "parse.y"
{ PUSH_ACT(E_REGION_SEP); ;
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
#line 219 "parse.y"

