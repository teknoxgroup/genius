/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

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
     LOCAL = 280,
     WHILE = 281,
     UNTIL = 282,
     FOR = 283,
     SUM = 284,
     PROD = 285,
     DO = 286,
     IF = 287,
     THEN = 288,
     ELSE = 289,
     TO = 290,
     BY = 291,
     IN = 292,
     AT = 293,
     MAKEIMAGPARENTH = 294,
     SEPAR = 295,
     NEXTROW = 296,
     EQUALS = 297,
     DEFEQUALS = 298,
     TRANSPOSE = 299,
     ELTELTDIV = 300,
     ELTELTMUL = 301,
     ELTELTPLUS = 302,
     ELTELTMINUS = 303,
     ELTELTEXP = 304,
     ELTELTMOD = 305,
     DOUBLEFACT = 306,
     EQ_CMP = 307,
     NE_CMP = 308,
     CMP_CMP = 309,
     LT_CMP = 310,
     GT_CMP = 311,
     LE_CMP = 312,
     GE_CMP = 313,
     LOGICAL_XOR = 314,
     LOGICAL_OR = 315,
     LOGICAL_AND = 316,
     LOGICAL_NOT = 317,
     LOWER_THAN_ELSE = 318,
     MOD = 319,
     ELTELTBACKDIV = 320,
     UPLUS = 321,
     UMINUS = 322
   };
#endif
/* Tokens.  */
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
#define LOCAL 280
#define WHILE 281
#define UNTIL 282
#define FOR 283
#define SUM 284
#define PROD 285
#define DO 286
#define IF 287
#define THEN 288
#define ELSE 289
#define TO 290
#define BY 291
#define IN 292
#define AT 293
#define MAKEIMAGPARENTH 294
#define SEPAR 295
#define NEXTROW 296
#define EQUALS 297
#define DEFEQUALS 298
#define TRANSPOSE 299
#define ELTELTDIV 300
#define ELTELTMUL 301
#define ELTELTPLUS 302
#define ELTELTMINUS 303
#define ELTELTEXP 304
#define ELTELTMOD 305
#define DOUBLEFACT 306
#define EQ_CMP 307
#define NE_CMP 308
#define CMP_CMP 309
#define LT_CMP 310
#define GT_CMP 311
#define LE_CMP 312
#define GE_CMP 313
#define LOGICAL_XOR 314
#define LOGICAL_OR 315
#define LOGICAL_AND 316
#define LOGICAL_NOT 317
#define LOWER_THAN_ELSE 318
#define MOD 319
#define ELTELTBACKDIV 320
#define UPLUS 321
#define UMINUS 322




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

