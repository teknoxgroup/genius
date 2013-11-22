/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
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



/* Line 268 of yacc.c  */
#line 98 "y.tab.c"

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
     SWAPWITH = 299,
     TRANSPOSE = 300,
     ELTELTDIV = 301,
     ELTELTMUL = 302,
     ELTELTPLUS = 303,
     ELTELTMINUS = 304,
     ELTELTEXP = 305,
     ELTELTMOD = 306,
     DOUBLEFACT = 307,
     EQ_CMP = 308,
     NE_CMP = 309,
     CMP_CMP = 310,
     LT_CMP = 311,
     GT_CMP = 312,
     LE_CMP = 313,
     GE_CMP = 314,
     LOGICAL_XOR = 315,
     LOGICAL_OR = 316,
     LOGICAL_AND = 317,
     LOGICAL_NOT = 318,
     INCREMENT = 319,
     LOWER_THAN_ELSE = 320,
     LOWER_THAN_INCREMENT = 321,
     MOD = 322,
     ELTELTBACKDIV = 323,
     UPLUS = 324,
     UMINUS = 325
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
#define SWAPWITH 299
#define TRANSPOSE 300
#define ELTELTDIV 301
#define ELTELTMUL 302
#define ELTELTPLUS 303
#define ELTELTMINUS 304
#define ELTELTEXP 305
#define ELTELTMOD 306
#define DOUBLEFACT 307
#define EQ_CMP 308
#define NE_CMP 309
#define CMP_CMP 310
#define LT_CMP 311
#define GT_CMP 312
#define LE_CMP 313
#define GE_CMP 314
#define LOGICAL_XOR 315
#define LOGICAL_OR 316
#define LOGICAL_AND 317
#define LOGICAL_NOT 318
#define INCREMENT 319
#define LOWER_THAN_ELSE 320
#define LOWER_THAN_INCREMENT 321
#define MOD 322
#define ELTELTBACKDIV 323
#define UPLUS 324
#define UMINUS 325




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 47 "parse.y"

	mpw_t val;
	char *id;



