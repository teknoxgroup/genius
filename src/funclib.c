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
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "funclib.h"
#include "mymath.h"
#include "calc.h"

extern void (*errorout)(char *);
extern calc_error_t error_num;

static tree_t *
warranty_op(tree_t * * a)
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
	return NULL;
}

static tree_t *
exit_op(tree_t * * a)
{
	error_num = EOF_ERROR;
	return NULL;
}

static tree_t *
ni_op(tree_t * * a)
{
	(*errorout)("We are the Knights Who Say... Ni!");
	error_num = IGNORE_ERROR;
	return NULL;
}

static tree_t *
shrubbery_op(tree_t * * a)
{
	(*errorout)("Then, when you have found the shrubbery, you must\n"
		    "cut down the mightiest tree in the forest... with...\n"
		    "A HERRING!");
	error_num = IGNORE_ERROR;
	return NULL;
}
	
/*print function*/
static tree_t *
print_op(tree_t * * a)
{
	makeexprstr(NULL,stdout,a[0]);
	puts("");
	return makenum_ui(1);
}

/*sin function*/
static tree_t *
sin_op(tree_t * * a)
{
	mpw_t fr;
	mpw_t pitmp;

	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	if(mpw_is_complex(a[0]->data.val))
		return NULL;

	mpw_init(fr);
	mpw_set(fr,a[0]->data.val);

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
static tree_t *
cos_op(tree_t * * a)
{
	mpw_t fr;
	mpw_t pitmp;

	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	if(mpw_is_complex(a[0]->data.val))
		return NULL;

	mpw_init(fr);
	mpw_set(fr,a[0]->data.val);


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
static tree_t *
tan_op(tree_t * * a)
{
	mpw_t fr;
	mpw_t fr2;
	mpw_t pitmp;

	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	if(mpw_is_complex(a[0]->data.val))
		return NULL;

	mpw_init(fr);
	mpw_set(fr,a[0]->data.val);


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
static tree_t *
e_op(tree_t * * a)
{
	mpw_t fr;

	tree_t *n;

	mpw_init(fr);
	mympw_gete(fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

/*pi function (or pi variable or whatever)*/
static tree_t *
pi_op(tree_t * * a)
{
	mpw_t fr;

	tree_t *n;

	mpw_init(fr);
	mympw_getpi(fr);

	n=makenum(fr);
	mpw_clear(fr);
	return n;
}

static tree_t *
is_complex_op(tree_t * * a)
{
	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	if(mpw_is_complex(a[0]->data.val))
		n = makenum_ui(1);
	else
		n = makenum_ui(0);
	return n;
}

static tree_t *
round_op(tree_t * * a)
{
	mpw_t fr;
	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	mpw_init(fr);
	mpw_round(fr,a[0]->data.val);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static tree_t *
Re_op(tree_t * * a)
{
	mpw_t fr;
	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	mpw_init(fr);
	mpw_re(fr,a[0]->data.val);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static tree_t *
Im_op(tree_t * * a)
{
	mpw_t fr;
	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	mpw_init(fr);
	mpw_im(fr,a[0]->data.val);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

static tree_t *
sqrt_op(tree_t * * a)
{
	mpw_t fr;
	tree_t *n;

	if(a[0]->type!=NUMBER_NODE)
		return NULL;
	mpw_init(fr);
	mpw_sqrt(fr,a[0]->data.val);
	n = makenum(fr);
	mpw_clear(fr);
	return n;
}

/*add the routines to the dictionary*/
void
funclib_addall(void)
{
	d_addfunc(d_makebifunc("warranty",warranty_op,0,FALSE));
	d_addfunc(d_makebifunc("exit",exit_op,0,FALSE));
	d_addfunc(d_makebifunc("quit",exit_op,0,FALSE));
	d_addfunc(d_makebifunc("print",print_op,1,FALSE));
	d_addfunc(d_makebifunc("ni",ni_op,0,FALSE));
	d_addfunc(d_makebifunc("shrubbery",shrubbery_op,0,FALSE));
	d_addfunc(d_makebifunc("sin",sin_op,1,FALSE));
	d_addfunc(d_makebifunc("cos",cos_op,1,FALSE));
	d_addfunc(d_makebifunc("tan",tan_op,1,FALSE));
	d_addfunc(d_makebifunc("pi",pi_op,0,FALSE));
	d_addfunc(d_makebifunc("e",e_op,0,FALSE));
	d_addfunc(d_makebifunc("is_complex",is_complex_op,1,FALSE));
	d_addfunc(d_makebifunc("round",round_op,1,FALSE));
	d_addfunc(d_makebifunc("Re",Re_op,1,FALSE));
	d_addfunc(d_makebifunc("Im",Im_op,1,FALSE));
	d_addfunc(d_makebifunc("sqrt",sqrt_op,1,FALSE));
}