extern gboolean gel_return_ret; /*should the lexer return on \n*/

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

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 47 "parse.y"
{
	mpw_t val;
	char *id;
}
/* Line 187 of yacc.c.  */
#line 261 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 274 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
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
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  47
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2422

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  90
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNRULES -- Number of states.  */
#define YYNSTATES  304

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      78,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    77,     2,     2,     2,    71,    88,    73,
      79,    80,    68,    66,    82,    67,    89,    69,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    65,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    83,    70,    84,    76,     2,    85,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    86,    81,    87,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      72,    74,    75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    31,
      35,    39,    43,    46,    51,    54,    56,    60,    65,    70,
      74,    79,    83,    87,    91,    95,    99,   103,   107,   111,
     115,   119,   123,   127,   131,   135,   139,   143,   147,   151,
     155,   159,   163,   167,   171,   175,   179,   183,   187,   190,
     193,   196,   199,   202,   205,   208,   212,   216,   220,   225,
     230,   237,   243,   250,   256,   263,   267,   272,   278,   282,
     287,   292,   297,   302,   307,   316,   327,   334,   343,   354,
     361,   370,   381,   388,   393,   400,   402,   405,   408,   410,
     414,   419,   423,   428,   434,   439,   443,   446,   449,   452,
     455,   457,   459,   461,   463,   465,   467,   469,   472,   474,
     478,   480,   482,   488,   495,   500,   509,   519,   527,   535,
     544,   551,   555,   557,   561,   566,   568,   572
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      91,     0,    -1,     3,    92,    78,    -1,     3,     4,    78,
      -1,     3,     5,    78,    -1,     3,     7,    78,    -1,     3,
       9,    78,    -1,     3,    10,    78,    -1,     3,    11,    78,
      -1,     3,    12,    78,    -1,     3,     8,    78,    -1,     3,
       6,    78,    -1,     3,    78,    -1,     3,    92,    40,    78,
      -1,     1,    78,    -1,     1,    -1,    92,    40,    92,    -1,
      25,    68,    40,    92,    -1,    25,    98,    40,    92,    -1,
      92,    64,    92,    -1,    79,    92,    40,    80,    -1,    79,
      92,    80,    -1,    79,    92,    39,    -1,    92,    42,    92,
      -1,    92,    43,    92,    -1,    81,    92,    81,    -1,    92,
      66,    92,    -1,    92,    47,    92,    -1,    92,    67,    92,
      -1,    92,    48,    92,    -1,    92,    68,    92,    -1,    92,
      46,    92,    -1,    92,    69,    92,    -1,    92,    45,    92,
      -1,    92,    70,    92,    -1,    92,    72,    92,    -1,    92,
      71,    92,    -1,    92,    50,    92,    -1,    92,    54,    92,
      -1,    92,    52,    92,    -1,    92,    53,    92,    -1,    92,
      55,    92,    -1,    92,    56,    92,    -1,    92,    57,    92,
      -1,    92,    58,    92,    -1,    92,    61,    92,    -1,    92,
      60,    92,    -1,    92,    59,    92,    -1,    62,    92,    -1,
      92,    77,    -1,    92,    51,    -1,    92,    73,    -1,    92,
      44,    -1,    67,    92,    -1,    66,    92,    -1,    92,    76,
      92,    -1,    92,    49,    92,    -1,    92,    65,    92,    -1,
      92,    38,    65,    80,    -1,    92,    38,    92,    80,    -1,
      92,    38,    92,    82,    92,    80,    -1,    92,    38,    92,
      82,    80,    -1,    92,    38,    92,    82,    65,    80,    -1,
      92,    38,    82,    92,    80,    -1,    92,    38,    65,    82,
      92,    80,    -1,    83,   100,    84,    -1,    85,    83,   100,
      84,    -1,    79,    99,    82,    92,    80,    -1,    86,    99,
      87,    -1,    85,    86,    99,    87,    -1,    26,    92,    31,
      92,    -1,    27,    92,    31,    92,    -1,    31,    92,    26,
      92,    -1,    31,    92,    27,    92,    -1,    28,    94,    96,
      92,    35,    92,    31,    92,    -1,    28,    94,    96,    92,
      35,    92,    36,    92,    31,    92,    -1,    28,    94,    37,
      92,    31,    92,    -1,    29,    94,    96,    92,    35,    92,
      31,    92,    -1,    29,    94,    96,    92,    35,    92,    36,
      92,    31,    92,    -1,    29,    94,    37,    92,    31,    92,
      -1,    30,    94,    96,    92,    35,    92,    31,    92,    -1,
      30,    94,    96,    92,    35,    92,    36,    92,    31,    92,
      -1,    30,    94,    37,    92,    31,    92,    -1,    32,    92,
      33,    92,    -1,    32,    92,    33,    92,    34,    92,    -1,
      94,    -1,    85,    94,    -1,    88,    94,    -1,    93,    -1,
      94,    79,    80,    -1,    94,    79,    99,    80,    -1,    93,
      79,    80,    -1,    93,    79,    99,    80,    -1,    92,    17,
      79,    99,    80,    -1,    92,    17,    79,    80,    -1,    16,
      94,    97,    -1,    19,    95,    -1,    16,    97,    -1,    85,
      97,    -1,    20,    92,    -1,    21,    -1,    22,    -1,    23,
      -1,    24,    -1,    13,    -1,    14,    -1,    89,    -1,    68,
      94,    -1,    15,    -1,    94,    96,    92,    -1,    42,    -1,
      43,    -1,    79,    98,    80,    96,    92,    -1,    79,    98,
      18,    80,    96,    92,    -1,    79,    80,    96,    92,    -1,
      79,    98,    80,    83,    98,    84,    96,    92,    -1,    79,
      98,    18,    80,    83,    98,    84,    96,    92,    -1,    79,
      80,    83,    98,    84,    96,    92,    -1,    79,    98,    80,
      83,    84,    96,    92,    -1,    79,    98,    18,    80,    83,
      84,    96,    92,    -1,    79,    80,    83,    84,    96,    92,
      -1,    98,    82,    94,    -1,    94,    -1,    99,    82,    92,
      -1,    99,    82,    41,    92,    -1,    92,    -1,   100,    41,
      99,    -1,    99,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   116,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   132,   133,   137,   141,
     142,   144,   145,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   169,
     170,   171,   172,   173,   174,   176,   177,   178,   179,   184,
     185,   186,   187,   188,   189,   190,   191,   193,   201,   203,
     204,   205,   206,   207,   208,   209,   210,   217,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   241,   242,   243,   244,
     246,   247,   249,   250,   251,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   264,   265,   268,   271,   274,
     284,   285,   288,   296,   304,   312,   320,   328,   337,   345,
     353,   363,   364,   367,   370,   371,   374,   375
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STARTTOK", "LOADFILE", "LOADFILE_GLOB",
  "LOAD_PLUGIN", "CHANGEDIR", "PWD", "LS", "LS_ARG", "HELP", "HELP_ARG",
  "NUMBER", "STRING", "FUNCID", "FUNCTION", "CALL", "THREEDOTS",
  "PARAMETER", "RETURNTOK", "BAILOUT", "EXCEPTION", "CONTINUE", "BREAK",
  "LOCAL", "WHILE", "UNTIL", "FOR", "SUM", "PROD", "DO", "IF", "THEN",
  "ELSE", "TO", "BY", "IN", "AT", "MAKEIMAGPARENTH", "SEPAR", "NEXTROW",
  "EQUALS", "DEFEQUALS", "TRANSPOSE", "ELTELTDIV", "ELTELTMUL",
  "ELTELTPLUS", "ELTELTMINUS", "ELTELTEXP", "ELTELTMOD", "DOUBLEFACT",
  "EQ_CMP", "NE_CMP", "CMP_CMP", "LT_CMP", "GT_CMP", "LE_CMP", "GE_CMP",
  "LOGICAL_XOR", "LOGICAL_OR", "LOGICAL_AND", "LOGICAL_NOT",
  "LOWER_THAN_ELSE", "MOD", "':'", "'+'", "'-'", "'*'", "'/'", "'\\\\'",
  "'%'", "ELTELTBACKDIV", "'''", "UPLUS", "UMINUS", "'^'", "'!'", "'\\n'",
  "'('", "')'", "'|'", "','", "'['", "']'", "'`'", "'{'", "'}'", "'&'",
  "'.'", "$accept", "fullexpr", "expr", "deref", "ident", "paramdef",
  "anyequals", "funcdef", "identlist", "exprlist", "matrixrows", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    58,    43,    45,    42,    47,
      92,    37,   320,    39,   321,   322,    94,    33,    10,    40,
      41,   124,    44,    91,    93,    96,   123,   125,    38,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    90,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    94,    95,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    98,    98,    99,    99,    99,   100,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     4,     2,     1,     3,     4,     4,     3,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     4,     4,
       6,     5,     6,     5,     6,     3,     4,     5,     3,     4,
       4,     4,     4,     4,     8,    10,     6,     8,    10,     6,
       8,    10,     6,     4,     6,     1,     2,     2,     1,     3,
       4,     3,     4,     5,     4,     3,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       1,     1,     5,     6,     4,     8,     9,     7,     7,     8,
       6,     3,     1,     3,     4,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    15,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,   105,   108,     0,     0,     0,
     100,   101,   102,   103,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,   106,     0,    88,    85,     1,     3,     4,
      11,     5,    10,     6,     7,     8,     9,     0,     0,    97,
       0,    96,    99,     0,   122,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    54,    53,   107,   125,     0,     0,
     125,   127,     0,     0,     0,    86,    98,     0,    87,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,    49,     2,     0,     0,     0,     0,    95,
     110,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
      21,     0,    25,     0,     0,    65,     0,     0,    68,     0,
       0,     0,     0,    13,    16,    23,    24,    33,    31,    27,
      29,    56,    37,    39,    40,    38,    41,    42,    43,    44,
      47,    46,    45,    19,    57,    26,    28,    30,    32,    34,
      36,    35,    55,    91,     0,    89,     0,     0,     0,     0,
       0,   109,    17,    18,   121,    70,    71,     0,     0,     0,
       0,     0,     0,    72,    73,    83,    20,     0,   123,   123,
     126,    66,    69,    94,     0,    58,     0,     0,    59,     0,
      92,    90,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   124,    67,    93,     0,    63,
       0,    61,     0,     0,     0,     0,     0,     0,     0,   112,
      76,     0,    79,     0,    82,     0,    84,    64,    62,    60,
     120,     0,     0,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,   118,     0,    74,     0,
      77,     0,    80,     0,   119,     0,   115,     0,     0,     0,
     116,    75,    78,    81
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    80,    45,    46,    61,   132,    59,    65,    81,
      82
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -67
static const yytype_int16 yypact[] =
{
      49,   -65,   294,    37,   -67,   -62,   -37,   -11,     5,    14,
      43,    44,    75,    78,   -67,   -67,   -67,    -1,    34,   730,
     -67,   -67,   -67,   -67,    -6,   730,   730,    34,    34,    34,
     730,   730,   730,   730,   730,    34,   -67,   730,   730,   730,
     -14,   730,    34,   -67,  1097,   -20,    20,   -67,   -67,   -67,
     -67,   -67,   -67,   -67,   -67,   -67,   -67,    -5,    92,   -67,
      21,   -67,  2096,    31,   -67,   -25,  1382,  1439,    82,    86,
     121,  1154,  1484,  2183,   116,   116,   -67,   872,    79,   827,
    2006,    96,   -30,   730,   730,   -67,   -67,   -60,   -67,   101,
     319,   421,   730,   730,   -67,   730,   730,   730,   730,   730,
     730,   -67,   730,   730,   730,   730,   730,   730,   730,   730,
     730,   730,   730,   730,   730,   730,   730,   730,   730,   730,
     730,   -67,   730,   -67,   -67,   498,   523,   -23,     0,   -67,
     -67,   -67,   730,   730,   730,   730,    34,   730,   730,   730,
     730,   730,   730,   730,   730,   730,   730,   730,   -67,   600,
     -67,   628,   -67,   628,   730,   -67,   -29,   -34,   -67,   705,
     -12,   730,   782,   -67,  2096,  2225,  2225,   124,   124,   320,
     320,   116,   124,  2309,  2309,  2267,  2309,  2309,  2309,  2309,
    2141,  2141,  2183,  2225,  2345,   320,   320,   124,   124,   124,
     124,   124,   116,   -67,    68,   -67,    90,   -10,   730,    77,
     -17,  2225,  2096,  2096,   -67,  2096,  2096,  1541,  1586,  1643,
    1688,  1745,  1790,  2096,  2096,  2051,   -67,   730,   917,  2006,
      96,   -67,   -67,   -67,    94,   -67,   730,   962,   -67,   396,
     -67,   -67,    21,    -3,  2096,     4,    -8,   730,   730,   730,
     730,   730,   730,   730,   730,  2006,   -67,   -67,  1007,   -67,
     102,   -67,  1052,   730,    21,    -7,   730,    21,     2,  2096,
    2096,  1211,  2096,  1268,  2096,  1325,  2096,   -67,   -67,   -67,
    2096,   730,    21,    95,  2096,   730,    21,   730,   730,   730,
     730,   730,   730,  2096,   730,    21,  2096,   730,  2096,  1847,
    2096,  1904,  2096,  1961,  2096,   730,  2096,   730,   730,   730,
    2096,  2096,  2096,  2096
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -67,   -67,    -2,   -67,    16,   -67,   -66,   -19,   -51,     1,
      98
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      44,    16,   140,   142,   144,    16,   128,    16,    16,    16,
      16,   154,   154,     4,    16,   135,    48,    62,   199,   130,
     131,    86,   153,    66,    67,   130,   131,   158,    71,    72,
      73,    74,    75,    58,    60,    77,    79,    47,    78,   129,
      64,    49,    87,    68,    69,    70,   130,   131,   153,    16,
       1,    76,     2,   222,   155,   221,    85,   136,    88,   125,
     197,   198,    63,   130,   131,    57,   236,    50,   225,    83,
     226,   134,    84,    64,   232,   127,   257,   272,    57,   136,
     200,   254,   136,    51,   136,   157,   276,   255,   162,   164,
     165,   166,    52,   167,   168,   169,   170,   171,   172,   126,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   139,
     192,    53,    54,   141,   130,   131,   194,   196,   130,   131,
     201,   164,   202,   203,   237,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   233,   164,   230,   218,
     153,   219,   204,    55,    90,   220,    56,   235,   143,   227,
     224,   151,    90,   130,   131,    99,   253,   101,    94,   256,
     231,    57,   153,    99,   247,   101,   153,   136,   153,   285,
     159,   156,   268,     0,     0,   258,     0,     0,   271,     0,
       0,   275,   122,   123,     0,     0,   234,   121,     0,     0,
     122,   123,     0,     0,   273,     0,   284,     0,     0,     0,
     287,     0,     0,    64,     0,   245,     0,     0,     0,   295,
       0,     0,     0,     0,   248,     0,     0,   252,     0,     0,
       0,     0,     0,     0,     0,   259,   260,   261,   262,   263,
     264,   265,   266,     0,     0,     0,     0,     0,     0,     0,
       0,   270,    64,     0,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   283,
       0,    64,     0,   286,     0,   288,   289,   290,   291,   292,
     293,     0,   294,     0,     0,   296,     0,     0,     0,     0,
       0,     0,     0,   300,     0,   301,   302,   303,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,     0,
       0,     0,    14,    15,    16,    17,     0,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,     0,     0,     0,    32,     0,    90,     0,
      33,    34,    35,     0,    94,    95,    96,     0,     0,    99,
     100,   101,    36,    37,     0,    38,     0,    39,     0,    40,
      41,    32,    42,    43,   160,    33,    34,    35,   116,   117,
     118,   119,   120,   121,     0,     0,   122,   123,    37,     0,
      38,   161,    39,     0,    40,    41,     0,    42,    43,    14,
      15,    16,    17,     0,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,    14,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     0,     0,     0,     0,    32,     0,
       0,   250,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,   251,    38,     0,    39,
       0,    40,    41,    32,    42,    43,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
      37,     0,    38,     0,    39,     0,    40,    41,     0,    42,
      43,    14,    15,    16,    17,     0,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,    14,    15,    16,    17,
       0,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,     0,     0,     0,
      32,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,   193,    38,
       0,    39,     0,    40,    41,    32,    42,    43,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,   195,    38,     0,    39,     0,    40,    41,
       0,    42,    43,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,     0,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,    32,     0,     0,     0,    33,    34,    35,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
     216,    38,     0,    39,     0,    40,    41,     0,    42,    43,
      32,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,     0,    40,    41,     0,    42,    43,    14,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,    32,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,   223,    38,     0,    39,     0,
      40,    41,    32,    42,    43,     0,    33,    34,    35,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,     0,    40,    41,     0,    42,    43,
      90,     0,   133,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    89,     0,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   122,   123,
       0,     0,   228,     0,   229,    90,     0,   133,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    89,
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   122,   123,     0,     0,     0,   152,     0,
      90,   148,   149,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    89,     0,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   122,   123,
       0,     0,   150,     0,     0,    90,     0,   133,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    89,
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   122,   123,     0,     0,   246,     0,     0,
      90,     0,   133,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    89,     0,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   122,   123,
       0,     0,   249,     0,     0,    90,     0,   133,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    89,
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   122,   123,     0,     0,   267,     0,     0,
      90,     0,   133,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    89,     0,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   122,   123,
       0,     0,   269,     0,     0,    90,     0,    91,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,     0,
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    89,     0,   122,   123,   124,     0,     0,     0,     0,
     145,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,   133,     0,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    89,     0,
     122,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   278,     0,    90,
       0,   133,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    89,     0,   122,   123,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   279,
       0,     0,     0,     0,   280,     0,    90,     0,   133,     0,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    89,     0,   122,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,     0,     0,
       0,   282,     0,    90,     0,   133,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,     0,     0,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    89,
       0,   122,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
      90,     0,   133,     0,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,     0,     0,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    89,     0,   122,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,    90,     0,   133,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    89,     0,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,     0,     0,   122,   123,   147,     0,     0,
       0,     0,    90,     0,   133,     0,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    89,     0,
     122,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,    90,
       0,   133,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    89,     0,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   122,   123,     0,
       0,   239,     0,     0,    90,     0,   133,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      89,     0,   122,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,     0,
       0,    90,     0,   133,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    89,     0,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,     0,     0,   122,
     123,     0,     0,   241,     0,     0,    90,     0,   133,     0,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    89,     0,   122,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,    90,     0,   133,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    89,     0,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   122,   123,     0,     0,   243,     0,     0,    90,     0,
     133,     0,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,     0,     0,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    89,     0,   122,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,    90,     0,   133,     0,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,     0,
       0,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    89,     0,   122,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,    90,     0,   133,     0,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    89,     0,
     122,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   299,     0,     0,     0,     0,     0,     0,    90,
       0,   133,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    89,     0,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   122,   123,     0,
       0,     0,     0,     0,    90,     0,   133,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    89,     0,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
       0,     0,   122,   123,     0,   244,     0,     0,     0,    90,
       0,     0,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    89,     0,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   122,   123,     0,
       0,     0,     0,     0,    90,     0,     0,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    89,     0,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
       0,     0,   122,   123,     0,     0,     0,     0,     0,    90,
       0,     0,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
      89,     0,   111,     0,     0,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   122,   123,     0,
       0,    90,     0,     0,     0,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    89,     0,     0,     0,     0,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,     0,     0,   122,
     123,     0,     0,    90,     0,     0,     0,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    89,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   122,   123,     0,     0,    90,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,   105,   106,   107,   108,    89,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   122,   123,     0,     0,    90,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     0,   105,   106,   107,   108,     0,     0,
       0,     0,     0,     0,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    90,     0,   122,   123,     0,     0,    94,
      95,    96,    97,    98,    99,   100,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   122,   123
};

