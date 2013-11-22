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

#include <string.h>
#include <glib.h>
#include <math.h>
#include "calc.h"
#include "util.h"
#include "mymath.h"

#include "mpwrap.h"

extern void (*errorout)(char *);
extern calc_error_t error_num;

static MpwRealNum *free_reals = NULL;
static int free_reals_n = 0;

static MpwRealNum *zero = NULL;
static MpwRealNum *one = NULL;

#define GET_NEW_REAL(n) {				\
	if(!free_reals) {				\
		n = g_new0(MpwRealNum,1);		\
	} else {					\
		n = free_reals;				\
		free_reals = free_reals->alloc.next;	\
		free_reals_n--;				\
	}						\
}
#define MAKE_COPY(n) {					\
	if(n->alloc.usage>1) {				\
		MpwRealNum *m;				\
		GET_NEW_REAL(m);			\
		m->alloc.usage = 1;			\
		mpwl_init_type(m,n->type);		\
		mpwl_set(m,n);				\
		n = m;					\
	}						\
}
#define MAKE_REAL(n) {					\
	if(n->type==MPW_COMPLEX) {			\
		n->type=MPW_REAL;			\
		if(n->i != zero) {			\
			n->i->alloc.usage--;		\
			if(n->i->alloc.usage==0)	\
				mpwl_free(n->i);	\
			n->i = zero;			\
			zero->alloc.usage++;		\
		}					\
	}						\
}

/*************************************************************************/
/*low level stuff prototypes                                             */
/*************************************************************************/

/*my own power function for floats, very simple :) */
static void mympf_pow_ui(mpf_t rop,mpf_t op,unsigned long int e);
static void mympf_pow_z(mpf_t rop,mpf_t op,mpz_t e);

/*my own power function for ints, very simple :) */
static void mympz_pow_z(mpz_t rop,mpz_t op,mpz_t e);

/*clear extra variables of type type, if type=op->type nothing is done*/
static void mpwl_clear_extra_type(MpwRealNum *op,int type);

/*only set the type, don't free it, and don't set the type variable
  create an extra type of the variable for temporary use*/
static void mpwl_make_extra_type(MpwRealNum *op,int type);

static void mpwl_make_type(MpwRealNum *op,int type);

/*this only adds a value of that type but does nto clear the old one!
  retuns the new extra type set*/
static int mpwl_make_same_extra_type(MpwRealNum *op1,MpwRealNum *op2);

static void mympq_set_f(mpq_t rop,mpf_t op);
/*make new type and clear the old one*/
static void mpwl_make_same_type(MpwRealNum *op1,MpwRealNum *op2);

static void mpwl_clear(MpwRealNum *op);

static void mpwl_init_type(MpwRealNum *op,int type);

static void mpwl_free(MpwRealNum *op);

static int mpwl_sgn(MpwRealNum *op);

static void mpwl_set_si(MpwRealNum *rop,signed long int i);
static void mpwl_set_ui(MpwRealNum *rop,unsigned long int i);
static void mpwl_set_d(MpwRealNum *rop,double d);

static void mpwl_move(MpwRealNum *rop,MpwRealNum *op);

static void mpwl_set(MpwRealNum *rop,MpwRealNum *op);

static int mympn_add(long *res, long op1, long op2);
static int mympn_sub(long *res, long op1, long op2);

