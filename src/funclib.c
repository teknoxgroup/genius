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
#include <libintl.h>
#define _(x) gettext(x)
#endif

#include <string.h>
#include <math.h>
#include <glib.h>
#include <limits.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "funclib.h"
#include "calc.h"
#include "matrix.h"
#include "matrixw.h"
#include "matop.h"

extern void (*errorout)(char *);
extern void (*infoout)(char *);
extern calc_error_t error_num;
extern int got_eof;
extern calcstate_t calcstate;

EFunc *_internal_ln_function = NULL;
EFunc *_internal_exp_function = NULL;

/*maximum number of primes to precalculate and store*/
#define MAXPRIMES 100000
GArray *primes = NULL;
int numprimes = 0;

static mpw_t e_cache;
static int e_iscached = FALSE;

void
break_fp_caches(void)
{
	if(e_iscached) {
		e_iscached = FALSE;
		mpw_clear(e_cache);
	}
}

extern ETree *free_trees;

static ETree *
warranty_op(ETree * * a, int *exception)
{
	char *p;
	p = g_strdup_printf(_("Genius %s\n"
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
		    "    USA.\n"),VERSION);
	(*infoout)(p);
	g_free(p);
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
	(*infoout)("We are the Knights Who Say... Ni!");
	if(exception) *exception = TRUE; /*raise exception*/
	error_num = IGNORE_ERROR;
	return NULL;
}

static ETree *
shrubbery_op(ETree * * a, int *exception)
{
	(*infoout)("Then, when you have found the shrubbery, you must\n"
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
		(*errorout)(a[0]->str.str);
	else {
		GString *gs;
		gs = g_string_new(NULL);
		pretty_print_etree(gs,NULL,a[0],1);
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
		fprintf(outputfp,"%s\n",a[0]->str.str);
	else {
		pretty_print_etree(NULL,outputfp,a[0],1);
		fprintf(outputfp,"\n");
	}
	return makenum_null();
}
/*print function*/
static ETree *
printn_op(ETree * * a, int *exception)
{
	if(a[0]->type==STRING_NODE)
		fprintf(outputfp,"%s",a[0]->str.str);
	else
		print_etree(NULL,outputfp,a[0],1);
	fflush(outputfp);
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
	fprintf(outputfp,"%s: ",a[0]->str.str);
	pretty_print_etree(NULL,outputfp,a[1],1);
	fprintf(outputfp,"\n");
	return makenum_null();
}

static ETree *
apply_func_to_matrixen(ETree *mat1, ETree *mat2,
		       ETree * (*function)(ETree **a,int *exception),
		       char *ident)
{
	MatrixW *m1 = NULL;
	MatrixW *m2 = NULL;
	MatrixW *new;
	ETree *re_node = NULL;
	int reverse = FALSE;
	ETree *n;
	int i,j;
	int quote = 0;

	if(mat1->type == MATRIX_NODE &&
	   mat2->type == MATRIX_NODE) {
		m1 = mat1->mat.matrix;
		m2 = mat2->mat.matrix;
		quote = mat1->mat.quoted || mat2->mat.quoted;
	} else if(mat1->type == MATRIX_NODE) {
		m1 = mat1->mat.matrix;
		quote = mat1->mat.quoted;
		re_node = mat2;
	} else /*if(mat2->type == MATRIX_NODE)*/ {
		m1 = mat2->mat.matrix;
		quote = mat2->mat.quoted;
		re_node = mat1;
		reverse = TRUE;
	}
	
	if(m2 && (matrixw_width(m1) != matrixw_width(m2) ||
		  matrixw_height(m1) != matrixw_height(m2))) {
		(*errorout)(_("Cannot apply function to two differently sized matrixes"));
		return NULL;
	}
	
	/*make us a new empty node*/
	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	new = n->mat.matrix = matrixw_new();
	n->mat.quoted = quote;
	matrixw_set_size(new,matrixw_width(m1),matrixw_height(m1));

	for(i=0;i<matrixw_width(m1);i++) {
		for(j=0;j<matrixw_height(m1);j++) {
			ETree *t[2];
			ETree *e;
			int ex = FALSE;
			if(!reverse) {
				t[0] = matrixw_index(m1,i,j);
				t[1] = m2?matrixw_index(m2,i,j):re_node;
			} else {
				t[0] = m2?matrixw_index(m2,i,j):re_node;
				t[1] = matrixw_index(m1,i,j);
			}
			e = (*function)(t,&ex);
			/*FIXME: handle exceptions*/
			if(!e) {
				ETree *nn;
				ETree *ni;
				GET_NEW_NODE(nn);
				nn->type = OPERATOR_NODE;
				nn->op.oper = E_CALL;
				nn->op.args = NULL;
				nn->op.nargs = 3;
				
				GET_NEW_NODE(ni);
				ni->type = IDENTIFIER_NODE;
				ni->id.id = d_intern(ident);
				
				nn->op.args = g_list_prepend(nn->op.args,copynode(t[1]));
				nn->op.args = g_list_prepend(nn->op.args,copynode(t[0]));
				nn->op.args = g_list_prepend(nn->op.args,ni);

				matrixw_set_index(new,i,j) = nn;
			} else {
				matrixw_set_index(new,i,j) = e;
			}
		}
	}
	return n;
}

static ETree *
apply_func_to_matrix(ETree *mat, 
		     ETree * (*function)(ETree **a,int *exception),
		     char *ident)
{
	MatrixW *m;
	MatrixW *new;
	ETree *n;
	int i,j;

	m = mat->mat.matrix;
	
	/*make us a new empty node*/
	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	new = n->mat.matrix = matrixw_new();
	n->mat.quoted = mat->mat.quoted;
	matrixw_set_size(new,matrixw_width(m),matrixw_height(m));

	for(i=0;i<matrixw_width(m);i++) {
		for(j=0;j<matrixw_height(m);j++) {
			ETree *t[1];
			ETree *e;
			int ex = FALSE;
			t[0] = matrixw_index(m,i,j);
			e = (*function)(t,&ex);
			/*FIXME: handle exceptions*/
			if(!e) {
				ETree *nn;
				ETree *ni;
				GET_NEW_NODE(nn);
				nn->type = OPERATOR_NODE;
				nn->op.oper = E_CALL;
				nn->op.args = NULL;
				nn->op.nargs = 2;
				
				GET_NEW_NODE(ni);
				ni->type = IDENTIFIER_NODE;
				ni->id.id = d_intern(ident);
				
				nn->op.args = g_list_prepend(nn->op.args,copynode(t[0]));
				nn->op.args = g_list_prepend(nn->op.args,ni);

				matrixw_set_index(new,i,j) = nn;
			} else {
				matrixw_set_index(new,i,j) = e;
			}
		}
	}
	return n;
}

/*sin function*/
static ETree *
sin_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],sin_op,"sin");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("sin: argument not a number"));
		return NULL;
	}

	mpw_init(fr);

	mpw_sin(fr,a[0]->val.value);

	return makenum_use(fr);
}

