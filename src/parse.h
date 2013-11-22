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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 54 "parse.y"
typedef union YYSTYPE {
	mpw_t val;
	char *id;
} YYSTYPE;
/* Line 1275 of yacc.c.  */
#line 174 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



