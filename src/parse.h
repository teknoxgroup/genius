/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
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

/* Line 1685 of yacc.c  */
#line 47 "parse.y"

	mpw_t val;
	char *id;



/* Line 1685 of yacc.c  */
#line 198 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