static void mpwl_add(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);
static void mpwl_sub(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static void mpwl_mul(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);
static void mpwl_mul_ui(MpwRealNum *rop,MpwRealNum *op,unsigned long int i);

static void mpwl_div(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);
static void mpwl_div_ui(MpwRealNum *rop,MpwRealNum *op,unsigned long int i);
static void mpwl_ui_div(MpwRealNum *rop,unsigned long int i,MpwRealNum *op);

static void mpwl_mod(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static void mpwl_gcd(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static void mpwl_neg(MpwRealNum *rop,MpwRealNum *op);

static void mpwl_fac_ui(MpwRealNum *rop,unsigned int op);

static void mpwl_fac(MpwRealNum *rop,MpwRealNum *op);

static int mpwl_pow_q(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

/*power to an unsigned long and optionaly invert the answer*/
static void mpwl_pow_ui(MpwRealNum *rop,MpwRealNum *op1,unsigned int e,
	int reverse);

static void mpwl_pow_z(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static int mpwl_pow_f(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static int mpwl_pow(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2);

static int mpwl_sqrt(MpwRealNum *rop,MpwRealNum *op);

static int mpwl_cmp(MpwRealNum *op1, MpwRealNum *op2);

static int mpwl_cmp_ui(MpwRealNum *op, unsigned long int i);

static void mpwl_make_int(MpwRealNum *rop, int floats);

/*make number into a float, this might be neccessary for unprecise
  calculations*/
static void mpwl_make_float(MpwRealNum *rop);

/*round to an integer*/
static void mpwl_round(MpwRealNum *rop);
static void mpwl_ceil(MpwRealNum *rop);
static void mpwl_floor(MpwRealNum *rop);

static void mpwl_set_str_float(MpwRealNum *rop,char *s,int base);
static void mpwl_set_str_int(MpwRealNum *rop,char *s,int base);

/**************/
/*output stuff*/

/*round off the number at some digits*/
static void str_make_max_digits(char *s,int digits);

/*formats a floating point with mantissa in p and exponent in e*/
static char * str_format_float(char *p,long int e,int scientific_notation);

static char * str_getstring_z(mpz_t num, int max_digits,
	int scientific_notation);
static char * str_getstring_q(mpq_t num, int max_digits,
	int scientific_notation);
static char * str_getstring_f(mpf_t num, int max_digits,
	int scientific_notation);

static char * mpwl_getstring(MpwRealNum * num, int max_digits,
	int scientific_notation, int results_as_floats);

/*************************************************************************/
/*low level stuff                                                        */
/*************************************************************************/

/*my own power function for floats, very simple :) */
static void
mympf_pow_ui(mpf_t rop,mpf_t op,unsigned long int e)
{
	mpf_t answer;
	mpf_t base;

	if(e==0) {
		mpf_set_ui(rop,1);
		return;
	} else if(e==1) {
		mpf_set(rop,op);
		return;
	}

	mpf_init_set(base,op);
	mpf_set_ui(rop,1);
	while(e>0) {
		if(e & 0x1) { /*if odd*/
			mpf_mul(rop,rop,base);
			e--;
		} else { /* even */
			mpf_mul(base,base,base);
			e >>= 1; /*e /= 2*/
		}
	}
	mpf_clear(base);
}

/*my own power function for floats, very simple :) */
static void
mympf_pow_z(mpf_t rop,mpf_t op,mpz_t e)
{
	unsigned long limb;
	mpf_t answer;
	mpf_t tmp;
	mpz_t locale;

	mpf_init_set_ui(answer,1);
	mpf_init(tmp);
	mpz_init_set(locale,e);

	do {
		limb=mpz_get_ui(locale);
		mpz_tdiv_q_2exp(locale,locale,CHAR_BIT*sizeof(unsigned long));

		if(limb==0)
			continue;

		mympf_pow_ui(tmp,op,limb);

		mpf_mul(answer,answer,tmp);
	} while(mpz_sgn(locale)!=0);

	mpf_set(rop,answer);
	mpf_clear(answer);
	mpf_clear(tmp);
	mpz_clear(locale);
}


/*my own power function for ints, very simple :) */
static void
mympz_pow_z(mpz_t rop,mpz_t op,mpz_t e)
{
	unsigned long limb;
	mpz_t answer;
	mpz_t tmp;
	mpz_t locale;

	mpz_init_set_ui(answer,1);
	mpz_init(tmp);
	mpz_init_set(locale,e);

	do {
		limb=mpz_get_ui(locale);
		mpz_tdiv_q_2exp(locale,locale,CHAR_BIT*sizeof(unsigned long));

		if(limb==0)
			continue;

		mpz_pow_ui(tmp,op,limb);

		mpz_mul(answer,answer,tmp);
	} while(mpz_sgn(locale)!=0);

	mpz_set(rop,answer);
	mpz_clear(answer);
	mpz_clear(tmp);
	mpz_clear(locale);
}

/*clear extra variables of type type, if type=op->type nothing is done*/
static void
mpwl_clear_extra_type(MpwRealNum *op,int type)
{
	if(op->type==type)
		return;
	switch(type) {
	case MPW_INTEGER:
		if(op->data.ival)
			mpz_clear(op->data.ival);
		break;
	case MPW_RATIONAL:
		if(op->data.rval)
			mpq_clear(op->data.rval);
		break;
	case MPW_FLOAT:
		if(op->data.fval)
			mpf_clear(op->data.fval);
		break;
	}
}

static void
mympq_set_f(mpq_t rop,mpf_t op)
{
	char *s;
	long int e;

	s=mpf_get_str(NULL,&e,10,0,op);
	e-=strlen(s);
	if(e>0) {
		s=my_realloc(s,strlen(s)+1,strlen(s)+e+1);
		for(;e>0;e--)
			strcat(s,"0");
	}
	mpz_set_str(mpq_numref(rop),s,10);
	mpz_set_ui(mpq_denref(rop),10);
	mpz_pow_ui(mpq_denref(rop),mpq_denref(rop),-e);

	g_free(s);

	mpq_canonicalize(rop);

}

/*only set the type, don't free it, and don't set the type variable
  create an extra type of the variable for temporary use*/
static void
mpwl_make_extra_type(MpwRealNum *op,int type)
{
	if(op->type==type)
		return;
	switch(type) {
	case MPW_NATIVEINT:
		if(op->type==MPW_INTEGER)
			op->data.nval = mpz_get_si(op->data.ival);
		else if(op->type==MPW_RATIONAL)
			op->data.nval = mpq_get_d(op->data.rval);
		else if(op->type==MPW_FLOAT)
			op->data.nval = mpf_get_d(op->data.fval);
		break;
	case MPW_INTEGER:
		if(!op->data.ival)
			op->data.ival = g_new(__mpz_struct,1);
		mpz_init(op->data.ival);
		if(op->type==MPW_FLOAT)
			mpz_set_f(op->data.ival,op->data.fval);
		else if(op->type==MPW_RATIONAL)
			mpz_set_q(op->data.ival,op->data.rval);
		else if(op->type==MPW_NATIVEINT)
			mpz_set_si(op->data.ival,op->data.nval);
		break;
	case MPW_RATIONAL:
		if(!op->data.rval)
			op->data.rval = g_new(__mpq_struct,1);
		mpq_init(op->data.rval);
		if(op->type==MPW_INTEGER)
			mpq_set_z(op->data.rval,op->data.ival);
		else if(op->type==MPW_FLOAT)
			mympq_set_f(op->data.rval,op->data.fval);
		else if(op->type==MPW_NATIVEINT)
			mpq_set_si(op->data.rval,op->data.nval,1);
		break;
	case MPW_FLOAT:
		if(!op->data.fval)
			op->data.fval = g_new(__mpf_struct,1);
		mpf_init(op->data.fval);
		if(op->type==MPW_INTEGER)
			mpf_set_z(op->data.fval,op->data.ival);
		else if(op->type==MPW_RATIONAL)
			mpf_set_q(op->data.fval,op->data.rval);
		else if(op->type==MPW_NATIVEINT)
			mpf_set_si(op->data.fval,op->data.nval);
		break;
	}
}

static void
mpwl_make_type(MpwRealNum *op,int type)
{
	int t;

	if(op->type==type)
		return;
	t=op->type;
	mpwl_make_extra_type(op,type);
	op->type=type;
	mpwl_clear_extra_type(op,t);
}

/*this only adds a value of that type but does nto clear the old one!
  retuns the new extra type set*/
static int
mpwl_make_same_extra_type(MpwRealNum *op1,MpwRealNum *op2)
{
	if(op1->type==op2->type)
		return op1->type;
	else if(op1->type > op2->type) {
		mpwl_make_extra_type(op2,op1->type);
		return op1->type;
	} else { /*if(op1->type < op2->type)*/
		mpwl_make_extra_type(op1,op2->type);
		return op2->type;
	}
}

/*make new type and clear the old one*/
static void
mpwl_make_same_type(MpwRealNum *op1,MpwRealNum *op2)
{
	if(op1->type==op2->type)
		return;
	else if(op1->type > op2->type)
		mpwl_make_type(op2,op1->type);
	else /*if(op1->type < op2->type)*/
		mpwl_make_type(op1,op2->type);
}

static void
mpwl_clear(MpwRealNum *op)
{
	if(!op) return;

	switch(op->type) {
	case MPW_INTEGER:
		if(op->data.ival)
			mpz_clear(op->data.ival);
		break;
	case MPW_RATIONAL:
		if(op->data.rval)
			mpq_clear(op->data.rval);
		break;
	case MPW_FLOAT:
		if(op->data.fval)
			mpf_clear(op->data.fval);
		break;
	}
}

static void
mpwl_init_type(MpwRealNum *op,int type)
{
	if(!op) return;

	op->type=type;

	switch(type) {
	case MPW_NATIVEINT:
		op->data.nval = 0;
		break;
	case MPW_INTEGER:
		if(!op->data.ival)
			op->data.ival = g_new(__mpz_struct,1);
		mpz_init(op->data.ival);
		break;
	case MPW_RATIONAL:
		if(!op->data.rval)
			op->data.rval = g_new(__mpq_struct,1);
		mpq_init(op->data.rval);
		break;
	case MPW_FLOAT:
		if(!op->data.fval)
			op->data.fval = g_new(__mpf_struct,1);
		mpf_init(op->data.fval);
		break;
	}
}

static void
mpwl_free(MpwRealNum *op)
{
	if(!op) return;
	mpwl_clear(op);
	/*FIXME: the 2000 should be settable*/
	if(free_reals_n>2000) {
		if(op->data.fval)
			g_free(op->data.fval);
		if(op->data.rval)
			g_free(op->data.rval);
		if(op->data.ival)
			g_free(op->data.ival);
		g_free(op);
	} else {
		op->alloc.next = free_reals;
		free_reals = op;
		free_reals_n++;
	}
}

static int
mpwl_sgn(MpwRealNum *op)
{
	switch(op->type) {
	case MPW_NATIVEINT: 
		if(op->data.nval>0)
			return 1;
		else if(op->data.nval<0)
			return -1;
		else
			return 0;
	case MPW_FLOAT: return mpf_sgn(op->data.fval);
	case MPW_RATIONAL: return mpq_sgn(op->data.rval);
	case MPW_INTEGER: return mpz_sgn(op->data.ival);
	}
	return 0;
}

static int
mpwl_cmp(MpwRealNum *op1, MpwRealNum *op2)
{
	int r=0;
	int t;

	t=mpwl_make_same_extra_type(op1,op2);
	switch(t) {
	case MPW_NATIVEINT:
		if(op1->data.nval > op2->data.nval)
			return 1;
		else if(op1->data.nval < op2->data.nval)
			return -1;
		else
			return 0;
	case MPW_FLOAT:
		r=mpf_cmp(op1->data.fval,op2->data.fval);
		break;
	case MPW_RATIONAL:
		r=mpq_cmp(op1->data.rval,op2->data.rval);
		break;
	case MPW_INTEGER:
		r=mpz_cmp(op1->data.ival,op2->data.ival);
		break;
	}
	mpwl_clear_extra_type(op1,t);
	mpwl_clear_extra_type(op2,t);
	return r;
}

static int
mpwl_cmp_ui(MpwRealNum *op, unsigned long int i)
{
	switch(op->type) {
	case MPW_NATIVEINT:
		if(op->data.nval > i)
			return 1;
		else if(op->data.nval < i)
			return -1;
		else
			return 0;
	case MPW_FLOAT: return mpf_cmp_ui(op->data.fval,i);
	case MPW_RATIONAL: return mpq_cmp_ui(op->data.rval,i,1);
	case MPW_INTEGER: return mpz_cmp_ui(op->data.ival,i);
	}
	return 0;
}

static void
mpwl_set_d(MpwRealNum *rop,double d)
{
	switch(rop->type) {
	case MPW_FLOAT:
		mpf_set_d(rop->data.fval,d);
		break;
	case MPW_RATIONAL:
	case MPW_INTEGER:
		mpwl_clear(rop);
	case MPW_NATIVEINT:
		mpwl_init_type(rop,MPW_FLOAT);
		mpf_set_d(rop->data.fval,d);
		break;
	}
}

static void
mpwl_set_si(MpwRealNum *rop,signed long int i)
{
	switch(rop->type) {
	case MPW_FLOAT:
		mpwl_clear(rop);
		break;
	case MPW_RATIONAL:
		mpwl_clear(rop);
		break;
	case MPW_INTEGER:
		mpwl_clear(rop);
		break;
	}
	if(i==LONG_MIN) {
		mpwl_init_type(rop,MPW_INTEGER);
		mpz_set_si(rop->data.ival,i);
		return;
	}
	rop->type = MPW_NATIVEINT;
	rop->data.nval = i;
}

static void
mpwl_set_ui(MpwRealNum *rop,unsigned long int i)
{
	if(i>LONG_MAX) {
		switch(rop->type) {
		case MPW_FLOAT:
			mpwl_clear(rop);
			mpwl_init_type(rop,MPW_INTEGER);
			mpz_set_ui(rop->data.ival,i);
			break;
		case MPW_RATIONAL:
			mpq_set_ui(rop->data.rval,i,1);
			break;
		case MPW_NATIVEINT:
			mpwl_init_type(rop,MPW_INTEGER);
		case MPW_INTEGER:
			mpz_set_ui(rop->data.ival,i);
			break;
		}
	} else {
		switch(rop->type) {
		case MPW_FLOAT:
			mpwl_clear(rop);
			break;
		case MPW_RATIONAL:
			mpwl_clear(rop);
			break;
		case MPW_INTEGER:
			mpwl_clear(rop);
			break;
		}
		rop->type = MPW_NATIVEINT;
		rop->data.nval = i;
	}
}

static void
mpwl_move(MpwRealNum *rop,MpwRealNum *op)
{
	if(rop==op)
		return;

	rop->type = op->type;
	rop->data.nval = op->data.nval;
	if(rop->data.ival) g_free(rop->data.ival);
	if(rop->data.rval) g_free(rop->data.rval);
	if(rop->data.fval) g_free(rop->data.fval);
	rop->data.ival = op->data.ival;
	rop->data.rval = op->data.rval;
	rop->data.fval = op->data.fval;
	op->type = MPW_NATIVEINT;
	op->data.nval = 0;
	op->data.ival = NULL;
	op->data.rval = NULL;
	op->data.fval = NULL;
}

static void
mpwl_set(MpwRealNum *rop,MpwRealNum *op)
{
	if(rop==op)
		return;
	else if(rop->type==op->type) {
		switch(op->type) {
		case MPW_NATIVEINT:
			rop->data.nval = op->data.nval;
			break;
		case MPW_FLOAT:
			mpf_set(rop->data.fval,op->data.fval);
			break;
		case MPW_RATIONAL:
			mpq_set(rop->data.rval,op->data.rval);
			mpwl_make_int(rop,FALSE);
			break;
		case MPW_INTEGER:
			mpz_set(rop->data.ival,op->data.ival);
			break;
		}
	} else {
		mpwl_clear(rop);
		mpwl_init_type(rop,op->type);
		mpwl_set(rop,op);
	}
}

static int
mympn_add(long *res, long op1, long op2)
{
	long r;
	if(op1>=0 && op2>=0) {
		r = op1+op2;
		if(r<0)
			return FALSE;
	} else if (op1<0 && op2<0) {
		r = op1+op2;
		if(r>=0 || r==LONG_MIN)
			return FALSE;
	} else {
		/*we would get a one off error!*/
		if(op1 == LONG_MIN ||
		   op2 == LONG_MIN)
			return FALSE;
		r = op1+op2;
	}
	*res = r;

	return TRUE;
}

static void
mpwl_add(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	int t;
	MpwRealNum r={0};
	
	/*special case*/
	if(op1->type==op2->type && rop!=op1 && rop!=op2) {
		if(rop->type != op1->type) {
			mpwl_clear(rop);
			mpwl_init_type(rop,op1->type);
		}
		switch(op1->type) {
		case MPW_NATIVEINT:
			if(!mympn_add(&rop->data.nval,op1->data.nval,op2->data.nval)) {
				mpwl_init_type(rop,MPW_INTEGER);
				mpz_set_si(rop->data.ival,op1->data.nval);
				if(op2->data.nval>=0)
					mpz_add_ui(rop->data.ival,rop->data.ival,
						   op2->data.nval);
				else
					mpz_sub_ui(rop->data.ival,rop->data.ival,
						   -(op2->data.nval));

			}
			break;
		case MPW_FLOAT:
			mpf_add(rop->data.fval,op1->data.fval,op2->data.fval);
			break;
		case MPW_RATIONAL:
			mpq_add(rop->data.rval,op1->data.rval,op2->data.rval);
			mpwl_make_int(rop,FALSE);
			break;
		case MPW_INTEGER:
			mpz_add(rop->data.ival,op1->data.ival,op2->data.ival);
			break;
		}
		return;
	}

	t=mpwl_make_same_extra_type(op1,op2);

	mpwl_init_type(&r,t);

	switch(t) {
	case MPW_NATIVEINT:
		if(!mympn_add(&r.data.nval,op1->data.nval,op2->data.nval)) {
			mpwl_init_type(&r,MPW_INTEGER);
			mpz_set_si(r.data.ival,op1->data.nval);
			if(op2->data.nval>=0)
				mpz_add_ui(r.data.ival,r.data.ival,op2->data.nval);
			else
				mpz_sub_ui(r.data.ival,r.data.ival,-(op2->data.nval));

		}
		break;
	case MPW_FLOAT:
		mpf_add(r.data.fval,op1->data.fval,op2->data.fval);
		break;
	case MPW_RATIONAL:
		mpq_add(r.data.rval,op1->data.rval,op2->data.rval);
		mpwl_make_int(&r,FALSE);
		break;
	case MPW_INTEGER:
		mpz_add(r.data.ival,op1->data.ival,op2->data.ival);
		break;
	}
	mpwl_clear_extra_type(op1,t);
	mpwl_clear_extra_type(op2,t);
	mpwl_move(rop,&r);
}

static int
mympn_sub(long *res, long op1, long op2)
{
	long r;
	if(op1>=0 && op2<0) {
		r = op1-op2;
		if(r<0)
			return FALSE;
	} else if (op1<0 && op2>=0) {
		r = op1-op2;
		if(r>=0 || r==LONG_MIN)
			return FALSE;
	} else {
		/*we would get a one off error!*/
		if(op1 == LONG_MIN ||
		   op2 == LONG_MIN)
			return FALSE;
		r = op1-op2;
	}
	*res = r;

	return TRUE;
}

static void
mpwl_sub(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	int t;
	MpwRealNum r={0};

	/*special case*/
	if(op1->type==op2->type && rop!=op1 && rop!=op2) {
		if(rop->type != op1->type) {
			mpwl_clear(rop);
			mpwl_init_type(rop,op1->type);
		}
		switch(op1->type) {
		case MPW_NATIVEINT:
			if(!mympn_sub(&rop->data.nval,op1->data.nval,op2->data.nval)) {
				mpwl_init_type(rop,MPW_INTEGER);
				mpz_set_si(rop->data.ival,op1->data.nval);
				if(op2->data.nval>=0)
					mpz_add_ui(rop->data.ival,rop->data.ival,op2->data.nval);
				else
					mpz_sub_ui(rop->data.ival,rop->data.ival,-(op2->data.nval));

			}
			break;
		case MPW_FLOAT:
			mpf_sub(rop->data.fval,op1->data.fval,op2->data.fval);
			break;
		case MPW_RATIONAL:
			mpq_sub(rop->data.rval,op1->data.rval,op2->data.rval);
			mpwl_make_int(rop,FALSE);
			break;
		case MPW_INTEGER:
			mpz_sub(rop->data.ival,op1->data.ival,op2->data.ival);
			break;
		}
		return;
	}

	t=mpwl_make_same_extra_type(op1,op2);

	mpwl_init_type(&r,t);

	switch(t) {
	case MPW_NATIVEINT:
		if(!mympn_sub(&r.data.nval,op1->data.nval,op2->data.nval)) {
			mpwl_init_type(&r,MPW_INTEGER);
			mpz_set_si(r.data.ival,op1->data.nval);
			if(op2->data.nval>=0)
				mpz_sub_ui(r.data.ival,r.data.ival,op2->data.nval);
			else
				mpz_add_ui(r.data.ival,r.data.ival,-(op2->data.nval));

		}
		break;
	case MPW_FLOAT:
		mpf_sub(r.data.fval,op1->data.fval,op2->data.fval);
		break;
	case MPW_RATIONAL:
		mpq_sub(r.data.rval,op1->data.rval,op2->data.rval);
		mpwl_make_int(&r,FALSE);
		break;
	case MPW_INTEGER:
		mpz_sub(r.data.ival,op1->data.ival,op2->data.ival);
		break;
	}
	mpwl_clear_extra_type(op1,t);
	mpwl_clear_extra_type(op2,t);
	mpwl_move(rop,&r);
}

static void
mpwl_mul(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	int t;
	MpwRealNum r={0};

	/*special case*/
	if(op1->type==op2->type && rop!=op1 && rop!=op2) {
		if(rop->type != op1->type) {
			mpwl_clear(rop);
			mpwl_init_type(rop,op1->type);
		}
		switch(op1->type) {
		case MPW_NATIVEINT:
			mpwl_init_type(rop,MPW_INTEGER);
			mpz_set_si(rop->data.ival,op1->data.nval);
			if(op2->data.nval>=0)
				mpz_mul_ui(rop->data.ival,rop->data.ival,op2->data.nval);
			else {
				mpz_mul_ui(rop->data.ival,rop->data.ival,-(op2->data.nval));
				mpz_neg(rop->data.ival,rop->data.ival);
			}
			break;
		case MPW_FLOAT:
			mpf_mul(rop->data.fval,op1->data.fval,op2->data.fval);
			break;
		case MPW_RATIONAL:
			mpq_mul(rop->data.rval,op1->data.rval,op2->data.rval);
			mpwl_make_int(rop,FALSE);
			break;
		case MPW_INTEGER:
			mpz_mul(rop->data.ival,op1->data.ival,op2->data.ival);
			break;
		}
		return;
	}

	t=mpwl_make_same_extra_type(op1,op2);

	mpwl_init_type(&r,t);

	switch(t) {
	case MPW_NATIVEINT:
		mpwl_init_type(&r,MPW_INTEGER);
		mpz_set_si(r.data.ival,op1->data.nval);
		if(op2->data.nval>=0)
			mpz_mul_ui(r.data.ival,r.data.ival,op2->data.nval);
		else {
			mpz_mul_ui(r.data.ival,r.data.ival,-(op2->data.nval));
			mpz_neg(r.data.ival,r.data.ival);
		}
		break;
	case MPW_FLOAT:
		mpf_mul(r.data.fval,op1->data.fval,op2->data.fval);
		break;
	case MPW_RATIONAL:
		mpq_mul(r.data.rval,op1->data.rval,op2->data.rval);
		mpwl_make_int(&r,FALSE);
		break;
	case MPW_INTEGER:
		mpz_mul(r.data.ival,op1->data.ival,op2->data.ival);
		break;
	}
	mpwl_clear_extra_type(op1,t);
	mpwl_clear_extra_type(op2,t);
	mpwl_move(rop,&r);
}

static void
mpwl_mul_ui(MpwRealNum *rop,MpwRealNum *op,unsigned long int i)
{
	if(rop->type!=op->type) {
		mpwl_clear(rop);
		mpwl_init_type(rop,op->type);
	}

	switch(op->type) {
	case MPW_NATIVEINT:
		if(rop!=op) {
			mpwl_init_type(rop,MPW_INTEGER);
			mpz_set(rop->data.ival,op->data.ival);
			mpz_mul_ui(rop->data.ival,rop->data.ival,i);
		} else {
			mpwl_make_type(rop,MPW_INTEGER);
			mpz_mul_ui(rop->data.ival,rop->data.ival,i);
		}
		break;
	case MPW_FLOAT:
		mpf_mul_ui(rop->data.fval,op->data.fval,i);
		break;
	case MPW_RATIONAL:
		mpz_mul_ui(mpq_numref(rop->data.rval),
			   mpq_numref(op->data.rval),i);
		mpwl_make_int(rop,FALSE);
		break;
	case MPW_INTEGER:
		mpz_mul_ui(rop->data.ival,op->data.ival,i);
		break;
	}
}

static void
mpwl_div(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	int t;
	MpwRealNum r = {0};

	if(mpwl_sgn(op2)==0) {
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}

	/*special case*/
	if(op1->type > MPW_INTEGER && op1->type==op2->type && rop!=op2) {
		if(rop->type != op1->type) {
			mpwl_clear(rop);
			mpwl_init_type(rop,op1->type);
		}
		switch(op1->type) {
		case MPW_FLOAT:
			mpf_div(rop->data.fval,op1->data.fval,op2->data.fval);
			break;
		case MPW_RATIONAL:
			mpq_div(rop->data.rval,op1->data.rval,op2->data.rval);
			mpwl_make_int(rop,FALSE);
			break;
		case MPW_INTEGER: break;
		}
		return;
	}

	t=mpwl_make_same_extra_type(op1,op2);

	switch(t) {
	case MPW_FLOAT:
		mpwl_init_type(&r,t);
		mpf_div(r.data.fval,op1->data.fval,op2->data.fval);
		break;
	case MPW_RATIONAL:
		mpwl_init_type(&r,t);
		mpq_div(r.data.rval,op1->data.rval,op2->data.rval);
		mpwl_make_int(&r,FALSE);
		break;
	case MPW_INTEGER:
		mpwl_init_type(&r,MPW_RATIONAL);
		mpq_set_z(r.data.rval,op1->data.ival);
		mpz_set(mpq_denref(r.data.rval),
			op2->data.ival);
		break;
	case MPW_NATIVEINT:
		mpwl_init_type(&r,MPW_RATIONAL);
		if(op2->data.nval>0)
			mpq_set_si(r.data.rval,
				   op1->data.nval,
				   op2->data.nval);
		else
			mpq_set_si(r.data.rval,
				   op1->data.nval,
				   -(op2->data.nval));
		break;
	}
	mpwl_clear_extra_type(op1,t);
	mpwl_clear_extra_type(op2,t);
	mpwl_move(rop,&r);

}

static void
mpwl_div_ui(MpwRealNum *rop,MpwRealNum *op,unsigned long int i)
{
	int t;
	if(i==0) {
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}

	switch(op->type) {
	case MPW_FLOAT:
		if(rop->type!=MPW_FLOAT) {
			mpwl_clear(rop);
			mpwl_init_type(rop,MPW_FLOAT);
		}
		mpf_div_ui(rop->data.fval,op->data.fval,i);
		break;
	case MPW_RATIONAL:
		if(rop->type!=MPW_RATIONAL) {
			mpwl_clear(rop);
			mpwl_init_type(rop,MPW_RATIONAL);
		}
		mpz_mul_ui(mpq_denref(rop->data.rval),
			   mpq_denref(op->data.rval),i);
		mpwl_make_int(rop,FALSE);
		break;
	case MPW_INTEGER:
		t = rop->type;
		mpwl_make_extra_type(rop,MPW_RATIONAL);
		rop->type = MPW_RATIONAL;
		mpq_set_z(rop->data.rval,op->data.ival);
		mpz_set_ui(mpq_denref(rop->data.rval),i);
		mpwl_clear_extra_type(rop,t);
		break;
	case MPW_NATIVEINT:
		t = rop->type;
		mpwl_make_extra_type(rop,MPW_RATIONAL);
		rop->type = MPW_RATIONAL;
		mpq_set_si(rop->data.rval,op->data.nval,i);
		mpwl_clear_extra_type(rop,t);
		break;
	}
}

static void
mpwl_ui_div(MpwRealNum *rop,unsigned long int i,MpwRealNum *op)
{
	int t;
	if(mpwl_sgn(op)==0) {
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}

	switch(op->type) {
	case MPW_FLOAT:
		if(rop->type!=MPW_FLOAT) {
			mpwl_clear(rop);
			mpwl_init_type(rop,MPW_FLOAT);
		}
		mpf_ui_div(rop->data.fval,i,op->data.fval);
		break;
	case MPW_RATIONAL:
		if(rop->type!=MPW_RATIONAL) {
			mpwl_clear(rop);
			mpwl_init_type(rop,MPW_RATIONAL);
		}
		mpq_inv(rop->data.rval,op->data.rval);
		mpz_mul_ui(mpq_numref(rop->data.rval),
			   mpq_numref(rop->data.rval),i);
		mpwl_make_int(rop,FALSE);
		break;
	case MPW_INTEGER:
		t = rop->type;
		mpwl_make_extra_type(rop,MPW_RATIONAL);
		rop->type = MPW_RATIONAL;
		mpz_set_ui(mpq_numref(rop->data.rval),i);
		mpz_set(mpq_denref(rop->data.rval),op->data.ival);
		mpwl_clear_extra_type(rop,t);
		break;
	case MPW_NATIVEINT:
		t = rop->type;
		mpwl_make_extra_type(rop,MPW_RATIONAL);
		rop->type = MPW_RATIONAL;
		if(op->data.nval>0)
			mpq_set_ui(rop->data.rval,i,
				   op->data.nval);
		else
			mpq_set_ui(rop->data.rval,i,
				   -(op->data.nval));
		mpwl_clear_extra_type(rop,t);
		break;
	}
}

static void
mpwl_mod(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	if(mpwl_sgn(op2)==0) {
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}

	if(op1->type<=MPW_INTEGER && op2->type<=MPW_INTEGER) {
		int t=mpwl_make_same_extra_type(op1,op2);
		int t1;

		switch(t) {
		case MPW_INTEGER:
			t1 = rop->type;
			mpwl_make_extra_type(rop,MPW_INTEGER);
			rop->type = MPW_INTEGER;
			mpz_mod(rop->data.ival,op1->data.ival,op2->data.ival);
			mpwl_clear_extra_type(rop,t1);
			break;
		case MPW_NATIVEINT:
			if(rop->type!=MPW_NATIVEINT) {
				mpwl_clear(rop);
				mpwl_init_type(rop,MPW_INTEGER);
			}
			rop->data.nval = op1->data.nval % op2->data.nval;
			break;
		}
		mpwl_clear_extra_type(op1,t);
		mpwl_clear_extra_type(op2,t);
	} else {
		(*errorout)("Can't do modulo of floats or rationals!");
		error_num=NUMERICAL_MPW_ERROR;
	}
}

static void
mpwl_gcd(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	if(op1->type<=MPW_INTEGER && op2->type<=MPW_INTEGER) {
		int t=mpwl_make_same_extra_type(op1,op2);
		int t1;
		long a,b;

		switch(t) {
		case MPW_INTEGER:
			t1 = rop->type;
			mpwl_make_extra_type(rop,MPW_INTEGER);
			rop->type = MPW_INTEGER;
			mpz_gcd(rop->data.ival,op1->data.ival,op2->data.ival);
			mpwl_clear_extra_type(rop,t1);
			break;
		case MPW_NATIVEINT:
			if(rop->type!=MPW_NATIVEINT) {
				mpwl_clear(rop);
				mpwl_init_type(rop,MPW_INTEGER);
			}
			a = op1->data.nval;
		        b = op2->data.nval;
			while(b!=0) {
				long tmp = a%b;
				a = b;
				b = tmp;
			}
			rop->data.nval = a>0?a:-a;
			break;
		}
		mpwl_clear_extra_type(op1,t);
		mpwl_clear_extra_type(op2,t);
	} else {
		(*errorout)("Can't do GCD of floats or rationals!");
		error_num=NUMERICAL_MPW_ERROR;
	}
}

static void
mpwl_neg(MpwRealNum *rop,MpwRealNum *op)
{
	if(rop->type!=op->type) {
		mpwl_clear(rop);
		mpwl_init_type(rop,op->type);
	}

	switch(op->type) {
	case MPW_FLOAT:
		mpf_neg(rop->data.fval,op->data.fval);
		break;
	case MPW_RATIONAL:
		mpq_neg(rop->data.rval,op->data.rval);
		break;
	case MPW_INTEGER:
		mpz_neg(rop->data.ival,op->data.ival);
		break;
	case MPW_NATIVEINT:
		rop->data.nval = -(op->data.nval);
		break;
	}
}

static void
mpwl_fac_ui(MpwRealNum *rop,unsigned int op)
{
	if(rop->type!=MPW_INTEGER) {
		mpwl_clear(rop);
		mpwl_init_type(rop,MPW_INTEGER);
	}
	mpz_fac_ui(rop->data.ival,op);
}

static void
mpwl_fac(MpwRealNum *rop,MpwRealNum *op)
{
	if(op->type!=MPW_INTEGER && op->type!=MPW_NATIVEINT) {
		(*errorout)("Can't do factorials of rationals or floats!");
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}
	if(op->type==MPW_INTEGER) {
		if(mpz_cmp_ui(op->data.ival,ULONG_MAX)>0) {
			(*errorout)("Number too large to compute factorial!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		if(mpz_sgn(op->data.ival)<0) {
			(*errorout)("Can't do factorials of negative numbers!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		mpwl_fac_ui(rop,mpz_get_ui(op->data.ival));
	} else {
		if(op->data.nval<0) {
			(*errorout)("Can't do factorials of negative numbers!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		mpwl_fac_ui(rop,op->data.nval);
	}
}

static int
mpwl_pow_q(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	mpf_t fr;
	mpf_t fr2;
	mpf_t frt;
	unsigned long i;
	mpf_t de;
	mpz_t des;
	int t;
	int reverse=FALSE;

	if(op2->type!=MPW_RATIONAL) {
		error_num=INTERNAL_MPW_ERROR;
		return FALSE;
	}

	i=mpz_get_ui(mpq_denref(op2->data.rval));
	if(mpwl_sgn(op1)<0 && !(i & 0x1)) {
		/*it's gonna be complex*/
		error_num=NUMERICAL_MPW_ERROR;
		return TRUE;
	}

	mpz_init_set(des,mpq_denref(op2->data.rval));
	mpz_sub_ui(des,des,1);

	mpf_init(de);
	mpf_set_z(de,mpq_denref(op2->data.rval));


	if(mpq_sgn(op2->data.rval)<0)
		reverse=TRUE;

	mpwl_make_extra_type(op1,MPW_FLOAT);
	t=MPW_FLOAT;


	/*
	 * Newton's method: Xn+1 = Xn - f(Xn)/f'(Xn)
	 */
	
	mpf_init(fr);
	mpf_init(fr2);
	mpf_init(frt);
	mpf_div_ui(fr,op1->data.fval,2); /*use half the value
					     as an initial guess*/
	for(;;) {
		mympf_pow_z(fr2,fr,mpq_denref(op2->data.rval));
		mpf_sub(fr2,fr2,op1->data.fval);

		mympf_pow_z(frt,fr,des);
		mpf_mul(frt,frt,de);
		mpf_div(fr2,fr2,frt);
		mpf_neg(fr2,fr2);
		mpf_add(fr2,fr2,fr);

		
		if(mpf_cmp(fr2,fr)==0)
			break;
		mpf_set(fr,fr2);
	}
	mpf_clear(fr2);
	mpf_clear(frt);
	mpz_clear(des);
	mpf_clear(de);

	if(reverse) {
		/*numerator will be negative*/
		mpz_neg(mpq_numref(op2->data.rval),mpq_numref(op2->data.rval));
		mympf_pow_z(fr,fr,mpq_numref(op2->data.rval));
		mpz_neg(mpq_numref(op2->data.rval),mpq_numref(op2->data.rval));

		mpf_ui_div(fr,1,fr);
	} else
		mympf_pow_z(fr,fr,mpq_numref(op2->data.rval));

	/*op1 might have equaled rop so clear extra type here*/
	mpwl_clear_extra_type(op1,t);

	mpwl_clear(rop);
	mpwl_init_type(rop,MPW_FLOAT);
	mpf_set(rop->data.fval,fr);
	return FALSE;
}

/*power to an unsigned long and optionaly invert the answer*/
static void
mpwl_pow_ui(MpwRealNum *rop,MpwRealNum *op1,unsigned int e, int reverse)
{
	MpwRealNum r={0};

	switch(op1->type) {
	case MPW_RATIONAL:
		mpwl_init_type(&r,MPW_RATIONAL);
		mpz_pow_ui(mpq_numref(r.data.rval),
			   mpq_numref(op1->data.rval),e);
		mpz_pow_ui(mpq_denref(r.data.rval),
			   mpq_denref(op1->data.rval),e);
		/*the exponent was negative! reverse the result!*/
		if(reverse)
			mpq_inv(r.data.rval,r.data.rval);
		break;
	case MPW_NATIVEINT:
		if(!reverse) {
			mpwl_init_type(&r,MPW_INTEGER);
			mpz_set_si(r.data.ival,op1->data.nval);
			mpz_pow_ui(r.data.ival, r.data.ival,e);
		} else {
			mpwl_init_type(&r,MPW_RATIONAL);
			mpz_set_si(mpq_denref(r.data.rval),op1->data.nval);
			mpz_pow_ui(mpq_denref(r.data.rval),
				   mpq_denref(r.data.rval),e);
			mpz_set_ui(mpq_numref(r.data.rval),1);
		}
		break;
	case MPW_INTEGER:
		if(!reverse) {
			mpwl_init_type(&r,MPW_INTEGER);
			mpz_pow_ui(r.data.ival,
				   op1->data.ival,e);
		} else {
			mpwl_init_type(&r,MPW_RATIONAL);
			mpz_pow_ui(mpq_denref(r.data.rval),
				   op1->data.ival,e);
			mpz_set_ui(mpq_numref(r.data.rval),1);
		}
		break;
	case MPW_FLOAT:
		mpwl_init_type(&r,MPW_FLOAT);
		mympf_pow_ui(r.data.fval,op1->data.fval,e);

		if(reverse)
			mpf_ui_div(r.data.fval,1,r.data.fval);
		break;
	}
	mpwl_move(rop,&r);
}

static void
mpwl_pow_z(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	if(op2->type!=MPW_INTEGER) {
		error_num=INTERNAL_MPW_ERROR;
		return;
	}

	if(mpz_cmp_ui(op2->data.ival,ULONG_MAX)>0) {
		MpwRealNum r={0};
		int reverse;
		
		if(mpz_sgn(op2->data.ival)<0) {
			reverse = TRUE;
			mpz_neg(op2->data.ival,op2->data.ival);
		}

		switch(op1->type) {
		case MPW_RATIONAL:
			mpwl_init_type(&r,MPW_RATIONAL);
			mympz_pow_z(mpq_numref(r.data.rval),
				    mpq_numref(op1->data.rval),
				    op2->data.ival);
			mympz_pow_z(mpq_denref(r.data.rval),
				    mpq_denref(op1->data.rval),
				    op2->data.ival);
			/*the exponent was negative! reverse the result!*/
			if(reverse)
				mpq_inv(r.data.rval,r.data.rval);
			break;
		case MPW_NATIVEINT:
			if(!reverse) {
				mpwl_init_type(&r,MPW_INTEGER);
				mpz_set_si(r.data.ival,op1->data.nval);
				mympz_pow_z(r.data.ival, r.data.ival,
					    op2->data.ival);
			} else {
				mpwl_init_type(&r,MPW_RATIONAL);
				mpz_set_si(mpq_denref(r.data.rval),op1->data.nval);
				mympz_pow_z(mpq_denref(r.data.rval),
					    mpq_denref(r.data.rval),
					    op2->data.ival);
				mpz_set_ui(mpq_numref(r.data.rval),1);
			}
			break;
		case MPW_INTEGER:
			if(!reverse) {
				mpwl_init_type(&r,MPW_INTEGER);
				mympz_pow_z(r.data.ival,
					    op1->data.ival,
					    op2->data.ival);
			} else {
				mpwl_init_type(&r,MPW_RATIONAL);
				mympz_pow_z(mpq_denref(r.data.rval),
					    op1->data.ival,
					    op2->data.ival);
				mpz_set_ui(mpq_numref(r.data.rval),1);
			}
			break;
		case MPW_FLOAT:
			mpwl_init_type(&r,MPW_FLOAT);
			mympf_pow_z(r.data.fval,op1->data.fval,
				    op2->data.ival);

			if(reverse)
				mpf_ui_div(r.data.fval,1,r.data.fval);
			break;
		}
		if(reverse)
			mpz_neg(op2->data.ival,op2->data.ival);

		mpwl_move(rop,&r);

	} else {
		if(mpz_sgn(op2->data.ival)==0)
			mpwl_set_ui(rop,1);
		else if(mpz_sgn(op2->data.ival)>0)
			mpwl_pow_ui(rop,op1,mpz_get_ui(op2->data.ival),FALSE);
		else
			mpwl_pow_ui(rop,op1,mpz_get_ui(op2->data.ival),TRUE);
	}
}

static int
mpwl_pow_f(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	MpwRealNum r={0};
	int retval;

	if(op2->type!=MPW_FLOAT) {
		error_num=INTERNAL_MPW_ERROR;
		return FALSE;
	}

	mpwl_init_type(&r,MPW_FLOAT);
	mpwl_set(&r,op2);
	mpwl_make_type(&r,MPW_RATIONAL);
	retval = mpwl_pow_q(rop,op1,&r);
	mpwl_clear(&r);
	return retval;
}

static int
mpwl_pow(MpwRealNum *rop,MpwRealNum *op1,MpwRealNum *op2)
{
	if(mpwl_sgn(op2)==0) {
		mpwl_set_ui(rop,1);
		return FALSE;
	}

	switch(op2->type) {
	case MPW_NATIVEINT:
		if(op2->data.nval == 0)
			mpwl_set_ui(rop,1);
		else if(op2->data.nval>0)
			mpwl_pow_ui(rop,op1,op2->data.nval,FALSE);
		else
			mpwl_pow_ui(rop,op1,op2->data.nval,TRUE);
		break;
	case MPW_FLOAT: return mpwl_pow_f(rop,op1,op2);
	case MPW_RATIONAL: return mpwl_pow_q(rop,op1,op2);
	case MPW_INTEGER: mpwl_pow_z(rop,op1,op2); break;
	}
	return FALSE;
}

static int
mpwl_sqrt(MpwRealNum *rop,MpwRealNum *op)
{
	MpwRealNum r={0};
	int complex=FALSE;

	mpwl_init_type(&r,MPW_FLOAT);
	if(mpwl_sgn(op)<0) {
		complex = TRUE;
		mpwl_neg(op,op);
	}
	mpwl_make_extra_type(op,MPW_FLOAT);
	mpf_sqrt(r.data.fval,op->data.fval);
	mpwl_clear_extra_type(op,MPW_FLOAT);
	if(complex)
		mpwl_neg(op,op);

	mpwl_move(rop,&r);
	return complex;
}

static void
mpwl_make_int(MpwRealNum *rop, int floats)
{
	mpf_t fr;
	switch(rop->type) {
		case MPW_NATIVEINT:
		case MPW_INTEGER: return;
		case MPW_RATIONAL:
			mpq_canonicalize(rop->data.rval);
			if(mpz_cmp_ui(mpq_denref(rop->data.rval),1)==0) {
				if(!rop->data.ival)
					rop->data.ival = g_new(__mpz_struct,1);
				mpz_init_set(rop->data.ival,
					mpq_numref(rop->data.rval));
				mpq_clear(rop->data.rval);
				rop->type=MPW_INTEGER;
			}
			break;
		case MPW_FLOAT:
			if(!floats) return;
			/*gotta find a better way of doing this!*/

			if(!rop->data.ival)
				rop->data.ival = g_new(__mpz_struct,1);
			mpz_init(rop->data.ival);
			mpz_set_f(rop->data.ival,rop->data.fval);
			mpf_init(fr);
			mpf_set_z(fr,rop->data.ival);
			if(mpf_cmp(fr,rop->data.fval)==0) {
				rop->type=MPW_INTEGER;
				mpf_clear(rop->data.fval);
			} else
				mpz_clear(rop->data.ival);
			mpf_clear(fr);
			break;
	}

}

/*make number into a float, this might be neccessary for unprecise
  calculations*/
static void
mpwl_make_float(MpwRealNum *rop)
{
	mpwl_make_type(rop,MPW_FLOAT);
}

static void
mpwl_round(MpwRealNum *rop)
{
	if(rop->type > MPW_INTEGER) {
		if(rop->type == MPW_FLOAT) {
			mpf_t tmp;
			mpf_init_set_d(tmp,0.5);
			if(mpf_sgn(rop->data.fval)<0)
				mpf_sub(rop->data.fval,rop->data.fval,tmp);
			else
				mpf_add(rop->data.fval,rop->data.fval,tmp);
			mpf_clear(tmp);
		} else /*MPW_RATIONAL*/ {
			mpq_t tmp;
			mpq_init(tmp);
			mpq_set_ui(tmp,1,2);
			if(mpq_sgn(rop->data.rval)<0)
				mpq_sub(rop->data.rval,rop->data.rval,tmp);
			else
				mpq_add(rop->data.rval,rop->data.rval,tmp);
			mpq_clear(tmp);
		}
		mpwl_make_type(rop,MPW_INTEGER);
	}
}

static void
mpwl_ceil(MpwRealNum *rop)
{
	if(rop->type > MPW_INTEGER) {
		if(rop->type == MPW_FLOAT) {
			mpf_t fr;
			rop->type=MPW_INTEGER;
			if(!rop->data.ival)
				rop->data.ival = g_new(__mpz_struct,1);
			mpz_init(rop->data.ival);
			mpz_set_f(rop->data.ival,rop->data.fval);
			mpf_init(fr);
			mpf_set_z(fr,rop->data.ival);
			if(mpf_cmp(fr,rop->data.fval)==0) {
				mpf_clear(rop->data.fval);
			} else {
				if(mpf_sgn(rop->data.fval)>0)
					mpz_add_ui(rop->data.ival,
						   rop->data.ival,1);
				mpf_clear(rop->data.fval);
			}
			mpf_clear(fr);
		} else /*MPW_RATIONAL*/ {
			mpq_canonicalize(rop->data.rval);
			if(mpz_cmp_ui(mpq_denref(rop->data.rval),1)==0) {
				rop->type=MPW_INTEGER;
				if(!rop->data.ival)
					rop->data.ival = g_new(__mpz_struct,1);
				mpz_init_set(rop->data.ival,
					     mpq_numref(rop->data.rval));
				mpq_clear(rop->data.rval);
			} else {
				if(mpq_sgn(rop->data.rval)>0) {
					mpwl_make_type(rop,MPW_INTEGER);
					mpz_add_ui(rop->data.ival,
						   rop->data.ival,1);
				} else 
					mpwl_make_type(rop,MPW_INTEGER);
			}
		}
	}
}

static void
mpwl_floor(MpwRealNum *rop)
{
	if(rop->type > MPW_INTEGER) {
		if(rop->type == MPW_FLOAT) {
			mpf_t fr;
			rop->type=MPW_INTEGER;
			if(!rop->data.ival)
				rop->data.ival = g_new(__mpz_struct,1);
			mpz_init(rop->data.ival);
			mpz_set_f(rop->data.ival,rop->data.fval);
			mpf_init(fr);
			mpf_set_z(fr,rop->data.ival);
			if(mpf_cmp(fr,rop->data.fval)==0) {
				mpf_clear(rop->data.fval);
			} else {
				if(mpf_sgn(rop->data.fval)<0)
					mpz_sub_ui(rop->data.ival,
						   rop->data.ival,1);
				mpf_clear(rop->data.fval);
			}
			mpf_clear(fr);
		} else /*MPW_RATIONAL*/ {
			mpq_canonicalize(rop->data.rval);
			if(mpz_cmp_ui(mpq_denref(rop->data.rval),1)==0) {
				rop->type=MPW_INTEGER;
				if(!rop->data.ival)
					rop->data.ival = g_new(__mpz_struct,1);
				mpz_init_set(rop->data.ival,
					     mpq_numref(rop->data.rval));
				mpq_clear(rop->data.rval);
			} else {
				if(mpq_sgn(rop->data.rval)<0) {
					mpwl_make_type(rop,MPW_INTEGER);
					mpz_sub_ui(rop->data.ival,
						   rop->data.ival,1);
				} else 
					mpwl_make_type(rop,MPW_INTEGER);
			}
		}
	}
}

static void
mpwl_trunc(MpwRealNum *rop)
{
	if(rop->type > MPW_INTEGER)
		mpwl_make_type(rop,MPW_INTEGER);
}

static void
mpwl_set_str_float(MpwRealNum *rop,char *s,int base)
{
	if(rop->type!=MPW_FLOAT) {
		mpwl_clear(rop);
		mpwl_init_type(rop,MPW_FLOAT);
	}
	mpf_set_str(rop->data.fval,s,base);
}

static void
mpwl_set_str_int(MpwRealNum *rop,char *s,int base)
{
	if(rop->type!=MPW_INTEGER) {
		mpwl_clear(rop);
		mpwl_init_type(rop,MPW_INTEGER);
	}
	mpz_set_str(rop->data.ival,s,base);
	if(mpz_cmp_si(rop->data.ival,LONG_MAX)>0 ||
	   mpz_cmp_si(rop->data.ival,LONG_MIN+1)<0)
		return;
	mpwl_make_type(rop,MPW_NATIVEINT);
}


/**************/
/*output stuff*/

/*round off the number at some digits*/
static void
str_make_max_digits(char *s,int digits)
{
	int i;
	int sd=0; /*digit where the number starts*/

	if(s[0]=='-')
		sd=1;

	if(!s || digits<=0)
		return;

	digits+=sd;

	if(strlen(s)<=digits)
		return;

	if(s[digits]<'5') {
		s[digits]='\0';
		return;
	}
	s[digits]='\0';

	for(i=digits-1;i>=sd;i--) {
		if(s[i]<'9') {
			s[i]++;
			return;
		}
		s[i]='\0';
	}
	shiftstr(s,1);
	s[sd]='1';
}

/*formats a floating point with mantissa in p and exponent in e*/
static char *
str_format_float(char *p,long int e,int scientific_notation)
{
	long int len;
	int i;
	if(((e-1)<-8 || (e-1)>8) || scientific_notation) {
		if(e!=0)
			p=my_realloc(p,strlen(p)+1,
				strlen(p)+1+((int)log10(abs(e))+2)+1);
		else
			p=my_realloc(p,strlen(p)+1,strlen(p)+3);
			
		if(p[0]=='-') {
			if(strlen(p)>2) {
				shiftstr(p+2,1);
				p[2]='.';
			}
		} else {
			if(strlen(p)>1) {
				shiftstr(p+1,1);
				p[1]='.';
			}
		}
		sprintf(p,"%se%ld",p,e-1);
	} else if(e>0) {
		len=strlen(p);
		if(p[0]=='-')
			len--;
		if(e>len) {
			p=my_realloc(p,strlen(p)+1,
				strlen(p)+1+e-len);
			for(i=0;i<e-len;i++)
				strcat(p,"0");
		} else if(e<len) {
			if(p[0]=='-') {
				shiftstr(p+1+e,1);
				p[e+1]='.';
			} else {
				shiftstr(p+e,1);
				p[e]='.';
			}
		}
	} else { /*e<=0*/
		if(strlen(p)==0) {
			p=g_strdup("0");
		} else {
			p=my_realloc(p,strlen(p)+1,
				strlen(p)+1+(-e)+2);
			if(p[0]=='-') {
				shiftstr(p+1,2+(-e));
				p[1]='0';
				p[2]='.';
				for(i=0;i<(-e);i++)
					p[i+3]='0';
			} else {
				shiftstr(p,2+(-e));
				p[0]='0';
				p[1]='.';
				for(i=0;i<(-e);i++)
					p[i+2]='0';
			}
		}
	}
	return p;
}

static char *
str_getstring_n(long num, int max_digits,int scientific_notation)
{
	char *p,*p2;
	mpf_t fr;

	p = g_strdup_printf("%ld",num);
	if(max_digits>0 && max_digits<strlen(p)) {
		mpf_init(fr);
		mpf_set_si(fr,num);
		p2=str_getstring_f(fr,max_digits,scientific_notation);
		mpf_clear(fr);
		if(strlen(p2)>=strlen(p)) {
			g_free(p2);
			return p;
		} else  {
			g_free(p);
			return p2;
		}
	}
	return p;
}

static char *
str_getstring_z(mpz_t num, int max_digits,int scientific_notation)
{
	char *p,*p2;
	mpf_t fr;

	p=mpz_get_str(NULL,10,num);
	if(max_digits>0 && max_digits<strlen(p)) {
		mpf_init(fr);
		mpf_set_z(fr,num);
		p2=str_getstring_f(fr,max_digits,scientific_notation);
		mpf_clear(fr);
		if(strlen(p2)>=strlen(p)) {
			g_free(p2);
			return p;
		} else  {
			g_free(p);
			return p2;
		}
	}
	return p;
}

static char *
str_getstring_q(mpq_t num, int max_digits,int scientific_notation)
{
	char *p,*p2;
	mpf_t fr;

	p=mpz_get_str(NULL,10,mpq_numref(num));
	p=appendstr(p,"/");
	p2=mpz_get_str(NULL,10,mpq_denref(num));
	p=appendstr(p,p2);
	g_free(p2);
	if(max_digits>0 && max_digits<strlen(p)) {
		mpf_init(fr);
		mpf_set_q(fr,num);
		p2=str_getstring_f(fr,max_digits,scientific_notation);
		mpf_clear(fr);
		if(strlen(p2)>=strlen(p)) {
			g_free(p2);
			return p;
		} else  {
			g_free(p);
			return p2;
		}
	}
	return p;
}

static char *
str_getstring_f(mpf_t num, int max_digits,int scientific_notation)
{
	char *p;
	long e;

	p=mpf_get_str(NULL,&e,10,0,num);
	str_make_max_digits(p,max_digits);
	p=str_format_float(p,e,scientific_notation);

	return p;
}

static char *
mpwl_getstring(MpwRealNum * num, int max_digits,int scientific_notation,
	int results_as_floats)
{
	mpf_t fr;
	char *p;
	switch(num->type) {
	case MPW_RATIONAL:
		if(results_as_floats) {
			mpf_init(fr);
			mpf_set_q(fr,num->data.rval);
			p=str_getstring_f(fr,max_digits,
					  scientific_notation);
			mpf_clear(fr);
			return p;
		}
		return str_getstring_q(num->data.rval,max_digits,
				       scientific_notation);
	case MPW_NATIVEINT:
		if(results_as_floats) {
			mpf_init(fr);
			mpf_set_si(fr,num->data.nval);
			p=str_getstring_f(fr,max_digits,
					  scientific_notation);
			mpf_clear(fr);
			return p;
		}
		return str_getstring_n(num->data.nval,max_digits,
				       scientific_notation);
	case MPW_INTEGER:
		if(results_as_floats) {
			mpf_init(fr);
			mpf_set_z(fr,num->data.ival);
			p=str_getstring_f(fr,max_digits,
					  scientific_notation);
			mpf_clear(fr);
			return p;
		}
		return str_getstring_z(num->data.ival,max_digits,
				       scientific_notation);
	case MPW_FLOAT:
		return str_getstring_f(num->data.fval,max_digits,
				       scientific_notation);
	}
	/*something bad happened*/
	return NULL;
}

#define mpw_uncomplex(rop)					\
{								\
	if(mpwl_sgn(rop->i)==0) {				\
		rop->type=MPW_REAL;				\
		if(rop->i->type!=MPW_NATIVEINT) {		\
			mpwl_clear(rop->i);			\
			mpwl_init_type(rop->i,MPW_NATIVEINT);	\
		}						\
	}							\
}

/*************************************************************************/
/*high level stuff                                                       */
/*************************************************************************/

/*set default precision*/
void
mpw_set_default_prec(unsigned long int i)
{
	mpf_set_default_prec(i);
}

/*initialize a number*/
void
mpw_init(mpw_ptr op)
{
	op->type=MPW_REAL;
	op->r = zero;
	zero->alloc.usage++;
	op->i = zero;
	zero->alloc.usage++;
}

void
mpw_init_set(mpw_ptr rop,mpw_ptr op)
{
	rop->type=op->type;
	rop->r = op->r;
	rop->r->alloc.usage++;
	rop->i = op->i;
	rop->i->alloc.usage++;
	mpw_uncomplex(rop);
}

/*clear memory held by number*/
void
mpw_clear(mpw_ptr op)
{
	op->r->alloc.usage--;
	op->i->alloc.usage--;
	if(op->r->alloc.usage==0)
		mpwl_free(op->r);
	if(op->i->alloc.usage==0)
		mpwl_free(op->i);
	op->type=0;
}

/*make them the same type without loosing information*/
void
mpw_make_same_type(mpw_ptr op1,mpw_ptr op2)
{
	MAKE_COPY(op1->r);
	MAKE_COPY(op2->r);
	mpwl_make_same_type(op1->r,op2->r);
	if(op1->type==MPW_COMPLEX || op2->type==MPW_COMPLEX) {
		MAKE_COPY(op1->i);
		MAKE_COPY(op2->i);
		mpwl_make_same_type(op1->i,op2->i);
	}
}

void
mpw_set(mpw_ptr rop,mpw_ptr op)
{
	rop->type=op->type;
	rop->r = op->r;
	rop->r->alloc.usage++;
	rop->i = op->i;
	rop->i->alloc.usage++;
	mpw_uncomplex(rop);
}

void
mpw_set_d(mpw_ptr rop,double d)
{
	MAKE_REAL(rop);
	MAKE_COPY(rop->r);
	mpwl_set_d(rop->r,d);
}

void
mpw_set_si(mpw_ptr rop,signed long int i)
{
	MAKE_REAL(rop);
	if(i==0) {
		if(rop->r != zero) {
			rop->r->alloc.usage--;
			if(rop->r->alloc.usage==0)
				mpwl_free(rop->r);
			rop->r = zero;
			zero->alloc.usage++;
		}
	} else if(i==1) {
		if(rop->r != one) {
			rop->r->alloc.usage--;
			if(rop->r->alloc.usage==0)
				mpwl_free(rop->r);
			rop->r = one;
			one->alloc.usage++;
		}
	} else {
		MAKE_COPY(rop->r);
		mpwl_set_si(rop->r,i);
	}
}

void
mpw_set_ui(mpw_ptr rop,unsigned long int i)
{
	MAKE_REAL(rop);
	if(i==0) {
		if(rop->r != zero) {
			rop->r->alloc.usage--;
			if(rop->r->alloc.usage==0)
				mpwl_free(rop->r);
			rop->r = zero;
			zero->alloc.usage++;
		}
	} else if(i==1) {
		if(rop->r != one) {
			rop->r->alloc.usage--;
			if(rop->r->alloc.usage==0)
				mpwl_free(rop->r);
			rop->r = one;
			one->alloc.usage++;
		}
	} else {
		MAKE_COPY(rop->r);
		mpwl_set_ui(rop->r,i);
	}
}

int
mpw_sgn(mpw_ptr op)
{
	if(op->type==MPW_REAL) {
		return mpwl_sgn(op->r);
	} else {
		(*errorout)("Can't compare complex numbers");
		error_num=NUMERICAL_MPW_ERROR;
	}
	return 0;
}

void
mpw_abs(mpw_ptr rop,mpw_ptr op)
{
	if(op->type==MPW_REAL) {
		if(mpwl_sgn(op->r)<0)
			mpw_neg(rop,op);
		else
			mpw_set(rop,op);
	} else {
		(*errorout)("Can't make complex numbers absolute");
		error_num=NUMERICAL_MPW_ERROR;
	}
}

void
mpw_neg(mpw_ptr rop,mpw_ptr op)
{
	MAKE_COPY(rop->r);
	mpwl_neg(rop->r,op->r);
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
	} else {
		MAKE_COPY(rop->i);
		mpwl_neg(rop->i,op->i);
	}
	rop->type = op->type;
}

void
mpw_add(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_add(rop->r,op1->r,op2->r);
	} else {
		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);
		rop->type = MPW_COMPLEX;
		mpwl_add(rop->r,op1->r,op2->r);
		mpwl_add(rop->i,op1->i,op2->i);

		mpw_uncomplex(rop);
	}
}

void
mpw_sub(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_sub(rop->r,op1->r,op2->r);
	} else {
		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);
		rop->type = MPW_COMPLEX;
		mpwl_sub(rop->r,op1->r,op2->r);
		mpwl_sub(rop->i,op1->i,op2->i);

		mpw_uncomplex(rop);
	}
}

void
mpw_mul(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_mul(rop->r,op1->r,op2->r);
	} else {
		MpwRealNum tr={0};
		MpwRealNum ti={0};
		
		rop->type = MPW_COMPLEX;

		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);

		mpwl_mul(rop->r,op1->r,op2->r);
		mpwl_mul(rop->i,op1->i,op2->r);

		mpwl_init_type(&tr,op1->i->type);
		mpwl_init_type(&ti,op1->r->type);

		/* tmp->type = MPW_COMPLEX; */
		mpwl_mul(&tr,op1->i,op2->i);
		mpwl_neg(&tr,&tr);
		mpwl_mul(&ti,op1->r,op2->i);
		
		mpwl_add(rop->r,rop->r,&tr);
		mpwl_add(rop->i,rop->i,&ti);

		mpwl_clear(&tr);
		mpwl_clear(&ti);

		mpw_uncomplex(rop);
	}
}

void
mpw_mul_ui(mpw_ptr rop,mpw_ptr op, unsigned int i)
{
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_mul_ui(rop->r,op->r,i);
	} else {
		rop->type = MPW_COMPLEX;
		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);
		mpwl_mul_ui(rop->r,op->r,i);
		mpwl_mul_ui(rop->i,op->i,i);

		mpw_uncomplex(rop);
	}
}

void
mpw_div(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		if(mpwl_sgn(op2->r)==0) {
			(*errorout)("Division by zero!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_div(rop->r,op1->r,op2->r);
	} else {
		MpwRealNum t1={0};
		MpwRealNum t2={0};
		if(mpwl_sgn(op2->r)==0 && mpwl_sgn(op2->i)==0) {
			(*errorout)("Division by zero!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		rop->type = MPW_COMPLEX;

		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);

		mpwl_init_type(&t1,MPW_INTEGER);
		mpwl_init_type(&t2,MPW_INTEGER);

		/*real part (r1r2 + i1i2)/(r2r2 + i2i2)*/
		mpwl_mul(rop->r,op1->r,op2->r);
		mpwl_mul(&t1,op1->i,op2->i);
		mpwl_add(rop->r,rop->r,&t1);

		mpwl_mul(&t1,op2->r,op2->r);
		mpwl_mul(&t2,op2->i,op2->i);
		mpwl_add(&t2,&t2,&t1);

		mpwl_div(rop->r,rop->r,&t2);


		/*imaginary part (i1r2 - r1i2)/(r2r2 + i2i2)*/
		mpwl_mul(rop->i,op1->i,op2->r);
		mpwl_mul(&t1,op1->r,op2->i);
		mpwl_neg(&t1,&t1);
		mpwl_add(rop->i,rop->i,&t1);

		/*t2 is calculated above*/

		mpwl_div(rop->i,rop->i,&t2);

		mpwl_clear(&t1);
		mpwl_clear(&t2);

		mpw_uncomplex(rop);
	}
}

void
mpw_div_ui(mpw_ptr rop,mpw_ptr op, unsigned int i)
{
	if(i==0) {
		(*errorout)("Division by zero!");
		error_num=NUMERICAL_MPW_ERROR;
		return;
	}
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_div_ui(rop->r,op->r,i);
	} else {
		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);
		mpwl_div_ui(rop->r,op->r,i);
		mpwl_div_ui(rop->i,op->i,i);

		mpw_uncomplex(rop);
	}
}

void
mpw_ui_div(mpw_ptr rop,unsigned int i,mpw_ptr op)
{
	if(op->type==MPW_REAL) {
		if(mpwl_sgn(op->r)==0) {
			(*errorout)("Division by zero!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_ui_div(rop->r,i,op->r);
	} else {
		MpwRealNum t1={0};
		MpwRealNum t2={0};
		if(mpwl_sgn(op->r)==0 && mpwl_sgn(op->i)==0) {
			(*errorout)("Division by zero!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		rop->type = MPW_COMPLEX;

		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);

		mpwl_init_type(&t1,MPW_NATIVEINT);
		mpwl_init_type(&t2,MPW_NATIVEINT);

		/*real part (r1r2)/(r2r2 + i2i2)*/
		mpwl_mul_ui(rop->r,op->r,i);

		mpwl_mul(&t1,op->r,op->r);
		mpwl_mul(&t2,op->i,op->i);
		mpwl_add(&t2,&t2,&t1);

		mpwl_div(rop->r,rop->r,&t2);


		/*imaginary part (- r1i2)/(r2r2 + i2i2)*/
		mpwl_mul_ui(rop->i,op->i,i);
		mpwl_neg(rop->i,rop->i);

		/*t2 is calculated above*/

		mpwl_div(rop->i,rop->i,&t2);

		mpwl_clear(&t1);
		mpwl_clear(&t2);

		mpw_uncomplex(rop);
	}
}

void
mpw_mod(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		if(mpwl_sgn(op2->r)==0) {
			(*errorout)("Division by zero!");
			error_num=NUMERICAL_MPW_ERROR;
			return;
		}
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_mod(rop->r,op1->r,op2->r);
	} else {
		error_num=NUMERICAL_MPW_ERROR;
		(*errorout)("Can't modulo complex numbers");
	}
}

void
mpw_gcd(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_gcd(rop->r,op1->r,op2->r);
	} else {
		error_num=NUMERICAL_MPW_ERROR;
		(*errorout)("Can't GCD complex numbers");
	}
}

void
mpw_pow(mpw_ptr rop,mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		if(mpwl_pow(rop->r,op1->r,op2->r)) {
			g_warning("complex numbers unsupported yet");
			error_num=NUMERICAL_MPW_ERROR;
		}
	} else {
		/*FIXME: complex numbers*/
		(*errorout)("pow: can't deal with complex numbers");
	}
}

void
mpw_sqrt(mpw_ptr rop,mpw_ptr op)
{
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		if(mpwl_sqrt(rop->r,op->r)) {
			MpwRealNum *t;
			t = rop->r;
			rop->r = rop->i;
			rop->i = t;
			rop->type=MPW_COMPLEX;
		}
	} else {
		/*FIXME: complex numbers*/
		(*errorout)("sqrt: can't deal with complex numbers");
	}
}

void
mpw_pow_ui(mpw_ptr rop,mpw_ptr op, unsigned long int e)
{
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_pow_ui(rop->r,op->r,e,FALSE);
	} else {
		/*FIXME: complex numbers*/
		(*errorout)("pow_ui: can't deal with complex numbers");
	}
}

