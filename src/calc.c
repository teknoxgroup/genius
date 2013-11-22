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

#include <glib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glib.h>

#ifdef WITH_READLINE_SUPPORT
#include <readline/readline.h>
#endif

#include "calc.h"
#include "eval.h"
#include "util.h"
#include "dict.h"
#include "funclib.h"

#include "mpwrap.h"

#include "parse.h"

extern int lex_fd[2];
extern int first_tok;
extern int lex_init;
extern int use_readline;

extern FILE *yyin;
extern char *yytext;
extern int yydebug;

extern ETree *returnval;

/* stack ... has to be global:-( */
evalstack_t evalstack=NULL;

/*error .. global as well*/
calc_error_t error_num = NO_ERROR;
int got_eof = FALSE;

/*the current state of the calculator*/
calcstate_t calcstate;

/*error reporting function*/
void (*errorout)(char *)=NULL;

static void
appendout_c(GString *gs, FILE *out, char c)
{
	if(!out)
		g_string_append_c(gs,c);
	else
		fputc(c,out);
}

static void
appendout(GString *gs, FILE *out, char *s)
{
	if(!out)
		g_string_append(gs,s);
	else
		fputs(s,out);
}

static void
append_binaryoper(GString *gs,FILE *out,char *p, ETree *n)
{
	ETree *l,*r;
	GET_LR(n,l,r);
	appendout_c(gs,out,'(');
	print_etree(gs,out,l);
	appendout(gs,out,p);
	print_etree(gs,out,r);
	appendout_c(gs,out,')');
}

static void
append_unaryoper(GString *gs,FILE *out,char *p, ETree *n)
{
	ETree *l;
	GET_L(n,l);
	appendout_c(gs,out,'(');
	appendout(gs,out,p);
	print_etree(gs,out,l);
	appendout_c(gs,out,')');
}