static const yytype_int16 yycheck[] =
{
       2,    15,    68,    69,    70,    15,    57,    15,    15,    15,
      15,    41,    41,    78,    15,    40,    78,    19,    18,    42,
      43,    40,    82,    25,    26,    42,    43,    87,    30,    31,
      32,    33,    34,    17,    18,    37,    38,     0,    37,    58,
      24,    78,    41,    27,    28,    29,    42,    43,    82,    15,
       1,    35,     3,    87,    84,    84,    40,    82,    42,    79,
      83,   127,    68,    42,    43,    79,    83,    78,    80,    83,
      82,    40,    86,    57,    84,    80,    84,    84,    79,    82,
      80,    84,    82,    78,    82,    84,    84,    83,    90,    91,
      92,    93,    78,    95,    96,    97,    98,    99,   100,    79,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    37,
     122,    78,    78,    37,    42,    43,   125,   126,    42,    43,
     132,   133,   134,   135,   200,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   197,   149,    80,   151,
      82,   153,   136,    78,    38,   154,    78,    80,    37,   161,
     159,    82,    38,    42,    43,    49,   232,    51,    44,   235,
      80,    79,    82,    49,    80,    51,    82,    82,    82,    84,
      79,    83,    80,    -1,    -1,   236,    -1,    -1,   254,    -1,
      -1,   257,    76,    77,    -1,    -1,   198,    73,    -1,    -1,
      76,    77,    -1,    -1,   255,    -1,   272,    -1,    -1,    -1,
     276,    -1,    -1,   197,    -1,   217,    -1,    -1,    -1,   285,
      -1,    -1,    -1,    -1,   226,    -1,    -1,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,   239,   240,   241,
     242,   243,   244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   236,    -1,   256,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
      -1,   255,    -1,   275,    -1,   277,   278,   279,   280,   281,
     282,    -1,   284,    -1,    -1,   287,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   295,    -1,   297,   298,   299,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    62,    -1,    38,    -1,
      66,    67,    68,    -1,    44,    45,    46,    -1,    -1,    49,
      50,    51,    78,    79,    -1,    81,    -1,    83,    -1,    85,
      86,    62,    88,    89,    65,    66,    67,    68,    68,    69,
      70,    71,    72,    73,    -1,    -1,    76,    77,    79,    -1,
      81,    82,    83,    -1,    85,    86,    -1,    88,    89,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    65,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      -1,    85,    86,    62,    88,    89,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    81,    -1,    83,    -1,    85,    86,    -1,    88,
      89,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    -1,    85,    86,    62,    88,    89,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    -1,    85,    86,
      -1,    88,    89,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    62,    -1,    -1,    -1,    66,    67,    68,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    -1,    85,    86,    -1,    88,    89,
      62,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    85,    86,    -1,    88,    89,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    -1,
      85,    86,    62,    88,    89,    -1,    66,    67,    68,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    85,    86,    -1,    88,    89,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    17,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      -1,    -1,    80,    -1,    82,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    17,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    -1,    -1,    -1,    81,    -1,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    17,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      -1,    -1,    80,    -1,    -1,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    17,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    -1,    -1,    80,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    17,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      -1,    -1,    80,    -1,    -1,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    17,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    -1,    -1,    80,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    17,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    77,
      -1,    -1,    80,    -1,    -1,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    17,    -1,    76,    77,    78,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    17,    -1,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      -1,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    17,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    17,    -1,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    -1,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    17,
      -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    17,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    17,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    -1,    76,    77,    33,    -1,    -1,
      -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    17,    -1,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    17,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    -1,
      -1,    35,    -1,    -1,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      17,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    17,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    76,
      77,    -1,    -1,    35,    -1,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    17,    -1,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    17,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    -1,    -1,    35,    -1,    -1,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    -1,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    17,    -1,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    17,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    17,    -1,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    17,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    17,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    -1,    34,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    17,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    17,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    -1,    61,    -1,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    -1,    76,    77,    -1,
      -1,    38,    -1,    -1,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    -1,    76,
      77,    -1,    -1,    38,    -1,    -1,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    17,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    17,    -1,    -1,    -1,
      -1,    -1,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    77,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    38,    -1,    76,    77,    -1,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      -1,    76,    77
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    91,    78,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    62,    66,    67,    68,    78,    79,    81,    83,
      85,    86,    88,    89,    92,    93,    94,     0,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    79,    94,    97,
      94,    95,    92,    68,    94,    98,    92,    92,    94,    94,
      94,    92,    92,    92,    92,    92,    94,    92,    99,    92,
      92,    99,   100,    83,    86,    94,    97,    99,    94,    17,
      38,    40,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    76,    77,    78,    79,    79,    80,    98,    97,
      42,    43,    96,    40,    40,    40,    82,    31,    31,    37,
      96,    37,    96,    37,    96,    26,    27,    33,    39,    40,
      80,    82,    81,    82,    41,    84,   100,    99,    87,    79,
      65,    82,    92,    78,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    80,    99,    80,    99,    83,    96,    18,
      80,    92,    92,    92,    94,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    80,    41,    92,    92,
      99,    84,    87,    80,    99,    80,    82,    92,    80,    82,
      80,    80,    84,    98,    92,    80,    83,    96,    31,    35,
      31,    35,    31,    35,    34,    92,    80,    80,    92,    80,
      65,    80,    92,    96,    84,    83,    96,    84,    98,    92,
      92,    92,    92,    92,    92,    92,    92,    80,    80,    80,
      92,    96,    84,    98,    92,    96,    84,    31,    36,    31,
      36,    31,    36,    92,    96,    84,    92,    96,    92,    92,
      92,    92,    92,    92,    92,    96,    92,    31,    31,    31,
      92,    92,    92,    92
};

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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
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

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
#line 116 "parse.y"
    { YYACCEPT; }
    break;

  case 3:
#line 117 "parse.y"
    { gel_command = GEL_LOADFILE; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 4:
#line 118 "parse.y"
    { gel_command = GEL_LOADFILE_GLOB; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 5:
#line 119 "parse.y"
    { gel_command = GEL_CHANGEDIR; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 6:
#line 120 "parse.y"
    { gel_command = GEL_LS; YYACCEPT; }
    break;

  case 7:
#line 121 "parse.y"
    { gel_command = GEL_LS_ARG; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 8:
#line 122 "parse.y"
    { gel_command = GEL_HELP; YYACCEPT; }
    break;

  case 9:
#line 123 "parse.y"
    { gel_command = GEL_HELP_ARG; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 10:
#line 124 "parse.y"
    { gel_command = GEL_PWD; YYACCEPT; }
    break;

  case 11:
#line 125 "parse.y"
    { gel_command = GEL_LOADPLUGIN; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 12:
#line 126 "parse.y"
    { YYACCEPT; }
    break;

  case 13:
#line 127 "parse.y"
    { gp_push_null(); PUSH_ACT(GEL_E_SEPAR); YYACCEPT; }
    break;

  case 14:
#line 128 "parse.y"
    { gel_return_ret = TRUE; yyclearin; YYABORT; }
    break;

  case 15:
#line 129 "parse.y"
    { gel_return_ret = TRUE; }
    break;

  case 16:
#line 132 "parse.y"
    { PUSH_ACT(GEL_E_SEPAR); }
    break;

  case 17:
#line 133 "parse.y"
    { if ( ! gp_push_local_all ()) {
						SYNTAX_ERROR;
					  }
       					}
    break;

  case 18:
#line 137 "parse.y"
    { if ( ! gp_push_local_idents ()) {
						SYNTAX_ERROR;
					  }
					}
    break;

  case 19:
#line 141 "parse.y"
    { PUSH_ACT(GEL_E_MOD_CALC); }
    break;

  case 20:
#line 142 "parse.y"
    { gp_push_null(); PUSH_ACT(GEL_E_SEPAR);
					  gp_push_spacer(); }
    break;

  case 21:
#line 144 "parse.y"
    { gp_push_spacer(); }
    break;

  case 22:
#line 145 "parse.y"
    { mpw_t i;
					  mpw_init (i);
					  mpw_i (i);
					  gp_push_spacer();
					  gel_stack_push(&gel_parsestack,
							 gel_makenum_use(i));
					  PUSH_ACT(GEL_E_MUL); }
    break;

  case 23:
#line 152 "parse.y"
    { PUSH_ACT(GEL_E_EQUALS); }
    break;

  case 24:
#line 153 "parse.y"
    { PUSH_ACT(GEL_E_DEFEQUALS); }
    break;

  case 25:
#line 154 "parse.y"
    { PUSH_ACT(GEL_E_ABS); }
    break;

  case 26:
#line 155 "parse.y"
    { PUSH_ACT(GEL_E_PLUS); }
    break;

  case 27:
#line 156 "parse.y"
    { PUSH_ACT(GEL_E_ELTPLUS); }
    break;

  case 28:
#line 157 "parse.y"
    { PUSH_ACT(GEL_E_MINUS); }
    break;

  case 29:
#line 158 "parse.y"
    { PUSH_ACT(GEL_E_ELTMINUS); }
    break;

  case 30:
#line 159 "parse.y"
    { PUSH_ACT(GEL_E_MUL); }
    break;

  case 31:
#line 160 "parse.y"
    { PUSH_ACT(GEL_E_ELTMUL); }
    break;

  case 32:
#line 161 "parse.y"
    { PUSH_ACT(GEL_E_DIV); }
    break;

  case 33:
#line 162 "parse.y"
    { PUSH_ACT(GEL_E_ELTDIV); }
    break;

  case 34:
#line 163 "parse.y"
    { PUSH_ACT(GEL_E_BACK_DIV); }
    break;

  case 35:
#line 164 "parse.y"
    { PUSH_ACT(GEL_E_ELT_BACK_DIV); }
    break;

  case 36:
#line 165 "parse.y"
    { PUSH_ACT(GEL_E_MOD); }
    break;

  case 37:
#line 166 "parse.y"
    { PUSH_ACT(GEL_E_ELTMOD); }
    break;

  case 38:
#line 167 "parse.y"
    { PUSH_ACT(GEL_E_CMP_CMP); }
    break;

  case 39:
#line 169 "parse.y"
    { PUSH_ACT(GEL_E_EQ_CMP); }
    break;

  case 40:
#line 170 "parse.y"
    { PUSH_ACT(GEL_E_NE_CMP); }
    break;

  case 41:
#line 171 "parse.y"
    { PUSH_ACT(GEL_E_LT_CMP); }
    break;

  case 42:
#line 172 "parse.y"
    { PUSH_ACT(GEL_E_GT_CMP); }
    break;

  case 43:
#line 173 "parse.y"
    { PUSH_ACT(GEL_E_LE_CMP); }
    break;

  case 44:
#line 174 "parse.y"
    { PUSH_ACT(GEL_E_GE_CMP); }
    break;

  case 45:
#line 176 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_AND); }
    break;

  case 46:
#line 177 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_OR); }
    break;

  case 47:
#line 178 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_XOR); }
    break;

  case 48:
#line 179 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_NOT); }
    break;

  case 49:
#line 184 "parse.y"
    { PUSH_ACT(GEL_E_FACT); }
    break;

  case 50:
#line 185 "parse.y"
    { PUSH_ACT(GEL_E_DBLFACT); }
    break;

  case 51:
#line 186 "parse.y"
    { PUSH_ACT(GEL_E_CONJUGATE_TRANSPOSE); }
    break;

  case 52:
#line 187 "parse.y"
    { PUSH_ACT(GEL_E_TRANSPOSE); }
    break;

  case 53:
#line 188 "parse.y"
    { PUSH_ACT(GEL_E_NEG); }
    break;

  case 55:
#line 190 "parse.y"
    { PUSH_ACT(GEL_E_EXP); }
    break;

  case 56:
#line 191 "parse.y"
    { PUSH_ACT(GEL_E_ELTEXP); }
    break;

  case 57:
#line 193 "parse.y"
    {
				if (gp_prepare_push_region_sep ()) {
					PUSH_ACT(GEL_E_REGION_SEP_BY);
				} else {
					PUSH_ACT(GEL_E_REGION_SEP);
				}
					}
    break;

  case 58:
#line 201 "parse.y"
    { /* FIXME: do nothing?, this is just a 
					     get all */ }
    break;

  case 59:
#line 203 "parse.y"
    { PUSH_ACT(GEL_E_GET_VELEMENT); }
    break;

  case 60:
#line 204 "parse.y"
    { PUSH_ACT(GEL_E_GET_ELEMENT); }
    break;

  case 61:
#line 205 "parse.y"
    { PUSH_ACT(GEL_E_GET_ROW_REGION); }
    break;

  case 62:
#line 206 "parse.y"
    { PUSH_ACT(GEL_E_GET_ROW_REGION); }
    break;

  case 63:
#line 207 "parse.y"
    { PUSH_ACT(GEL_E_GET_COL_REGION); }
    break;

  case 64:
#line 208 "parse.y"
    { PUSH_ACT(GEL_E_GET_COL_REGION); }
    break;

  case 65:
#line 209 "parse.y"
    { if(!gp_push_matrix(FALSE)) {SYNTAX_ERROR;} }
    break;

  case 66:
#line 210 "parse.y"
    { if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;} }
    break;

  case 67:
#line 217 "parse.y"
    {
			if(!gp_push_matrix_row()) {SYNTAX_ERROR;}
			if(!gp_push_marker(GEL_MATRIX_START_NODE)) {SYNTAX_ERROR;}
			if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;}
					}
    break;

  case 68:
#line 222 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 69:
#line 223 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 70:
#line 224 "parse.y"
    { PUSH_ACT(GEL_E_WHILE_CONS); }
    break;

  case 71:
#line 225 "parse.y"
    { PUSH_ACT(GEL_E_UNTIL_CONS); }
    break;

  case 72:
#line 226 "parse.y"
    { PUSH_ACT(GEL_E_DOWHILE_CONS); }
    break;

  case 73:
#line 227 "parse.y"
    { PUSH_ACT(GEL_E_DOUNTIL_CONS); }
    break;

  case 74:
#line 228 "parse.y"
    { PUSH_ACT(GEL_E_FOR_CONS); }
    break;

  case 75:
#line 229 "parse.y"
    { PUSH_ACT(GEL_E_FORBY_CONS); }
    break;

  case 76:
#line 230 "parse.y"
    { PUSH_ACT(GEL_E_FORIN_CONS); }
    break;

  case 77:
#line 231 "parse.y"
    { PUSH_ACT(GEL_E_SUM_CONS); }
    break;

  case 78:
#line 232 "parse.y"
    { PUSH_ACT(GEL_E_SUMBY_CONS); }
    break;

  case 79:
#line 233 "parse.y"
    { PUSH_ACT(GEL_E_SUMIN_CONS); }
    break;

  case 80:
#line 234 "parse.y"
    { PUSH_ACT(GEL_E_PROD_CONS); }
    break;

  case 81:
#line 235 "parse.y"
    { PUSH_ACT(GEL_E_PRODBY_CONS); }
    break;

  case 82:
#line 236 "parse.y"
    { PUSH_ACT(GEL_E_PRODIN_CONS); }
    break;

  case 83:
#line 237 "parse.y"
    { PUSH_ACT(GEL_E_IF_CONS); }
    break;

  case 84:
#line 238 "parse.y"
    { PUSH_ACT(GEL_E_IFELSE_CONS); }
    break;

  case 85:
#line 239 "parse.y"
    { gp_convert_identifier_to_bool ();
					  /* convert true/false to bool */}
    break;

  case 86:
#line 241 "parse.y"
    { PUSH_ACT(GEL_E_QUOTE); }
    break;

  case 87:
#line 242 "parse.y"
    { PUSH_ACT(GEL_E_REFERENCE); }
    break;

  case 89:
#line 244 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 90:
#line 246 "parse.y"
    { PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 91:
#line 247 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 92:
#line 249 "parse.y"
    { PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 93:
#line 250 "parse.y"
    { PUSH_ACT(GEL_E_CALL); }
    break;

  case 94:
#line 251 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_CALL); }
    break;

  case 95:
#line 253 "parse.y"
    { PUSH_ACT(GEL_E_DEFEQUALS); }
    break;

  case 99:
#line 257 "parse.y"
    { PUSH_ACT(GEL_E_RETURN); }
    break;

  case 100:
#line 258 "parse.y"
    { PUSH_ACT(GEL_E_BAILOUT); }
    break;

  case 101:
#line 259 "parse.y"
    { PUSH_ACT(GEL_E_EXCEPTION); }
    break;

  case 102:
#line 260 "parse.y"
    { PUSH_ACT(GEL_E_CONTINUE); }
    break;

  case 103:
#line 261 "parse.y"
    { PUSH_ACT(GEL_E_BREAK); }
    break;

  case 104:
#line 262 "parse.y"
    { gel_stack_push(&gel_parsestack,
							 gel_makenum_use((yyvsp[(1) - (1)].val))); }
    break;

  case 105:
#line 264 "parse.y"
    { PUSH_CONST_STRING((yyvsp[(1) - (1)].id)); }
    break;

  case 106:
#line 265 "parse.y"
    { gp_push_null(); }
    break;

  case 107:
#line 268 "parse.y"
    { PUSH_ACT(GEL_E_DEREFERENCE); }
    break;

  case 108:
#line 271 "parse.y"
    { PUSH_IDENTIFIER((yyvsp[(1) - (1)].id)); }
    break;

  case 109:
#line 274 "parse.y"
    {
			gp_prepare_push_param (FALSE);
			PUSH_ACT (GEL_E_PARAMETER);
		}
    break;

  case 112:
#line 288 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 113:
#line 296 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 114:
#line 304 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 115:
#line 312 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 116:
#line 320 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 117:
#line 328 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 118:
#line 337 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 119:
#line 345 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 120:
#line 353 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 122:
#line 364 "parse.y"
    { if(!gp_push_marker(GEL_EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 125:
#line 371 "parse.y"
    { if(!gp_push_marker(GEL_EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 126:
#line 374 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} }
    break;

  case 127:
#line 375 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} if(!gp_push_marker(GEL_MATRIX_START_NODE)) {SYNTAX_ERROR;} }
    break;


/* Line 1267 of yacc.c.  */
#line 2837 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
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
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 378 "parse.y"


