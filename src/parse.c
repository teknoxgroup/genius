/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STARTTOK = 258,
     LOADFILE = 259,
     LOADFILE_GLOB = 260,
     LOAD_PLUGIN = 261,
     CHANGEDIR = 262,
     PWD = 263,
     LS = 264,
     LS_ARG = 265,
     HELP = 266,
     HELP_ARG = 267,
     NUMBER = 268,
     STRING = 269,
     FUNCID = 270,
     FUNCTION = 271,
     CALL = 272,
     THREEDOTS = 273,
     PARAMETER = 274,
     RETURNTOK = 275,
     BAILOUT = 276,
     EXCEPTION = 277,
     CONTINUE = 278,
     BREAK = 279,
     WHILE = 280,
     UNTIL = 281,
     FOR = 282,
     SUM = 283,
     PROD = 284,
     DO = 285,
     IF = 286,
     THEN = 287,
     ELSE = 288,
     TO = 289,
     BY = 290,
     IN = 291,
     AT = 292,
     MAKEIMAGPARENTH = 293,
     SEPAR = 294,
     NEXTROW = 295,
     EQUALS = 296,
     DEFEQUALS = 297,
     TRANSPOSE = 298,
     ELTELTDIV = 299,
     ELTELTMUL = 300,
     ELTELTPLUS = 301,
     ELTELTMINUS = 302,
     ELTELTEXP = 303,
     ELTELTMOD = 304,
     DOUBLEFACT = 305,
     EQ_CMP = 306,
     NE_CMP = 307,
     CMP_CMP = 308,
     LT_CMP = 309,
     GT_CMP = 310,
     LE_CMP = 311,
     GE_CMP = 312,
     LOGICAL_XOR = 313,
     LOGICAL_OR = 314,
     LOGICAL_AND = 315,
     LOGICAL_NOT = 316,
     LOWER_THEN_ELSE = 317,
     MOD = 318,
     ELTELTBACKDIV = 319,
     UPLUS = 320,
     UMINUS = 321
   };
#endif
#define STARTTOK 258
#define LOADFILE 259
#define LOADFILE_GLOB 260
#define LOAD_PLUGIN 261
#define CHANGEDIR 262
#define PWD 263
#define LS 264
#define LS_ARG 265
#define HELP 266
#define HELP_ARG 267
#define NUMBER 268
#define STRING 269
#define FUNCID 270
#define FUNCTION 271
#define CALL 272
#define THREEDOTS 273
#define PARAMETER 274
#define RETURNTOK 275
#define BAILOUT 276
#define EXCEPTION 277
#define CONTINUE 278
#define BREAK 279
#define WHILE 280
#define UNTIL 281
#define FOR 282
#define SUM 283
#define PROD 284
#define DO 285
#define IF 286
#define THEN 287
#define ELSE 288
#define TO 289
#define BY 290
#define IN 291
#define AT 292
#define MAKEIMAGPARENTH 293
#define SEPAR 294
#define NEXTROW 295
#define EQUALS 296
#define DEFEQUALS 297
#define TRANSPOSE 298
#define ELTELTDIV 299
#define ELTELTMUL 300
#define ELTELTPLUS 301
#define ELTELTMINUS 302
#define ELTELTEXP 303
#define ELTELTMOD 304
#define DOUBLEFACT 305
#define EQ_CMP 306
#define NE_CMP 307
#define CMP_CMP 308
#define LT_CMP 309
#define GT_CMP 310
#define LE_CMP 311
#define GE_CMP 312
#define LOGICAL_XOR 313
#define LOGICAL_OR 314
#define LOGICAL_AND 315
#define LOGICAL_NOT 316
#define LOWER_THEN_ELSE 317
#define MOD 318
#define ELTELTBACKDIV 319
#define UPLUS 320
#define UMINUS 321




/* Copy the first part of user declarations.  */
#line 21 "parse.y"

#include "config.h"

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

extern GSList *gel_parsestack;

extern int return_ret; /*should the lexer return on \n*/
extern char *loadfile;
extern char *loadfile_glob;
extern char *changedir;
extern char *changedir_glob;
extern gboolean pwd_command;
extern gboolean ls_command;
extern char *load_plugin;