/*sinh function*/
static ETree *
sinh_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],sinh_op,"sinh");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("sinh: argument not a number"));
		return NULL;
	}

	mpw_init(fr);

	mpw_sinh(fr,a[0]->val.value);

	return makenum_use(fr);
}

/*cos function*/
static ETree *
cos_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],cos_op,"cos");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("cos: argument not a number"));
		return NULL;
	}

	mpw_init(fr);

	mpw_cos(fr,a[0]->val.value);

	return makenum_use(fr);
}

/*cosh function*/
static ETree *
cosh_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],cosh_op,"cosh");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("cosh: argument not a number"));
		return NULL;
	}

	mpw_init(fr);

	mpw_cosh(fr,a[0]->val.value);

	return makenum_use(fr);
}

/*tan function*/
static ETree *
tan_op(ETree * * a, int *exception)
{
	mpw_t fr;
	mpw_t fr2;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],tan_op,"tan");

	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->val.value)) {
		(*errorout)(_("tan: argument not a real number"));
		return NULL;
	}

	mpw_init(fr);
	mpw_set(fr,a[0]->val.value);

	/*is this algorithm always precise??? sin/cos*/
	mpw_init(fr2);
	mpw_cos(fr2,fr);
	mpw_sin(fr,fr);
	mpw_div(fr,fr,fr2);
	mpw_clear(fr2);

	return makenum_use(fr);
}

/*atan (arctan) function*/
static ETree *
atan_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],atan_op,"atan");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("atan: argument not a number"));
		return NULL;
	}

	mpw_init(fr);

	mpw_arctan(fr,a[0]->val.value);

	return makenum_use(fr);
}
	

/*e function (or e variable actually)*/
static ETree *
e_op(ETree * * a, int *exception)
{
	if(e_iscached)
		return makenum(e_cache);

	mpw_init(e_cache);
	mpw_set_ui(e_cache,1);
	mpw_exp(e_cache,e_cache);
	e_iscached = TRUE;
	return makenum(e_cache);
}

/*pi function (or pi variable or whatever)*/
static ETree *
pi_op(ETree * * a, int *exception)
{
	mpw_t fr;

	mpw_init(fr);
	mpw_pi(fr);

	return makenum_use(fr);
}

static ETree *
is_null_op(ETree * * a, int *exception)
{
	if(a[0]->type==NULL_NODE)
		return makenum_ui(1);
	else
		return makenum_ui(0);
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
is_function_op(ETree * * a, int *exception)
{
	if(a[0]->type==FUNCTION_NODE)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_function_ref_op(ETree * * a, int *exception)
{
	if(a[0]->type==OPERATOR_NODE &&
	   a[0]->op.oper == E_REFERENCE) {
		ETree *arg = a[0]->op.args->data;
		g_assert(arg);
		if(arg->type==IDENTIFIER_NODE &&
		   d_lookup_global(arg->id.id))
			return makenum_ui(1);
	}
	return makenum_ui(0);
}
static ETree *
is_complex_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE)
		return makenum_ui(0);
	else if(mpw_is_complex(a[0]->val.value))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_real_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE)
		return makenum_ui(0);
	else if(mpw_is_complex(a[0]->val.value))
		return makenum_ui(0);
	else
		return makenum_ui(1);
}
static ETree *
is_integer_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->val.value))
		return makenum_ui(0);
	else if(mpw_is_integer(a[0]->val.value))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_rational_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->val.value))
		return makenum_ui(0);
	else if(mpw_is_rational(a[0]->val.value))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
is_float_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE ||
	   mpw_is_complex(a[0]->val.value))
		return makenum_ui(0);
	else if(mpw_is_float(a[0]->val.value))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
trunc_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],trunc_op,"trunc");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("trunc: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_trunc(fr,a[0]->val.value);
	return makenum_use(fr);
}
static ETree *
floor_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],floor_op,"floor");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("floor: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_floor(fr,a[0]->val.value);
	return makenum_use(fr);
}
static ETree *
ceil_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],ceil_op,"ceil");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("ceil: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_ceil(fr,a[0]->val.value);
	return makenum_use(fr);
}
static ETree *
round_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],round_op,"round");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("round: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_round(fr,a[0]->val.value);
	return makenum_use(fr);
}
static ETree *
float_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],float_op,"float");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("float: argument not a number"));
		return NULL;
	}
	mpw_init_set(fr,a[0]->val.value);
	mpw_make_float(fr);
	return makenum_use(fr);
}

static ETree *
Re_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],Re_op,"Re");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("Re: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_re(fr,a[0]->val.value);
	return makenum_use(fr);
}

static ETree *
Im_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],Im_op,"Im");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("Im: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_im(fr,a[0]->val.value);
	return makenum_use(fr);
}

