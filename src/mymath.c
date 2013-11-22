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

#include "mpwrap.h"
#include "mymath.h"

/*get sin*/
void
mympw_sin(mpw_t rop, mpw_t op)
{
	mpw_t ftmp;
	mpw_t fres;
	mpw_t foldres;
	unsigned int i;
	int negate=TRUE;

	/*special case*/
	if(mpw_cmp_ui(op,0)==0) {
		mpw_set_ui(rop,0);
		return;
	}

	mpw_init(ftmp);
	mpw_init(fres);
	mpw_init(foldres);
	mpw_set(foldres,op);

	for(i=3;;i+=2) {
		mpw_fac_ui(ftmp,i);
		mpw_pow_ui(fres,op,i);
		mpw_div(fres,fres,ftmp);
		if(negate)
			mpw_neg(fres,fres);
		negate= !negate;
		mpw_add(fres,fres,foldres);

		if(mpw_cmp(foldres,fres)==0)
			break;
		mpw_set(foldres,fres);
	}
	
	mpw_clear(ftmp);
	mpw_clear(foldres);

	mpw_set(rop,fres);

	mpw_clear(fres);
}

/*get cos*/
void
mympw_cos(mpw_t rop, mpw_t op)
{
	mpw_t ftmp;
	mpw_t fres;
	mpw_t foldres;
	unsigned int i;
	int negate=TRUE;

	/*special case*/
	if(mpw_cmp_ui(op,0)==0) {
		mpw_set_ui(rop,1);
		return;
	}

	mpw_init(ftmp);
	mpw_init(fres);
	mpw_init(foldres);
	mpw_set_ui(foldres,1);

	for(i=2;;i+=2) {
		mpw_fac_ui(ftmp,i);
		mpw_pow_ui(fres,op,i);
		mpw_div(fres,fres,ftmp);
		if(negate)
			mpw_neg(fres,fres);
		negate= !negate;
		mpw_add(fres,fres,foldres);

		if(mpw_cmp(foldres,fres)==0)
			break;
		mpw_set(foldres,fres);
	}
	
	mpw_clear(ftmp);
	mpw_clear(foldres);

	mpw_set(rop,fres);

	mpw_clear(fres);
}

/*get e*/
void
mympw_gete(mpw_t rop)
{
	static mpw_t cache;
	static int iscached=FALSE;

	mpw_t fres;
	mpw_t foldres;
	unsigned int i;

	if(iscached)
		mpw_set(rop,cache);

	/*taylor series for x=1*/
	mpw_init(fres);
	mpw_init(foldres);
	mpw_set_d(foldres,2.0);

	for(i=2;;i++) {
		mpw_fac_ui(fres,i);
		mpw_ui_div(fres,1,fres);
		mpw_add(fres,fres,foldres);

		if(mpw_cmp(foldres,fres)==0)
			break;
		mpw_set(foldres,fres);
	}
	
	mpw_clear(foldres);

	iscached=TRUE;
	mpw_init(cache);
	mpw_set(cache,fres);
	mpw_set(rop,fres);

	mpw_clear(fres);
}

/*get the value for pi*/
void
mympw_getpi(mpw_t rop)
{
	static mpw_t cache;
	static int iscached=FALSE;

	mpw_t fr;
	mpw_t fr2;
	mpw_t frt;

	if(iscached)
		mpw_set(rop,cache);

	/*
	 * Newton's method: Xn+1 = Xn - f(Xn)/f'(Xn)
	 */
	
	mpw_init(fr);
	mpw_init(fr2);
	mpw_init(frt);
	mpw_set_d(fr,3.14159265358979323846); /*use quite a precise guess
						as the initial one*/
	for(;;) {
		mympw_sin(fr2,fr);

		mympw_cos(frt,fr);
		mpw_div(fr2,fr2,frt);
		mpw_neg(fr2,fr2);
		mpw_add(fr2,fr2,fr);
		
		if(mpw_cmp(fr2,fr)==0)
			break;
		mpw_set(fr,fr2);
	}
	mpw_clear(fr2);
	mpw_clear(frt);

	iscached=TRUE;
	mpw_init(cache);
	mpw_set(cache,fr);
	mpw_set(rop,fr);
	mpw_clear(fr);
}