/* prototype for yylex */
int yylex(void);
void yyerror(char *);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 54 "parse.y"
typedef union YYSTYPE {
	mpw_t val;
	char *id;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 246 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 258 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  46
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2226

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  89
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  11
/* YYNRULES -- Number of rules. */
#define YYNRULES  119
/* YYNRULES -- Number of states. */
#define YYNSTATES  273

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      77,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,     2,     2,    70,    87,    72,
      78,    79,    67,    65,    81,    66,    88,    68,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    82,    69,    83,    75,     2,    84,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    85,    80,    86,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    71,
      73,    74
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    31,
      35,    39,    43,    46,    51,    54,    56,    60,    64,    69,
      73,    77,    81,    85,    89,    93,    97,   101,   105,   109,
     113,   117,   121,   125,   129,   133,   137,   141,   145,   149,
     153,   157,   161,   165,   169,   173,   177,   180,   183,   186,
     189,   192,   195,   198,   202,   206,   210,   215,   220,   227,
     233,   240,   246,   253,   257,   262,   268,   272,   277,   282,
     287,   292,   297,   306,   317,   324,   333,   344,   351,   360,
     371,   378,   383,   390,   392,   395,   398,   400,   404,   409,
     413,   418,   424,   429,   433,   436,   439,   442,   445,   447,
     449,   451,   453,   455,   457,   459,   462,   464,   468,   470,
     472,   478,   485,   490,   494,   496,   500,   505,   507,   511
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      90,     0,    -1,     3,    91,    77,    -1,     3,     4,    77,
      -1,     3,     5,    77,    -1,     3,     7,    77,    -1,     3,
       9,    77,    -1,     3,    10,    77,    -1,     3,    11,    77,
      -1,     3,    12,    77,    -1,     3,     8,    77,    -1,     3,
       6,    77,    -1,     3,    77,    -1,     3,    91,    39,    77,
      -1,     1,    77,    -1,     1,    -1,    91,    39,    91,    -1,
      91,    63,    91,    -1,    78,    91,    39,    79,    -1,    78,
      91,    79,    -1,    78,    91,    38,    -1,    91,    41,    91,
      -1,    91,    42,    91,    -1,    80,    91,    80,    -1,    91,
      65,    91,    -1,    91,    46,    91,    -1,    91,    66,    91,
      -1,    91,    47,    91,    -1,    91,    67,    91,    -1,    91,
      45,    91,    -1,    91,    68,    91,    -1,    91,    44,    91,
      -1,    91,    69,    91,    -1,    91,    71,    91,    -1,    91,
      70,    91,    -1,    91,    49,    91,    -1,    91,    53,    91,
      -1,    91,    51,    91,    -1,    91,    52,    91,    -1,    91,
      54,    91,    -1,    91,    55,    91,    -1,    91,    56,    91,
      -1,    91,    57,    91,    -1,    91,    60,    91,    -1,    91,
      59,    91,    -1,    91,    58,    91,    -1,    61,    91,    -1,
      91,    76,    -1,    91,    50,    -1,    91,    72,    -1,    91,
      43,    -1,    66,    91,    -1,    65,    91,    -1,    91,    75,
      91,    -1,    91,    48,    91,    -1,    91,    64,    91,    -1,
      91,    37,    64,    79,    -1,    91,    37,    91,    79,    -1,
      91,    37,    91,    81,    91,    79,    -1,    91,    37,    91,
      81,    79,    -1,    91,    37,    91,    81,    64,    79,    -1,
      91,    37,    81,    91,    79,    -1,    91,    37,    64,    81,
      91,    79,    -1,    82,    99,    83,    -1,    84,    82,    99,
      83,    -1,    78,    98,    81,    91,    79,    -1,    85,    98,
      86,    -1,    84,    85,    98,    86,    -1,    25,    91,    30,
      91,    -1,    26,    91,    30,    91,    -1,    30,    91,    25,
      91,    -1,    30,    91,    26,    91,    -1,    27,    93,    95,
      91,    34,    91,    30,    91,    -1,    27,    93,    95,    91,
      34,    91,    35,    91,    30,    91,    -1,    27,    93,    36,
      91,    30,    91,    -1,    28,    93,    95,    91,    34,    91,
      30,    91,    -1,    28,    93,    95,    91,    34,    91,    35,
      91,    30,    91,    -1,    28,    93,    36,    91,    30,    91,
      -1,    29,    93,    95,    91,    34,    91,    30,    91,    -1,
      29,    93,    95,    91,    34,    91,    35,    91,    30,    91,
      -1,    29,    93,    36,    91,    30,    91,    -1,    31,    91,
      32,    91,    -1,    31,    91,    32,    91,    33,    91,    -1,
      93,    -1,    84,    93,    -1,    87,    93,    -1,    92,    -1,
      93,    78,    79,    -1,    93,    78,    98,    79,    -1,    92,
      78,    79,    -1,    92,    78,    98,    79,    -1,    91,    17,
      78,    98,    79,    -1,    91,    17,    78,    79,    -1,    16,
      93,    96,    -1,    19,    94,    -1,    16,    96,    -1,    84,
      96,    -1,    20,    91,    -1,    21,    -1,    22,    -1,    23,
      -1,    24,    -1,    13,    -1,    14,    -1,    88,    -1,    67,
      93,    -1,    15,    -1,    93,    95,    91,    -1,    41,    -1,
      42,    -1,    78,    97,    79,    95,    91,    -1,    78,    97,
      18,    79,    95,    91,    -1,    78,    79,    95,    91,    -1,
      97,    81,    93,    -1,    93,    -1,    98,    81,    91,    -1,
      98,    81,    40,    91,    -1,    91,    -1,    99,    40,    98,
      -1,    98,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   123,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   139,   140,   141,   143,
     144,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   168,   169,   170,
     171,   172,   173,   175,   176,   177,   178,   183,   184,   185,
     186,   187,   188,   189,   190,   192,   200,   202,   203,   204,
     205,   206,   207,   208,   209,   216,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   240,   241,   242,   243,   245,   246,
     248,   249,   250,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   263,   264,   267,   270,   273,   283,   284,
     287,   292,   297,   307,   308,   311,   314,   315,   318,   319
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STARTTOK", "LOADFILE", "LOADFILE_GLOB",
  "LOAD_PLUGIN", "CHANGEDIR", "PWD", "LS", "LS_ARG", "HELP", "HELP_ARG",
  "NUMBER", "STRING", "FUNCID", "FUNCTION", "CALL", "THREEDOTS",
  "PARAMETER", "RETURNTOK", "BAILOUT", "EXCEPTION", "CONTINUE", "BREAK",
  "WHILE", "UNTIL", "FOR", "SUM", "PROD", "DO", "IF", "THEN", "ELSE", "TO",
  "BY", "IN", "AT", "MAKEIMAGPARENTH", "SEPAR", "NEXTROW", "EQUALS",
  "DEFEQUALS", "TRANSPOSE", "ELTELTDIV", "ELTELTMUL", "ELTELTPLUS",
  "ELTELTMINUS", "ELTELTEXP", "ELTELTMOD", "DOUBLEFACT", "EQ_CMP",
  "NE_CMP", "CMP_CMP", "LT_CMP", "GT_CMP", "LE_CMP", "GE_CMP",
  "LOGICAL_XOR", "LOGICAL_OR", "LOGICAL_AND", "LOGICAL_NOT",
  "LOWER_THEN_ELSE", "MOD", "':'", "'+'", "'-'", "'*'", "'/'", "'\\\\'",
  "'%'", "ELTELTBACKDIV", "'''", "UPLUS", "UMINUS", "'^'", "'!'", "'\\n'",
  "'('", "')'", "'|'", "','", "'['", "']'", "'`'", "'{'", "'}'", "'&'",
  "'.'", "$accept", "fullexpr", "expr", "deref", "ident", "paramdef",
  "anyequals", "funcdef", "identlist", "exprlist", "matrixrows", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,    58,    43,    45,    42,    47,    92,
      37,   319,    39,   320,   321,    94,    33,    10,    40,    41,
     124,    44,    91,    93,    96,   123,   125,    38,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    89,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    93,    94,    95,    95,
      96,    96,    96,    97,    97,    98,    98,    98,    99,    99
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     4,     2,     1,     3,     3,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     4,     4,     6,     5,
       6,     5,     6,     3,     4,     5,     3,     4,     4,     4,
       4,     4,     8,    10,     6,     8,    10,     6,     8,    10,
       6,     4,     6,     1,     2,     2,     1,     3,     4,     3,
       4,     5,     4,     3,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     1,     1,
       5,     6,     4,     3,     1,     3,     4,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    15,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   102,   103,   106,     0,     0,     0,
      98,    99,   100,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    12,     0,     0,     0,     0,
       0,     0,   104,     0,    86,    83,     1,     3,     4,    11,
       5,    10,     6,     7,     8,     9,     0,     0,    95,     0,
      94,    97,     0,     0,     0,     0,     0,     0,     0,    46,
      52,    51,   105,   117,     0,     0,   117,   119,     0,     0,
       0,    84,    96,     0,    85,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,    48,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    47,
       2,     0,     0,     0,   114,     0,    93,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,    19,     0,    23,     0,     0,    63,
       0,     0,    66,     0,     0,     0,     0,    13,    16,    21,
      22,    31,    29,    25,    27,    54,    35,    37,    38,    36,
      39,    40,    41,    42,    45,    44,    43,    17,    55,    24,
      26,    28,    30,    32,    34,    33,    53,    89,     0,    87,
       0,     0,     0,     0,     0,   107,    68,    69,     0,     0,
       0,     0,     0,     0,    70,    71,    81,    18,     0,   115,
     115,   118,    64,    67,    92,     0,    56,     0,     0,    57,
       0,    90,    88,   112,     0,     0,   113,     0,     0,     0,
       0,     0,     0,     0,   116,    65,    91,     0,    61,     0,
      59,     0,     0,   110,    74,     0,    77,     0,    80,     0,
      82,    62,    60,    58,   111,     0,     0,     0,     0,     0,
       0,    72,     0,    75,     0,    78,     0,     0,     0,     0,
      73,    76,    79
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     3,    76,    44,    45,    60,   129,    58,   125,    77,
      78
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -70
static const short yypact[] =
{
      57,   -69,   264,    18,   -70,   -56,   -44,   -30,     1,     2,
      11,    41,    42,    43,   -70,   -70,   -70,   -11,    29,   696,
     -70,   -70,   -70,   -70,   696,   696,    29,    29,    29,   696,
     696,   696,   696,   696,    29,   -70,   696,   696,   696,   -14,
     696,    29,   -70,  1057,   -16,    44,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,   -70,   -10,    45,   -70,    24,
     -70,  1906,  1289,  1333,   -26,     4,    15,  1113,  1378,  1991,
     107,   107,   -70,   837,   -29,   793,  1818,    40,   -34,   696,
     696,   -70,   -70,   -61,   -70,    48,   289,   390,   696,   696,
     -70,   696,   696,   696,   696,   696,   696,   -70,   696,   696,
     696,   696,   696,   696,   696,   696,   696,   696,   696,   696,
     696,   696,   696,   696,   696,   696,   696,   -70,   696,   -70,
     -70,   466,   491,    24,   -70,    -9,   -70,   -70,   -70,   696,
     696,   696,   696,   696,   696,   696,   696,   696,   696,   696,
     696,   696,   -70,   567,   -70,   595,   -70,   595,   696,   -70,
     -33,   -27,   -70,   671,   -18,   696,   748,   -70,  1906,  2032,
    2032,     5,     5,   123,   123,   107,     5,  2114,  2114,  2073,
    2114,  2114,  2114,  2114,  1950,  1950,  1991,  2032,  2150,   123,
     123,     5,     5,     5,     5,     5,   107,   -70,    -6,   -70,
      -5,   696,    61,    24,    29,  2032,  1906,  1906,  1422,  1466,
    1510,  1554,  1598,  1642,  1906,  1906,  1862,   -70,   696,   881,
    1818,    40,   -70,   -70,   -70,    36,   -70,   696,   925,   -70,
     365,   -70,   -70,  1906,    24,   696,   -70,   696,   696,   696,
     696,   696,   696,   696,  1818,   -70,   -70,   969,   -70,    63,
     -70,  1013,   696,  1906,  1906,  1157,  1906,  1201,  1906,  1245,
    1906,   -70,   -70,   -70,  1906,   696,   696,   696,   696,   696,
     696,  1906,  1686,  1906,  1730,  1906,  1774,   696,   696,   696,
    1906,  1906,  1906
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -70,   -70,    -2,   -70,   -15,   -70,   -28,   -25,   -70,     3,
      67
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short yytable[] =
{
      43,    16,    57,    59,    16,    16,   148,   148,     4,   192,
     133,    64,    65,    66,    82,   127,   128,    61,    46,    72,
     147,    47,    62,    63,    81,   152,    84,    67,    68,    69,
      70,    71,   126,    48,    73,    75,   134,   136,   138,    74,
     135,   124,    86,    83,    16,   127,   128,    49,    90,   149,
     212,   137,   145,    95,   147,    97,   127,   128,     1,   213,
       2,   216,   121,   217,    56,   127,   128,    56,    79,   123,
     193,    80,   194,   221,   222,   147,   147,   117,    50,    51,
     118,   119,     0,   151,   156,   158,   159,   160,    52,   161,
     162,   163,   164,   165,   166,   191,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   236,   186,   147,    53,    54,
      55,   147,   122,    56,   188,   190,   153,   195,   158,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     224,   158,   252,   209,    86,   210,   150,     0,     0,     0,
       0,   211,     0,   218,     0,    95,   215,    97,     0,     0,
      86,     0,     0,     0,     0,   225,    90,    91,    92,     0,
       0,    95,    96,    97,     0,     0,     0,     0,     0,   226,
       0,     0,   118,   119,     0,     0,     0,     0,     0,   223,
     112,   113,   114,   115,   116,   117,   242,     0,   118,   119,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,   241,     0,
       0,     0,     0,   243,     0,   244,   245,   246,   247,   248,
     249,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   261,   262,   263,   264,   265,   266,     0,
       0,     0,     0,     0,     0,   270,   271,   272,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,     0,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,     0,     0,     0,     0,    31,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,     0,    37,     0,    38,     0,    39,    40,
      31,    41,    42,   154,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,    37,
     155,    38,     0,    39,    40,     0,    41,    42,    14,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,     0,     0,     0,    31,     0,     0,   239,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,   240,    37,     0,    38,     0,    39,
      40,    31,    41,    42,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,    36,     0,
      37,     0,    38,     0,    39,    40,     0,    41,    42,    14,
      15,    16,    17,     0,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,     0,     0,
       0,     0,     0,     0,    14,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,    31,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,   187,    37,     0,    38,     0,
      39,    40,    31,    41,    42,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
     189,    37,     0,    38,     0,    39,    40,     0,    41,    42,
      14,    15,    16,    17,     0,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    31,     0,
       0,     0,    32,    33,    34,   208,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,   207,    37,     0,    38,
       0,    39,    40,     0,    41,    42,    31,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,     0,    37,     0,    38,     0,    39,
      40,     0,    41,    42,    14,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,    14,
      15,    16,    17,     0,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,     0,     0,
       0,     0,    31,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
     214,    37,     0,    38,     0,    39,    40,    31,    41,    42,
       0,    32,    33,    34,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,    36,     0,    37,     0,    38,     0,
      39,    40,     0,    41,    42,    86,     0,   130,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,     0,
      85,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,     0,   118,   119,     0,     0,   219,     0,   220,
      86,     0,   130,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    85,     0,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,   118,   119,
       0,     0,     0,   146,    86,   142,   143,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,    85,     0,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,     0,   118,   119,     0,     0,   144,     0,    86,     0,
     130,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,    85,     0,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,     0,   118,   119,     0,     0,
     235,     0,    86,     0,   130,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    85,     0,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,     0,
     118,   119,     0,     0,   238,     0,    86,     0,   130,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
      85,     0,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     0,     0,   118,   119,     0,     0,   251,     0,
      86,     0,   130,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    85,     0,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,   118,   119,
       0,     0,   253,     0,    86,     0,    87,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,     0,     0,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      85,     0,   118,   119,   120,     0,     0,     0,   139,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,   130,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    85,     0,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,   255,   118,   119,
       0,     0,   256,     0,    86,     0,   130,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,    85,     0,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,   257,   118,   119,     0,     0,   258,     0,    86,     0,
     130,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,    85,     0,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,   259,   118,   119,     0,     0,
     260,     0,    86,     0,   130,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    85,     0,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,     0,   131,
     118,   119,     0,     0,     0,     0,    86,     0,   130,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
      85,     0,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     0,   132,   118,   119,     0,     0,     0,     0,
      86,     0,   130,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,     0,    85,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,   118,   119,
     141,     0,     0,     0,     0,    86,     0,   130,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    85,
       0,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,   227,   118,   119,     0,     0,     0,     0,    86,
       0,   130,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    85,     0,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,   118,   119,     0,
     228,     0,     0,    86,     0,   130,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,    85,     0,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,     0,
     229,   118,   119,     0,     0,     0,     0,    86,     0,   130,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    85,     0,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,   118,   119,     0,   230,     0,
       0,    86,     0,   130,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    85,     0,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,   231,   118,
     119,     0,     0,     0,     0,    86,     0,   130,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    85,
       0,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,     0,   118,   119,     0,   232,     0,     0,    86,
       0,   130,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    85,     0,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,   267,   118,   119,     0,
       0,     0,     0,    86,     0,   130,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,    85,     0,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,     0,
     268,   118,   119,     0,     0,     0,     0,    86,     0,   130,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    85,     0,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,   269,   118,   119,     0,     0,     0,
       0,    86,     0,   130,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    85,     0,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,     0,   118,
     119,     0,     0,     0,     0,    86,     0,   130,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    85,
       0,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,     0,   118,   119,   233,     0,     0,     0,    86,
       0,     0,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,    85,     0,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,   118,   119,     0,
       0,     0,     0,    86,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,    85,     0,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,     0,
       0,   118,   119,     0,     0,     0,     0,    86,     0,     0,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    85,     0,
     107,     0,     0,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,   118,   119,     0,    86,     0,
       0,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    85,
       0,     0,     0,     0,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,     0,   118,   119,     0,    86,
       0,     0,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      85,     0,     0,     0,     0,     0,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,   118,   119,     0,
      86,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
     104,    85,     0,     0,     0,     0,     0,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,   118,   119,
       0,    86,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,     0,   101,   102,
     103,   104,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    86,     0,   118,
     119,     0,     0,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,   118,   119
};

static const short yycheck[] =
{
       2,    15,    17,    18,    15,    15,    40,    40,    77,    18,
      36,    26,    27,    28,    39,    41,    42,    19,     0,    34,
      81,    77,    24,    25,    39,    86,    41,    29,    30,    31,
      32,    33,    57,    77,    36,    37,    64,    65,    66,    36,
      36,    56,    37,    40,    15,    41,    42,    77,    43,    83,
      83,    36,    81,    48,    81,    50,    41,    42,     1,    86,
       3,    79,    78,    81,    78,    41,    42,    78,    82,    79,
      79,    85,    81,    79,    79,    81,    81,    72,    77,    77,
      75,    76,    -1,    80,    86,    87,    88,    89,    77,    91,
      92,    93,    94,    95,    96,   123,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    79,   118,    81,    77,    77,
      77,    81,    78,    78,   121,   122,    78,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      79,   143,    79,   145,    37,   147,    79,    -1,    -1,    -1,
      -1,   148,    -1,   155,    -1,    48,   153,    50,    -1,    -1,
      37,    -1,    -1,    -1,    -1,   193,    43,    44,    45,    -1,
      -1,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,   194,
      -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,   191,
      67,    68,    69,    70,    71,    72,   224,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,   225,    -1,   227,   228,   229,   230,   231,
     232,   233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   255,   256,   257,   258,   259,   260,    -1,
      -1,    -1,    -1,    -1,    -1,   267,   268,   269,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    -1,    80,    -1,    82,    -1,    84,    85,
      61,    87,    88,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    -1,    61,    -1,    -1,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    84,
      85,    61,    87,    88,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    -1,
      80,    -1,    82,    -1,    84,    85,    -1,    87,    88,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,
      84,    85,    61,    87,    88,    -1,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    -1,    82,    -1,    84,    85,    -1,    87,    88,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    61,    -1,
      -1,    -1,    65,    66,    67,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    87,    88,    61,    -1,    -1,    -1,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      85,    -1,    87,    88,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    -1,    82,    -1,    84,    85,    61,    87,    88,
      -1,    65,    66,    67,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      84,    85,    -1,    87,    88,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      17,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    -1,    75,    76,    -1,    -1,    79,    -1,    81,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    17,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    -1,    75,    76,
      -1,    -1,    -1,    80,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    17,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    -1,    75,    76,    -1,    -1,    79,    -1,    37,    -1,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    17,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    -1,    75,    76,    -1,    -1,
      79,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    17,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    -1,
      75,    76,    -1,    -1,    79,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      17,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    -1,    75,    76,    -1,    -1,    79,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    17,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    -1,    75,    76,
      -1,    -1,    79,    -1,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      17,    -1,    75,    76,    77,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    17,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    30,    75,    76,
      -1,    -1,    35,    -1,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    17,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      -1,    30,    75,    76,    -1,    -1,    35,    -1,    37,    -1,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    17,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    30,    75,    76,    -1,    -1,
      35,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    17,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    -1,    30,
      75,    76,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      17,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    -1,    30,    75,    76,    -1,    -1,    -1,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    17,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    -1,    75,    76,
      32,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    17,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    30,    75,    76,    -1,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    17,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    -1,    75,    76,    -1,
      34,    -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    17,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      30,    75,    76,    -1,    -1,    -1,    -1,    37,    -1,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    17,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    -1,    75,    76,    -1,    34,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    17,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    30,    75,
      76,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    17,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    -1,    75,    76,    -1,    34,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    17,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    30,    75,    76,    -1,
      -1,    -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    17,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      30,    75,    76,    -1,    -1,    -1,    -1,    37,    -1,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    17,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    30,    75,    76,    -1,    -1,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    17,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    -1,    -1,    75,
      76,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    17,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    -1,    -1,    75,    76,    33,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    17,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    17,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    -1,
      -1,    75,    76,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    17,    -1,
      60,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    -1,    75,    76,    -1,    37,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    17,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    -1,    -1,    75,    76,    -1,    37,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      17,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    -1,    -1,    75,    76,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    17,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    -1,    -1,    75,    76,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    37,    -1,    75,
      76,    -1,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    -1,    -1,    75,    76
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,     3,    90,    77,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    61,    65,    66,    67,    77,    78,    80,    82,    84,
      85,    87,    88,    91,    92,    93,     0,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    78,    93,    96,    93,
      94,    91,    91,    91,    93,    93,    93,    91,    91,    91,
      91,    91,    93,    91,    98,    91,    91,    98,    99,    82,
      85,    93,    96,    98,    93,    17,    37,    39,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    78,    79,    93,    97,    96,    41,    42,    95,
      39,    30,    30,    36,    95,    36,    95,    36,    95,    25,
      26,    32,    38,    39,    79,    81,    80,    81,    40,    83,
      99,    98,    86,    78,    64,    81,    91,    77,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    79,    98,    79,
      98,    95,    18,    79,    81,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    79,    40,    91,
      91,    98,    83,    86,    79,    98,    79,    81,    91,    79,
      81,    79,    79,    91,    79,    95,    93,    30,    34,    30,
      34,    30,    34,    33,    91,    79,    79,    91,    79,    64,
      79,    91,    95,    91,    91,    91,    91,    91,    91,    91,
      91,    79,    79,    79,    91,    30,    35,    30,    35,    30,
      35,    91,    91,    91,    91,    91,    91,    30,    30,    30,
      91,    91,    91
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 123 "parse.y"
    { YYACCEPT; }
    break;

  case 3:
#line 124 "parse.y"
    { gel_command = GEL_LOADFILE; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 4:
#line 125 "parse.y"
    { gel_command = GEL_LOADFILE_GLOB; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 5:
#line 126 "parse.y"
    { gel_command = GEL_CHANGEDIR; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 6:
#line 127 "parse.y"
    { gel_command = GEL_LS; YYACCEPT; }
    break;

  case 7:
#line 128 "parse.y"
    { gel_command = GEL_LS_ARG; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 8:
#line 129 "parse.y"
    { gel_command = GEL_HELP; YYACCEPT; }
    break;

  case 9:
#line 130 "parse.y"
    { gel_command = GEL_HELP_ARG; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 10:
#line 131 "parse.y"
    { gel_command = GEL_PWD; YYACCEPT; }
    break;

  case 11:
#line 132 "parse.y"
    { gel_command = GEL_LOADPLUGIN; gel_command_arg = yyvsp[-1].id; YYACCEPT; }
    break;

  case 12:
#line 133 "parse.y"
    { YYACCEPT; }
    break;

  case 13:
#line 134 "parse.y"
    { gp_push_null(); PUSH_ACT(E_SEPAR); YYACCEPT; }
    break;

  case 14:
#line 135 "parse.y"
    { return_ret = TRUE; yyclearin; YYABORT; }
    break;

  case 15:
#line 136 "parse.y"
    { return_ret = TRUE; }
    break;

  case 16:
#line 139 "parse.y"
    { PUSH_ACT(E_SEPAR); }
    break;

  case 17:
#line 140 "parse.y"
    { PUSH_ACT(E_MOD_CALC); }
    break;

  case 18:
#line 141 "parse.y"
    { gp_push_null(); PUSH_ACT(E_SEPAR);
					  gp_push_spacer(); }
    break;

  case 19:
#line 143 "parse.y"
    { gp_push_spacer(); }
    break;

  case 20:
#line 144 "parse.y"
    { mpw_t i;
					  mpw_init (i);
					  mpw_i (i);
					  gp_push_spacer();
					  stack_push(&gel_parsestack,
						     gel_makenum_use(i));
					  PUSH_ACT(E_MUL); }
    break;

  case 21:
#line 151 "parse.y"
    { PUSH_ACT(E_EQUALS); }
    break;

  case 22:
#line 152 "parse.y"
    { PUSH_ACT(E_DEFEQUALS); }
    break;

  case 23:
#line 153 "parse.y"
    { PUSH_ACT(E_ABS); }
    break;

  case 24:
#line 154 "parse.y"
    { PUSH_ACT(E_PLUS); }
    break;

  case 25:
#line 155 "parse.y"
    { PUSH_ACT(E_ELTPLUS); }
    break;

  case 26:
#line 156 "parse.y"
    { PUSH_ACT(E_MINUS); }
    break;

  case 27:
#line 157 "parse.y"
    { PUSH_ACT(E_ELTMINUS); }
    break;

  case 28:
#line 158 "parse.y"
    { PUSH_ACT(E_MUL); }
    break;

  case 29:
#line 159 "parse.y"
    { PUSH_ACT(E_ELTMUL); }
    break;

  case 30:
#line 160 "parse.y"
    { PUSH_ACT(E_DIV); }
    break;

  case 31:
#line 161 "parse.y"
    { PUSH_ACT(E_ELTDIV); }
    break;

  case 32:
#line 162 "parse.y"
    { PUSH_ACT(E_BACK_DIV); }
    break;

  case 33:
#line 163 "parse.y"
    { PUSH_ACT(E_ELT_BACK_DIV); }
    break;

  case 34:
#line 164 "parse.y"
    { PUSH_ACT(E_MOD); }
    break;

  case 35:
#line 165 "parse.y"
    { PUSH_ACT(E_ELTMOD); }
    break;

  case 36:
#line 166 "parse.y"
    { PUSH_ACT(E_CMP_CMP); }
    break;

  case 37:
#line 168 "parse.y"
    { PUSH_ACT(E_EQ_CMP); }
    break;

  case 38:
#line 169 "parse.y"
    { PUSH_ACT(E_NE_CMP); }
    break;

  case 39:
#line 170 "parse.y"
    { PUSH_ACT(E_LT_CMP); }
    break;

  case 40:
#line 171 "parse.y"
    { PUSH_ACT(E_GT_CMP); }
    break;

  case 41:
#line 172 "parse.y"
    { PUSH_ACT(E_LE_CMP); }
    break;

  case 42:
#line 173 "parse.y"
    { PUSH_ACT(E_GE_CMP); }
    break;

  case 43:
#line 175 "parse.y"
    { PUSH_ACT(E_LOGICAL_AND); }
    break;

  case 44:
#line 176 "parse.y"
    { PUSH_ACT(E_LOGICAL_OR); }
    break;

  case 45:
#line 177 "parse.y"
    { PUSH_ACT(E_LOGICAL_XOR); }
    break;

  case 46:
#line 178 "parse.y"
    { PUSH_ACT(E_LOGICAL_NOT); }
    break;

  case 47:
#line 183 "parse.y"
    { PUSH_ACT(E_FACT); }
    break;

  case 48:
#line 184 "parse.y"
    { PUSH_ACT(E_DBLFACT); }
    break;

  case 49:
#line 185 "parse.y"
    { PUSH_ACT(E_CONJUGATE_TRANSPOSE); }
    break;

  case 50:
#line 186 "parse.y"
    { PUSH_ACT(E_TRANSPOSE); }
    break;

  case 51:
#line 187 "parse.y"
    { PUSH_ACT(E_NEG); }
    break;

  case 53:
#line 189 "parse.y"
    { PUSH_ACT(E_EXP); }
    break;

  case 54:
#line 190 "parse.y"
    { PUSH_ACT(E_ELTEXP); }
    break;

  case 55:
#line 192 "parse.y"
    {
				if (gp_prepare_push_region_sep ()) {
					PUSH_ACT(E_REGION_SEP_BY);
				} else {
					PUSH_ACT(E_REGION_SEP);
				}
					}
    break;

  case 56:
#line 200 "parse.y"
    { /* FIXME: do nothing?, this is just a 
					     get all */ }
    break;

  case 57:
#line 202 "parse.y"
    { PUSH_ACT(E_GET_VELEMENT); }
    break;

  case 58:
#line 203 "parse.y"
    { PUSH_ACT(E_GET_ELEMENT); }
    break;

  case 59:
#line 204 "parse.y"
    { PUSH_ACT(E_GET_ROW_REGION); }
    break;

  case 60:
#line 205 "parse.y"
    { PUSH_ACT(E_GET_ROW_REGION); }
    break;

  case 61:
#line 206 "parse.y"
    { PUSH_ACT(E_GET_COL_REGION); }
    break;

  case 62:
#line 207 "parse.y"
    { PUSH_ACT(E_GET_COL_REGION); }
    break;

  case 63:
#line 208 "parse.y"
    { if(!gp_push_matrix(FALSE)) {SYNTAX_ERROR;} }
    break;

  case 64:
#line 209 "parse.y"
    { if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;} }
    break;

  case 65:
#line 216 "parse.y"
    {
			if(!gp_push_matrix_row()) {SYNTAX_ERROR;}
			if(!gp_push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;}
			if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;}
					}
    break;

  case 66:
#line 221 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 67:
#line 222 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 68:
#line 223 "parse.y"
    { PUSH_ACT(E_WHILE_CONS); }
    break;

  case 69:
#line 224 "parse.y"
    { PUSH_ACT(E_UNTIL_CONS); }
    break;

  case 70:
#line 225 "parse.y"
    { PUSH_ACT(E_DOWHILE_CONS); }
    break;

  case 71:
#line 226 "parse.y"
    { PUSH_ACT(E_DOUNTIL_CONS); }
    break;

  case 72:
#line 227 "parse.y"
    { PUSH_ACT(E_FOR_CONS); }
    break;

  case 73:
#line 228 "parse.y"
    { PUSH_ACT(E_FORBY_CONS); }
    break;

  case 74:
#line 229 "parse.y"
    { PUSH_ACT(E_FORIN_CONS); }
    break;

  case 75:
#line 230 "parse.y"
    { PUSH_ACT(E_SUM_CONS); }
    break;

  case 76:
#line 231 "parse.y"
    { PUSH_ACT(E_SUMBY_CONS); }
    break;

  case 77:
#line 232 "parse.y"
    { PUSH_ACT(E_SUMIN_CONS); }
    break;

  case 78:
#line 233 "parse.y"
    { PUSH_ACT(E_PROD_CONS); }
    break;

  case 79:
#line 234 "parse.y"
    { PUSH_ACT(E_PRODBY_CONS); }
    break;

  case 80:
#line 235 "parse.y"
    { PUSH_ACT(E_PRODIN_CONS); }
    break;

  case 81:
#line 236 "parse.y"
    { PUSH_ACT(E_IF_CONS); }
    break;

  case 82:
#line 237 "parse.y"
    { PUSH_ACT(E_IFELSE_CONS); }
    break;

  case 83:
#line 238 "parse.y"
    { gp_convert_identifier_to_bool ();
					  /* convert true/false to bool */}
    break;

  case 84:
#line 240 "parse.y"
    { PUSH_ACT(E_QUOTE); }
    break;

  case 85:
#line 241 "parse.y"
    { PUSH_ACT(E_REFERENCE); }
    break;

  case 87:
#line 243 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); }
    break;

  case 88:
#line 245 "parse.y"
    { PUSH_ACT(E_DIRECTCALL); }
    break;

  case 89:
#line 246 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); }
    break;

  case 90:
#line 248 "parse.y"
    { PUSH_ACT(E_DIRECTCALL); }
    break;

  case 91:
#line 249 "parse.y"
    { PUSH_ACT(E_CALL); }
    break;

  case 92:
#line 250 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); }
    break;

  case 93:
#line 252 "parse.y"
    { PUSH_ACT(E_DEFEQUALS); }
    break;

  case 97:
#line 256 "parse.y"
    { PUSH_ACT(E_RETURN); }
    break;

  case 98:
#line 257 "parse.y"
    { PUSH_ACT(E_BAILOUT); }
    break;

  case 99:
#line 258 "parse.y"
    { PUSH_ACT(E_EXCEPTION); }
    break;

  case 100:
#line 259 "parse.y"
    { PUSH_ACT(E_CONTINUE); }
    break;

  case 101:
#line 260 "parse.y"
    { PUSH_ACT(E_BREAK); }
    break;

  case 102:
#line 261 "parse.y"
    { stack_push(&gel_parsestack,
						     gel_makenum_use(yyvsp[0].val)); }
    break;

  case 103:
#line 263 "parse.y"
    { PUSH_CONST_STRING(yyvsp[0].id); }
    break;

  case 104:
#line 264 "parse.y"
    { gp_push_null(); }
    break;

  case 105:
#line 267 "parse.y"
    { PUSH_ACT(E_DEREFERENCE); }
    break;

  case 106:
#line 270 "parse.y"
    { PUSH_IDENTIFIER(yyvsp[0].id); }
    break;

  case 107:
#line 273 "parse.y"
    {
			gp_prepare_push_param (FALSE);
			PUSH_ACT (E_PARAMETER);
		}
    break;

  case 110:
#line 287 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 111:
#line 292 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 112:
#line 297 "parse.y"
    {
			if ( ! gp_push_marker (EXPRLIST_START_NODE)) {
				SYNTAX_ERROR;
			}
			if ( ! gp_push_func (FALSE /* vararg */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 114:
#line 308 "parse.y"
    { if(!gp_push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 117:
#line 315 "parse.y"
    { if(!gp_push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 118:
#line 318 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} }
    break;

  case 119:
#line 319 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} if(!gp_push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 2357 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 322 "parse.y"