int
mpw_cmp(mpw_ptr op1, mpw_ptr op2)
{
	if(op1->type==MPW_REAL && op2->type==MPW_REAL) {
		return mpwl_cmp(op1->r,op2->r);
	} else {
		(*errorout)("Can't compare complex numbers");
		error_num=NUMERICAL_MPW_ERROR;
		return 0;
	}
}

int
mpw_cmp_ui(mpw_ptr op, unsigned long int i)
{
	if(op->type==MPW_REAL) {
		return mpwl_cmp_ui(op->r,i);
	} else {
		(*errorout)("Can't compare complex numbers");
		error_num=NUMERICAL_MPW_ERROR;
		return 0;
	}
}

int
mpw_eql(mpw_ptr op1, mpw_ptr op2)
{
	return (mpwl_cmp(op1->r,op2->r)==0 && mpwl_cmp(op1->i,op2->i)==0);
}

int
mpw_eql_ui(mpw_ptr op, unsigned long int i)
{
	if(op->type==MPW_REAL) {
		return mpwl_cmp_ui(op->r,i);
	} else {
		return FALSE;
	}
}

void
mpw_fac_ui(mpw_ptr rop,unsigned long int i)
{
	MAKE_REAL(rop);
	MAKE_COPY(rop->r);
	mpwl_fac_ui(rop->r,i);
}