static ETree *
sqrt_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],sqrt_op,"sqrt");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("sqrt: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_sqrt(fr,a[0]->val.value);
	return makenum_use(fr);
}

static ETree *
exp_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE) {
		if(matrixw_width(a[0]->mat.matrix) !=
		   matrixw_height(a[0]->mat.matrix)) {
			(*errorout)(_("exp: matrix argument is not square"));
			return NULL;
		}
		return funccall(_internal_exp_function,a,1);
	}

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("exp: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_exp(fr,a[0]->val.value);
	return makenum_use(fr);
}

static ETree *
ln_op(ETree * * a, int *exception)
{
	mpw_t fr;

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],ln_op,"ln");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("ln: argument not a number"));
		return NULL;
	}
	mpw_init(fr);
	mpw_ln(fr,a[0]->val.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(fr);
		return NULL;
	}
	return makenum_use(fr);
}

/*gcd function*/
static ETree *
gcd_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],gcd_op,"gcd");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("gcd: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_gcd(tmp,
		a[0]->val.value,
		a[1]->val.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}

	return makenum_use(tmp);
}

/*lcm function*/
static ETree *
lcm_op(ETree * * a, int *exception)
{
	mpw_t tmp;
	mpw_t prod;

	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],lcm_op,"lcm");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("lcm: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_gcd(tmp,
		a[0]->val.value,
		a[1]->val.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}
	mpw_init(prod);
	mpw_mul(prod,
		a[0]->val.value,
		a[1]->val.value);
	mpw_div(tmp,prod,tmp);
	mpw_clear(prod);

	return makenum_use(tmp);
}

/*jacobi function*/
static ETree *
jacobi_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],jacobi_op,"jacobi");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("jacobi: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_jacobi(tmp,
		   a[0]->val.value,
		   a[1]->val.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}

	return makenum_use(tmp);
}

/*legendre function*/
static ETree *
legendre_op(ETree * * a, int *exception)
{
	mpw_t tmp;

	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],legendre_op,"legendre");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("legendre: arguments must be numbers"));
		return NULL;
	}

	mpw_init(tmp);
	mpw_legendre(tmp,
		     a[0]->val.value,
		     a[1]->val.value);
	if(error_num) {
		error_num = 0;
		mpw_clear(tmp);
		return NULL;
	}

	return makenum_use(tmp);
}

/*perfect square testing function*/
static ETree *
perfect_square_op(ETree * * a, int *exception)
{
	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],perfect_square_op,"perfect_square");

	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("perfect_square: argument must be a number"));
		return NULL;
	}

	if(mpw_perfect_square(a[0]->val.value)) {
		return makenum_ui(1);
	} else {
		if(error_num) {
			error_num = 0;
			return NULL;
		}
		return makenum_ui(0);
	}
}

/*max function*/
static ETree *
max_op(ETree * * a, int *exception)
{
	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],max_op,"max");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("max: arguments must be numbers"));
		return NULL;
	}

	if(mpw_cmp(a[0]->val.value,a[1]->val.value)>0)
		return makenum(a[0]->val.value);
	else {
		if(error_num) {
			error_num = 0;
			return NULL;
		}
		return makenum(a[1]->val.value);
	}
}

/*min function*/
static ETree *
min_op(ETree * * a, int *exception)
{
	if(a[0]->type==MATRIX_NODE ||
	   a[1]->type==MATRIX_NODE)
		return apply_func_to_matrixen(a[0],a[1],min_op,"min");

	if(a[0]->type!=VALUE_NODE ||
	   a[1]->type!=VALUE_NODE) {
		(*errorout)(_("min: arguments must be numbers"));
		return NULL;
	}

	if(mpw_cmp(a[0]->val.value,a[1]->val.value)>0)
		return makenum(a[1]->val.value);
	else {
		if(error_num) {
			error_num = 0;
			return NULL;
		}
		return makenum(a[0]->val.value);
	}
}

static ETree *
is_value_only_op(ETree * * a, int *exception)
{
	if(a[0]->type!=MATRIX_NODE) {
		(*errorout)(_("is_value_only: argument not a matrix"));
		return NULL;
	}
	
	if(is_matrix_value_only(a[0]->mat.matrix))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
I_op(ETree * * a, int *exception)
{
	ETree *n;
	long size;
	int i,j;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->val.value)) {
		(*errorout)(_("I: argument not an integer"));
		return NULL;
	}

	size = mpw_get_long(a[0]->val.value);
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
	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = matrixw_new();
	n->mat.quoted = 0;
	matrixw_set_size(n->mat.matrix,size,size);
	
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			if(i==j)
				matrixw_set_index(n->mat.matrix,i,j) =
					makenum_ui(1);

	return n;
}