static void
appendoper(GString *gs,FILE *out, ETree *n)
{
	ETree *l,*r,*rr;
	GList *li;

	switch(n->data.oper) {
		case E_SEPAR:
			append_binaryoper(gs,out,";",n); break;
		case E_EQUALS:
			append_binaryoper(gs,out,"=",n); break;
		case E_PLUS:
			append_binaryoper(gs,out,"+",n); break;
		case E_MINUS:
			append_binaryoper(gs,out,"-",n); break;
		case E_MUL:
			append_binaryoper(gs,out,"*",n); break;
		case E_DIV:
			append_binaryoper(gs,out,"/",n); break;
		case E_MOD:
			append_binaryoper(gs,out,"%",n); break;
		case E_NEG:
			append_unaryoper(gs,out,"-",n); break;
		case E_EXP:
			append_binaryoper(gs,out,"^",n); break;
		case E_FACT:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout_c(gs,out,'!');
			appendout_c(gs,out,')');
			break;

		case E_EQ_CMP:
			append_binaryoper(gs,out,"==",n); break;
		case E_NE_CMP:
			append_binaryoper(gs,out,"!=",n); break;
		case E_CMP_CMP:
			append_binaryoper(gs,out,"<=>",n); break;
		case E_LT_CMP:
			append_binaryoper(gs,out,"<",n); break;
		case E_GT_CMP:
			append_binaryoper(gs,out,">",n); break;
		case E_LE_CMP:
			append_binaryoper(gs,out,"<=",n); break;
		case E_GE_CMP:
			append_binaryoper(gs,out,">=",n); break;
		case E_LOGICAL_AND:
			append_binaryoper(gs,out," and ",n); break;
		case E_LOGICAL_OR:
			append_binaryoper(gs,out," or ",n); break;
		case E_LOGICAL_XOR:
			append_binaryoper(gs,out," xor ",n); break;
		case E_LOGICAL_NOT:
			append_unaryoper(gs,out,"not ",n); break;

		case E_GET_ELEMENT:
			GET_LRR(n,l,r,rr);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@[");
			print_etree(gs,out,r);
			appendout_c(gs,out,',');
			print_etree(gs,out,rr);
			appendout(gs,out,"])");
			break;
		case E_GET_ROW:
			GET_LR(n,l,r);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@[");
			print_etree(gs,out,r);
			appendout(gs,out,",])");
			break;
		case E_GET_COLUMN:
			GET_LR(n,l,r);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@[,");
			print_etree(gs,out,r);
			appendout(gs,out,"])");
			break;

		case E_REFERENCE:
			append_unaryoper(gs,out,"&",n); break;
		case E_DEREFERENCE:
			append_unaryoper(gs,out,"*",n); break;

		case E_IF_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(if ");
			print_etree(gs,out,l);
			appendout(gs,out," then ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_IFELSE_CONS:
			GET_LRR(n,l,r,rr);
			appendout(gs,out,"(if ");
			print_etree(gs,out,l);
			appendout(gs,out," then ");
			print_etree(gs,out,r);
			appendout(gs,out," else ");
			print_etree(gs,out,rr);
			appendout(gs,out,")");
			break;
		case E_WHILE_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(while ");
			print_etree(gs,out,l);
			appendout(gs,out," do ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_UNTIL_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(until ");
			print_etree(gs,out,l);
			appendout(gs,out," do ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_DOWHILE_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(do ");
			print_etree(gs,out,l);
			appendout(gs,out," while ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_DOUNTIL_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(do ");
			print_etree(gs,out,l);
			appendout(gs,out," until ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;

		case E_DIRECTCALL:
		case E_CALL:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			if(l->type==IDENTIFIER_NODE) {
				appendout(gs,out,l->data.id);
			} else if(l->type == OPERATOR_NODE && l->data.oper == E_DEREFERENCE) {
				ETree *t;
				GET_L(l,t);
				if(t->type!=IDENTIFIER_NODE) {
					(*errorout)(_("Bad identifier for function node!"));
					appendout(gs,out,"???)");
					break;
				}
				appendout_c(gs,out,'*');
				appendout(gs,out,t->data.id);
			} else {
				(*errorout)(_("Bad identifier for function node!"));
				appendout(gs,out,"???)");
				break;
			}
			appendout_c(gs,out,'(');
			li = n->args->next;
			print_etree(gs,out,li->data);
			li=g_list_next(li);
			for(;li!=NULL;li=g_list_next(li)) {
				appendout_c(gs,out,',');
				print_etree(gs,out,li->data);
			}
			appendout(gs,out,"))");
			break;
		case E_RETURN:
			append_unaryoper(gs,out,"return ",n); break;

		default:
			(*errorout)(_("Unexpected operator!"));
			appendout(gs,out,"(???)");
		       break;
	}
}


/*make a string representation of an expression*/
void
print_etree(GString *gs, FILE *out, ETree *n)
{
	char *p;
	
	if(!n) {
		(*errorout)(_("NULL tree!"));
		appendout(gs,out,"(???)");
		return;
	}

	switch(n->type) {
	case NULL_NODE:
		appendout(gs,out,"(null)");
		break;
	case VALUE_NODE:
		p=mpw_getstring(n->data.value,calcstate.max_digits,
				calcstate.scientific_notation,
				calcstate.results_as_floats);
		appendout(gs,out,p);
		g_free(p);
		break;
	case MATRIX_NODE:
	case MATRIX_ROW_NODE:
		/*FIXME: add matrixen*/
		appendout(gs,out,"(FIXME:matrix)");
		break;
	case OPERATOR_NODE:
		appendoper(gs,out,n);
		break;
	case IDENTIFIER_NODE:
		appendout(gs,out,n->data.id);
		break;
	case STRING_NODE:
		appendout_c(gs,out,'"');
		appendout(gs,out,n->data.id);
		appendout_c(gs,out,'"');
		break;
	case FUNCTION_NODE:
		{
			GList *li;
			EFunc *f;
			
			f = n->data.func;
			if(!f) {
				(*errorout)(_("NULL function!"));
				appendout(gs,out,"(???)");
				break;
			}

			appendout(gs,out,"(`(");

			for(li=f->named_args; li!=NULL; li=g_list_next(li)) {
				if(li!=f->named_args)
					appendout_c(gs,out,',');
				appendout(gs,out,li->data);
			}

			if(f->type==USER_FUNC) {
				appendout(gs,out,"){");
				print_etree(gs,out,f->data.user);
				appendout(gs,out,"})");
			} else {
				(*errorout)(_("Unexpected function type!"));
				appendout(gs,out,"){???}");
			}
			break;
		}
	default:
		(*errorout)(_("Unexpected node!"));
		appendout(gs,out,"(???)");
	       break;
	}
}

/*add the right parenthesis and brackets to the end of the expression*/
char *
addparenth(char *s)
{
	GList *stack = NULL;
	char *p;
	int l;

	for(p=s;*p!='\0';p++) {
		switch(*p) {
			case '[':
				stack_push(&stack,"]");
				break;
			case '(':
				stack_push(&stack,")");
				break;
			case '{':
				stack_push(&stack,"}");
				break;
			case ']':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != ']') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
			case ')':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != ')') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
			case '}':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != '}') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
		}
	}
	l = strlen(s);
	s = g_realloc(s,l+g_list_length(stack)+1);
	while(stack)
		s[l++]=*((char *)stack_pop(&stack));
	s[l]='\0';
	return s;
}

char *
evalexp(char * str, FILE *infile, FILE *outfile, char *prefix,
	calcstate_t state,void (*errorfunc)(char *))
{
	char * p;
	int stacklen;
	ETree *ret;
	int willquit = FALSE;

	/*init the context stack and clear out any stale dictionaries
	  except the global one, if this is the first time called it
	  will also register the builtin routines with the global
	  dictionary*/
	d_singlecontext();
	
	errorout=errorfunc;

	error_num=NO_ERROR;

	/*set the state variable for calculator*/
	calcstate=state;
	
	mpw_init_mp();
	mpw_set_default_prec(state.float_prec);
	
	first_tok = STARTTOK;

	use_readline = FALSE;
	if(str || !infile) {
		pipe(lex_fd);
		yyin=fdopen(lex_fd[0],"r");
		if(str) {
			write(lex_fd[1],str,strlen(str));
		}
#ifdef WITH_READLINE_SUPPORT
	 	else {
			char *s = readline("genius> ");
			char buf[256];
			if(!s) {
				got_eof = TRUE;
				return NULL;
			}
			if(*s)
				add_history(s);
			write(lex_fd[1],s,strlen(s));
			free(s);
			use_readline = TRUE;
		}
#endif
		write(lex_fd[1],"\n",1);
		close(lex_fd[1]);
	} else
		yyin = infile;

	lex_init = TRUE;
	/*yydebug=TRUE;*/  /*turn debugging of parsing on here!*/
	yyparse();
	
	/*while(yyparse() && !feof(yyin))
		;*/

	if(str || !infile)
		close(lex_fd[0]);

	/*catch parsing errors*/
	if(error_num!=NO_ERROR) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		return NULL;
	}
	
	stacklen = g_list_length(evalstack);
	
	if(stacklen==0)
		return NULL;

	/*stack is supposed to have only ONE entry*/
	if(stacklen!=1) {
		printf("stack size: %d\n",(int)g_list_length(evalstack));
		while(evalstack)
			freetree(stack_pop(&evalstack));
		(*errorout)(_("ERROR: Probably corrupt stack!"));
		return NULL;
	}

	if(returnval)
		freetree(returnval);
	returnval = NULL;
	ret = evalnode(evalstack->data);
	freetree(stack_pop(&evalstack));
	if(!ret && returnval)
		ret = returnval;
	else if(returnval)
		freetree(returnval);
	returnval = NULL;

	/*catch evaluation errors*/
	if(!ret)
		return NULL;
	if(error_num!=NO_ERROR) {
		freetree(ret);
		return NULL;
	}

	p = NULL;
	if(ret->type != NULL_NODE) {
		GString *gs = g_string_new("");
		if(prefix) {
			if(outfile)
				fputs(prefix,outfile);
			else
				g_string_append(gs,prefix);
		}
		print_etree(gs, outfile,ret);
		if(outfile) {
			g_string_free(gs,TRUE);
			fputc('\n',outfile);
		} else {
			p = gs->str;
			g_string_free(gs,FALSE);
		}
	}

	freetree(ret);

	return p;
}

void
yyerror(char *s)
{
	char *out=NULL;
	out=g_strconcat(_("ERROR: "),s,_(" before '"),yytext,"'",NULL);
	
	(*errorout)(out);
	g_free(out);
	error_num=PARSE_ERROR;
}