void
mpw_fac(mpw_ptr rop,mpw_ptr op)
{
	if(op->type==MPW_REAL) {
		MAKE_REAL(rop);
		MAKE_COPY(rop->r);
		mpwl_fac(rop->r,op->r);
	} else {
		(*errorout)("Can't make factorials of complex numbers");
		error_num=NUMERICAL_MPW_ERROR;
	}
}

/*make a number int if possible, if floats is true try to convert to an
  int if the two equal*/
void
mpw_make_int(mpw_ptr rop,int floats)
{
	if(rop->type==MPW_REAL) {
		/*floats might be destructive so make
		  us a copy*/
		if(floats)
			MAKE_COPY(rop->r);
		mpwl_make_int(rop->r,floats);
	} else {
		/*floats might be destructive so make
		  us a copy*/
		if(floats) {
			MAKE_COPY(rop->r);
			MAKE_COPY(rop->i);
		}
		mpwl_make_int(rop->r,floats);
		mpwl_make_int(rop->i,floats);
	}
}

/*make number into a float, this might be neccessary for unprecise
  calculations*/
void
mpw_make_float(mpw_ptr rop)
{
	if(rop->type==MPW_REAL) {
		MAKE_COPY(rop->r);
		mpwl_make_float(rop->r);
	} else {
		MAKE_COPY(rop->r);
		MAKE_COPY(rop->i);
		mpwl_make_float(rop->r);
		mpwl_make_float(rop->i);
	}
}

