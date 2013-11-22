/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 47 "parse.y"
{
	mpw_t val;
	char *id;
}
/* Line 1489 of yacc.c.  */
#line 188 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