static ETree *
rows_op(ETree * * a, int *exception)
{
	if(a[0]->type!=MATRIX_NODE) {
		(*errorout)(_("rows: argument not a matrix"));
		return NULL;
	}
	return makenum_ui(matrixw_height(a[0]->mat.matrix));
}
static ETree *
columns_op(ETree * * a, int *exception)
{
	if(a[0]->type!=MATRIX_NODE) {
		(*errorout)(_("columns: argument not a matrix"));
		return NULL;
	}
	return makenum_ui(matrixw_width(a[0]->mat.matrix));
}
static ETree *
set_size_op(ETree * * a, int *exception)
{
	ETree *n;
	long w,h;
	if(a[0]->type!=MATRIX_NODE ||
	   a[1]->type!=VALUE_NODE ||
	   a[2]->type!=VALUE_NODE) {
		(*errorout)(_("set_size: wrong argument type"));
		return NULL;
	}
	w = mpw_get_long(a[1]->val.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	h = mpw_get_long(a[2]->val.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(w<=0 || h<=0) {
		(*errorout)(_("set_size: rows/columns negative or 0"));
		return NULL;
	}
	if(w>INT_MAX || h>INT_MAX) {
		(*errorout)(_("set_size: rows/columns too large"));
		return NULL;
	}
	n = copynode(a[0]);
	matrixw_set_size(n->mat.matrix,h,w);
	return n;
}

static ETree *
det_op(ETree * * a, int *exception)
{
	mpw_t ret;
	if(a[0]->type!=MATRIX_NODE ||
	   !is_matrix_value_only(a[0]->mat.matrix)) {
		(*errorout)(_("det: argument not a value only matrix"));
		return NULL;
	}
	mpw_init(ret);
	if(!value_matrix_det(ret,a[0]->mat.matrix)) {
		mpw_clear(ret);
		return NULL;
	}
	return makenum_use(ret);
}
static ETree *
ref_op(ETree * * a, int *exception)
{
	ETree *n;
	if(a[0]->type!=MATRIX_NODE ||
	   !is_matrix_value_only(a[0]->mat.matrix)) {
		(*errorout)(_("ref: argument not a value only matrix"));
		return NULL;
	}

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix =
		value_matrix_gauss(a[0]->mat.matrix,FALSE,FALSE,FALSE,NULL,NULL);
	n->mat.quoted = 0;
	return n;
}
static ETree *
rref_op(ETree * * a, int *exception)
{
	ETree *n;
	if(a[0]->type!=MATRIX_NODE ||
	   !is_matrix_value_only(a[0]->mat.matrix)) {
		(*errorout)(_("rref: argument not a value only matrix"));
		return NULL;
	}

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix =
		value_matrix_gauss(a[0]->mat.matrix,TRUE,FALSE,FALSE,NULL,NULL);
	n->mat.quoted = 0;
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

	if(a[0]->type==MATRIX_NODE)
		return apply_func_to_matrix(a[0],prime_op,"prime");

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->val.value)) {
		(*errorout)(_("prime: argument not an integer"));
		return NULL;
	}

	num = mpw_get_long(a[0]->val.value);
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
		primes = g_array_append_val(primes,b);
		b = 3;
		primes = g_array_append_val(primes,b);
		b = 5;
		primes = g_array_append_val(primes,b);
		b = 7;
		primes = g_array_append_val(primes,b);
		numprimes = 4;
	}
	
	if(num-1 < numprimes)
		return makenum_ui(g_array_index(primes,unsigned long,num-1));
	

	primes = g_array_set_size(primes,num);
	for(i=g_array_index(primes,unsigned long,numprimes-1)+1;
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

static void
poly_cut_zeros(MatrixW *m)
{
	int i;
	int cutoff;
	for(i=matrixw_width(m)-1;i>=1;i--) {
		ETree *t = matrixw_index(m,i,0);
	       	if(mpw_sgn(t->val.value)!=0)
			break;
	}
	cutoff = i+1;
	if(cutoff==matrixw_width(m))
		return;
	matrixw_set_size(m,cutoff,1);
}

static int
check_poly(ETree * *a, int args, char *func, int complain)
{
	int i,j;

	for(j=0;j<args;j++) {
		if(a[j]->type!=MATRIX_NODE ||
		   matrixw_height(a[j]->mat.matrix)!=1) {
			char buf[256];
			if(!complain) return FALSE;
			g_snprintf(buf,256,_("%s: arguments not horizontal vectors"),func);
			(*errorout)(buf);
			return FALSE;
		}

		for(i=0;i<matrixw_width(a[j]->mat.matrix);i++) {
			ETree *t = matrixw_index(a[j]->mat.matrix,i,0);
			if(t->type != VALUE_NODE) {
				char buf[256];
				if(!complain) return FALSE;
				g_snprintf(buf,256,_("%s: arguments not numeric only vectors"),func);
				(*errorout)(buf);
				return FALSE;
			}
		}
	}
	return TRUE;
}

static ETree *
addpoly_op(ETree * * a, int *exception)
{
	ETree *n;
	long size;
	int i;
	MatrixW *m1,*m2,*mn;
	
	if(!check_poly(a,2,"addpoly",TRUE))
		return NULL;

	m1 = a[0]->mat.matrix;
	m2 = a[1]->mat.matrix;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = mn = matrixw_new();
	n->mat.quoted = 0;
	size = MAX(matrixw_width(m1), matrixw_width(m2));
	matrixw_set_size(mn,size,1);
	
	for(i=0;i<size;i++) {
		if(i<matrixw_width(m1) &&
		   i<matrixw_width(m2)) {
			ETree *l,*r;
			mpw_t t;
			mpw_init(t);
			l = matrixw_index(m1,i,0);
			r = matrixw_index(m2,i,0);
			mpw_add(t,l->val.value,r->val.value);
			matrixw_set_index(mn,i,0) = makenum_use(t);
		} else if(i<matrixw_width(m1)) {
			matrixw_set_index(mn,i,0) =
				copynode(matrixw_set_index(m1,i,0));
		} else /*if(i<matrixw_width(m2)*/ {
			matrixw_set_index(mn,i,0) =
				copynode(matrixw_set_index(m2,i,0));
		}
	}
	
	poly_cut_zeros(mn);

	return n;
}

static ETree *
subpoly_op(ETree * * a, int *exception)
{
	ETree *n;
	long size;
	int i;
	MatrixW *m1,*m2,*mn;
	
	if(!check_poly(a,2,"subpoly",TRUE))
		return NULL;

	m1 = a[0]->mat.matrix;
	m2 = a[1]->mat.matrix;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = mn = matrixw_new();
	n->mat.quoted = 0;
	size = MAX(matrixw_width(m1), matrixw_width(m2));
	matrixw_set_size(mn,size,1);

	for(i=0;i<size;i++) {
		if(i<matrixw_width(m1) &&
		   i<matrixw_width(m2)) {
			ETree *l,*r;
			mpw_t t;
			mpw_init(t);
			l = matrixw_index(m1,i,0);
			r = matrixw_index(m2,i,0);
			mpw_sub(t,l->val.value,r->val.value);
			matrixw_set_index(mn,i,0) = makenum_use(t);
		} else if(i<matrixw_width(m1)) {
			matrixw_set_index(mn,i,0) =
				copynode(matrixw_set_index(m1,i,0));
		} else /*if(i<matrixw_width(m2))*/ {
			ETree *nn,*r;
			r = matrixw_index(m2,i,0);
			nn = makenum_ui(0);
			mpw_neg(nn->val.value,r->val.value);
			matrixw_set_index(mn,i,0) = nn;
		}
	}
	
	poly_cut_zeros(mn);

	return n;
}

static ETree *
mulpoly_op(ETree * * a, int *exception)
{
	ETree *n;
	long size;
	int i,j;
	mpw_t accu;
	MatrixW *m1,*m2,*mn;
	
	if(!check_poly(a,2,"mulpoly",TRUE))
		return NULL;
	m1 = a[0]->mat.matrix;
	m2 = a[1]->mat.matrix;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = mn = matrixw_new();
	n->mat.quoted = 0;
	size = matrixw_width(m1) + matrixw_width(m2);
	matrixw_set_size(mn,size,1);
	
	mpw_init(accu);
		
	for(i=0;i<matrixw_width(m1);i++) {
		for(j=0;j<matrixw_width(m2);j++) {
			ETree *l,*r,*nn;
			l = matrixw_index(m1,i,0);
			r = matrixw_index(m2,j,0);
			if(mpw_sgn(l->val.value)==0 ||
			   mpw_sgn(r->val.value)==0)
				continue;
			mpw_mul(accu,l->val.value,r->val.value);
			nn = matrixw_set_index(mn,i+j,0);
			if(nn)
				mpw_add(nn->val.value,nn->val.value,accu);
			else 
				matrixw_set_index(mn,i+j,0) =
					makenum(accu);
		}
	}

	mpw_clear(accu);
	
	poly_cut_zeros(mn);

	return n;
}

static ETree *
derpoly_op(ETree * * a, int *exception)
{
	ETree *n;
	int i;
	MatrixW *m,*mn;
	
	if(!check_poly(a,1,"derpoly",TRUE))
		return NULL;

	m = a[0]->mat.matrix;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = mn = matrixw_new();
	n->mat.quoted = 0;
	if(matrixw_width(m)==1) {
		matrixw_set_size(mn,1,1);
		return n;
	}
	matrixw_set_size(mn,matrixw_width(m)-1,1);
	
	for(i=1;i<matrixw_width(m);i++) {
		ETree *r;
		mpw_t t;
		mpw_init(t);
		r = matrixw_index(m,i,0);
		mpw_mul_ui(t,r->val.value,i);
		matrixw_set_index(mn,i-1,0) = makenum_use(t);
	}
	
	poly_cut_zeros(mn);

	return n;
}

static ETree *
der2poly_op(ETree * * a, int *exception)
{
	ETree *n;
	int i;
	MatrixW *m,*mn;
	
	if(!check_poly(a,1,"der2poly",TRUE))
		return NULL;

	m = a[0]->mat.matrix;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = mn = matrixw_new();
	n->mat.quoted = 0;
	if(matrixw_width(m)<=2) {
		matrixw_set_size(mn,1,1);
		return n;
	}
	matrixw_set_size(mn,matrixw_width(m)-2,1);
	
	for(i=2;i<matrixw_width(m);i++) {
		ETree *r;
		mpw_t t;
		r = matrixw_index(m,i,0);
		mpw_init(t);
		mpw_mul_ui(t,r->val.value,i*(i-1));
		matrixw_set_index(mn,i-2,0) = makenum_use(t);
	}
	
	poly_cut_zeros(mn);

	return n;
}

static ETree *
trimpoly_op(ETree * * a, int *exception)
{
	ETree *n;
	
	if(!check_poly(a,1,"trimpoly",TRUE))
		return NULL;

	GET_NEW_NODE(n);
	n->type = MATRIX_NODE;
	n->mat.matrix = matrixw_copy(a[0]->mat.matrix);
	n->mat.quoted = 0;
	
	poly_cut_zeros(n->mat.matrix);

	return n;
}

static ETree *
is_poly_op(ETree * * a, int *exception)
{
	if(check_poly(a,1,"is_poly",FALSE))
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
polytostring_op(ETree * * a, int *exception)
{
	ETree *n;
	int i;
	GString *gs;
	int any = FALSE;
	MatrixW *m;
	char *var;
	
	if(!check_poly(a,1,"polytostring",TRUE))
		return NULL;
	
	if(a[1]->type!=STRING_NODE) {
		(*errorout)(_("polytostring: 2nd argument not a string"));
		return NULL;
	}
	
	m = a[0]->mat.matrix;
	var = a[1]->str.str;
	
	gs = g_string_new("");

	for(i=matrixw_width(m)-1;i>=0;i--) {
		ETree *t;
		t = matrixw_index(m,i,0);
		if(mpw_sgn(t->val.value)==0)
			continue;
		/*positive*/
		if(mpw_sgn(t->val.value)>0) {
			if(any) g_string_append(gs," + ");
			if(i==0)
				print_etree(gs,NULL,t,1);
			else if(mpw_cmp_ui(t->val.value,1)!=0) {
				print_etree(gs,NULL,t,1);
				g_string_append_c(gs,'*');
			}
			/*negative*/
		} else {
			if(any) g_string_append(gs," - ");
			else g_string_append_c(gs,'-');
			mpw_neg(t->val.value,t->val.value);
			if(i==0)
				print_etree(gs,NULL,t,1);
			else if(mpw_cmp_ui(t->val.value,1)!=0) {
				print_etree(gs,NULL,t,1);
				g_string_append_c(gs,'*');
			}
			mpw_neg(t->val.value,t->val.value);
		}
		if(i==1)
			g_string_sprintfa(gs,"%s",var);
		else if(i>1)
			g_string_sprintfa(gs,"%s^%d",var,i);
		any = TRUE;
	}
	if(!any)
		g_string_append(gs,"0");

	GET_NEW_NODE(n);
	n->type = STRING_NODE;
	n->str.str = gs->str;
	
	g_string_free(gs,FALSE);

	return n;
}

static ETree *
ptf_makenew_power(Token *id, int power)
{
	ETree *n;
	ETree *tokn;
	GET_NEW_NODE(tokn);
	tokn->type = IDENTIFIER_NODE;
	tokn->id.id = id;

	if(power == 1)
		return tokn;

	GET_NEW_NODE(n);
	n->type = OPERATOR_NODE;
	n->op.oper = E_EXP;
	n->op.args = g_list_append(NULL,tokn);
	n->op.args = g_list_append(n->op.args,makenum_ui(power));
	n->op.nargs = 2;

	return n;
}

static ETree *
ptf_makenew_term(mpw_t mul, Token *id, int power)
{
	ETree *n;
	
	if(power==0) {
		n = makenum(mul);
	} else if(mpw_cmp_ui(mul,1)==0) {
		n = ptf_makenew_power(id,power);
	} else {
		GET_NEW_NODE(n);
		n->type = OPERATOR_NODE;
		n->op.oper = E_MUL;
		n->op.args = g_list_append(NULL,makenum(mul));
		n->op.args = g_list_append(n->op.args,ptf_makenew_power(id,power));
		n->op.nargs = 2;
	}
	return n;
}

static ETree *
polytofunc_op(ETree * * a, int *exception)
{
	ETree *n;
	ETree *nn = NULL;
	int i;
	MatrixW *m;

	static Token *var = NULL;
	
	if(!check_poly(a,1,"polytofunc",TRUE))
		return NULL;
	
	if(!var)
		var = d_intern("x");
	
	m = a[0]->mat.matrix;

	for(i=matrixw_width(m)-1;i>=0;i--) {
		ETree *t;
		t = matrixw_index(m,i,0);
		if(mpw_sgn(t->val.value)==0)
			continue;
		
		if(!nn)
			nn = ptf_makenew_term(t->val.value,var,i);
		else {
			ETree *nnn;
			GET_NEW_NODE(nnn);
			nnn->type = OPERATOR_NODE;
			nnn->op.oper = E_PLUS;
			nnn->op.args = g_list_append(NULL,nn);
			nnn->op.args =
				g_list_append(nnn->op.args,
					      ptf_makenew_term(t->val.value,
							       var,i));
			nnn->op.nargs = 2;
			nn = nnn;
		}
	}
	if(!nn)
		nn = makenum_ui(0);

	GET_NEW_NODE(n);
	n->type = FUNCTION_NODE;
	n->func.func = d_makeufunc(NULL,nn,g_list_append(NULL,var),1);
	n->func.func->context = -1;

	return n;
}

static ETree *
help_op(ETree * * a, int *exception)
{
	GList *li;
	
	li = g_list_last(d_getcontext());
	for(;li;li=g_list_previous(li)) {
		EFunc *f = li->data;
		char *s;
		if(!f->id || !f->id->token ||
		   strcmp(f->id->token,"ni")==0 ||
		   strcmp(f->id->token,"shrubbery")==0)
			continue;
		s = g_strdup_printf("%-20s - %s",
				    f->id->token,get_description(f->id->token));
		(*infoout)(s);
		g_free(s);
	}

	return makenum_null();
}

static ETree *
sethelp_op(ETree * * a, int *exception)
{
	if(a[0]->type!=STRING_NODE ||
	   a[1]->type!=STRING_NODE) {
		(*errorout)(_("sethelp: arguments must be strings (function name,help text)"));
		return NULL;
	}
	
	add_description(a[0]->str.str,a[1]->str.str);

	return makenum_null();
}

static ETree *
set_float_prec_op(ETree * * a, int *exception)
{
	long bits;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->val.value)) {
		(*errorout)(_("set_float_prec: argument not an integer"));
		return NULL;
	}

	bits = mpw_get_long(a[0]->val.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(bits<60 || bits>16384) {
		(*errorout)(_("set_float_prec: argument should be between 60 and 16384"));
		return NULL;
	}
	
	if(calcstate.float_prec != bits) {
		calcstate.float_prec = bits;
		mpw_set_default_prec(calcstate.float_prec);
		if(statechange_hook)
			(*statechange_hook)(calcstate);
	}

	return makenum_ui(calcstate.float_prec);
}

static ETree *
get_float_prec_op(ETree * * a, int *exception)
{
	return makenum_ui(calcstate.float_prec);
}

static ETree *
set_max_digits_op(ETree * * a, int *exception)
{
	long digits;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->val.value)) {
		(*errorout)(_("set_max_digits: argument not an integer"));
		return NULL;
	}

	digits = mpw_get_long(a[0]->val.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(digits<0 || digits>256) {
		(*errorout)(_("set_max_digits: argument should be between 0 and 256"));
		return NULL;
	}
	
	if(calcstate.max_digits != digits) {
		calcstate.max_digits = digits;
		if(statechange_hook)
			(*statechange_hook)(calcstate);
	}

	return makenum_ui(calcstate.max_digits);
}