/*init the mp stuff*/
void
mpw_init_mp(void)
{
	mp_set_memory_functions(my_malloc,my_realloc,my_free);
	GET_NEW_REAL(zero);
	mpwl_init_type(zero,MPW_NATIVEINT);
	mpwl_set_ui(zero,0);
	zero->alloc.usage = 1;
	GET_NEW_REAL(one);
	mpwl_init_type(one,MPW_NATIVEINT);
	mpwl_set_ui(one,1);
	one->alloc.usage = 1;
}

char *
mpw_getstring(mpw_ptr num, int max_digits,int scientific_notation,
	      int results_as_floats)
{
	mpw_uncomplex(num);
	if(num->type==MPW_REAL) {
		return mpwl_getstring(num->r,max_digits,scientific_notation,
			results_as_floats);
	} else {
		char *p1,*p2,*r;
		p1 = mpwl_getstring(num->r,max_digits,scientific_notation,
			results_as_floats);
		p2 = mpwl_getstring(num->i,max_digits,scientific_notation,
			results_as_floats);
		r = g_strconcat("(",p1,"+",p2,"i)",NULL);
		g_free(p1);
		g_free(p2);
		return r;
	}
}

void
mpw_set_str_float(mpw_ptr rop,char *s,int base)
{
	MAKE_REAL(rop);
	MAKE_COPY(rop->r);
	mpwl_set_str_float(rop->r,s,base);
}

