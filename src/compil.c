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

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#define _(x) x
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "mpwrap.h"
#include "eval.h"
#include "calc.h"
#include "dict.h"
#include "util.h"
#include "matrix.h"
#include "matrixw.h"

#include "compil.h"

extern void (*errorout)(char *);

extern ETree *free_trees;

/*sort of weird encoding, use 'a'+upper 4 bits and 'a'+lower 4 bits*/
static void
append_string(GString *gs,char *s)
{
	char *p;
	int i;
	char out[3]="aa";
	for(p=s;*p;p++) {
		out[0]='a'+((*p)&0xF);
		out[1]='a'+((*p)>>4);
		g_string_append(gs,out);
	}
}

/*sort of weird encoding, use 'a'+upper 4 bits and 'a'+lower 4 bits*/
static char *
decode_string(char *s)
{
	int len = strlen(s);
	char *p,*pp,*ps;
	if(len%2 == 1)
		return NULL;
	
	/*the 0 takes care of the termination*/
	p = g_new0(char,(len/2)+1);
	
	for(ps=s,pp=p;*ps;ps+=2,pp++) {
		if(*ps<'a' || *ps >'a'+0xF ||
		   *(ps+1)<'a' || *(ps+1) >'a'+0xF) {
			g_free(p);
			return NULL;
		}
		*pp = (*ps-'a') + ((*(ps+1)-'a')<<4);
	}
	return p;
}

static void
compile_node(ETree *t,GString *gs)
{
	char *s;
	int i,j;
	GList *li;
	g_string_sprintfa(gs,";%d;%d",t->type,t->nargs);
	switch(t->type) {
	case NULL_NODE:
		break;
	case VALUE_NODE:
		s = mpw_getstring(t->data.value,0,FALSE,FALSE);
		g_string_append_c(gs,';');
		g_string_append(gs,s);
		g_free(s);
		break;
	case MATRIX_NODE:
		g_string_sprintfa(gs,";%dx%d",matrixw_width(t->data.matrix),
				 matrixw_height(t->data.matrix));
		for(i=0;i<matrixw_width(t->data.matrix);i++) {
			for(j=0;j<matrixw_height(t->data.matrix);j++) {
				ETree *tt = matrixw_set_index(t->data.matrix,i,j);
				if(!tt) g_string_append(gs,";0");
				else {
					g_string_append(gs,";N");
					compile_node(tt,gs);
				}
			}
		}
		break;
	case OPERATOR_NODE:
		g_string_sprintfa(gs,";%d",t->data.oper);
		break;
	case IDENTIFIER_NODE:
		g_string_sprintfa(gs,";%s",t->data.id->token);
		break;
	case STRING_NODE:
		if(*t->data.str) {
			g_string_append_c(gs,';');
			append_string(gs,t->data.str);
		} else {
			g_string_append(gs,";E");
		}
		break;
	case FUNCTION_NODE:
		g_assert(t->data.func->type==USER_FUNC);
		g_assert(t->data.func->id==NULL);
		g_string_sprintfa(gs,";%d",t->data.func->nargs);
		for(li=t->data.func->named_args;li;li=g_list_next(li)) {
			Token *tok = li->data;
			g_string_sprintfa(gs,";%s",tok->token);
		}
		compile_node(t->data.func->data.user,gs);
		break;
	case COMPARISON_NODE:
		for(li=t->data.comp;li;li=g_list_next(li)) {
			int oper = GPOINTER_TO_INT(li->data);
			g_string_sprintfa(gs,";%d",oper);
		}
		break;
	default:
		g_assert_not_reached(); break;
	}
	for(li=t->args;li;li=g_list_next(li)) {
		ETree *tt = li->data;
		compile_node(tt,gs);
	}
}

char *
compile_tree(ETree *t)
{
	GString *gs;
	char *s;
	
	gs = g_string_new("T");
	
	compile_node(t,gs);
	
	s = gs->str;
	g_string_free(gs,FALSE);
	return s;
}

