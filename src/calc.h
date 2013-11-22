/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998 the Free Software Foundation.
 *
 * Author: George Lebl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

#ifndef _CALC_H_
#define _CALC_H_

#include <stdio.h>

#include "structs.h"

typedef enum {	NO_ERROR=0,
		PARSE_ERROR,
		INTERNAL_MPW_ERROR,
		NUMERICAL_MPW_ERROR,
		EOF_ERROR,
		EOE_ERROR, /*end of expression*/
		IGNORE_ERROR /*set this if you want to ignore the result*/
	} calc_error_t;

typedef struct _calcstate_t {
	/*about incoming stuff*/
	int float_prec;        /*precision of floats to use*/
	/*about outgoing stuff*/
	int max_digits; /*max digits in the display 0= as many as I got*/
	int make_floats_ints;  /*make ints from floats during calculations*/
	int results_as_floats;  /*give result as float*/
	int scientific_notation; /*always scientific notation*/
} calcstate_t;


/*so we can use and set the yyparse function for parse errors*/
int yyparse(void);

/*make a string representation of an expression*/
void print_etree(GString *gs, FILE *out, ETree *n);

/*add the right parenthesis and brackets to the end of the expression*/
char * addparenth(char *s);

/*this is the function to be mostly called outsied of calc.c
  evaluate the xpression string and give back a string with the
  result, expression is in str or if str is NULL then in infd*/
char * evalexp(char * str, FILE *infile, FILE *outfile, char *prefix,
	       calcstate_t state,void (*errorfunc)(char *));

#endif