/* Line 293 of yacc.c  */
#line 281 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 293 "y.tab.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2515

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  93
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNRULES -- Number of states.  */
#define YYNSTATES  310

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   325

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      81,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,     2,     2,    74,    91,    76,
      82,    83,    71,    69,    85,    70,    92,    72,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    86,    73,    87,    79,     2,    88,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    89,    84,    90,     2,     2,     2,     2,
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
      65,    66,    67,    75,    77,    78
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    31,
      35,    39,    43,    46,    51,    54,    56,    60,    65,    70,
      74,    79,    83,    87,    91,    95,    98,   103,   107,   111,
     115,   119,   123,   127,   131,   135,   139,   143,   147,   151,
     155,   159,   163,   167,   171,   175,   179,   183,   187,   191,
     195,   199,   202,   205,   208,   211,   214,   217,   220,   224,
     228,   232,   237,   242,   249,   255,   262,   268,   275,   279,
     284,   290,   294,   299,   304,   309,   314,   319,   328,   339,
     346,   355,   366,   373,   382,   393,   400,   405,   412,   414,
     417,   420,   422,   426,   431,   435,   440,   446,   451,   455,
     458,   461,   464,   467,   469,   471,   473,   475,   477,   479,
     481,   484,   486,   490,   492,   494,   500,   507,   512,   521,
     531,   539,   547,   556,   563,   567,   569,   573,   578,   580,
     584
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      94,     0,    -1,     3,    95,    81,    -1,     3,     4,    81,
      -1,     3,     5,    81,    -1,     3,     7,    81,    -1,     3,
       9,    81,    -1,     3,    10,    81,    -1,     3,    11,    81,
      -1,     3,    12,    81,    -1,     3,     8,    81,    -1,     3,
       6,    81,    -1,     3,    81,    -1,     3,    95,    40,    81,
      -1,     1,    81,    -1,     1,    -1,    95,    40,    95,    -1,
      25,    71,    40,    95,    -1,    25,   101,    40,    95,    -1,
      95,    67,    95,    -1,    82,    95,    40,    83,    -1,    82,
      95,    83,    -1,    82,    95,    39,    -1,    95,    42,    95,
      -1,    95,    43,    95,    -1,    64,    95,    -1,    64,    95,
      36,    95,    -1,    95,    44,    95,    -1,    84,    95,    84,
      -1,    95,    69,    95,    -1,    95,    48,    95,    -1,    95,
      70,    95,    -1,    95,    49,    95,    -1,    95,    71,    95,
      -1,    95,    47,    95,    -1,    95,    72,    95,    -1,    95,
      46,    95,    -1,    95,    73,    95,    -1,    95,    75,    95,
      -1,    95,    74,    95,    -1,    95,    51,    95,    -1,    95,
      55,    95,    -1,    95,    53,    95,    -1,    95,    54,    95,
      -1,    95,    56,    95,    -1,    95,    57,    95,    -1,    95,
      58,    95,    -1,    95,    59,    95,    -1,    95,    62,    95,
      -1,    95,    61,    95,    -1,    95,    60,    95,    -1,    63,
      95,    -1,    95,    80,    -1,    95,    52,    -1,    95,    76,
      -1,    95,    45,    -1,    70,    95,    -1,    69,    95,    -1,
      95,    79,    95,    -1,    95,    50,    95,    -1,    95,    68,
      95,    -1,    95,    38,    68,    83,    -1,    95,    38,    95,
      83,    -1,    95,    38,    95,    85,    95,    83,    -1,    95,
      38,    95,    85,    83,    -1,    95,    38,    95,    85,    68,
      83,    -1,    95,    38,    85,    95,    83,    -1,    95,    38,
      68,    85,    95,    83,    -1,    86,   103,    87,    -1,    88,
      86,   103,    87,    -1,    82,   102,    85,    95,    83,    -1,
      89,   102,    90,    -1,    88,    89,   102,    90,    -1,    26,
      95,    31,    95,    -1,    27,    95,    31,    95,    -1,    31,
      95,    26,    95,    -1,    31,    95,    27,    95,    -1,    28,
      97,    99,    95,    35,    95,    31,    95,    -1,    28,    97,
      99,    95,    35,    95,    36,    95,    31,    95,    -1,    28,
      97,    37,    95,    31,    95,    -1,    29,    97,    99,    95,
      35,    95,    31,    95,    -1,    29,    97,    99,    95,    35,
      95,    36,    95,    31,    95,    -1,    29,    97,    37,    95,
      31,    95,    -1,    30,    97,    99,    95,    35,    95,    31,
      95,    -1,    30,    97,    99,    95,    35,    95,    36,    95,
      31,    95,    -1,    30,    97,    37,    95,    31,    95,    -1,
      32,    95,    33,    95,    -1,    32,    95,    33,    95,    34,
      95,    -1,    97,    -1,    88,    97,    -1,    91,    97,    -1,
      96,    -1,    97,    82,    83,    -1,    97,    82,   102,    83,
      -1,    96,    82,    83,    -1,    96,    82,   102,    83,    -1,
      95,    17,    82,   102,    83,    -1,    95,    17,    82,    83,
      -1,    16,    97,   100,    -1,    19,    98,    -1,    16,   100,
      -1,    88,   100,    -1,    20,    95,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    13,    -1,    14,    -1,    92,
      -1,    71,    97,    -1,    15,    -1,    97,    99,    95,    -1,
      42,    -1,    43,    -1,    82,   101,    83,    99,    95,    -1,
      82,   101,    18,    83,    99,    95,    -1,    82,    83,    99,
      95,    -1,    82,   101,    83,    86,   101,    87,    99,    95,
      -1,    82,   101,    18,    83,    86,   101,    87,    99,    95,
      -1,    82,    83,    86,   101,    87,    99,    95,    -1,    82,
     101,    83,    86,    87,    99,    95,    -1,    82,   101,    18,
      83,    86,    87,    99,    95,    -1,    82,    83,    86,    87,
      99,    95,    -1,   101,    85,    97,    -1,    97,    -1,   102,
      85,    95,    -1,   102,    85,    41,    95,    -1,    95,    -1,
     103,    41,   102,    -1,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   121,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   137,   138,   142,   146,
     147,   149,   150,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   177,   178,   179,   180,   181,   182,   184,   185,
     186,   187,   192,   193,   194,   195,   196,   197,   198,   199,
     201,   209,   211,   212,   213,   214,   215,   216,   217,   218,
     225,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   249,
     250,   251,   252,   254,   255,   257,   258,   259,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   272,   273,
     276,   279,   282,   292,   293,   296,   304,   312,   320,   328,
     336,   345,   353,   361,   371,   372,   375,   378,   379,   382,
     383
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
  "EQUALS", "DEFEQUALS", "SWAPWITH", "TRANSPOSE", "ELTELTDIV", "ELTELTMUL",
  "ELTELTPLUS", "ELTELTMINUS", "ELTELTEXP", "ELTELTMOD", "DOUBLEFACT",
  "EQ_CMP", "NE_CMP", "CMP_CMP", "LT_CMP", "GT_CMP", "LE_CMP", "GE_CMP",
  "LOGICAL_XOR", "LOGICAL_OR", "LOGICAL_AND", "LOGICAL_NOT", "INCREMENT",
  "LOWER_THAN_ELSE", "LOWER_THAN_INCREMENT", "MOD", "':'", "'+'", "'-'",
  "'*'", "'/'", "'\\\\'", "'%'", "ELTELTBACKDIV", "'\\''", "UPLUS",
  "UMINUS", "'^'", "'!'", "'\\n'", "'('", "')'", "'|'", "','", "'['",
  "']'", "'`'", "'{'", "'}'", "'&'", "'.'", "$accept", "fullexpr", "expr",
  "deref", "ident", "paramdef", "anyequals", "funcdef", "identlist",
  "exprlist", "matrixrows", 0
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
     315,   316,   317,   318,   319,   320,   321,   322,    58,    43,
      45,    42,    47,    92,    37,   323,    39,   324,   325,    94,
      33,    10,    40,    41,   124,    44,    91,    93,    96,   123,
     125,    38,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    93,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    97,    98,    99,    99,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   103,
     103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     4,     2,     1,     3,     4,     4,     3,
       4,     3,     3,     3,     3,     2,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     4,     6,     5,     6,     5,     6,     3,     4,
       5,     3,     4,     4,     4,     4,     4,     8,    10,     6,
       8,    10,     6,     8,    10,     6,     4,     6,     1,     2,
       2,     1,     3,     4,     3,     4,     5,     4,     3,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     1,     1,     5,     6,     4,     8,     9,
       7,     7,     8,     6,     3,     1,     3,     4,     1,     3,
       1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    15,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   108,   111,     0,     0,     0,
     103,   104,   105,   106,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    12,     0,     0,
       0,     0,     0,     0,   109,     0,    91,    88,     1,     3,
       4,    11,     5,    10,     6,     7,     8,     9,     0,     0,
     100,     0,    99,   102,     0,   125,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    25,    57,    56,   110,   128,
       0,     0,   128,   130,     0,     0,     0,    89,   101,     0,
      90,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,    52,     2,     0,     0,
       0,     0,    98,   113,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,    21,     0,    28,     0,     0,    68,
       0,     0,    71,     0,     0,     0,     0,    13,    16,    23,
      24,    27,    36,    34,    30,    32,    59,    40,    42,    43,
      41,    44,    45,    46,    47,    50,    49,    48,    19,    60,
      29,    31,    33,    35,    37,    39,    38,    58,    94,     0,
      92,     0,     0,     0,     0,     0,   112,    17,    18,   124,
      73,    74,     0,     0,     0,     0,     0,     0,    75,    76,
      86,    26,    20,     0,   126,   126,   129,    69,    72,    97,
       0,    61,     0,     0,    62,     0,    95,    93,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   127,    70,    96,     0,    66,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   115,    79,     0,    82,     0,
      85,     0,    87,    67,    65,    63,   123,     0,     0,     0,
     116,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,     0,   121,     0,    77,     0,    80,     0,    83,     0,
     122,     0,   118,     0,     0,     0,   119,    78,    81,    84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    82,    46,    47,    62,   135,    60,    66,    83,
      84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -72