static ETree *
decompile_node(void)
{
	ETree *n;
	char *p;
	int type=-1;
	int nargs=-1;
	int oper;
	int i,j;
	int w,h;
	MatrixW *m;
	GList *li,*oli;
	EFunc *func;
	mpw_t tmp;

	p = strtok(NULL,";");
	if(!p) return NULL;
	sscanf(p,"%d",&type);
	if(type==-1) return NULL;

	p = strtok(NULL,";");
	if(!p) return NULL;
	sscanf(p,"%d",&nargs);
	if(nargs==-1) return NULL;
	
	switch(type) {
	case NULL_NODE:
		return makenum_null();
	case VALUE_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		mpw_init(tmp);
		mpw_set_str(tmp,p,10);
		return makenum_use(tmp);
	case MATRIX_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		h = w = -1;
		sscanf(p,"%dx%d",&w,&h);
		if(h==-1 || w ==-1)
			return NULL;
		m = matrixw_new();
		matrixw_set_size(m,w,h);
		for(i=0;i<w;i++) {
			for(j=0;j<h;j++) {
				p = strtok(NULL,";");
				if(!p) {
					matrixw_free(m);
					return NULL;
				}
				if(*p=='N') {
					ETree *tt = decompile_node();
					if(!tt) {
						matrixw_free(m);
						return NULL;
					}
					matrixw_set_index(m,i,j)=tt;
				} else if(*p!='0') {
					matrixw_free(m);
					return NULL;
				}
			}
		}
		GET_NEW_NODE(n);
		n->type = MATRIX_NODE;
		n->args = NULL;
		n->nargs = 0;
		n->data.matrix = m;
		return n;
	case OPERATOR_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		oper = -1;
		sscanf(p,"%d",&oper);
		if(oper==-1) return NULL;
		
		li = NULL;
		for(i=0;i<nargs;i++) {
			ETree *tt = decompile_node();
			if(!tt) {
				g_list_foreach(li,(GFunc)freetree,NULL);
				g_list_free(li);
				return NULL;
			}
			li = g_list_append(li,tt);
		}

		GET_NEW_NODE(n);
		n->type = OPERATOR_NODE;
		n->args = li;
		n->nargs = nargs;
		n->data.oper = oper;
		return n;
	case IDENTIFIER_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		GET_NEW_NODE(n);
		n->type = IDENTIFIER_NODE;
		n->args = NULL;
		n->nargs = 0;
		n->data.id = d_intern(p);
		return n;
	case STRING_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		
		if(*p=='E')
			p = g_strdup("");
		else {
			p = decode_string(p);
			if(!p) return NULL;
		}
		GET_NEW_NODE(n);
		n->type = STRING_NODE;
		n->args = NULL;
		n->nargs = 0;
		n->data.str = p;
		return n;
	case FUNCTION_NODE:
		p = strtok(NULL,";");
		if(!p) return NULL;
		nargs = -1;
		sscanf(p,"%d",&nargs);
		if(nargs==-1) return NULL;

		li = NULL;
		for(i=0;i<nargs;i++) {
			p = strtok(NULL,";");
			if(!p) {
				g_list_free(li);
				return NULL;
			}
			li = g_list_append(li,d_intern(p));
		}
		
		n = decompile_node();
		if(!n) {
			g_list_free(li);
			return NULL;
		}

		func = d_makeufunc(NULL,n,li,nargs);
		func->context = -1;

		GET_NEW_NODE(n);
		n->type = FUNCTION_NODE;
		n->args = NULL;
		n->nargs = 0;
		n->data.func = func;
		return n;
	case COMPARISON_NODE:
		oli = NULL;
		for(i=0;i<nargs-1;i++) {
			p = strtok(NULL,";");
			if(!p) {
				g_list_free(oli);
				return NULL;
			}
			j = -1;
			sscanf(p,"%d",&j);
			if(j==-1) {
				g_list_free(oli);
				return NULL;
			}
			oli = g_list_append(oli,GINT_TO_POINTER(j));
		}

		li = NULL;
		for(i=0;i<nargs;i++) {
			ETree *tt = decompile_node();
			if(!tt) {
				g_list_foreach(li,(GFunc)freetree,NULL);
				g_list_free(li);
				g_list_free(oli);
				return NULL;
			}
			li = g_list_append(li,tt);
		}

		GET_NEW_NODE(n);
		n->type = COMPARISON_NODE;
		n->args = li;
		n->nargs = nargs;
		n->data.comp = oli;
		return n;
	default:
		return NULL;
	}
}

ETree *
decompile_tree(char *s)
{
	ETree *t;
	char *p;
	
	if(!s) return NULL;
	
	p = strtok(s,";");
	
	if(strcmp(p,"T")!=0) {
		(*errorout)(_("Bad tree record when decompiling"));
		return NULL;
	}
	
	t = decompile_node();
	if(!t) {
		(*errorout)(_("Bad tree record when decompiling"));
		return NULL;
	}

	g_free(s);
	
	return t;
}
