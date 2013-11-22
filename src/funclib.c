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

#include "config.h"

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#define _(x) x
#endif

#include <string.h>
#include <math.h>
#include <glib.h>
#include <limits.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "funclib.h"
#include "mymath.h"
#include "calc.h"
#include "matrix.h"

extern void (*errorout)(char *);
extern calc_error_t error_num;
extern int got_eof;

/*maximum number of primes to precalculate and store*/
#define MAXPRIMES 100000
GArray *primes = NULL;
int numprimes = 0;

static ETree *
warranty_op(ETree * * a, int *exception)
{
	(*errorout)(_("Genius "VERSION"\n"
		    "Copyright (c) 1997,1998,1999 Free Software Foundation, Inc.\n\n"
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
		    "    USA.\n"));
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
	
/*error printing function*/
static ETree *
error_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		(*errorout)(a[0]->data.str);
	else {
		GString *gs;
		gs = g_string_new(NULL);
		pretty_print_etree(gs,NULL,a[0]);
		(*errorout)(gs->str);
		g_string_free(gs,TRUE);
	}
	return makenum_null();
}
/*print function*/
static ETree *
print_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		puts(a[0]->data.str);
	else {
		pretty_print_etree(NULL,stdout,a[0]);
		puts("");
	}
	return makenum_null();
}
/*print function*/
static ETree *
printn_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		printf("%s",a[0]->data.str);
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
		(*errorout)(_("display: first argument must be string!"));
		return NULL;
	}
	printf("%s: ",a[0]->data.str);
	pretty_print_etree(NULL,stdout,a[1]);
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
		(*errorout)(_("sin: argument not a real number"));
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
		(*errorout)(_("cos: argument not a real number"));
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
		(*errorout)(_("tan: argument not a real number"));
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

	/*is this algorithm always precise??? sin/cos*/
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
	static mpw_t cache;
	static int iscached = FALSE;
	ETree *n;
	
	if(iscached)
		return makenum(cache);

	mpw_init(cache);
	mpw_set_ui(cache,1);
	mpw_exp(cache,cache);
	n = makenum(cache);
	iscached = TRUE;
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
is_value_op(ETree * * a, int *exception)
{
	if(a[0]->type==VALUE_NODE)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_string_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_matrix_op(ETree * * a, int *exception)
{
	if(a[0]->type==MATRIX_NODE)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_complex_op(ETree * * a, int *exception)
{
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("is_complex: argument not a number"));
		return NULL;
	}
	if(mpw_is_complex(a[0]->data.value))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}
static ETree *
is_integer_op(ETree * * a, int *exception)
{
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("is_integer: argument not a number"));
		return NULL;
	}
	if(mpw_is_integer(a[0]->data.value))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}
static ETree *
is_rational_op(ETree * * a, int *exception)
{
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("is_rational: argument not a number"));
		return NULL;
	}
	if(mpw_is_rational(a[0]->data.value))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}
static ETree *
is_float_op(ETree * * a, int *exception)
{
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("is_float: argument not a number"));
		return NULL;
	}
	if(mpw_is_float(a[0]->data.value))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}

static ETree *
trunc_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("trunc: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_trunc(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}
static ETree *
floor_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("floor: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_floor(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}
static ETree *
ceil_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("ceil: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_ceil(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}
static ETree *
round_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("round: argument not a number"));
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
		(*errorout)(_("Re: argument not a number"));
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
		(*errorout)(_("Im: argument not a number"));
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
		(*errorout)(_("sqrt: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_sqrt(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
exp_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("exp: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_exp(fr,a[0]->data.value);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
ln_op(ETree * * a, int *exception)
{
	mpw_t fr;
	ETree *n;

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("ln: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_ln(fr,a[0]->data.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(fr);
		return NULL;
	}
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static ETree *
is_null_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t pitmp;

	ETree *n;

	if(a[0]->type!=NULL_NODE)
		return makenum_ui(0);
	else
		return makenum_ui(1);
}

/*gcd function*/
static ETree *
gcd_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("gcd: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_gcd(tmp,
		a[0]->data.value,
		a[1]->data.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}

	n=makenum(tmp);
	mpw_clear(tmp);
	return n;
}

/*lcm function*/
static ETree *
lcm_op(ETree * * a, int *exception)
{
	mpw_t tmp;
	mpw_t prod;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("lcm: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_gcd(tmp,
		a[0]->data.value,
		a[1]->data.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}
	mpw_init(prod);
	mpw_mul(prod,
		a[0]->data.value,
		a[1]->data.value);
	mpw_div(tmp,prod,tmp);
	mpw_clear(prod);

	n=makenum(tmp);
	mpw_clear(tmp);
	return n;
}

/*sin function*/
static ETree *
max_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("max: arguments must be numbers"));
		return NULL;
	}

	if(mpw_cmp(a[0]->data.value,a[1]->data.value)>0)
		n = makenum(a[0]->data.value);
	else {
		if(error_num) {
			error_num = 0;
			return NULL;
		}
		n = makenum(a[1]->data.value);
	}

	return n;
}
/*sin function*/
static ETree *
min_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	ETree *n;

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("min: arguments must be numbers"));
		return NULL;
	}

	if(mpw_cmp(a[0]->data.value,a[1]->data.value)>0)
		n = makenum(a[1]->data.value);
	else {
		if(error_num) {
			error_num = 0;
			return NULL;
		}
		n = makenum(a[0]->data.value);
	}

	return n;
}