void
mpw_set_str_int(mpw_ptr rop,char *s,int base)
{
	MAKE_REAL(rop);
	MAKE_COPY(rop->r);
	mpwl_set_str_int(rop->r,s,base);
}

void
mpw_set_str_complex(mpw_ptr rop,char *s,int base)
{
	char *p;
	int size;

	rop->type = MPW_COMPLEX;

	p = g_strdup(s);
	size = strlen(p);
	if(p[size-1] == 'i')
		p[size-1] = '\0';
	MAKE_COPY(rop->i);
	mpwl_set_str_float(rop->i,p,base);

	g_free(p);

	mpw_uncomplex(rop);
}

int
mpw_is_complex(mpw_ptr op)
{
	mpw_uncomplex(op);
	return op->type == MPW_COMPLEX;
}

int
mpw_is_integer(mpw_ptr op)
{
	if(op->type == MPW_COMPLEX) {
		(*errorout)("Can't determine type of a complex number");
		error_num=NUMERICAL_MPW_ERROR;
		return FALSE;
	}
	return op->r->type == MPW_INTEGER;
}

int
mpw_is_rational(mpw_ptr op)
{
	if(op->type == MPW_COMPLEX) {
		(*errorout)("Can't determine type of a complex number");
		error_num=NUMERICAL_MPW_ERROR;
		return FALSE;
	}
	return op->r->type == MPW_RATIONAL;
}