static const yytype_int16 yypact[] =
{
      65,   -69,   300,    40,   -72,   -31,   -18,     4,     8,    21,
      44,    50,    51,    76,   -72,   -72,   -72,    -8,    42,   820,
     -72,   -72,   -72,   -72,    -4,   820,   820,    42,    42,    42,
     820,   820,   820,   820,   820,   820,    42,   -72,   820,   820,
     820,   -10,   820,    42,   -72,  1198,    -9,     2,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,     0,    79,
     -72,    35,   -72,  2135,   131,   -72,   -24,  1442,  1488,     5,
      12,    85,  1258,  1536,  2224,  1996,   127,   127,   -72,   968,
      82,   921,  2042,    87,   -28,   820,   820,   -72,   -72,   -71,
     -72,   100,   330,   440,   820,   820,   820,   -72,   820,   820,
     820,   820,   820,   820,   -72,   820,   820,   820,   820,   820,
     820,   820,   820,   820,   820,   820,   820,   820,   820,   820,
     820,   820,   820,   820,   -72,   820,   -72,   -72,   520,   550,
     -21,   -14,   -72,   -72,   -72,   820,   820,   820,   820,    42,
     820,   820,   820,   820,   820,   820,   820,   820,   820,   820,
     820,   820,   -72,   630,   -72,   710,   -72,   710,   820,   -72,
     -23,   -65,   -72,   740,     3,   820,   875,   -72,  2135,  2267,
    2267,  2310,   124,   124,   631,   631,   127,   124,  2396,  2396,
    2353,  2396,  2396,  2396,  2396,  2181,  2181,  2224,  2267,  2435,
     631,   631,   124,   124,   124,   124,   124,   127,   -72,    41,
     -72,    69,    -7,   820,    92,   -16,  2267,  2135,  2135,   -72,
    2135,  2135,  1582,  1628,  1674,  1720,  1766,  1812,  2135,  2135,
    2089,  2135,   -72,   820,  1014,  2042,    87,   -72,   -72,   -72,
      75,   -72,   820,  1060,   -72,   410,   -72,   -72,    35,    81,
    2135,     9,    -6,   820,   820,   820,   820,   820,   820,   820,
     820,  2042,   -72,   -72,  1106,   -72,   101,   -72,  1152,   820,
      35,    -5,   820,    35,    93,  2135,  2135,  1304,  2135,  1350,
    2135,  1396,  2135,   -72,   -72,   -72,  2135,   820,    35,    96,
    2135,   820,    35,   820,   820,   820,   820,   820,   820,  2135,
     820,    35,  2135,   820,  2135,  1858,  2135,  1904,  2135,  1950,
    2135,   820,  2135,   820,   820,   820,  2135,  2135,  2135,  2135
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,    -2,   -72,    17,   -72,   -68,    -3,   -52,     1,
     102
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      45,   143,   145,   147,   204,    16,   131,    16,    16,    16,
      16,    16,     4,   158,   157,    16,   138,    63,   158,   162,
     157,   133,   134,    67,    68,   228,   133,   134,    72,    73,
      74,    75,    76,    77,    59,    61,    79,    81,    88,    80,
      48,    65,   142,    89,    69,    70,    71,   133,   134,   144,
      49,   133,   134,    78,   133,   134,   132,    16,    87,   159,
      90,   139,   203,    50,   227,   202,     1,    64,     2,   205,
     242,   139,    58,   128,    58,    65,    85,   133,   134,    86,
     238,   263,   278,   130,   129,    51,   231,   161,   232,    52,
     166,   168,   169,   170,   171,   261,   172,   173,   174,   175,
     176,   177,    53,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   146,   197,   236,    54,   157,   133,   134,   199,
     201,    55,    56,   206,   168,   207,   208,   243,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     239,   168,   237,   224,   157,   225,   209,    57,   253,   226,
     157,    58,    92,   233,   230,    92,   139,   155,   260,    97,
     259,   137,   157,   262,   102,   241,   104,   102,   139,   104,
     282,   139,   163,   291,   274,     0,     0,   160,     0,     0,
     264,     0,   277,     0,     0,   281,     0,     0,     0,     0,
     124,   240,     0,   125,   126,     0,   125,   126,     0,   279,
     290,     0,     0,     0,   293,     0,     0,     0,     0,    65,
       0,   251,     0,   301,     0,     0,     0,     0,     0,     0,
     254,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,   265,   266,   267,   268,   269,   270,   271,   272,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,    65,
     280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,     0,    65,   292,
       0,   294,   295,   296,   297,   298,   299,     0,   300,     0,
       0,   302,     0,     0,     0,     0,     0,     0,     0,   306,
       0,   307,   308,   309,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,     0,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    32,    33,     0,     0,     0,   164,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,    39,   165,    40,     0,    41,    42,
       0,    43,    44,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,   256,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,   257,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,    38,     0,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,   198,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,   200,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,    97,    98,    99,     0,
       0,   102,   103,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,     0,     0,     0,     0,    34,
      35,    36,   119,   120,   121,   122,   123,   124,     0,     0,
     125,   126,    38,   222,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,   223,     0,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    32,    33,     0,     0,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,   229,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    14,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,     0,     0,     0,     0,    34,
      35,    36,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,    39,     0,    40,     0,    41,    42,
       0,    43,    44,    92,     0,   136,     0,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    91,     0,
       0,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,     0,     0,   125,   126,     0,     0,   234,    92,
     235,   136,     0,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,    91,     0,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,     0,     0,
     125,   126,     0,     0,     0,   156,    92,   152,   153,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    91,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,     0,   125,   126,     0,
       0,   154,    92,     0,   136,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    91,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,     0,   125,   126,     0,     0,   252,    92,     0,
     136,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    91,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,     0,     0,   125,
     126,     0,     0,   255,    92,     0,   136,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    91,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,     0,   125,   126,     0,     0,   273,
      92,     0,   136,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    91,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,     0,
       0,   125,   126,     0,     0,   275,    92,     0,    93,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    91,     0,   125,   126,   127,
       0,     0,     0,     0,   148,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,   136,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    91,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   283,     0,   125,   126,     0,
     284,     0,    92,     0,   136,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    91,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   285,     0,   125,   126,     0,   286,     0,    92,     0,
     136,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    91,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   287,     0,   125,
     126,     0,   288,     0,    92,     0,   136,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    91,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   140,     0,   125,   126,     0,     0,     0,
      92,     0,   136,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    91,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   141,
       0,   125,   126,     0,     0,     0,    92,     0,   136,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,     0,     0,    91,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,     0,   125,   126,   150,
       0,     0,     0,     0,    92,     0,   136,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    91,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   244,     0,   125,   126,     0,     0,     0,
      92,     0,   136,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    91,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,     0,
       0,   125,   126,   245,     0,     0,    92,     0,   136,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    91,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   246,     0,   125,   126,     0,
       0,     0,    92,     0,   136,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    91,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,     0,   125,   126,   247,     0,     0,    92,     0,
     136,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    91,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   248,     0,   125,
     126,     0,     0,     0,    92,     0,   136,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    91,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,     0,   125,   126,   249,     0,     0,
      92,     0,   136,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    91,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   303,
       0,   125,   126,     0,     0,     0,    92,     0,   136,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    91,     0,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   304,     0,   125,   126,     0,
       0,     0,    92,     0,   136,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    91,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   305,     0,   125,   126,     0,     0,     0,    92,     0,
     136,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    91,     0,     0,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,     0,     0,   125,
     126,     0,   151,     0,    92,     0,     0,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    91,
       0,     0,     0,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,     0,   125,   126,     0,     0,     0,
      92,     0,   136,     0,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,    91,     0,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,     0,
       0,   125,   126,   250,     0,     0,     0,    92,     0,     0,
       0,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    91,     0,     0,     0,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,     0,     0,   125,   126,
       0,     0,     0,    92,     0,     0,     0,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    91,     0,
       0,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,     0,     0,   125,   126,     0,     0,     0,    92,
       0,     0,     0,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    91,     0,   114,     0,     0,     0,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,     0,     0,
     125,   126,    92,     0,     0,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    91,     0,     0,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     0,     0,   125,   126,    92,     0,     0,     0,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    91,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,   122,   123,   124,     0,     0,   125,   126,    92,     0,
       0,     0,     0,     0,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      91,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,   122,   123,   124,     0,     0,   125,
     126,    92,     0,     0,     0,     0,     0,     0,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,    91,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,   122,   123,   124,
       0,     0,   125,   126,    92,     0,     0,     0,     0,     0,
       0,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,   108,   109,   110,   111,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    92,     0,   125,   126,     0,     0,     0,
      97,    98,    99,   100,   101,   102,   103,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,   122,
     123,   124,     0,     0,   125,   126
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-72))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-1))