static ETree *
I_op(ETree * * a, int *exception)
{
	ETree *n;
	long size;
	int i,j;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->data.value)) {
		(*errorout)(_("I: argument not an integer"));
		return NULL;
	}

	size = mpw_get_long(a[0]->data.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(size<=0) {
		(*errorout)(_("I: argument can't be negative or 0"));
		return NULL;
	}
	if(size>INT_MAX) {
		(*errorout)(_("I: argument too large"));
		return NULL;
	}

	/*make us a new empty node*/
	n = makenum_null();
	n->type = MATRIX_NODE;
	n->data.matrix = matrix_new();
	matrix_set_size(n->data.matrix,size,size);
	
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			matrix_index(n->data.matrix,i,j) =
				makenum_ui(i==j?1:0);

	return n;
}

static int
is_prime(unsigned long testnum)
{
	int i;
	unsigned long s = (unsigned long)sqrt(testnum);
	
	for(i=0;g_array_index(primes,unsigned long,i)<=s && i<numprimes;i++) {
		if((testnum%g_array_index(primes,unsigned long,i))==0) {
			return 0;
		}
	}
	return 1;
}


static ETree *
prime_op(ETree * * a, int *exception)
{
	long num;
	unsigned long i;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->data.value)) {
		(*errorout)(_("prime: argument not an integer"));
		return NULL;
	}

	num = mpw_get_long(a[0]->data.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(num<=0) {
		(*errorout)(_("prime: argument can't be negative or 0"));
		return NULL;
	}
	if(num>MAXPRIMES) {
		(*errorout)(_("prime: argument is too large"));
		return NULL;
	}
	
	if(!primes) {
		unsigned long b;
		primes = g_array_new(FALSE,FALSE,sizeof(unsigned long));
		b = 2;
		g_array_append_val(primes,b);
		b = 3;
		g_array_append_val(primes,b);
		b = 5;
		g_array_append_val(primes,b);
		b = 7;
		g_array_append_val(primes,b);
		numprimes = 4;
	}
	
	if(num-1 < numprimes)
		return makenum_ui(g_array_index(primes,unsigned long,num-1));
	

	g_array_set_size(primes,num);
	for(i=g_array_index(primes,unsigned long,numprimes-1);
	    numprimes<=num-1 && i<=ULONG_MAX;i++) {
		if(is_prime(i))
			g_array_index(primes,unsigned long,numprimes++) = i;
	}
	
	if(numprimes<=num-1) {
		(*errorout)(_("prime: argument is too large"));
		return NULL;
	}
	return makenum_ui(g_array_index(primes,unsigned long,num-1));
}

/*add the routines to the dictionary*/
void
funclib_addall(void)
{
	d_addfunc(d_makebifunc(d_intern("warranty"),warranty_op,0));
	d_addfunc(d_makebifunc(d_intern("exit"),exit_op,0));
	d_addfunc(d_makebifunc(d_intern("quit"),exit_op,0));
	d_addfunc(d_makebifunc(d_intern("error"),error_op,1));
	d_addfunc(d_makebifunc(d_intern("print"),print_op,1));
	d_addfunc(d_makebifunc(d_intern("printn"),printn_op,1));
	d_addfunc(d_makebifunc(d_intern("display"),display_op,2));
	d_addfunc(d_makebifunc(d_intern("ni"),ni_op,0));
	d_addfunc(d_makebifunc(d_intern("shrubbery"),shrubbery_op,0));
	d_addfunc(d_makebifunc(d_intern("sin"),sin_op,1));
	d_addfunc(d_makebifunc(d_intern("cos"),cos_op,1));
	d_addfunc(d_makebifunc(d_intern("tan"),tan_op,1));
	d_addfunc(d_makebifunc(d_intern("pi"),pi_op,0));
	d_addfunc(d_makebifunc(d_intern("e"),e_op,0));
	d_addfunc(d_makebifunc(d_intern("sqrt"),sqrt_op,1));
	d_addfunc(d_makebifunc(d_intern("exp"),exp_op,1));
	d_addfunc(d_makebifunc(d_intern("ln"),ln_op,1));
	d_addfunc(d_makebifunc(d_intern("gcd"),gcd_op,2));
	d_addfunc(d_makebifunc(d_intern("lcm"),lcm_op,2));
	d_addfunc(d_makebifunc(d_intern("max"),max_op,2));
	d_addfunc(d_makebifunc(d_intern("min"),min_op,2));
	d_addfunc(d_makebifunc(d_intern("prime"),prime_op,1));
	d_addfunc(d_makebifunc(d_intern("round"),round_op,1));
	d_addfunc(d_makebifunc(d_intern("floor"),floor_op,1));
	d_addfunc(d_makebifunc(d_intern("ceil"),ceil_op,1));
	d_addfunc(d_makebifunc(d_intern("trunc"),trunc_op,1));
	d_addfunc(d_makebifunc(d_intern("Re"),Re_op,1));
	d_addfunc(d_makebifunc(d_intern("Im"),Im_op,1));
	d_addfunc(d_makebifunc(d_intern("I"),I_op,1));
	d_addfunc(d_makebifunc(d_intern("is_value"),is_value_op,1));
	d_addfunc(d_makebifunc(d_intern("is_string"),is_string_op,1));
	d_addfunc(d_makebifunc(d_intern("is_matrix"),is_matrix_op,1));
	d_addfunc(d_makebifunc(d_intern("is_complex"),is_complex_op,1));
	d_addfunc(d_makebifunc(d_intern("is_integer"),is_integer_op,1));
	d_addfunc(d_makebifunc(d_intern("is_rational"),is_rational_op,1));
	d_addfunc(d_makebifunc(d_intern("is_float"),is_float_op,1));
	d_addfunc(d_makebifunc(d_intern("is_null"),is_null_op,1));
}
