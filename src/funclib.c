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

#include <config.h>

#ifndef WITHOUT_GNOME
#include <gnome.h>
#else
#ifndef _
#define _(x) x
#endif
#endif

#include <string.h>
#include <glib.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "funclib.h"
#include "mymath.h"
#include "calc.h"

extern void (*errorout)(char *);
extern calc_error_t error_num;
extern int got_eof;

static ETree *
warranty_op(ETree * * a, int *exception)
{
	(*errorout)("Genius "VERSION"\n"
		    "Copyright (c) 1997,1998 Free Software Foundation, Inc.\n\n"
		    "    This program is free software; you can redistribute it and/or modify\n"
		    "    it under the terms of the GNU General Public License as published by\n"
		    "    the Free Software Foundation; either version 2 of the License , or\n"
		    "    (at your option) any later version.\n"
		    "\n"
		    "    This program is distributed in the hope that it will be useful,\n"
		    "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		    "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		    "    GNU General Public License for more details.\n"
		    "\n"
		    "    You should have received a copy of the GNU General Public License\n"
		    "    along with this program. If not, write to the Free Software\n"
		    "    Foundation,  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,\n"
		    "    USA.\n");
	error_num = IGNORE_ERROR;
	if(exception) *exception = TRUE; /*raise exception*/
	return NULL;
}

static ETree *
exit_op(ETree * * a, int *exception)
{
	got_eof = TRUE;
	if(exception) *exception = TRUE; /*raise exception*/
	return NULL;
}

static ETree *
ni_op(ETree * * a, int *exception)
{
	(*errorout)("We are the Knights Who Say... Ni!");
	if(exception) *exception = TRUE; /*raise exception*/
	error_num = IGNORE_ERROR;
	return NULL;
}

static ETree *
shrubbery_op(ETree * * a, int *exception)
{
	(*errorout)("Then, when you have found the shrubbery, you must\n"
		    "cut down the mightiest tree in the forest... with...\n"
		    "A HERRING!");
	if(exception) *exception = TRUE; /*raise exception*/
	error_num = IGNORE_ERROR;
	return NULL;
}
	
/*print function*/
static ETree *
print_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		printf("%s",a[0]->data.id);
	else
		print_etree(NULL,stdout,a[0]);
	puts("");
	return makenum_null();
}
/*print function*/
static ETree *
printn_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		printf("%s",a[0]->data.id);
	else
		print_etree(NULL,stdout,a[0]);
	fflush(stdout);
	return makenum_null();
}
/*print function*/
static ETree *
display_op(ETree * * a, int *exception)
{
	if(a[0]->type!=STRING_NODE) {
		(*errorout)("display: first argument must be string!");
		return NULL;
	}
	printf("%s: ",a[0]->data.id);
	print_etree(NULL,stdout,a[1]);
	puts("");
	return makenum_null();
}

/*sin function*/
static ETree *
sin_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t pitmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->data.value)) {
		(*errorout)("sin: argument not a real number");
		return NULL;
	}

	mpw_init(fr);
	mpw_set(fr,a[0]->data.value);

	mpw_init(pitmp);
	mympw_getpi(pitmp);
	mpw_mul_ui(pitmp,pitmp,2);

 	/*mod it down to -pi*2<fr<pi*2 */
 	while(mpw_cmp(fr,pitmp)>0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_neg(pitmp,pitmp);
 	while(mpw_cmp(fr,pitmp)<0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_clear(pitmp);

	mympw_sin(fr,fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

/*cos function*/
static ETree *
cos_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t pitmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->data.value)) {
		(*errorout)("cos: argument not a real number");
		return NULL;
	}

	mpw_init(fr);
	mpw_set(fr,a[0]->data.value);


	mpw_init(pitmp);
	mympw_getpi(pitmp);
	mpw_mul_ui(pitmp,pitmp,2);

 	/*get it down to -pi*2<fr<pi*2 */
 	while(mpw_cmp(fr,pitmp)>0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_neg(pitmp,pitmp);
 	while(mpw_cmp(fr,pitmp)<0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_clear(pitmp);


	mympw_cos(fr,fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

/*tan function*/
static ETree *
tan_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t fr2;
	mpw_t pitmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->data.value)) {
		(*errorout)("tan: argument not a real number");
		return NULL;
	}

	mpw_init(fr);
	mpw_set(fr,a[0]->data.value);


	mpw_init(pitmp);
	mympw_getpi(pitmp);
	mpw_mul_ui(pitmp,pitmp,2);

 	/*get it down to -pi*2<fr<pi*2 */
 	while(mpw_cmp(fr,pitmp)>0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_neg(pitmp,pitmp);
 	while(mpw_cmp(fr,pitmp)<0)
 		mpw_sub(fr,fr,pitmp);
 	mpw_clear(pitmp);

	/*is this algorithm allways precise??? sin/cos*/
	mpw_init(fr2);
	mympw_cos(fr2,fr);
	mympw_sin(fr,fr);
	mpw_div(fr,fr,fr2);
	mpw_clear(fr2);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

/*e function (or e variable actually)*/
static ETree *
e_op(ETree * * a, int *exception)
{
	mpw_t fr;

	ETree *n;

	mpw_init(fr);
	mympw_gete(fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

/*pi function (or pi variable or whatever)*/
static ETree *
pi_op(ETree * * a, int *exception)
{
	mpw_t fr;

	ETree *n;

	mpw_init(fr);
	mympw_getpi(fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
is_complex_op(ETree * * a, int *exception)
{
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)("is_complex: argument not a number");
		return NULL;
	}
	if(mpw_is_complex(a[0]->data.value))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}

static ETree *
round_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)("round: argument not a number");
		return NULL;
	}
	mpw_init(fr);
	mpw_round(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
Re_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)("Re: argument not a number");
		return NULL;
	}
	mpw_init(fr);
	mpw_re(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
Im_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)("Im: argument not a number");
		return NULL;
	}
	mpw_init(fr);
	mpw_im(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
sqrt_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)("sqrt: argument not a number");
		return NULL;
	}
	mpw_init(fr);
	mpw_sqrt(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
isnull_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t pitmp;

	ETree *n;

	if(a[0]->type!=NULL_NODE)
		return makenum_ui(0);
	else
		return makenum_ui(1);
}

/*add the routines to the dictionary*/
void
funclib_addall(void)
{
	d_addfunc(d_makebifunc("warranty",warranty_op,0));
	d_addfunc(d_makebifunc("exit",exit_op,0));
	d_addfunc(d_makebifunc("quit",exit_op,0));
	d_addfunc(d_makebifunc("print",print_op,1));
	d_addfunc(d_makebifunc("printn",printn_op,1));
	d_addfunc(d_makebifunc("display",display_op,2));
	d_addfunc(d_makebifunc("ni",ni_op,0));
	d_addfunc(d_makebifunc("shrubbery",shrubbery_op,0));
	d_addfunc(d_makebifunc("sin",sin_op,1));
	d_addfunc(d_makebifunc("cos",cos_op,1));
	d_addfunc(d_makebifunc("tan",tan_op,1));
	d_addfunc(d_makebifunc("pi",pi_op,0));
	d_addfunc(d_makebifunc("e",e_op,0));
	d_addfunc(d_makebifunc("is_complex",is_complex_op,1));
	d_addfunc(d_makebifunc("round",round_op,1));
	d_addfunc(d_makebifunc("Re",Re_op,1));
	d_addfunc(d_makebifunc("Im",Im_op,1));
	d_addfunc(d_makebifunc("sqrt",sqrt_op,1));
	d_addfunc(d_makebifunc("isnull",isnull_op,1));
}