static ETree *
get_max_digits_op(ETree * * a, int *exception)
{
	return makenum_ui(calcstate.max_digits);
}

static ETree *
set_results_as_floats_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("set_results_as_floats: argument not a value"));
		return NULL;
	}
	calcstate.results_as_floats = mpw_sgn(a[0]->val.value)!=0;
	if(statechange_hook)
		(*statechange_hook)(calcstate);

	if(calcstate.results_as_floats)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
get_results_as_floats_op(ETree * * a, int *exception)
{
	if(calcstate.results_as_floats)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
set_scientific_notation_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("set_scientific_notation: argument not a value"));
		return NULL;
	}
	calcstate.scientific_notation = mpw_sgn(a[0]->val.value)!=0;
	if(statechange_hook)
		(*statechange_hook)(calcstate);

	if(calcstate.scientific_notation)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
get_scientific_notation_op(ETree * * a, int *exception)
{
	if(calcstate.scientific_notation)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
set_full_expressions_op(ETree * * a, int *exception)
{
	if(a[0]->type!=VALUE_NODE) {
		(*errorout)(_("set_full_expressions: argument not a value"));
		return NULL;
	}
	calcstate.full_expressions = mpw_sgn(a[0]->val.value)!=0;
	if(statechange_hook)
		(*statechange_hook)(calcstate);

	if(calcstate.full_expressions)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}
static ETree *
get_full_expressions_op(ETree * * a, int *exception)
{
	if(calcstate.full_expressions)
		return makenum_ui(1);
	else
		return makenum_ui(0);
}

static ETree *
set_max_errors_op(ETree * * a, int *exception)
{
	long errors;

	if(a[0]->type!=VALUE_NODE ||
	   !mpw_is_integer(a[0]->val.value)) {
		(*errorout)(_("set_max_errors: argument not an integer"));
		return NULL;
	}

	errors = mpw_get_long(a[0]->val.value);
	if(error_num) {
		error_num = 0;
		return NULL;
	}
	if(errors<0) {
		(*errorout)(_("set_max_errors: argument should be larger then 0"));
		return NULL;
	}
	
	if(calcstate.max_errors != errors) {
		calcstate.max_errors = errors;
		if(statechange_hook)
			(*statechange_hook)(calcstate);
	}

	return makenum_ui(calcstate.max_errors);
}

static ETree *
get_max_errors_op(ETree * * a, int *exception)
{
	return makenum_ui(calcstate.max_errors);
}

/*add the routines to the dictionary*/
void
funclib_addall(void)
{
	d_addfunc(d_makebifunc(d_intern("warranty"),warranty_op,0));
	add_description("warranty",_("Gives the warranty information"));
	d_addfunc(d_makebifunc(d_intern("exit"),exit_op,0));
	add_description("exit",_("Exits the program"));
	d_addfunc(d_makebifunc(d_intern("quit"),exit_op,0));
	add_description("quit",_("Exits the program"));
	d_addfunc(d_makebifunc(d_intern("error"),error_op,1));
	add_description("error",_("Prints a string to the error stream"));
	d_addfunc(d_makebifunc(d_intern("print"),print_op,1));
	add_description("print",_("Prints an expression"));
	d_addfunc(d_makebifunc(d_intern("printn"),printn_op,1));
	add_description("printn",_("Prints an expression without a trailing newline"));
	d_addfunc(d_makebifunc(d_intern("display"),display_op,2));
	add_description("display",_("Display a string and an expression"));

	d_addfunc(d_makebifunc(d_intern("set_float_prec"),set_float_prec_op,1));
	add_description("set_float_prec",_("Set floating point precision"));
	d_addfunc(d_makebifunc(d_intern("get_float_prec"),get_float_prec_op,0));
	add_description("get_float_prec",_("Get floating point precision"));
	d_addfunc(d_makebifunc(d_intern("set_max_digits"),set_max_digits_op,1));
	d_addfunc(d_makebifunc(d_intern("get_max_digits"),get_max_digits_op,0));
	d_addfunc(d_makebifunc(d_intern("set_results_as_floats"),set_results_as_floats_op,1));
	d_addfunc(d_makebifunc(d_intern("get_results_as_floats"),get_results_as_floats_op,0));
	d_addfunc(d_makebifunc(d_intern("set_scientific_notation"),set_scientific_notation_op,1));
	d_addfunc(d_makebifunc(d_intern("get_scientific_notation"),get_scientific_notation_op,0));
	d_addfunc(d_makebifunc(d_intern("set_full_expressions"),set_full_expressions_op,1));
	d_addfunc(d_makebifunc(d_intern("get_full_expressions"),get_full_expressions_op,0));
	d_addfunc(d_makebifunc(d_intern("set_max_errors"),set_max_errors_op,1));
	add_description("set_max_errors",_("Set maximum number of errors printed"));
	d_addfunc(d_makebifunc(d_intern("get_max_errors"),get_max_errors_op,0));
	add_description("get_max_errors",_("Get maximum number of errors printed"));

	d_addfunc(d_makebifunc(d_intern("ni"),ni_op,0));
	d_addfunc(d_makebifunc(d_intern("shrubbery"),shrubbery_op,0));
	d_addfunc(d_makebifunc(d_intern("sin"),sin_op,1));
	add_description("sin",_("Calculates the sine function"));
	d_addfunc(d_makebifunc(d_intern("cos"),cos_op,1));
	add_description("cos",_("Calculates the cosine function"));
	d_addfunc(d_makebifunc(d_intern("sinh"),sinh_op,1));
	add_description("sinh",_("Calculates the hyperbolic sine function"));
	d_addfunc(d_makebifunc(d_intern("cosh"),cosh_op,1));
	add_description("cosh",_("Calculates the hyperbolic cosine function"));
	d_addfunc(d_makebifunc(d_intern("tan"),tan_op,1));
	add_description("tan",_("Calculates the tan function"));
	d_addfunc(d_makebifunc(d_intern("atan"),atan_op,1));
	add_description("atan",_("Calculates the arctan function"));
	d_addfunc(d_makebifunc(d_intern("pi"),pi_op,0));
	add_description("pi",_("The number pi"));
	d_addfunc(d_makebifunc(d_intern("e"),e_op,0));
	add_description("e",_("The natural number e"));
	d_addfunc(d_makebifunc(d_intern("sqrt"),sqrt_op,1));
	add_description("sqrt",_("The square root"));
	d_addfunc(d_makebifunc(d_intern("exp"),exp_op,1));
	add_description("exp",_("The exponential function"));
	d_addfunc(d_makebifunc(d_intern("ln"),ln_op,1));
	add_description("ln",_("The natural logarithm function"));
	d_addfunc(d_makebifunc(d_intern("gcd"),gcd_op,2));
	add_description("gcd",_("Greatest common divisor"));
	d_addfunc(d_makebifunc(d_intern("lcm"),lcm_op,2));
	add_description("lcm",_("Least common multiplier"));
	d_addfunc(d_makebifunc(d_intern("jacobi"),jacobi_op,2));
	d_addfunc(d_makebifunc(d_intern("legendre"),legendre_op,2));
	d_addfunc(d_makebifunc(d_intern("perfect_square"),perfect_square_op,1));
	add_description("perfect_square",_("Check a number for being a perfect square"));
	d_addfunc(d_makebifunc(d_intern("max"),max_op,2));
	add_description("max",_("Return the larger of two arguments"));
	d_addfunc(d_makebifunc(d_intern("min"),min_op,2));
	add_description("min",_("Return the smaller of two arguments"));
	d_addfunc(d_makebifunc(d_intern("prime"),prime_op,1));
	add_description("prime",_("Return the n'th prime (up to a limit)"));
	d_addfunc(d_makebifunc(d_intern("round"),round_op,1));
	add_description("round",_("Round a number"));
	d_addfunc(d_makebifunc(d_intern("floor"),floor_op,1));
	add_description("floor",_("Get the highest integer less then or equal to n"));
	d_addfunc(d_makebifunc(d_intern("ceil"),ceil_op,1));
	add_description("ceil",_("Get the lowest integer more then or equal to n"));
	d_addfunc(d_makebifunc(d_intern("trunc"),trunc_op,1));
	add_description("trunc",_("Truncate a number to an integer"));
	d_addfunc(d_makebifunc(d_intern("float"),float_op,1));
	add_description("float",_("Make number a float"));
	d_addfunc(d_makebifunc(d_intern("Re"),Re_op,1));
	add_description("Re",_("Get the real part of a complex number"));
	d_addfunc(d_makebifunc(d_intern("Im"),Im_op,1));
	add_description("Im",_("Get the imaginary part of a complex number"));
	d_addfunc(d_makebifunc(d_intern("I"),I_op,1));
	add_description("I",_("Make an identity matrix of a given size"));
	d_addfunc(d_makebifunc(d_intern("rows"),rows_op,1));
	add_description("rows",_("Get the rows of a column"));
	d_addfunc(d_makebifunc(d_intern("columns"),columns_op,1));
	add_description("columns",_("Get the columns of a column"));
	d_addfunc(d_makebifunc(d_intern("set_size"),set_size_op,3));
	add_description("set_size",_("Make new matrix of given size from old one"));
	d_addfunc(d_makebifunc(d_intern("det"),det_op,1));
	add_description("det",_("Get the determinant of a matrix"));
	d_addfunc(d_makebifunc(d_intern("ref"),ref_op,1));
	add_description("ref",_("Get the row echelon form of a matrix"));
	d_addfunc(d_makebifunc(d_intern("rref"),rref_op,1));
	add_description("rref",_("Get the reduced row echelon form of a matrix"));
	d_addfunc(d_makebifunc(d_intern("is_value_only"),is_value_only_op,1));
	d_addfunc(d_makebifunc(d_intern("is_null"),is_null_op,1));
	d_addfunc(d_makebifunc(d_intern("is_value"),is_value_op,1));
	d_addfunc(d_makebifunc(d_intern("is_string"),is_string_op,1));
	d_addfunc(d_makebifunc(d_intern("is_matrix"),is_matrix_op,1));
	d_addfunc(d_makebifunc(d_intern("is_function"),is_function_op,1));
	d_addfunc(d_makebifunc(d_intern("is_function_ref"),is_function_ref_op,1));
	d_addfunc(d_makebifunc(d_intern("is_complex"),is_complex_op,1));
	d_addfunc(d_makebifunc(d_intern("is_real"),is_real_op,1));
	d_addfunc(d_makebifunc(d_intern("is_integer"),is_integer_op,1));
	d_addfunc(d_makebifunc(d_intern("is_rational"),is_rational_op,1));
	d_addfunc(d_makebifunc(d_intern("is_float"),is_float_op,1));
	d_addfunc(d_makebifunc(d_intern("addpoly"),addpoly_op,2));
	d_addfunc(d_makebifunc(d_intern("subpoly"),subpoly_op,2));
	d_addfunc(d_makebifunc(d_intern("mulpoly"),mulpoly_op,2));
	d_addfunc(d_makebifunc(d_intern("derpoly"),derpoly_op,1));
	d_addfunc(d_makebifunc(d_intern("der2poly"),der2poly_op,1));
	d_addfunc(d_makebifunc(d_intern("trimpoly"),trimpoly_op,1));
	d_addfunc(d_makebifunc(d_intern("is_poly"),is_poly_op,1));
	d_addfunc(d_makebifunc(d_intern("polytostring"),polytostring_op,2));
	d_addfunc(d_makebifunc(d_intern("polytofunc"),polytofunc_op,1));
	d_addfunc(d_makebifunc(d_intern("help"),help_op,0));
	add_description("help",_("Display function list with small help"));
	d_addfunc(d_makebifunc(d_intern("sethelp"),sethelp_op,2));
	
	/*temporary until well done internal functions are done*/
	_internal_ln_function = d_makeufunc(d_intern("<internal>ln"),
					    /*FIXME:this is not the correct 
					      function*/
					    parseexp("error(\"ln not finished\")",
						     NULL,FALSE,FALSE,NULL),
					    g_list_append(NULL,d_intern("x")),1);
	_internal_exp_function = d_makeufunc(d_intern("<internal>exp"),
					     parseexp("sum = float(x^0); "
						      "fact = 1; "
						      "for i = 1 to 100 do "
						      "(fact = fact * x / i; "
						      "sum = sum + fact) ; sum",
						      NULL,FALSE,FALSE,NULL),
					     g_list_append(NULL,d_intern("x")),1);
}