static const yytype_int16 yycheck[] =
{
       2,    69,    70,    71,    18,    15,    58,    15,    15,    15,
      15,    15,    81,    41,    85,    15,    40,    19,    41,    90,
      85,    42,    43,    25,    26,    90,    42,    43,    30,    31,
      32,    33,    34,    35,    17,    18,    38,    39,    41,    38,
       0,    24,    37,    42,    27,    28,    29,    42,    43,    37,
      81,    42,    43,    36,    42,    43,    59,    15,    41,    87,
      43,    85,   130,    81,    87,    86,     1,    71,     3,    83,
      86,    85,    82,    82,    82,    58,    86,    42,    43,    89,
      87,    87,    87,    83,    82,    81,    83,    86,    85,    81,
      92,    93,    94,    95,    96,    86,    98,    99,   100,   101,
     102,   103,    81,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    37,   125,    83,    81,    85,    42,    43,   128,
     129,    81,    81,   135,   136,   137,   138,   205,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     202,   153,    83,   155,    85,   157,   139,    81,    83,   158,
      85,    82,    38,   165,   163,    38,    85,    85,    87,    45,
     238,    40,    85,   241,    50,    83,    52,    50,    85,    52,
      87,    85,    82,    87,    83,    -1,    -1,    85,    -1,    -1,
     242,    -1,   260,    -1,    -1,   263,    -1,    -1,    -1,    -1,
      76,   203,    -1,    79,    80,    -1,    79,    80,    -1,   261,
     278,    -1,    -1,    -1,   282,    -1,    -1,    -1,    -1,   202,
      -1,   223,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,
     232,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   259,    -1,   242,
     262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   277,    -1,    -1,   261,   281,
      -1,   283,   284,   285,   286,   287,   288,    -1,   290,    -1,
      -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   301,
      -1,   303,   304,   305,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    63,    64,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    84,    85,    86,    -1,    88,    89,
      -1,    91,    92,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    63,    64,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    71,    72,    73,    74,    75,    76,    -1,    -1,
      79,    80,    82,    83,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    71,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    84,    -1,    86,    -1,    88,    89,
      -1,    91,    92,    38,    -1,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    17,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    -1,    79,    80,    -1,    -1,    83,    38,
      85,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    17,    -1,    -1,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    -1,
      79,    80,    -1,    -1,    -1,    84,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    17,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    -1,    79,    80,    -1,
      -1,    83,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    17,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    -1,    79,    80,    -1,    -1,    83,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    17,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    -1,    79,
      80,    -1,    -1,    83,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    17,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,    83,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    17,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      -1,    79,    80,    -1,    -1,    83,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    17,    -1,    79,    80,    81,
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    17,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    31,    -1,    79,    80,    -1,
      36,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    17,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    31,    -1,    79,    80,    -1,    36,    -1,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    17,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    31,    -1,    79,
      80,    -1,    36,    -1,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    17,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    31,    -1,    79,    80,    -1,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    17,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    31,
      -1,    79,    80,    -1,    -1,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    17,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    -1,    79,    80,    33,
      -1,    -1,    -1,    -1,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    17,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    31,    -1,    79,    80,    -1,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    17,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      -1,    79,    80,    35,    -1,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    17,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    31,    -1,    79,    80,    -1,
      -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    17,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    -1,    79,    80,    35,    -1,    -1,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    17,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    31,    -1,    79,
      80,    -1,    -1,    -1,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    17,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    35,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    17,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    31,
      -1,    79,    80,    -1,    -1,    -1,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    17,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    31,    -1,    79,    80,    -1,
      -1,    -1,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    17,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    31,    -1,    79,    80,    -1,    -1,    -1,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    17,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    -1,    79,
      80,    -1,    36,    -1,    38,    -1,    -1,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    17,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,    -1,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    17,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    -1,
      -1,    79,    80,    34,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    17,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    17,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    -1,    79,    80,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    17,    -1,    62,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    -1,    -1,
      79,    80,    38,    -1,    -1,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    -1,    -1,    79,    80,    38,    -1,    -1,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    79,    80,    38,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    -1,    -1,    79,
      80,    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    -1,    79,    80,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    38,    -1,    79,    80,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    -1,    79,    80
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    94,    81,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    63,    64,    69,    70,    71,    81,    82,    84,
      86,    88,    89,    91,    92,    95,    96,    97,     0,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    97,
     100,    97,    98,    95,    71,    97,   101,    95,    95,    97,
      97,    97,    95,    95,    95,    95,    95,    95,    97,    95,
     102,    95,    95,   102,   103,    86,    89,    97,   100,   102,
      97,    17,    38,    40,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    79,    80,    81,    82,    82,
      83,   101,   100,    42,    43,    99,    40,    40,    40,    85,
      31,    31,    37,    99,    37,    99,    37,    99,    26,    27,
      33,    36,    39,    40,    83,    85,    84,    85,    41,    87,
     103,   102,    90,    82,    68,    85,    95,    81,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    83,   102,
      83,   102,    86,    99,    18,    83,    95,    95,    95,    97,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    83,    41,    95,    95,   102,    87,    90,    83,
     102,    83,    85,    95,    83,    85,    83,    83,    87,   101,
      95,    83,    86,    99,    31,    35,    31,    35,    31,    35,
      34,    95,    83,    83,    95,    83,    68,    83,    95,    99,
      87,    86,    99,    87,   101,    95,    95,    95,    95,    95,
      95,    95,    95,    83,    83,    83,    95,    99,    87,   101,
      95,    99,    87,    31,    36,    31,    36,    31,    36,    95,
      99,    87,    95,    99,    95,    95,    95,    95,    95,    95,
      95,    99,    95,    31,    31,    31,    95,    95,    95,    95
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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

/* Line 1806 of yacc.c  */
#line 121 "parse.y"
    { YYACCEPT; }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 122 "parse.y"
    { gel_command = GEL_LOADFILE; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 123 "parse.y"
    { gel_command = GEL_LOADFILE_GLOB; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 124 "parse.y"
    { gel_command = GEL_CHANGEDIR; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 125 "parse.y"
    { gel_command = GEL_LS; YYACCEPT; }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 126 "parse.y"
    { gel_command = GEL_LS_ARG; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 127 "parse.y"
    { gel_command = GEL_HELP; YYACCEPT; }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 128 "parse.y"
    { gel_command = GEL_HELP_ARG; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 129 "parse.y"
    { gel_command = GEL_PWD; YYACCEPT; }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 130 "parse.y"
    { gel_command = GEL_LOADPLUGIN; gel_command_arg = (yyvsp[(2) - (3)].id); YYACCEPT; }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 131 "parse.y"
    { YYACCEPT; }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 132 "parse.y"
    { gp_push_null(); PUSH_ACT(GEL_E_SEPAR); YYACCEPT; }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 133 "parse.y"
    { gel_return_ret = TRUE; yyclearin; YYABORT; }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 134 "parse.y"
    { gel_return_ret = TRUE; }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 137 "parse.y"
    { PUSH_ACT(GEL_E_SEPAR); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 138 "parse.y"
    { if ( ! gp_push_local_all ()) {
						SYNTAX_ERROR;
					  }
       					}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 142 "parse.y"
    { if ( ! gp_push_local_idents ()) {
						SYNTAX_ERROR;
					  }
					}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 146 "parse.y"
    { PUSH_ACT(GEL_E_MOD_CALC); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 147 "parse.y"
    { gp_push_null(); PUSH_ACT(GEL_E_SEPAR);
					  gp_push_spacer(); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 149 "parse.y"
    { gp_push_spacer(); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 150 "parse.y"
    { mpw_t i;
					  mpw_init (i);
					  mpw_i (i);
					  gp_push_spacer();
					  gel_stack_push(&gel_parsestack,
							 gel_makenum_use(i));
					  PUSH_ACT(GEL_E_MUL); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 157 "parse.y"
    { PUSH_ACT(GEL_E_EQUALS); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 158 "parse.y"
    { PUSH_ACT(GEL_E_DEFEQUALS); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 159 "parse.y"
    { PUSH_ACT(GEL_E_INCREMENT); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 160 "parse.y"
    { PUSH_ACT(GEL_E_INCREMENT_BY); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 161 "parse.y"
    { PUSH_ACT(GEL_E_SWAPWITH); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 162 "parse.y"
    { PUSH_ACT(GEL_E_ABS); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 163 "parse.y"
    { PUSH_ACT(GEL_E_PLUS); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 164 "parse.y"
    { PUSH_ACT(GEL_E_ELTPLUS); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 165 "parse.y"
    { PUSH_ACT(GEL_E_MINUS); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 166 "parse.y"
    { PUSH_ACT(GEL_E_ELTMINUS); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 167 "parse.y"
    { PUSH_ACT(GEL_E_MUL); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 168 "parse.y"
    { PUSH_ACT(GEL_E_ELTMUL); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 169 "parse.y"
    { PUSH_ACT(GEL_E_DIV); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 170 "parse.y"
    { PUSH_ACT(GEL_E_ELTDIV); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 171 "parse.y"
    { PUSH_ACT(GEL_E_BACK_DIV); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 172 "parse.y"
    { PUSH_ACT(GEL_E_ELT_BACK_DIV); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 173 "parse.y"
    { PUSH_ACT(GEL_E_MOD); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 174 "parse.y"
    { PUSH_ACT(GEL_E_ELTMOD); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 175 "parse.y"
    { PUSH_ACT(GEL_E_CMP_CMP); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 177 "parse.y"
    { PUSH_ACT(GEL_E_EQ_CMP); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 178 "parse.y"
    { PUSH_ACT(GEL_E_NE_CMP); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 179 "parse.y"
    { PUSH_ACT(GEL_E_LT_CMP); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 180 "parse.y"
    { PUSH_ACT(GEL_E_GT_CMP); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 181 "parse.y"
    { PUSH_ACT(GEL_E_LE_CMP); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 182 "parse.y"
    { PUSH_ACT(GEL_E_GE_CMP); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 184 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_AND); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 185 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_OR); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 186 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_XOR); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 187 "parse.y"
    { PUSH_ACT(GEL_E_LOGICAL_NOT); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 192 "parse.y"
    { PUSH_ACT(GEL_E_FACT); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 193 "parse.y"
    { PUSH_ACT(GEL_E_DBLFACT); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 194 "parse.y"
    { PUSH_ACT(GEL_E_CONJUGATE_TRANSPOSE); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 195 "parse.y"
    { PUSH_ACT(GEL_E_TRANSPOSE); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 196 "parse.y"
    { PUSH_ACT(GEL_E_NEG); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 198 "parse.y"
    { PUSH_ACT(GEL_E_EXP); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 199 "parse.y"
    { PUSH_ACT(GEL_E_ELTEXP); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 201 "parse.y"
    {
				if (gp_prepare_push_region_sep ()) {
					PUSH_ACT(GEL_E_REGION_SEP_BY);
				} else {
					PUSH_ACT(GEL_E_REGION_SEP);
				}
					}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 209 "parse.y"
    { /* FIXME: do nothing?, this is just a 
					     get all */ }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 211 "parse.y"
    { PUSH_ACT(GEL_E_GET_VELEMENT); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 212 "parse.y"
    { PUSH_ACT(GEL_E_GET_ELEMENT); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 213 "parse.y"
    { PUSH_ACT(GEL_E_GET_ROW_REGION); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 214 "parse.y"
    { PUSH_ACT(GEL_E_GET_ROW_REGION); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 215 "parse.y"
    { PUSH_ACT(GEL_E_GET_COL_REGION); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 216 "parse.y"
    { PUSH_ACT(GEL_E_GET_COL_REGION); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 217 "parse.y"
    { if(!gp_push_matrix(FALSE)) {SYNTAX_ERROR;} }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 218 "parse.y"
    { if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;} }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 225 "parse.y"
    {
			if(!gp_push_matrix_row()) {SYNTAX_ERROR;}
			if(!gp_push_marker(GEL_MATRIX_START_NODE)) {SYNTAX_ERROR;}
			if(!gp_push_matrix(TRUE)) {SYNTAX_ERROR;}
					}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 230 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 231 "parse.y"
    {SYNTAX_ERROR;}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 232 "parse.y"
    { PUSH_ACT(GEL_E_WHILE_CONS); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 233 "parse.y"
    { PUSH_ACT(GEL_E_UNTIL_CONS); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 234 "parse.y"
    { PUSH_ACT(GEL_E_DOWHILE_CONS); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 235 "parse.y"
    { PUSH_ACT(GEL_E_DOUNTIL_CONS); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 236 "parse.y"
    { PUSH_ACT(GEL_E_FOR_CONS); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 237 "parse.y"
    { PUSH_ACT(GEL_E_FORBY_CONS); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 238 "parse.y"
    { PUSH_ACT(GEL_E_FORIN_CONS); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 239 "parse.y"
    { PUSH_ACT(GEL_E_SUM_CONS); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 240 "parse.y"
    { PUSH_ACT(GEL_E_SUMBY_CONS); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 241 "parse.y"
    { PUSH_ACT(GEL_E_SUMIN_CONS); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 242 "parse.y"
    { PUSH_ACT(GEL_E_PROD_CONS); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 243 "parse.y"
    { PUSH_ACT(GEL_E_PRODBY_CONS); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 244 "parse.y"
    { PUSH_ACT(GEL_E_PRODIN_CONS); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 245 "parse.y"
    { PUSH_ACT(GEL_E_IF_CONS); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 246 "parse.y"
    { PUSH_ACT(GEL_E_IFELSE_CONS); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 247 "parse.y"
    { gp_convert_identifier_to_bool ();
					  /* convert true/false to bool */}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 249 "parse.y"
    { PUSH_ACT(GEL_E_QUOTE); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 250 "parse.y"
    { PUSH_ACT(GEL_E_REFERENCE); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 252 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 254 "parse.y"
    { PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 255 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 257 "parse.y"
    { PUSH_ACT(GEL_E_DIRECTCALL); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 258 "parse.y"
    { PUSH_ACT(GEL_E_CALL); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 259 "parse.y"
    { gp_push_marker_simple(GEL_EXPRLIST_START_NODE);
					  PUSH_ACT(GEL_E_CALL); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 261 "parse.y"
    { PUSH_ACT(GEL_E_DEFEQUALS); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 265 "parse.y"
    { PUSH_ACT(GEL_E_RETURN); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 266 "parse.y"
    { PUSH_ACT(GEL_E_BAILOUT); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 267 "parse.y"
    { PUSH_ACT(GEL_E_EXCEPTION); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 268 "parse.y"
    { PUSH_ACT(GEL_E_CONTINUE); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 269 "parse.y"
    { PUSH_ACT(GEL_E_BREAK); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 270 "parse.y"
    { gel_stack_push(&gel_parsestack,
							 gel_makenum_use((yyvsp[(1) - (1)].val))); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 272 "parse.y"
    { PUSH_CONST_STRING((yyvsp[(1) - (1)].id)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 273 "parse.y"
    { gp_push_null(); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 276 "parse.y"
    { PUSH_ACT(GEL_E_DEREFERENCE); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 279 "parse.y"
    { PUSH_IDENTIFIER((yyvsp[(1) - (1)].id)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 282 "parse.y"
    {
			gp_prepare_push_param (FALSE);
			PUSH_ACT (GEL_E_PARAMETER);
		}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 296 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 304 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 312 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     FALSE /* extradict */,
					     FALSE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 320 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 328 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 336 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     TRUE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 345 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
						}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 353 "parse.y"
    {
			if ( ! gp_push_func (TRUE /* vararg */,
					     TRUE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
							}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 361 "parse.y"
    {
			if ( ! gp_push_func (FALSE /* vararg */,
					     FALSE /* arguments */,
					     FALSE /* extradict */,
					     TRUE /* never_subst */)) {
				SYNTAX_ERROR;
			}
					}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 372 "parse.y"
    { if(!gp_push_marker(GEL_EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 379 "parse.y"
    { if(!gp_push_marker(GEL_EXPRLIST_START_NODE)) {SYNTAX_ERROR;} }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 382 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 383 "parse.y"
    { if(!gp_push_matrix_row()) {SYNTAX_ERROR;} if(!gp_push_marker(GEL_MATRIX_START_NODE)) {SYNTAX_ERROR;} }
    break;



/* Line 1806 of yacc.c  */
#line 3164 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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
      if (!yypact_value_is_default (yyn))
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
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



/* Line 2067 of yacc.c  */
#line 386 "parse.y"


