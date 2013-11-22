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
#include <gnome.h>
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
extern GList * notation_stack;
extern int use_readline;

extern FILE *yyin;
extern char *yytext;
extern int yydebug;

/* stack ... has to be global:-( */
evalstack_t evalstack=NULL;

/*error .. global as well*/
calc_error_t error_num = NO_ERROR;

/*the current state of the calculator*/
calcstate_t calcstate;

/*error reporting function*/
void (*errorout)(char *)=NULL;

static char *
output_str(char *s, FILE *out, char *p)
{
	if(!out)
		return appendstr(s,p);
	fputs(p,out);
	return NULL;
}

/*spit out ordinary primitives, such as plus, minus etc*/
static char *
get_normalprim(char *s, FILE *out, char *p, tree_t *l, tree_t *r)
{
	s=makeexprstr(s,out,l);
	s=output_str(s,out,p);
	s=makeexprstr(s,out,r);
	return s;
}

/*appends to S the primitive op and it's arguments*/
static char *
get_prim(char *s, FILE *out, int op, tree_t *l, tree_t *r, tree_t *sr)
{
	char tmp[3];
	s=output_str(s,out,"(");
	switch(op) {
		case E_SEPAR: s=get_normalprim(s,out,";",l,r); break;
		case E_EQUALS: s=get_normalprim(s,out,"=",l,r); break;
		case E_PLUS: s=get_normalprim(s,out,"+",l,r); break;
		case E_MINUS: s=get_normalprim(s,out,"-",l,r); break;
		case E_MUL: s=get_normalprim(s,out,"*",l,r); break;
		case E_DIV: s=get_normalprim(s,out,"/",l,r); break;
		case E_MOD: s=get_normalprim(s,out,"%",l,r); break;
		case E_NEG: s=get_normalprim(s,out,"~",l,r); break;
		case E_EXP: s=get_normalprim(s,out,"^",l,r); break;
		case E_FACT: s=get_normalprim(s,out,"!",l,r); break;
		case E_FUNCDEF:
			if(l->type!=ACTION_NODE ||
				l->data.action.type!=FUNCTION_TYPE) {
				s=output_str(s,out,"?error?)");
				return s;
			}

			tmp[2]='\0';
			tmp[1]='\\';
			tmp[0]=(char)(l->data.action.data.func->nargs)
				+'0';
			s=output_str(s,out,tmp);
			s=output_str(s,out,l->data.action.data.func->id);
			s=output_str(s,out,"{");
			s=makeexprstr(s,out,r);
			s=output_str(s,out,"}");
			break;
		case E_IF_CONS:
			s=output_str(s,out,"if(");
			s=makeexprstr(s,out,l);
			s=output_str(s,out,")(");
			s=makeexprstr(s,out,r);
			s=output_str(s,out,")");
			break;
		case E_IFELSE_CONS:
			s=output_str(s,out,"ifelse(");
			s=makeexprstr(s,out,l);
			s=output_str(s,out,")(");
			s=makeexprstr(s,out,r);
			s=output_str(s,out,")(");
			s=makeexprstr(s,out,sr);
			s=output_str(s,out,")");
			break;
		case E_WHILE_CONS:
			s=output_str(s,out,"while(");
			s=makeexprstr(s,out,l);
			s=output_str(s,out,")(");
			s=makeexprstr(s,out,r);
			s=output_str(s,out,")");
			break;
		case E_EQ_CMP: s=get_normalprim(s,out,"==",l,r); break;
		case E_NE_CMP: s=get_normalprim(s,out,"!=",l,r); break;
		case E_CMP_CMP: s=get_normalprim(s,out,"<=>",l,r); break;
		case E_LT_CMP: s=get_normalprim(s,out,"<",l,r); break;
		case E_GT_CMP: s=get_normalprim(s,out,">",l,r); break;
		case E_LE_CMP: s=get_normalprim(s,out,"<=",l,r); break;
		case E_GE_CMP: s=get_normalprim(s,out,">=",l,r); break;
		case E_LOGICAL_AND: s=get_normalprim(s,out,"and",l,r); break;
		case E_LOGICAL_OR: s=get_normalprim(s,out,"or",l,r); break;
		case E_LOGICAL_XOR: s=get_normalprim(s,out,"xor",l,r); break;
		case E_LOGICAL_NOT: s=get_normalprim(s,out,"not",l,r); break;
		default:
			s=output_str(s,out,"<???>");
			break;
	}
	s=output_str(s,out,")");
	return s;
}

/*make a string representation of an expression*/
char *
makeexprstr(char *s, FILE *out, tree_t *n)
{
	char *p;
	int i;

	if(!n)
		return s;

	if(n->type==NUMBER_NODE) {
		p=mpw_getstring(n->data.val,calcstate.max_digits,
			calcstate.scientific_notation,
			calcstate.results_as_floats);
		/*if(!s)
			return p;*/
		s=output_str(s,out,p);
		g_free(p);
		return s;
	} else if(n->type==ACTION_NODE) {
		if(n->data.action.type==PRIMITIVE_TYPE) {
			s=get_prim(s,out,n->data.action.data.primitive,
				n->args[0],n->args[1],n->args[2]);
			return s;
		} else { /*type==FUNCTION_TYPE*/
			s=output_str(s,out,n->data.action.data.func->id);
			if(n->data.action.data.func->nargs<=0)
				return s;
			s=output_str(s,out,"(");
			s=makeexprstr(s,out, n->args[0]);
			for(i=1;i<n->data.action.data.func->nargs;i++) {
				s=output_str(s,out,",");
				s=makeexprstr(s,out, n->args[i]);
			}
			s=output_str(s,out,")");
			return s;
		}
	} else if(n->type == NULL_NODE) {
		s=output_str(s,out,"(null)");
		return s;
	}
	s=output_str(s,out,"(???)");
	return s;
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


	if(calcstate.notation_in==INFIX_NOTATION)
		first_tok = INFIX_EXPR;
	else if(calcstate.notation_in==POSTFIX_NOTATION)
		first_tok = POSTFIX_EXPR;
	else
		first_tok = PREFIX_EXPR;

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
				error_num = EOF_ERROR;
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
	notation_stack =
		g_list_prepend(NULL, GINT_TO_POINTER(calcstate.notation_in));
	/*yydebug=TRUE;*/  /*turn debugging of parsing on here!*/
	yyparse();
	
	while(notation_stack)
		stack_pop(&notation_stack);
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
	
	/*found end of file*/
	if(stacklen==0) {
		error_num = EOF_ERROR;
		return NULL;
	}

	/*stack is supposed to have only ONE entry*/
	if(stacklen!=1) {
		printf("stack size: %d\n",(int)g_list_length(evalstack));
		while(evalstack)
			freetree(stack_pop(&evalstack));
		(*errorout)(_("ERROR: Probably corrupt stack!"));
		return NULL;
	}

	evalstack->data = evalnode(evalstack->data);

	/*catch evaluation errors*/
	if(error_num!=NO_ERROR) {
		freetree(stack_pop(&evalstack));
		return NULL;
	}

	p = NULL;
	if(((tree_t *)evalstack->data)->type != NULL_NODE) {
		if(prefix) {
			if(outfile)
				fputs(prefix,outfile);
			else
				p = g_strdup(prefix);
		}
		p = makeexprstr(p, outfile,evalstack->data);
		if(outfile)
			fputc('\n',outfile);
	}

	freetree(stack_pop(&evalstack));

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

