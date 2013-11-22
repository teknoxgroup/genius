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
     ELTELTEXP = 301,
     ELTELTMOD = 302,
     DOUBLEFACT = 303,
     EQ_CMP = 304,
     NE_CMP = 305,
     CMP_CMP = 306,
     LT_CMP = 307,
     GT_CMP = 308,
     LE_CMP = 309,
     GE_CMP = 310,
     LOGICAL_XOR = 311,
     LOGICAL_OR = 312,
     LOGICAL_AND = 313,
     LOGICAL_NOT = 314,
     LOWER_THEN_ELSE = 315,
     MOD = 316,
     ELTELTBACKDIV = 317,
     UPLUS = 318,
     UMINUS = 319
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
#define ELTELTEXP 301
#define ELTELTMOD 302
#define DOUBLEFACT 303
#define EQ_CMP 304
#define NE_CMP 305
#define CMP_CMP 306
#define LT_CMP 307
#define GT_CMP 308
#define LE_CMP 309
#define GE_CMP 310
#define LOGICAL_XOR 311
#define LOGICAL_OR 312
#define LOGICAL_AND 313
#define LOGICAL_NOT 314
#define LOWER_THEN_ELSE 315
#define MOD 316
#define ELTELTBACKDIV 317
#define UPLUS 318
#define UMINUS 319




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

