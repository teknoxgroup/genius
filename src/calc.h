/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998, 1999 the Free Software Foundation.
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
#include <glib.h>

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
	int results_as_floats;  /*give result as float*/
	int scientific_notation; /*always scientific notation*/
	int full_expressions; /*print out expressions longer then a line*/
	int max_errors;	/*maximum error lines*/
} calcstate_t;

/*so we can use and set the yyparse function for parse errors*/
int yyparse(void);

/*make a string representation of an expression*/
/*if full_exp == -1 then the last maxline is used,
  if full_exp == 0 then maxline is reset
  if full_exp == 1 then maxline will be 0*/
void print_etree(GString *gs, FILE *out, ETree *n, int full_exp);
/*make a string representation of an expression, with the first node prettied
  (currently only for matrix)*/
void pretty_print_etree(GString *gs, FILE *out, ETree *n, int full_exp);

/*add the right parenthesis and brackets to the end of the expression*/
char * addparenth(char *s);

/*this is the function to be mostly called outsied of calc.c
  evaluate the xpression string and give back a string with the
  result, expression is in str or if str is NULL then in infd,
  pretty will use pretty_print_etree*/
void evalexp(char * str, FILE *infile, FILE *outfile, char **outstring, char *prefix,int pretty);
/*this is the normal evaluation for the frontends if they already parsed,
  it free's the parsed tree after use*/
void evalexp_parsed(ETree *parsed, FILE *outfile, char **outstring, char *prefix,int pretty);

/*these are parts of the above*/
/*note that parseexp will actually load AND execute files if there are load
  toplevel instructions, as those don't translate into an ETree*/
ETree * parseexp(char *str, FILE *infile, int load_files, int testparse, int *finished);
ETree * runexp(ETree *exp);

void compile_all_user_funcs(FILE *outfile);
void load_compiled_file(char *file, int warn);
void load_file(char *file, int warn);
void load_guess_file(char *file, int warn);
void set_new_calcstate(calcstate_t state);
void set_new_errorout(void (*func)(char *));
void set_new_infoout(void (*func)(char *));

/*This is for file/line info for errors*/
void push_file_info(char *file,int line);
void pop_file_info(void);
void incr_file_info(void);
void rewind_file_info(void);
void get_file_info(char **file, int *line);

extern FILE *outputfp;
extern void (*evalnode_hook)(void);
extern int run_hook_every;
extern void (*statechange_hook)(calcstate_t);

void add_description(char *func, char *desc);
char *get_description(char *func);

#endif