int
mpw_is_float(mpw_ptr op)
{
	if(op->type == MPW_COMPLEX) {
		(*errorout)("Can't determine type of a complex number");
		error_num=NUMERICAL_MPW_ERROR;
		return FALSE;
	}
	return op->r->type == MPW_FLOAT;
}

void
mpw_im(mpw_ptr rop, mpw_ptr op)
{
	MAKE_REAL(rop);
	rop->r=op->i;
	op->i->alloc.usage++;
}

void
mpw_re(mpw_ptr rop, mpw_ptr op)
{
	MAKE_REAL(rop);
	rop->r=op->r;
	op->r->alloc.usage++;
}

void
mpw_round(mpw_ptr rop, mpw_ptr op)
{
	mpw_set(rop,op);
	MAKE_COPY(rop->r);
	mpwl_round(rop->r);
	if(op->type==MPW_COMPLEX) {
		MAKE_COPY(rop->i);
		mpwl_round(rop->i);
		mpw_uncomplex(rop);
	}
}

void
mpw_floor(mpw_ptr rop, mpw_ptr op)
{
	mpw_set(rop,op);
	MAKE_COPY(rop->r);
	mpwl_floor(rop->r);
	if(op->type==MPW_COMPLEX) {
		MAKE_COPY(rop->i);
		mpwl_floor(rop->i);
		mpw_uncomplex(rop);
	}
}

void
mpw_ceil(mpw_ptr rop, mpw_ptr op)
{
	mpw_set(rop,op);
	MAKE_COPY(rop->r);
	mpwl_ceil(rop->r);
	if(op->type==MPW_COMPLEX) {
		MAKE_COPY(rop->i);
		mpwl_ceil(rop->i);
		mpw_uncomplex(rop);
	}
}

void
mpw_trunc(mpw_ptr rop, mpw_ptr op)
{
	mpw_set(rop,op);
	MAKE_COPY(rop->r);
	mpwl_trunc(rop->r);
	if(op->type==MPW_COMPLEX) {
		MAKE_COPY(rop->i);
		mpwl_trunc(rop->i);
		mpw_uncomplex(rop);
	}
}