extern GSList *evalstack;

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
#line 242 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 254 "y.tab.c"

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
#define YYLAST   2216

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  11
/* YYNRULES -- Number of rules. */
#define YYNRULES  117
/* YYNRULES -- Number of states. */
#define YYNSTATES  269

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   319

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      75,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,     2,     2,     2,    68,    85,    70,
      76,    77,    65,    63,    79,    64,    86,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    62,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    80,    67,    81,    73,     2,    82,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,    78,    84,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    69,    71,    72
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
     153,   157,   161,   165,   169,   172,   175,   178,   181,   184,
     187,   190,   194,   198,   202,   207,   212,   219,   225,   232,
     238,   245,   249,   254,   260,   264,   269,   274,   279,   284,
     289,   298,   309,   316,   325,   336,   343,   352,   363,   370,
     375,   382,   384,   387,   390,   392,   396,   401,   405,   410,
     416,   421,   425,   428,   431,   434,   437,   439,   441,   443,
     445,   447,   449,   451,   454,   456,   460,   462,   464,   470,
     477,   482,   486,   488,   492,   497,   499,   503
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      88,     0,    -1,     3,    89,    75,    -1,     3,     4,    75,
      -1,     3,     5,    75,    -1,     3,     7,    75,    -1,     3,
       9,    75,    -1,     3,    10,    75,    -1,     3,    11,    75,
      -1,     3,    12,    75,    -1,     3,     8,    75,    -1,     3,
       6,    75,    -1,     3,    75,    -1,     3,    89,    39,    75,
      -1,     1,    75,    -1,     1,    -1,    89,    39,    89,    -1,
      89,    61,    89,    -1,    76,    89,    39,    77,    -1,    76,
      89,    77,    -1,    76,    89,    38,    -1,    89,    41,    89,
      -1,    89,    42,    89,    -1,    78,    89,    78,    -1,    89,
      63,    89,    -1,    89,    64,    89,    -1,    89,    65,    89,
      -1,    89,    45,    89,    -1,    89,    66,    89,    -1,    89,
      44,    89,    -1,    89,    67,    89,    -1,    89,    69,    89,
      -1,    89,    68,    89,    -1,    89,    47,    89,    -1,    89,
      51,    89,    -1,    89,    49,    89,    -1,    89,    50,    89,
      -1,    89,    52,    89,    -1,    89,    53,    89,    -1,    89,
      54,    89,    -1,    89,    55,    89,    -1,    89,    58,    89,
      -1,    89,    57,    89,    -1,    89,    56,    89,    -1,    59,
      89,    -1,    89,    74,    -1,    89,    48,    -1,    89,    70,
      -1,    89,    43,    -1,    64,    89,    -1,    63,    89,    -1,
      89,    73,    89,    -1,    89,    46,    89,    -1,    89,    62,
      89,    -1,    89,    37,    62,    77,    -1,    89,    37,    89,
      77,    -1,    89,    37,    89,    79,    89,    77,    -1,    89,
      37,    89,    79,    77,    -1,    89,    37,    89,    79,    62,
      77,    -1,    89,    37,    79,    89,    77,    -1,    89,    37,
      62,    79,    89,    77,    -1,    80,    97,    81,    -1,    82,
      80,    97,    81,    -1,    76,    96,    79,    89,    77,    -1,
      83,    96,    84,    -1,    82,    83,    96,    84,    -1,    25,
      89,    30,    89,    -1,    26,    89,    30,    89,    -1,    30,
      89,    25,    89,    -1,    30,    89,    26,    89,    -1,    27,
      91,    93,    89,    34,    89,    30,    89,    -1,    27,    91,
      93,    89,    34,    89,    35,    89,    30,    89,    -1,    27,
      91,    36,    89,    30,    89,    -1,    28,    91,    93,    89,
      34,    89,    30,    89,    -1,    28,    91,    93,    89,    34,
      89,    35,    89,    30,    89,    -1,    28,    91,    36,    89,
      30,    89,    -1,    29,    91,    93,    89,    34,    89,    30,
      89,    -1,    29,    91,    93,    89,    34,    89,    35,    89,
      30,    89,    -1,    29,    91,    36,    89,    30,    89,    -1,
      31,    89,    32,    89,    -1,    31,    89,    32,    89,    33,
      89,    -1,    91,    -1,    82,    91,    -1,    85,    91,    -1,
      90,    -1,    91,    76,    77,    -1,    91,    76,    96,    77,
      -1,    90,    76,    77,    -1,    90,    76,    96,    77,    -1,
      89,    17,    76,    96,    77,    -1,    89,    17,    76,    77,
      -1,    16,    91,    94,    -1,    19,    92,    -1,    16,    94,
      -1,    82,    94,    -1,    20,    89,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    13,    -1,    14,    -1,    86,
      -1,    65,    91,    -1,    15,    -1,    91,    93,    89,    -1,
      41,    -1,    42,    -1,    76,    95,    77,    93,    89,    -1,
      76,    95,    18,    77,    93,    89,    -1,    76,    77,    93,
      89,    -1,    95,    79,    91,    -1,    91,    -1,    96,    79,
      89,    -1,    96,    79,    40,    89,    -1,    89,    -1,    97,
      40,    96,    -1,    96,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   123,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   139,   140,   141,   143,
     144,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   166,   167,   168,   169,   170,
     171,   173,   174,   175,   176,   181,   182,   183,   184,   185,
     186,   187,   188,   190,   198,   200,   201,   202,   203,   204,
     205,   206,   207,   214,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   238,   239,   240,   241,   243,   244,   246,   247,
     248,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   261,   262,   265,   268,   271,   281,   282,   285,   290,
     295,   305,   306,   309,   312,   313,   316,   317
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
  "DEFEQUALS", "TRANSPOSE", "ELTELTDIV", "ELTELTMUL", "ELTELTEXP",
  "ELTELTMOD", "DOUBLEFACT", "EQ_CMP", "NE_CMP", "CMP_CMP", "LT_CMP",
  "GT_CMP", "LE_CMP", "GE_CMP", "LOGICAL_XOR", "LOGICAL_OR", "LOGICAL_AND",
  "LOGICAL_NOT", "LOWER_THEN_ELSE", "MOD", "':'", "'+'", "'-'", "'*'",
  "'/'", "'\\\\'", "'%'", "ELTELTBACKDIV", "'''", "UPLUS", "UMINUS", "'^'",
  "'!'", "'\\n'", "'('", "')'", "'|'", "','", "'['", "']'", "'`'", "'{'",
  "'}'", "'&'", "'.'", "$accept", "fullexpr", "expr", "deref", "ident",
  "paramdef", "anyequals", "funcdef", "identlist", "exprlist",
  "matrixrows", 0
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
     315,   316,    58,    43,    45,    42,    47,    92,    37,   317,
      39,   318,   319,    94,    33,    10,    40,    41,   124,    44,
      91,    93,    96,   123,   125,    38,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    87,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    90,    91,    92,    93,    93,    94,    94,
      94,    95,    95,    96,    96,    96,    97,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     4,     2,     1,     3,     3,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     4,     4,     6,     5,     6,     5,
       6,     3,     4,     5,     3,     4,     4,     4,     4,     4,
       8,    10,     6,     8,    10,     6,     8,    10,     6,     4,
       6,     1,     2,     2,     1,     3,     4,     3,     4,     5,
       4,     3,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     1,     5,     6,
       4,     3,     1,     3,     4,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    15,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100,   101,   104,     0,     0,     0,
      96,    97,    98,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    12,     0,     0,     0,     0,
       0,     0,   102,     0,    84,    81,     1,     3,     4,    11,
       5,    10,     6,     7,     8,     9,     0,     0,    93,     0,
      92,    95,     0,     0,     0,     0,     0,     0,     0,    44,
      50,    49,   103,   115,     0,     0,   115,   117,     0,     0,
       0,    82,    94,     0,    83,     0,     0,     0,     0,     0,
      48,     0,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,     0,    45,     2,     0,
       0,     0,   112,     0,    91,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,    19,     0,    23,     0,     0,    61,     0,     0,
      64,     0,     0,     0,     0,    13,    16,    21,    22,    29,
      27,    52,    33,    35,    36,    34,    37,    38,    39,    40,
      43,    42,    41,    17,    53,    24,    25,    26,    28,    30,
      32,    31,    51,    87,     0,    85,     0,     0,     0,     0,
       0,   105,    66,    67,     0,     0,     0,     0,     0,     0,
      68,    69,    79,    18,     0,   113,   113,   116,    62,    65,
      90,     0,    54,     0,     0,    55,     0,    88,    86,   110,
       0,     0,   111,     0,     0,     0,     0,     0,     0,     0,
     114,    63,    89,     0,    59,     0,    57,     0,     0,   108,
      72,     0,    75,     0,    78,     0,    80,    60,    58,    56,
     109,     0,     0,     0,     0,     0,     0,    70,     0,    73,
       0,    76,     0,     0,     0,     0,    71,    74,    77
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     3,    76,    44,    45,    60,   127,    58,   123,    77,
      78
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -68
static const short yypact[] =
{
      56,   -67,   260,    18,   -68,   -54,   -42,    -4,    13,    45,
      46,    49,    67,    70,   -68,   -68,   -68,   -11,    27,   730,
     -68,   -68,   -68,   -68,   730,   730,    27,    27,    27,   730,
     730,   730,   730,   730,    27,   -68,   730,   730,   730,   -14,
     730,    27,   -68,  1075,   -27,   -16,   -68,   -68,   -68,   -68,
     -68,   -68,   -68,   -68,   -68,   -68,   -10,     6,   -68,    37,
     -68,  1886,  1297,  1339,   -26,     4,    22,  1129,  1382,  1967,
     110,   110,   -68,   865,    69,   823,  1802,    73,   -34,   730,
     730,   -68,   -68,   -59,   -68,    81,   285,   384,   730,   730,
     -68,   730,   730,   730,   730,   -68,   730,   730,   730,   730,
     730,   730,   730,   730,   730,   730,   730,   730,   730,   730,
     730,   730,   730,   730,   730,   -68,   730,   -68,   -68,   458,
     483,    37,   -68,    -9,   -68,   -68,   -68,   730,   730,   730,
     730,   730,   730,   730,   730,   730,   730,   730,   730,   730,
     -68,   557,   -68,   631,   -68,   631,   730,   -68,   -33,   -23,
     -68,   705,    36,   730,   780,   -68,  1886,  2040,  2040,   107,
     107,   110,   107,  2108,  2108,  2074,  2108,  2108,  2108,  2108,
    1928,  1928,  1967,  2006,  2142,     7,     7,   107,   107,   107,
     107,   107,   110,   -68,    39,   -68,    40,   730,    82,    37,
      27,  2040,  1886,  1886,  1424,  1466,  1508,  1550,  1592,  1634,
    1886,  1886,  1844,   -68,   730,   907,  1802,    73,   -68,   -68,
     -68,    61,   -68,   730,   949,   -68,   359,   -68,   -68,  1886,
      37,   730,   -68,   730,   730,   730,   730,   730,   730,   730,
    1802,   -68,   -68,   991,   -68,    83,   -68,  1033,   730,  1886,
    1886,  1171,  1886,  1213,  1886,  1255,  1886,   -68,   -68,   -68,
    1886,   730,   730,   730,   730,   730,   730,  1886,  1676,  1886,
    1718,  1886,  1760,   730,   730,   730,  1886,  1886,  1886
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -68,   -68,    -2,   -68,   -15,   -68,   -28,   -25,   -68,     3,
      84
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short yytable[] =
{
      43,    16,    57,    59,    16,    16,   146,   146,     4,   188,
     131,    64,    65,    66,    82,   125,   126,    61,    46,    72,
     145,    47,    62,    63,    81,   150,    84,    67,    68,    69,
      70,    71,   124,    48,    73,    75,   132,   134,   136,    74,
     133,   122,    16,    83,    86,   125,   126,   147,   208,   119,
      90,    91,    92,    93,    94,    95,   145,     1,   135,     2,
     120,   209,    56,   125,   126,    56,    79,   121,   189,    80,
     190,    49,   110,   111,   112,   113,   114,   115,   125,   126,
     116,   117,    56,   149,   154,   156,   157,   158,    50,   159,
     160,   161,   162,   187,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   212,   182,   213,   217,   218,   145,   145,
      51,    52,   184,   186,    53,   191,   156,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   232,   156,
     145,   205,    54,   206,    86,    55,     0,    86,   143,   207,
      90,   214,   145,    93,   211,    95,    93,   151,    95,   220,
     248,   221,     0,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222,     0,   115,     0,     0,
     116,   117,     0,   116,   117,   219,     0,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,     0,     0,     0,
       0,   233,     0,     0,   237,     0,     0,     0,     0,   239,
       0,   240,   241,   242,   243,   244,   245,   246,     0,     0,
       0,     0,     0,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
     258,   259,   260,   261,   262,     0,     0,     0,     0,     0,
       0,   266,   267,   268,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,     0,     0,     0,     0,     0,    14,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     0,    31,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,     0,    37,     0,
      38,     0,    39,    40,    31,    41,    42,   152,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,    37,   153,    38,     0,    39,    40,     0,
      41,    42,    14,    15,    16,    17,     0,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     0,     0,    31,     0,
       0,   235,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,   236,    37,     0,    38,
       0,    39,    40,    31,    41,    42,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
      36,     0,    37,     0,    38,     0,    39,    40,     0,    41,
      42,    14,    15,    16,    17,     0,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
       0,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     0,     0,    31,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,   183,    37,     0,    38,     0,
      39,    40,    31,    41,    42,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
     185,    37,     0,    38,     0,    39,    40,     0,    41,    42,
      14,    15,    16,    17,     0,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,   203,    37,     0,    38,     0,    39,
      40,     0,    41,    42,    14,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     0,     0,     0,     0,     0,     0,     0,
       0,   204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,    37,
       0,    38,     0,    39,    40,     0,    41,    42,    14,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,     0,    31,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,   210,    37,     0,    38,     0,    39,    40,    31,
      41,    42,     0,    32,    33,    34,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,    37,     0,
      38,     0,    39,    40,     0,    41,    42,    86,     0,   128,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     0,
      85,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,     0,   116,   117,     0,     0,   215,     0,   216,
      86,     0,   128,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    85,     0,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   117,     0,     0,
       0,   144,    86,   140,   141,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    85,     0,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,     0,     0,   116,   117,
       0,     0,   142,     0,    86,     0,   128,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    85,     0,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   117,     0,     0,   231,     0,    86,     0,   128,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    85,     0,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
       0,     0,   116,   117,     0,     0,   234,     0,    86,     0,
     128,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      85,     0,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,     0,     0,   116,   117,     0,     0,   247,     0,
      86,     0,   128,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    85,     0,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,     0,   116,   117,     0,     0,
     249,     0,    86,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,     0,     0,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    85,     0,   116,   117,
     118,     0,     0,     0,   137,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,     0,   128,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    85,     0,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
       0,   251,   116,   117,     0,     0,   252,     0,    86,     0,
     128,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      85,     0,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,     0,   253,   116,   117,     0,     0,   254,     0,
      86,     0,   128,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    85,     0,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,     0,   255,   116,   117,     0,     0,
     256,     0,    86,     0,   128,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    85,     0,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,     0,   129,   116,   117,
       0,     0,     0,     0,    86,     0,   128,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    85,     0,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,     0,   130,
     116,   117,     0,     0,     0,     0,    86,     0,   128,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,     0,    85,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
       0,     0,   116,   117,   139,     0,     0,     0,     0,    86,
       0,   128,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    85,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,   223,   116,   117,     0,     0,     0,
       0,    86,     0,   128,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    85,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   117,     0,
     224,     0,     0,    86,     0,   128,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    85,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,     0,   225,   116,
     117,     0,     0,     0,     0,    86,     0,   128,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    85,     0,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   117,     0,   226,     0,     0,    86,     0,   128,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    85,
       0,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,   227,   116,   117,     0,     0,     0,     0,    86,
       0,   128,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    85,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   117,     0,   228,     0,
       0,    86,     0,   128,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    85,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     0,   263,   116,   117,     0,
       0,     0,     0,    86,     0,   128,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    85,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,     0,   264,   116,
     117,     0,     0,     0,     0,    86,     0,   128,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    85,     0,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,     0,
     265,   116,   117,     0,     0,     0,     0,    86,     0,   128,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    85,
       0,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,     0,     0,   116,   117,     0,     0,     0,     0,    86,
       0,   128,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    85,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   117,   229,     0,     0,
       0,    86,     0,     0,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    85,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     0,     0,   116,   117,     0,
       0,     0,     0,    86,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    85,     0,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,     0,     0,   116,
     117,     0,     0,     0,     0,    86,     0,     0,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    85,     0,   105,     0,     0,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,     0,
       0,   116,   117,     0,    86,     0,     0,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    85,     0,     0,     0,     0,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,     0,     0,
     116,   117,     0,    86,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    86,     0,   116,
     117,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    86,     0,   116,   117,     0,     0,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    86,     0,   116,   117,     0,
       0,    90,    91,    92,    93,    94,    95,    96,    97,     0,
      99,   100,   101,   102,     0,     0,     0,     0,     0,     0,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    86,
       0,   116,   117,     0,     0,    90,    91,    92,    93,    94,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,     0,   116,   117
};

static const short yycheck[] =
{
       2,    15,    17,    18,    15,    15,    40,    40,    75,    18,
      36,    26,    27,    28,    39,    41,    42,    19,     0,    34,
      79,    75,    24,    25,    39,    84,    41,    29,    30,    31,
      32,    33,    57,    75,    36,    37,    64,    65,    66,    36,
      36,    56,    15,    40,    37,    41,    42,    81,    81,    76,
      43,    44,    45,    46,    47,    48,    79,     1,    36,     3,
      76,    84,    76,    41,    42,    76,    80,    77,    77,    83,
      79,    75,    65,    66,    67,    68,    69,    70,    41,    42,
      73,    74,    76,    80,    86,    87,    88,    89,    75,    91,
      92,    93,    94,   121,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    77,   116,    79,    77,    77,    79,    79,
      75,    75,   119,   120,    75,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    77,   141,
      79,   143,    75,   145,    37,    75,    -1,    37,    79,   146,
      43,   153,    79,    46,   151,    48,    46,    76,    48,    77,
      77,   189,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,    70,    -1,    -1,
      73,    74,    -1,    73,    74,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,    -1,   216,    -1,    -1,    -1,    -1,   221,
      -1,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   251,
     252,   253,   254,   255,   256,    -1,    -1,    -1,    -1,    -1,
      -1,   263,   264,   265,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    59,
      -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    -1,    78,    -1,
      80,    -1,    82,    83,    59,    85,    86,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    -1,
      85,    86,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    59,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,    80,
      -1,    82,    83,    59,    85,    86,    -1,    63,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    78,    -1,    80,    -1,    82,    83,    -1,    85,
      86,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    59,    -1,    -1,
      -1,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    78,    -1,    80,    -1,
      82,    83,    59,    85,    86,    -1,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    -1,    80,    -1,    82,    83,    -1,    85,    86,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    78,    -1,    80,    -1,    82,
      83,    -1,    85,    86,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    83,    -1,    85,    86,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    59,    -1,    -1,    -1,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    -1,    80,    -1,    82,    83,    59,
      85,    86,    -1,    63,    64,    65,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    83,    -1,    85,    86,    37,    -1,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    -1,
      17,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    73,    74,    -1,    -1,    77,    -1,    79,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      -1,    78,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    17,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    -1,    73,    74,
      -1,    -1,    77,    -1,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    17,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    -1,    77,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    17,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    -1,    77,    -1,    37,    -1,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    -1,    77,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      77,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    -1,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    17,    -1,    73,    74,
      75,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    17,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    30,    73,    74,    -1,    -1,    35,    -1,    37,    -1,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    30,    73,    74,    -1,    -1,    35,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    30,    73,    74,    -1,    -1,
      35,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    17,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    -1,    30,    73,    74,
      -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    17,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    30,
      73,    74,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    17,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    32,    -1,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    17,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    30,    73,    74,    -1,    -1,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    17,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    -1,    73,    74,    -1,
      34,    -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    17,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    30,    73,
      74,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    17,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      -1,    73,    74,    -1,    34,    -1,    -1,    37,    -1,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    17,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    30,    73,    74,    -1,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    17,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    73,    74,    -1,    34,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    17,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    30,    73,    74,    -1,
      -1,    -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    17,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    30,    73,
      74,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    17,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      30,    73,    74,    -1,    -1,    -1,    -1,    37,    -1,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    17,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    17,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    73,    74,    33,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    17,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    17,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    17,    -1,    58,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      -1,    73,    74,    -1,    37,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    17,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    37,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    37,    -1,    73,
      74,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    37,    -1,    73,    74,    -1,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    37,    -1,    73,    74,    -1,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    37,
      -1,    73,    74,    -1,    -1,    43,    44,    45,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    73,    74
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,     3,    88,    75,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    59,    63,    64,    65,    75,    76,    78,    80,    82,
      83,    85,    86,    89,    90,    91,     0,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    76,    91,    94,    91,
      92,    89,    89,    89,    91,    91,    91,    89,    89,    89,
      89,    89,    91,    89,    96,    89,    89,    96,    97,    80,
      83,    91,    94,    96,    91,    17,    37,    39,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    73,    74,    75,    76,
      76,    77,    91,    95,    94,    41,    42,    93,    39,    30,
      30,    36,    93,    36,    93,    36,    93,    25,    26,    32,
      38,    39,    77,    79,    78,    79,    40,    81,    97,    96,
      84,    76,    62,    79,    89,    75,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    77,    96,    77,    96,    93,    18,    77,
      79,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    77,    40,    89,    89,    96,    81,    84,
      77,    96,    77,    79,    89,    77,    79,    77,    77,    89,
      77,    93,    91,    30,    34,    30,    34,    30,    34,    33,
      89,    77,    77,    89,    77,    62,    77,    89,    93,    89,
      89,    89,    89,    89,    89,    89,    89,    77,    77,    77,
      89,    30,    35,    30,    35,    30,    35,    89,    89,    89,
      89,    89,    89,    30,    30,    30,    89,    89,    89
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
					  stack_push(&evalstack,
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
    { PUSH_ACT(E_MINUS); }
    break;

  case 26:
#line 156 "parse.y"
    { PUSH_ACT(E_MUL); }
    break;

  case 27:
#line 157 "parse.y"
    { PUSH_ACT(E_ELTMUL); }
    break;

  case 28:
#line 158 "parse.y"
    { PUSH_ACT(E_DIV); }
    break;

  case 29:
#line 159 "parse.y"
    { PUSH_ACT(E_ELTDIV); }
    break;

  case 30:
#line 160 "parse.y"
    { PUSH_ACT(E_BACK_DIV); }
    break;

  case 31:
#line 161 "parse.y"
    { PUSH_ACT(E_ELT_BACK_DIV); }
    break;

  case 32:
#line 162 "parse.y"
    { PUSH_ACT(E_MOD); }
    break;

  case 33:
#line 163 "parse.y"
    { PUSH_ACT(E_ELTMOD); }
    break;

  case 34:
#line 164 "parse.y"
    { PUSH_ACT(E_CMP_CMP); }
    break;

  case 35:
#line 166 "parse.y"
    { PUSH_ACT(E_EQ_CMP); }
    break;

  case 36:
#line 167 "parse.y"
    { PUSH_ACT(E_NE_CMP); }
    break;

  case 37:
#line 168 "parse.y"
    { PUSH_ACT(E_LT_CMP); }
    break;

  case 38:
#line 169 "parse.y"
    { PUSH_ACT(E_GT_CMP); }
    break;

  case 39:
#line 170 "parse.y"
    { PUSH_ACT(E_LE_CMP); }
    break;

  case 40:
#line 171 "parse.y"
    { PUSH_ACT(E_GE_CMP); }
    break;

  case 41:
#line 173 "parse.y"
    { PUSH_ACT(E_LOGICAL_AND); }
    break;

  case 42:
#line 174 "parse.y"
    { PUSH_ACT(E_LOGICAL_OR); }
    break;

  case 43:
#line 175 "parse.y"
    { PUSH_ACT(E_LOGICAL_XOR); }
    break;

  case 44:
#line 176 "parse.y"
    { PUSH_ACT(E_LOGICAL_NOT); }
    break;

  case 45:
#line 181 "parse.y"
    { PUSH_ACT(E_FACT); }
    break;

  case 46:
#line 182 "parse.y"
    { PUSH_ACT(E_DBLFACT); }
    break;

  case 47:
#line 183 "parse.y"
    { PUSH_ACT(E_CONJUGATE_TRANSPOSE); }
    break;

  case 48:
#line 184 "parse.y"
    { PUSH_ACT(E_TRANSPOSE); }
    break;

  case 49:
#line 185 "parse.y"
    { PUSH_ACT(E_NEG); }
    break;

  case 51:
#line 187 "parse.y"
    { PUSH_ACT(E_EXP); }
    break;

  case 52:
#line 188 "parse.y"
    { PUSH_ACT(E_ELTEXP); }
    break;

  case 53:
#line 190 "parse.y"
    {
				if (gp_prepare_push_region_sep ()) {
					PUSH_ACT(E_REGION_SEP_BY);
				} else {
					PUSH_ACT(E_REGION_SEP);
				}
					}
    break;

  case 54:
#line 198 "parse.y"
    { /* FIXME: do nothing?, this is just a 
					     get all */ }
    break;

  case 55:
#line 200 "parse.y"
    { PUSH_ACT(E_GET_VELEMENT); }
    break;

  case 56:
#line 201 "parse.y"
    { PUSH_ACT(E_GET_ELEMENT); }
    break;

  case 57:
#line 202 "parse.y"
    { PUSH_ACT(E_GET_ROW_REGION); }
    break;

  case 58:
#line 203 "parse.y"
    { PUSH_ACT(E_GET_ROW_REGION); }
    break;

  case 59:
#line 204 "parse.y"
    { PUSH_ACT(E_GET_COL_REGION); }
    break;

  case 60:
#line 205 "parse.y"
    { PUSH_ACT(E_GET_COL_REGION); }
    break;

  case 61:
#line 206 "parse.y"
    { if(!gp_push_matrix(FALSE)) {SYNTAX_ERROR;} }
    break;

  case 62:
#line 207 "parse.y"
    { if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;} }
    break;

  case 63:
#line 214 "parse.y"
    {
			if(!gp_push_matrix_row()) {SYNTAX_ERROR;}
			if(!gp_push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;}
			if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;}
					}
    break;

  case 64:
#line 219 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 65:
#line 220 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 66:
#line 221 "parse.y"
    { PUSH_ACT(E_WHILE_CONS); }
    break;

  case 67:
#line 222 "parse.y"
    { PUSH_ACT(E_UNTIL_CONS); }
    break;

  case 68:
#line 223 "parse.y"
    { PUSH_ACT(E_DOWHILE_CONS); }
    break;

  case 69:
#line 224 "parse.y"
    { PUSH_ACT(E_DOUNTIL_CONS); }
    break;

  case 70:
#line 225 "parse.y"
    { PUSH_ACT(E_FOR_CONS); }
    break;

  case 71:
#line 226 "parse.y"
    { PUSH_ACT(E_FORBY_CONS); }
    break;

  case 72:
#line 227 "parse.y"
    { PUSH_ACT(E_FORIN_CONS); }
    break;

  case 73:
#line 228 "parse.y"
    { PUSH_ACT(E_SUM_CONS); }
    break;

  case 74:
#line 229 "parse.y"
    { PUSH_ACT(E_SUMBY_CONS); }
    break;

  case 75:
#line 230 "parse.y"
    { PUSH_ACT(E_SUMIN_CONS); }
    break;

  case 76:
#line 231 "parse.y"
    { PUSH_ACT(E_PROD_CONS); }
    break;

  case 77:
#line 232 "parse.y"
    { PUSH_ACT(E_PRODBY_CONS); }
    break;

  case 78:
#line 233 "parse.y"
    { PUSH_ACT(E_PRODIN_CONS); }
    break;

  case 79:
#line 234 "parse.y"
    { PUSH_ACT(E_IF_CONS); }
    break;

  case 80:
#line 235 "parse.y"
    { PUSH_ACT(E_IFELSE_CONS); }
    break;

  case 81:
#line 236 "parse.y"
    { gp_convert_identifier_to_bool ();
					  /* convert true/false to bool */}
    break;

  case 82:
#line 238 "parse.y"
    { PUSH_ACT(E_QUOTE); }
    break;

  case 83:
#line 239 "parse.y"
    { PUSH_ACT(E_REFERENCE); }
    break;

  case 85:
#line 241 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); }
    break;

  case 86:
#line 243 "parse.y"
    { PUSH_ACT(E_DIRECTCALL); }
    break;

  case 87:
#line 244 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_DIRECTCALL); }
    break;

  case 88:
#line 246 "parse.y"
    { PUSH_ACT(E_DIRECTCALL); }
    break;

  case 89:
#line 247 "parse.y"
    { PUSH_ACT(E_CALL); }
    break;

  case 90:
#line 248 "parse.y"
    { gp_push_marker_simple(EXPRLIST_START_NODE);
					  PUSH_ACT(E_CALL); }
    break;

  case 91:
#line 250 "parse.y"
    { PUSH_ACT(E_DEFEQUALS); }
    break;

  case 95:
#line 254 "parse.y"
    { PUSH_ACT(E_RETURN); }
    break;

  case 96:
#line 255 "parse.y"
    { PUSH_ACT(E_BAILOUT); }
    break;

  case 97:
#line 256 "parse.y"
    { PUSH_ACT(E_EXCEPTION); }
    break;

  case 98:
#line 257 "parse.y"
    { PUSH_ACT(E_CONTINUE); }
    break;

  case 99:
#line 258 "parse.y"
    { PUSH_ACT(E_BREAK); }
    break;

  case 100:
#line 259 "parse.y"
    { stack_push(&evalstack,
						     gel_makenum_use(yyvsp[0].val)); }
    break;

  case 101:
#line 261 "parse.y"
    { PUSH_CONST_STRING(yyvsp[0].id); }
    break;

  case 102:
#line 262 "parse.y"
    { gp_push_null(); }
    break;

  case 103:
#line 265 "parse.y"
    { PUSH_ACT(E_DEREFERENCE); }
    break;

  case 104:
#line 268 "parse.y"
    { PUSH_IDENTIFIER(yyvsp[0].id); }
    break;

  case 105:
#line 271 "parse.y"
    {
			gp_prepare_push_param (FALSE);
			PUSH_ACT (E_PARAMETER);
		}
    break;

  case 108:
#line 285 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 109:
#line 290 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 110:
#line 295 "parse.y"
    {
			if ( ! gp_push_marker (EXPRLIST_START_NODE)) {
				SYNTAX_ERROR;
			}
			if ( ! gp_push_func (FALSE /* vararg */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 112:
#line 306 "parse.y"
    { if(!gp_push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 115:
#line 313 "parse.y"
    { if(!gp_push_marker(EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 116:
#line 316 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} }
    break;

  case 117:
#line 317 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} if(!gp_push_marker(MATRIX_START_NODE)) {SYNTAX_ERROR;} }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 2336 "y.tab.c"

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


#line 320 "parse.y"


